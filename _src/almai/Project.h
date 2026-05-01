/*! \file
    \brief Almai проект 
 */
#pragma once

#include "PrepromptDatabase.h"
//
#include "enums.h"
#include "utils.h"
//
#include "umba/umba.h"
//
#include "umba/string.h"
#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/parse_utils.h"

//

#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct Project
{
    std::unordered_map<std::string, std::vector<std::string> >       roles;


    template<typename WarningHandler>
    void normalizeRoles( const PrepromptDatabase &ppDb
                       , WarningHandler warningHandler
                       )
    {
        for(auto &[role, roleSkills] : roles)
        {
            for(auto &skill : roleSkills)
            {
                PrepromptCategorySetType skillCategories;

                std::string completeSkillName = ppDb.makeCompletePpId(skill, &skillCategories);
                bool bGood = false;
                auto msg = ppDb.makeCompletePpIdErrorMsg(skill, completeSkillName, skillCategories, &bGood);

                if (bGood)
                {
                    skill = completeSkillName;
                }
                else
                {
                    warningHandler(msg);
                }
            }
        }
    }

    template<typename ErrorHandler, typename WarningHandler>
    bool checkNormalize( const PrepromptDatabase &ppDb
                       , ErrorHandler   errorHandler
                       , WarningHandler warningHandler
                       )
    {
        UMBA_USED(errorHandler);
        normalizeRoles(ppDb, warningHandler);
        return true;
    }


    template<typename SkillNamePrepareHandler>
    bool updateRoleFromRoleString(const std::string &role, const std::string &str, SkillNamePrepareHandler skillNamePrepareHandler)
    {
        if (role.empty())
            return false;

        auto &rolesSet = roles[role];

        return umba::parse_utils::optionStringUpdateSet(str, rolesSet, skillNamePrepareHandler, umba::CaseOption::toLower);
    }

    template<typename SkillNamePrepareHandler>
    bool updateRoleFromRoleStringList(const std::string &role, const std::string &strList, SkillNamePrepareHandler skillNamePrepareHandler)
    {
        if (role.empty())
            return false;

        auto &rolesSet = roles[role];

        return umba::parse_utils::optionStringListUpdateSet(strList, rolesSet, skillNamePrepareHandler, umba::CaseOption::toLower);
    }


    template<typename SkillNamePrepareHandler>
    static
    marty::json parse(Project &p, const std::string &text, SkillNamePrepareHandler skillNamePrepareHandler, bool throwErrors)
    {
        auto j = yaml_json::parseToJson(text);


        if (j.find("roles")!=j.end())
        {
            //d.name = j["name"].get<std::string>();
            auto r = j["roles"];

            // https://github.com/nlohmann/json?tab=readme-ov-file#stl-like-access

            if (r.is_object())
            {
                for (const auto &[key, value] : r.items()) // object iteraion
                {
                    std::string role = key;
                    umba::string::case_convert(role, umba::CaseOption::toLower);
    
                    if (value.is_array())
                    {
                        for (nlohmann::json::iterator it = value.begin(); it!=value.end(); ++it)
                        {
                            std::string skillName = it->get<std::string>();
                            if (!p.updateRoleFromRoleString(role, skillName, skillNamePrepareHandler))
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role '" + role + "' with skill value '" + skillName + "'");
                            }
                        }
                    }
    
                    else if (value.is_string())
                    {
                            std::string skillsList = value.get<std::string>();
                            if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
                            }
                    }
    
                    else
                    {
                        throw std::runtime_error("role '" + role + "' is in " + marty::json_utils::nodeTypeName(value) + " format. Only string/array formats allowed");
                    }
    
                } // for (const auto &[key, value] : r.items())
            }


            else if (r.is_array())
            {
                //------------- !!!
                // parse array here
                for (nlohmann::json::iterator it = r.begin(); it!=r.end(); ++it) // array iteraion
                {
                    auto arrItem = *it;

                    if (arrItem.is_string())
                    {
                        std::string roleDefinitionString = arrItem.get<std::string>();
                        std::string role, skillsList;
                        if (!umba::parse_utils::optionStringSplitToPair(roleDefinitionString, role, skillsList, ":"))
                        {
                            // Нужна пара: "role: role-definition"
                            if (throwErrors)
                               throw std::runtime_error("failed to parse role definition string: '" + roleDefinitionString + "'");
                        }

                        umba::string::case_convert(role, umba::CaseOption::toLower);

                        if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
                        {
                            if (throwErrors)
                                throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
                        }
                    }


                    // Тут у нас могут быть объекты с одним или несколькими ключами
                    // compact mapping in sequence
                    // https://chat.deepseek.com/share/1gbmc561xdxi5teq79

                    // https://chat.deepseek.com/share/teujr7bferpccnduwp

                    else if (arrItem.is_object())
                    {
                        for (const auto &[key, value] : arrItem.items()) // object iteraion
                        {
                            std::string role = key;
                            umba::string::case_convert(role, umba::CaseOption::toLower);

                            if (role.empty())
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role - role name is empty");

                                continue;

                            }
                            
                            auto &rolesSet = p.roles[role];

                            yaml_json::parseJsonKeyValueArrayOrString( value // j
                                                          , rolesSet
                                                          , skillNamePrepareHandler
                                                          , throwErrors
                                                          , "value can be only a string type, role: '" + role + "'" // msgValueOnlyString
                                                          , "value can be only a string or array type, role: '" + role + "'" // msgValueOnlyStringOrArray
                                                          , "failed to update role '" + role + "'" // msgUpdateFailedWithVal
                                                          );

                        } // for (const auto &[key, value] : r.items()) // object iteraion

                    } // else if (arrItem.is_object())


                    else if (arrItem.is_array())
                    {
                        // std::string text = it->get<std::string>();
                        throw std::runtime_error("UNEXPECTED ARRAY ITEM");
                    }


                    else
                    {
                        if (throwErrors)
                            throw std::runtime_error( /* "'" */   /* + arrItemStr + */   /* "'" */ "!!!" " is in " + marty::json_utils::nodeTypeName(arrItem) + " format.");
                    }

                } // for (nlohmann::json::iterator it = r.begin(); it!=r.end(); ++it) // array iteraion

            } // if (r.is_object()) else if (r.is_array())


        } // if (j.find("roles")!=j.end())

        return j;
    }


}; // struct Project

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
StreamType& operator<<(StreamType &oss, const Project &p)
{
    oss << "Project:\n\n";

    oss << "  Roles:\n";
    for(const auto &[role,skills] : p.roles)
    {
        oss << "    " << role;

        for(std::size_t i=0; i!=skills.size(); ++i)
        {
            oss << (i==0 ? ": " : ", ") << skills[i];
        }

        oss << "\n";
    }

    oss << "\n";

    return oss;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------


