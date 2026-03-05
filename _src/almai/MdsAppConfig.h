#pragma once


#include "umba/umba.h"
//
#include "umba/string_plus.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>


struct AppConfig
{

    std::unordered_map<std::string, std::string>     langExtentionDict;

    bool addLangExtention(std::string &langName, std::string &langExtention)
    {
        umba::string_plus::trim(langName);
        if (langName.empty())
            return false;

        umba::string_plus::trim(langExtention);
        if (langExtention.empty())
            return false;

        umba::string_plus::tolower(langName);

        langExtentionDict[langName] = langExtention;

        return true;
    }


}; // struct AppConfig

