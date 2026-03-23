/*! \file
    \brief Локализация (заголовком и строк)
 */
#pragma once

/*
    --translation=ЯЗЫК:КЛЮЧ:ТЕКСТ
    --translation=ru:roles-title:Твоя роль
*/

//----------------------------------------------------------------------------
#include "utils.h"

//
#include "umba/string.h"

//
#include <string>
#include <unordered_map>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct LocalizationStrings
{

    std::unordered_map<std::string, std::string>   strings;


    bool addLocalization(std::string key, std::string text)
    {
        umba::string::trim(key);
        umba::string::trim(text);

        if (key.empty() || text.empty())
            return false;

        umba::string::tolower(key);

        strings[key] = text;

        return true;
    }

    bool addLocalization(const std::string &keyTextPair)
    {
        std::string f;
        std::string s;

        if (!umba::string::split_to_pair(keyTextPair, f, s, ':'))
            return false;
    
        return addLocalization(f, s);
    }


    std::string getLocalizedText(std::string key) const
    {
        umba::string::trim(key);
        umba::string::tolower(key);

        std::unordered_map<std::string, std::string>::const_iterator it = strings.find(key);
        if (it==strings.end())
            return std::string();

        return it->second;
    }


}; // struct LocalizationStrings

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct Localization
{

    std::unordered_map<std::string, LocalizationStrings> langStrings;


    bool addLocalization(std::string lang, std::string keyTextPair)
    {
        umba::string::trim(lang);
        umba::string::trim(keyTextPair);

        if ( /* lang.empty() || */ keyTextPair.empty())
            return false;

        umba::string::tolower(lang);

        return langStrings[lang].addLocalization(keyTextPair);
    }

    bool addLocalization(const std::string &langKeyTextTriplet)
    {
        std::string f;
        std::string s;

        if (!umba::string::split_to_pair(langKeyTextTriplet, f, s, ':'))
            return false;
    
        return addLocalization(f, s);
    }

    std::string getLocalizedTextImpl(std::string lang, const std::string &key) const
    {
        umba::string::trim(lang);
        umba::string::tolower(lang);

        std::unordered_map<std::string, LocalizationStrings>::const_iterator it = langStrings.find(lang);
        if (it==langStrings.end())
            return std::string();

        return it->second.getLocalizedText(key);
    }

    std::string getLocalizedText(std::string lang, const std::string &key) const
    {
        std::string 
        resText = getLocalizedTextImpl(lang, key);
        if (!resText.empty())
            return resText;

        resText = getLocalizedTextImpl("en", key);
        if (!resText.empty())
            return resText;

        resText = getLocalizedTextImpl("", key);
        if (!resText.empty())
            return resText;

        resText = key;

        if (!resText.empty() && (resText[0]>='a' && resText[0]<='z' ))
            resText[0] = resText[0] - 'a' + 'A';

        return resText;
    }


}; // struct Localization


//----------------------------------------------------------------------------


} // namespace almai


