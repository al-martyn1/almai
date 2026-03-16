/*! \file
    \brief Утилитарные функции проекта
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
inline
const char* getInvalidPathNameChars()
{
    static char chars[] = 
    { '?', '*', '|', '<', '>', '\"'
    , 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10
    , 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
    };

    return &chars[0];
}

//----------------------------------------------------------------------------
inline
const char* getInvalidFileNameChars()
{
    static char chars[] = 
    { '?', '*', '|', '<', '>', '\"', '/', '\\', ':'
    , 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10
    , 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
    };

    return &chars[0];
}

//----------------------------------------------------------------------------
inline
const char* getPossiblePathNameChars()
{
    static char chars[] = 
    { '.', '/', '\\'
    , 0x00
    };

    return &chars[0];
}

//----------------------------------------------------------------------------
inline
bool isValidPathName(const std::string &pathName)
{
    auto pos = pathName.find_first_of(getInvalidPathNameChars());
    return pos==pathName.npos; // Если недопустимых символов не найдено, то это хорошо
}

//----------------------------------------------------------------------------
inline
bool isValidPathPart(const std::string &pathName)
{
    auto pos = pathName.find_first_of(getInvalidFileNameChars());
    return pos==pathName.npos; // Если недопустимых символов не найдено, то это хорошо
}

//----------------------------------------------------------------------------
inline
bool isPossiblePath(const std::string &pathName)
{
    auto pos = pathName.find_first_of(getPossiblePathNameChars());
    return pos!=pathName.npos; // Если подходящих символов не найдено, то это хорошо
}

//----------------------------------------------------------------------------
inline
std::string normalizePathSeps(std::string str)
{
    for(auto &ch : str)
    {
        if (ch=='\\')
            ch = '/';
    }

    return str;
}

//----------------------------------------------------------------------------
inline
std::string stripLeadingChar(const std::string &name, char ch)
{
    char s[] = { ch, 0 };
    auto pos = name.find_first_not_of(s);
    return pos==name.npos ? std::string() : std::string(name, pos, name.size()-pos);
}

//----------------------------------------------------------------------------
inline
std::string stripLeadingPathSeps(const std::string &name)
{
    return stripLeadingChar(name, '/');
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> splitNormalizedPath(const std::string &p)
{
    return umba::string::split(p, '/', true /* skipEmpty */ );
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> reducePath(const std::vector<std::string> &pathParts)
{
    std::vector<std::string> res;

    for(const auto &part : pathParts)
    {
        if (part==".")
            continue;

        if (part=="..")
        {
            if (!res.empty())
                res.pop_back();
            continue;
        }

        res.push_back(part);
    }

    return res;
}

//----------------------------------------------------------------------------
inline
std::string mergePath(const std::vector<std::string> &pathParts)
{
     return umba::string::merge<std::string>(pathParts.begin(), pathParts.end(), std::string("/"), [](const auto &s){ return s; });
}

//----------------------------------------------------------------------------
inline
std::string makeNormalizedRelativePath(std::string name)
{
    name = normalizePathSeps(name);

    using umba::string::starts_with_and_strip;
    // umba::string::starts_with_and_strip(name, std::string("//?/UNC/")); // "\\?\UNC\"
    starts_with_and_strip(name, std::string("//?/"));
    name = stripLeadingChar(name, '~');
    name = stripLeadingChar(name, '/');

    auto pos = name.find_first_of("/:");
    if (pos!=name.npos && name[pos]==':')
    {
        pos = name.find_first_of("/");
        if (pos==name.npos) // У нас тут путь вида C:
            name.clear();
        else
            name.erase(0, pos);
    }
    
    name = stripLeadingChar(name, '/');

    auto 
    parts = splitNormalizedPath(name);
    parts = reducePath(parts);
    name  = mergePath(parts);

    return name;
}

//----------------------------------------------------------------------------
inline
std::string replaceInvalidFileNameChars(std::string name, bool replaceSpaceAlso)
{
    auto pos = name.find_first_of(getInvalidFileNameChars());
    while(pos!=name.npos)
    {
        name[pos] = '_';
        pos = name.find_first_of(getInvalidFileNameChars(), pos+1);
    }

    if (replaceSpaceAlso)
    {
        pos = name.find(' ');
        while(pos!=name.npos)
        {
            name[pos] = '_';
            pos = name.find(' ', pos+1);
        }
    }

    return name;

}

//----------------------------------------------------------------------------
inline
std::string replaceInvalidPathNameChars(std::string name, bool replaceSpaceAlso)
{
    auto pos = name.find_first_of(getInvalidPathNameChars());
    while(pos!=name.npos)
    {
        name[pos] = '_';
        pos = name.find_first_of(getInvalidPathNameChars(), pos+1);
    }

    if (replaceSpaceAlso)
    {
        pos = name.find(' ');
        while(pos!=name.npos)
        {
            name[pos] = '_';
            pos = name.find(' ', pos+1);
        }
    }

    return name;

}

