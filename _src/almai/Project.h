/*! \file
    \brief Almai проект 
 */
#pragma once

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
        auto j = parseToJson(text);


        if (j.find("roles")!=j.end())
        {
            //d.name = j["name"].get<std::string>();
            auto r = j["roles"];

            // if (!r.is_object())
            // {
            //     throw std::runtime_error("'roles' is in " + marty::json_utils::nodeTypeName(r) + " format. Only object format allowed");
            // }

            // https://github.com/nlohmann/json?tab=readme-ov-file#stl-like-access

            if (r.is_object())
            {
                for (const auto &[key, value] : r.items()) // object iteraion
                {
                    const std::string &role = key;
    
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

// roles:
//   # Можно записать и как object, но массив гарантирует порядок элементов, а object - не факт. Впрочем, другой нефакт в том, что при настройке ролей нам порядок не особо и важен
//   - c-cpp-dev: -, skills/cpp-dev; skills/c-dev, -skills/jni-master # тут в одну строку сбросили все предыдущие скилы для роли, и добавили skills/cpp-dev и skills/c-dev
//   - c-cpp-no-java-dev: skills/cpp-dev; skills/c-dev, -skills/jni-master # тут только добавили skills/cpp-dev и skills/c-dev
//   - uni-tester:
//     - - # Сбрасываем все предыдущие скилы для роли (а это будет валидно в yml?)
//     - skills/auto-tester
//     - skills/manual-tester
//   - super-architect:
//     - sw-architect # Архитектор ПО
//     - sys-architect # Архитектор всей системы


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
                            const std::string &role = key;

                            // if (value.is_object())
                            // {
                            //     for (const auto &[k2, v2] : r.items()) // object iteraion
                            // }

                            if (!value.is_string())
                            {
                                if (throwErrors)
                                    throw std::runtime_error("value in compact mapping in sequence can be only a string type, role: '" + role + "', value type: " + marty::json_utils::nodeTypeName(value));
                            }

                            std::string skillsList = value.get<std::string>();

                            if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
                            }

                        } // for (const auto &[key, value] : r.items()) // object iteraion

                    } // else if (arrItem.is_object())


                    else if (arrItem.is_array())
                    {
                        // std::string text = it->get<std::string>();
                        throw std::runtime_error("UNEXPECTED ARRAY ITEM");
                    }


                    else
                    {
                        //std::string arrItemStr = arrItem.get<std::string>();
    
                        if (throwErrors)
                            throw std::runtime_error( /* "'" */   /* + arrItemStr + */   /* "'" */ "!!!" " is in " + marty::json_utils::nodeTypeName(arrItem) + " format.");
    
                        // std::string altersListStr = it->get<std::string>();
                        // std::vector<std::string> altersList = splitString(altersListStr, '|');
                        // d.requires.emplace_back(altersList);
                    
                    }

                }



            }


        } // if (j.find("roles")!=j.end())

        return j;
    }


}; // struct Project

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------


