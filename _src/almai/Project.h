/*! \file
    \brief Утилитарные функции для преобразования ед. число <-> мн. число
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
    static
    marty::json parse(Project &p, const std::string &text, SkillNamePrepareHandler skillNamePrepareHandler)
    {
        auto j = parseToJson(text);

        if (j.find("roles")!=j.end())
        {
            //d.name = j["name"].get<std::string>();
            auto r = j["roles"];

            if (!r.is_object())
            {
                throw std::runtime_error("'roles' is in unknown format. Only object format allowed");
            }

            // https://github.com/nlohmann/json?tab=readme-ov-file#stl-like-access
            for (cons auto &[key, value] : r.items())
            {
                //std::cout << key << " : " << value << "\n";
            }


        }

        // if (j.find("description")!=j.end())
        // {
        //     d.description = j["description"].get<std::string>();
        // }

        return j;
    }


            // if (r.is_string())
            // {
            //     // parse string here
            //  
            //     std::string requiresListStr = r.get<std::string>();
            //     std::vector<std::string> requiresList = CommonDescription::splitString(requiresListStr, ',');
            //     for(const auto &altersListStr : requiresList)
            //     {
            //         std::vector<std::string> altersList = CommonDescription::splitString(altersListStr, '|');
            //         d.requires.emplace_back(altersList);
            //     }
            //  
            // }
            // else if (r.is_array())
            // {
            //     // parse array here
            //     for (nlohmann::json::iterator it = r.begin(); it!=r.end(); ++it)
            //     {
            //       std::string altersListStr = it->get<std::string>();
            //       std::vector<std::string> altersList = splitString(altersListStr, '|');
            //       d.requires.emplace_back(altersList);
            //     }
            // }
            // else
            // {
            //     throw std::runtime_error("'requires' is in unknown format. Only string/array formats allowed");
            // }



}; // struct Project

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------