//----------------------------------------------------------------------------
inline
std::string insertCounterIntoFilename(std::string name, std::size_t cnt, std::size_t numDigits=3)
{
    //std::ostringstream oss;
    auto cntStr = std::to_string(cnt);
    if (cntStr.size()<numDigits)
    {
        cntStr = std::string(numDigits-cntStr.size(), '0') + cntStr;
    }

    cntStr = "_" + cntStr;

    static std::string seps = "./";
    auto pos = name.find_last_of(seps);
    if (pos!=name.npos && name[pos]=='.')
        name.insert(pos, cntStr);
    else
        name += cntStr;

    return name;
}

    // void checkAddExtention(std::string ext)
    // {
    //     static std::string seps = "./";
    //  
    //     if (ext.empty())
    //         ext = "txt";
    //  
    //     for(auto &name : listingFilenames)
    //     {
    //         auto pos = name.find_last_of(seps);
    //         if (pos!=name.npos && name[pos]=='/')
    //             continue;
    //  
    //         if (pos==name.npos)
    //             name += ".";
    //  
    //         name += ext;
    //     }
    // }


//----------------------------------------------------------------------------
inline
const std::vector<std::string>& getAllowedFilenameEdgings()
{
    static std::vector<std::string> edgings = 
    { "**" // жирный
    , "__" // жирный
    , "~~" // зачеркнутый
    , "*"  // курсив
    , "_"  // курсив
    , "\"" // 
    , "\'" // 
    , "`"  // 
    };

    return edgings;
}

//----------------------------------------------------------------------------
inline
std::size_t stripFilenameEdging(std::string &name) // Возвращает true, если было обрезано обрамление
{
    umba::string::trim(name);

    const auto& edgings = getAllowedFilenameEdgings();

    for(auto i=0u; i!=edgings.size(); ++i)
    {
        const auto &e = edgings[i];

        if (umba::string::starts_with(name, e) && umba::string::ends_with(name, e))
        {
            umba::string::starts_with_and_strip(name, e);
            umba::string::ends_with_and_strip(name, e);
            umba::string::trim(name);
            return i;
        }
    }

    return std::size_t(-1);
}

//----------------------------------------------------------------------------
inline
std::size_t testFilenameEdging(std::string name) // Возвращает true, если было есть обрамление
{
    umba::string::trim(name);

    const auto& edgings = getAllowedFilenameEdgings();

    for(auto i=0u; i!=edgings.size(); ++i)
    {
        const auto &e = edgings[i];

        if (umba::string::starts_with(name, e) && umba::string::ends_with(name, e))
        {
            return i;
        }
    }

    return std::size_t(-1);

}

//----------------------------------------------------------------------------
// возвращает true, если найдено обрамление, иначе false
inline
bool findListingFilenames( std::vector<std::string>::const_iterator b
                         , std::vector<std::string>::const_iterator e
                         , std::vector<std::string> &resNames
                         ) 
{
/*
    Непустые строки перед листингом надо проверить, являются ли они именами файлов.
    Получаем в прямом порядке, отсекать надо начало, так как после текста может по ошибке 
    без разрыва вставлено имя файла, а затем листинг.

    Для всех элементов получим тип эджинга. Начиная с конца, если эджинг был, выцепляем
    элементы с одинаковым эджингом.

    Если эджинга не было, выцепляем строки, которые:
    - не содержат запрещённых символов
    - похожи на имя файла (содержат точку или разделители пути)

    Если таких строк нет вообще, считаем последнюю строку не именем файла, а описанием (но это делаем на уровне выше).
*/

    resNames.clear();

    if (b==e)
        return false;


    std::vector<std::size_t> edgings;

    for(auto it=b; it!=e; ++it)
    {
        std::string str = *it;
        edgings.emplace_back(stripFilenameEdging(str));
        resNames.emplace_back(str);
    }

    // лень возится с обратным порядком, поэтому обратим порядок
    std::reverse(resNames.begin(), resNames.end());
    std::reverse(edgings.begin() , edgings.end() );

    std::size_t edgingType = edgings[0];

    // Ловим количество одинаковых обрамлений
    std::size_t numEqualEdgings = 0;
    for(; numEqualEdgings!=edgings.size(); ++numEqualEdgings)
    {
        if (edgingType!=edgings[numEqualEdgings])
            break;
    }

    if (edgingType==std::size_t(-1))
    {
        // Обрамления нет

        // Надо проверить валидность имен (если это имена)
        // и надо проверить на вероятность того, что это путь ()

        std::size_t i = 0;

        for(; i!=numEqualEdgings; ++i)
        {
            if (!isValidPathName(resNames[i]))
                break;
            if (!isPossiblePath(resNames[i]))
                break;
        }

        numEqualEdgings = i;
    }

    if (!numEqualEdgings)
        numEqualEdgings = 1;

    resNames.erase( std::next(resNames.begin(), std::ptrdiff_t(numEqualEdgings)), resNames.end() );

    // Реверсим обратно
    std::reverse(resNames.begin(), resNames.end());

    return edgingType!=std::size_t(-1);
}

