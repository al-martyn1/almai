/*! @file
    @brief Types which hold descriptions
 */

#pragma once

#include "utils.h"

//
#include "marty_yaml_toml_json/marty_yaml_toml_json.h"
//
#include "umba/umba.h"
#include "umba/filename.h"
#include "umba/string.h"

//
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// almai::mdxml::
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct CommonDescription
{

    std::string       name;
    std::string       description;


protected:

    // static
    // std::string mergeLines(const std::vector<std::string> &lines)
    // {
    //     std::string res;
    //     for(const auto &l : lines)
    //     {
    //         res.append(l);
    //         res.append(1, '\n');
    //     }
    //  
    //     return res;
    // }

    static
    std::vector<std::string> splitString(const std::string &str, char ch)
    {
        std::vector<std::string> tmp = umba::filename::splitPathList(str, ch);

        for(auto &v : tmp)
            umba::string::trim(v);

        auto e = 
        std::remove_if( tmp.begin(), tmp.end()
                      , [](const auto &s)
                        {
                            return s.empty();
                        }
                      );
        
        return std::vector<std::string>(tmp.begin(), e);
    }



public:

    static
    marty::json parse(CommonDescription &d, const std::string &text)
    {
        auto j = parseToJson(text);

        if (j.find("name")!=j.end())
        {
            d.name = j["name"].get<std::string>();
        }

        if (j.find("description")!=j.end())
        {
            d.description = j["description"].get<std::string>();
        }

        return j;
    }

    static
    marty::json parse(CommonDescription &d, const std::vector<std::string> &lines)
    {
        return parse(d, umba::string::merge(lines.begin(), lines.end(), '\n'));
    }

    static
    CommonDescription parse(const std::string &text)
    {
        CommonDescription d;
        parse(d, text);
        return d;
    }

    static
    CommonDescription parse(const std::vector<std::string> &lines)
    {
        CommonDescription d;
        parse(d, lines);
        return d;
    }

}; // struct CommonDescription

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct PrepromptDescription : public CommonDescription
{
    std::vector< std::vector<std::string> >     requires;


    static
    marty::json parse(PrepromptDescription &d, const std::string &text)
    {
        marty::json j = CommonDescription::parse(d, text);

        if (j.find("requires")!=j.end())
        {
            auto r = j["requires"];

            if (r.is_string())
            {
                // parse string here

                std::string requiresListStr = r.get<std::string>();
                std::vector<std::string> requiresList = CommonDescription::splitString(requiresListStr, ',');
                for(const auto &altersListStr : requiresList)
                {
                    std::vector<std::string> altersList = CommonDescription::splitString(altersListStr, '|');
                    d.requires.emplace_back(altersList);
                }

            }
            else if (r.is_array())
            {
                // parse array here
                for (nlohmann::json::iterator it = r.begin(); it!=r.end(); ++it)
                {
                  std::string altersListStr = it->get<std::string>();
                  std::vector<std::string> altersList = splitString(altersListStr, '|');
                  d.requires.emplace_back(altersList);
                }
            }
            else
            {
                throw std::runtime_error("'requires' is in unknown format. Only string/array formats allowed");
            }

        }

        return j;
    }

    static
    marty::json parse(PrepromptDescription &d, const std::vector<std::string> &lines)
    {
        return parse(d, umba::string::merge(lines.begin(), lines.end(), '\n'));
    }

    static
    PrepromptDescription parse(const std::string &text)
    {
        PrepromptDescription d;
        parse(d, text);
        return d;
    }

    static
    PrepromptDescription parse(const std::vector<std::string> &lines)
    {
        PrepromptDescription d;
        parse(d, lines);
        return d;
    }



}; // struct PrepromptDescription

//----------------------------------------------------------------------------

// // other stuff
// j.size();     // 4 entries
// j.empty();    // false
// j.type();     // json::value_t::array
// j.clear();    // the array is empty again
//  
// // convenience type checkers
// j.is_null();
// j.is_boolean();
// j.is_number();
// j.is_object();
// j.is_array();
// j.is_string();

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

