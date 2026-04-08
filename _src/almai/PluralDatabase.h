/*! \file
    \brief Утилитарные функции для преобразования ед. число <-> мн. число
 */
#pragma once

#include "enums.h"
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
struct PluralDatabase
{
    std::unordered_map<std::string, std::string> s2p;
    std::unordered_map<std::string, std::string> p2s;

    // --add-plural-pair

    bool addWordForms(std::string singular, std::string plural)
    {
        umba::string::trim(singular);
        umba::string::trim(plural);

        if (singular.empty() || plural.empty())
            return false;

        umba::string::tolower(singular);
        umba::string::tolower(plural);
        s2p[singular] = plural;
        p2s[plural] = singular;

        return true;
    }

    bool addWordForms(const std::string &singularPluralPair)
    {
        std::string s;
        std::string p;

        if (!umba::string::split_to_pair(singularPluralPair, s, p, ':'))
            return false;
    
        return addWordForms(s, p);
    }

    std::string findPlural(std::string word) const
    {
        umba::string::trim(word);
        if (word.empty())
            return word;

        umba::string::tolower(word);

        std::unordered_map<std::string, std::string>::const_iterator it = s2p.find(word);
        if (it!=s2p.end())
            return it->second; // нашлось отображение слова singular -> plural, возвращаем plural

        it = p2s.find(word);
        if (it!=p2s.end())
            return it->first; // нашлось отображение слова plural -> singular, возвращаем plural

        // Теперь простое правило - если есть 's' на конце - это множественное число, возвращаем его
        // Если нет - добавляем 's' и возвращаем

        if (word.back()!='s')
            word.append(1, 's');
        
        return word;
    }

    std::string findSingular(std::string word) const
    {
        umba::string::trim(word);
        if (word.empty())
            return word;

        umba::string::tolower(word);

        std::unordered_map<std::string, std::string>::const_iterator it = p2s.find(word);
        if (it!=p2s.end())
            return it->second; // нашлось отображение слова plural -> singular, возвращаем singular

        it = s2p.find(word);
        if (it!=s2p.end())
            return it->first; // нашлось отображение слова singular -> plural, возвращаем singular

        // Теперь простое правило - если есть 's' на конце - это множественное число, обрезаем 's' и получаем единственное

        if (word.back()=='s')
            word.erase(word.size()-1, 1);
        
        return word;
    }

}; // struct PluralDatabase

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------