//----------------------------------------------------------------------------
inline
bool findListingFilenames( std::vector<std::string> textLines
                         , std::vector<std::string> &resNames
                         ) 
{
    return findListingFilenames(textLines.begin(), textLines.end(), resNames);
}

//----------------------------------------------------------------------------
inline
std::size_t geNumberOfFirstSameChars(const std::string &str)
{
    if (str.empty())
        return 0;

    char ch = str[0];

    std::size_t i = 0;
    for(; i!=str.size(); ++i)
    {
        if (ch!=str[i])
            break;
    }

    return i;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
enum class MdLineType
{
    emptyLine   = 0,
    regularLine    ,
    headerArx      ,           // # - Atx - word processor on Amiga, min 1 char
    headerSetext   ,           // ---- / ==== Setext (Structure Enhanced Text), min 1 char
    codeTilda      ,
    codeBacktick   ,
    codeIndentTab  ,
    codeIndentSpace,
    quotation                 // >

}; // enum class MdLineType

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
MdLineType detectMarkdownLineType(const std::string &str, char *pChar=0, std::size_t *pNumChars=0)
{
    if (str.empty() || umba::string::trim_copy(str).empty())
        return MdLineType::emptyLine;
        
    std::size_t nChars = geNumberOfFirstSameChars(str);

    if (pNumChars)
       *pNumChars = nChars;

    char ch = str[0];

    if (pChar)
       *pChar = ch;

    switch(ch)
    {
        case '#' : return MdLineType::headerArx;
        case '-' : return MdLineType::headerSetext;
        case '=' : return MdLineType::headerSetext;
        case '~' : return nChars>=3 ? MdLineType::codeTilda       : MdLineType::regularLine;
        case '`' : return nChars>=3 ? MdLineType::codeBacktick    : MdLineType::regularLine;
        case '\t': return nChars>=1 ? MdLineType::codeIndentTab   : MdLineType::regularLine;
        case ' ' : return nChars>=4 ? MdLineType::codeIndentSpace : MdLineType::regularLine;
        case '>' : return nChars>=1 ? MdLineType::quotation       : MdLineType::regularLine;
        default  : return MdLineType::regularLine;
    }
}

//----------------------------------------------------------------------------
inline
std::string extractCodeLangFromFencedCodeBlockMarker(std::string line)
{
    char markerChar = 0;
    std::size_t markerLen = 0;
    MdLineType mdLineType = almai::detectMarkdownLineType(line, &markerChar, &markerLen);
    if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
    {
        line.erase(0, markerLen);
        umba::string::trim(line);
        return line;
    }

    return std::string();
}

//----------------------------------------------------------------------------
std::string generateFence(FenceStyle fenseStyle, const std::vector<std::string> &fileLines) const
{
    std::unordered_set<std::size_t> foundBackticks;
    std::unordered_set<std::size_t> foundTildes;

    for(const auto &l : fileLines)
    {
        char mdChar = 0
        std::size_t mdNum = 0;
        MdLineType mdLineType = detectMarkdownLineType(l, &mdChar, &mdNum);

        if (mdLineType==MdLineType::codeBacktick)
        {
            foundBackticks.insert(mdNum);
        }
        else if (mdLineType==MdLineType::codeTilda)
        {
            foundTildes.insert(mdNum);
        }

    } // for(const auto &l : fileLines)

    std::size_t fenceLen = 0;

    if (fenseStyle==FenceStyle::auto_)
    {
        for(std::size_t i=3; i!=(std::size_t)-1; ++i)
        {
            if (foundTildes.find(i)==foundTildes.end())
            {
                fenceLen = i;
                fenseStyle = FenceStyle::tildes;
                break;
            }
            else if (foundBackticks.find(i)==foundBackticks.end())
            {
                fenceLen = i;
                fenseStyle = FenceStyle::backticks;
                break;
            }
        }
    }
    else
    {
        const std::unordered_set<std::size_t> &foundCounters = fenseStyle==FenceStyle::backticks ? foundBackticks : foundTildes;
        for(std::size_t i=3; i!=(std::size_t)-1; ++i)
        {
            if (foundCounters.find(i)==foundCounters.end())
            {
                fenceLen = i;
                break;
            }
        }
    }

    return std::string(fenceLen fenseStyle==FenceStyle::backticks ? '`' : '~');

}



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------


