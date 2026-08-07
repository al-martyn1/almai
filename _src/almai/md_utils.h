/*! \file
    \brief Утилитарные функции проекта
 */
#pragma once

#include "enums.h"
#include "almai_enums.h"
#include "log.h"
//
#include "umba/umba.h"
#include "umba/parse_utils.h"
#include "umba/filename.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
#include "umba/filename.h"
#include "umba/filesys.h"

#if defined(WIN32) || defined(_WIN32)
    #include "umba/clipboard_win32.h"
#endif

//
#include "marty_cpp/src_normalization.h"
//
#include "marty_yaml_toml_json/marty_yaml_toml_json.h"

//
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
extern unsigned lineNo;
extern std::string curFile;


//----------------------------------------------------------------------------
namespace md {

//----------------------------------------------------------------------------
using almai::MdLineType;
using almai::FenceStyle;

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
std::string unescapeMarkdownString(const std::string &str)
{
    std::string resStr; resStr.reserve(str.size());

    bool prevSlash = false;
    for(auto ch: str)
    {
        if (prevSlash)
        {
            resStr.append(1, ch);
            prevSlash = false;
        }
        else
        {
            if (ch=='\\')
                prevSlash = true;
            else
                resStr.append(1, ch);
        }
    }

    return resStr;
}

//----------------------------------------------------------------------------
// Возвращает имя файла, очищенное от коментариев тупого болванчика
inline
std::string tryRemoveCommentsFromFilename(std::string name, std::string *pCommentStr=0)
{
    auto ext = umba::filename::getExt(name);

    auto bracePos = ext.rfind('('); // тупой болванчик обычно заключает свои коментарии в круглые скобки
    auto spacePos = ext.rfind(' '); // но если нет скобок, то болванчик просто отделил свой коментарий пробелом - ну не может же он лепить без пробела же
    // Так или иначе, но в расширениях пробел недопустим (но болванчик может прилепить в скобках без пробела)

    if (bracePos==ext.npos && spacePos==ext.npos) // ничего не нашлось
    {
        if (pCommentStr)
            pCommentStr->clear();
        return name;
    }

    std::size_t extCommentStartPos = std::size_t(-1);

    if (bracePos!=ext.npos && spacePos!=ext.npos)
    {
        // найдены оба
        extCommentStartPos = spacePos<bracePos ? spacePos : bracePos;
    }
    else
    {
        // нашлось только одно, выбираем найденное
        extCommentStartPos = spacePos!=ext.npos ? spacePos : bracePos;
    }

    // if (extCommentStartPos==std::size_t(-1)) // на всякий случай
    // {
    //     if (pCommentStr)
    //         pCommentStr->clear();
    //     return name;
    // }

    auto commentStr = std::string(ext, extCommentStartPos); // от найденной позиции в расширении имени файла до конца

    auto commentPos = name.rfind(commentStr);
    if (commentPos==name.npos)
    {
        if (pCommentStr)
            pCommentStr->clear();
        return name;
    }

    auto commentText = std::string(name, commentPos);
    umba::string::trim(commentText);

    name.erase(commentPos);
    umba::string::trim(name);

    if (pCommentStr)
        *pCommentStr = commentText;

    return name;
}

//----------------------------------------------------------------------------
inline
std::size_t stripFilenameEdging( std::string &name
                               , std::string *pEdging=0 // Возвращает индекс обрамления, если было или std::size_t(-1)
                               , bool bCommentPrependName=true // поместить коментарий в начало имени файла, если коментарий есть
                               ) 
{
    umba::string::trim(name);

    const auto& edgings = getAllowedFilenameEdgings();

    std::string commentString;

    auto prepentComment = [&](const auto &str) -> std::string
    {
        if (!bCommentPrependName)
           return str;

        if (commentString.empty())
           return str;

        auto p = umba::filename::getPath(str);
        auto n = umba::filename::getFileName(str);

        return umba::filename::appendPath(p, commentString + " " + n);
    };

    for(auto i=0u; i!=edgings.size(); ++i)
    {
        const auto &e = edgings[i];
        if (e.empty())
            continue;

        //if (umba::string::starts_with(name, e) && umba::string::ends_with(name, e))
        if (umba::string::starts_with(name, e))
        {
            // Если короткий, то должно заканчиваться строго на него же, и сразу и вырезаем, если заканчивается
            if (e.size()<2 && umba::string::ends_with_and_strip(name, e)) 
            {
                umba::string::starts_with_and_strip(name, e);
                umba::string::trim(name);
                name = tryRemoveCommentsFromFilename(name, &commentString);
                name = unescapeMarkdownString(name);
                name = prepentComment(name);
                if (pEdging)
                   *pEdging = e;
                return i;
            }

            // ищем последнее вхождение эджинга
            auto lastEdgingPos = name.rfind(e);
            if (lastEdgingPos!=name.npos && lastEdgingPos!=0) // Позиция найдена, и она не нулевая - не позиция стартового эджинга
            {
                name.erase(lastEdgingPos); // до конца
                umba::string::starts_with_and_strip(name, e);
                umba::string::trim(name);
                name = tryRemoveCommentsFromFilename(name, &commentString);
                name = unescapeMarkdownString(name);
                name = prepentComment(name);
                if (pEdging)
                   *pEdging = e;
                return i;
            }

            // umba::string::starts_with_and_strip(name, e);
            // umba::string::ends_with_and_strip(name, e);
            // umba::string::trim(name);
            // return i;
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
                         , bool                      bCommentPrependName
                         , std::vector<std::string> *pResNames          = 0
                         , std::vector<std::string> *pRemainedTextLines = 0
                         , size_t                   *pNumEdgings        = 0
                         , size_t                   *pFoundEdgingType   = 0
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
    // if (pResNames)
    //     pResNames->clear();

    std::vector<std::string> resNames;

    if (b==e)
        return false;


    std::vector<std::size_t> edgings;

    for(auto it=b; it!=e; ++it)
    {
        std::string str = *it;
        edgings.emplace_back(stripFilenameEdging(str, 0, bCommentPrependName));
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

    if (pRemainedTextLines)
    {
        pRemainedTextLines->insert( pRemainedTextLines->end(), std::next(resNames.begin(), std::ptrdiff_t(numEqualEdgings)), resNames.end() );
        std::reverse(pRemainedTextLines->begin(), pRemainedTextLines->end());
    }

    resNames.erase( std::next(resNames.begin(), std::ptrdiff_t(numEqualEdgings)), resNames.end() );
    // Реверсим обратно
    std::reverse(resNames.begin(), resNames.end());

    if (pFoundEdgingType)
       *pFoundEdgingType = edgingType;

    if (pNumEdgings)
       *pNumEdgings = numEqualEdgings;

    if (pResNames)
       *pResNames = resNames;

    return edgingType!=std::size_t(-1);
}

//----------------------------------------------------------------------------
inline
bool findListingFilenames( std::vector<std::string>  textLines
                         , bool                      bCommentPrependName
                         , std::vector<std::string> *pResNames          = 0
                         , std::vector<std::string> *pRemainedTextLines = 0
                         , size_t                   *pNumEdgings        = 0
                         , size_t                   *pFoundEdgingType   = 0
                         ) 
{
    return findListingFilenames(textLines.begin(), textLines.end(), bCommentPrependName, pResNames, pRemainedTextLines, pNumEdgings, pFoundEdgingType);
}

//----------------------------------------------------------------------------
inline
std::size_t getNumberOfFirstSameChars(const std::string &str)
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
inline
MdLineType detectMarkdownLineType(const std::string &str, char *pChar=0, std::size_t *pNumChars=0)
{
    if (str.empty() || umba::string::trim_copy(str).empty())
        return MdLineType::emptyLine;
        
    std::size_t nChars = getNumberOfFirstSameChars(str);

    if (pNumChars)
       *pNumChars = nChars;

    char ch = str[0];

    if (pChar)
       *pChar = ch;

    if (ch==' ' && nChars<str.size())
    {
        if (str[nChars]=='-')
            return MdLineType::unorderedList;


        if (umba::string::is_digit(str[nChars]))
        {
            while(nChars<str.size() && umba::string::is_digit(str[nChars]))
                ++nChars;
        }

        if (nChars<str.size() && ch=='.')
            return MdLineType::orderedList;
    }


    switch(ch)
    {
        case '#' : return MdLineType::headerAtx;
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
    MdLineType mdLineType = detectMarkdownLineType(line, &markerChar, &markerLen);
    if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
    {
        line.erase(0, markerLen);
        umba::string::trim(line);
        return line;
    }

    return std::string();
}

//----------------------------------------------------------------------------
inline
std::string generateFence(FenceStyle fenseStyle, const std::vector<std::string> &fileLines)
{
    std::unordered_set<std::size_t> foundBackticks;
    std::unordered_set<std::size_t> foundTildes;

    for(const auto &l : fileLines)
    {
        char mdChar = 0;
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

    return std::string(fenceLen, fenseStyle==FenceStyle::backticks ? '`' : '~');

}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::vector<std::string> stripEmptyLeadingLines(const std::vector<std::string> &lines)
{
    std::vector<std::string> resLines; resLines.reserve(lines.size());
    for(auto l : lines)
    {
        umba::string::rtrim(l);

        if (!l.empty())
        {
            resLines.emplace_back(l);
            continue;
        }

        if (resLines.empty())
            continue;

        resLines.emplace_back(l);
    }

    return resLines;
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> stripEmptyTrailingLines(std::vector<std::string> lines)
{
    std::reverse(lines.begin(), lines.end());
    lines = stripEmptyLeadingLines(lines);
    std::reverse(lines.begin(), lines.end());
    return lines;
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> stripEmptyLeadingTrailingLines(std::vector<std::string> lines)
{
    return stripEmptyLeadingLines(stripEmptyTrailingLines(lines));
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct ListingInfo
{
    std::vector<std::string>  listingFilenames;
    std::vector<std::string>  listingCodeLines;
    std::string               foundLangName   ;

    void checkAddExtention(std::string ext)
    {
        static std::string seps = "./";

        if (ext.empty())
            ext = "txt";

        for(auto &name : listingFilenames)
        {
            auto pos = name.find_last_of(seps);

            if (pos==name.npos || name[pos]=='/')
            {
                name += ".";
                name += ext;
            }
        }
    }

    bool checkAutoEnumerate(std::unordered_map<std::string, std::size_t> &filenameCounters)
    {
        bool foundCollisions = false;

        for(auto &name : listingFilenames)
        {
            auto lowerCaseName = umba::string::tolower_copy(name);
            std::size_t counter = filenameCounters[lowerCaseName];
            if (!counter)
            {
                filenameCounters[lowerCaseName] = 1;
            }
            else
            {
                foundCollisions = true;
                name = insertCounterIntoFilename(name, counter);
                ++filenameCounters[lowerCaseName];
            }
        }

        return foundCollisions;
    }


}; // struct ListingInfo

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct MdArchivePart
{
    std::vector<std::string> allText  ; // Текст с листингами
    std::vector<std::string> textLines; // Только текст, без листингов
    std::vector<ListingInfo> listings ;

    //std::size_t              aitosaveCounter;

    void clear()
    {
        allText  .clear();
        textLines.clear();
        listings .clear();
    }

    bool empty() const
    {
        return allText.empty() && textLines.empty() && listings.empty();
    }

}; // struct MdArchivePart
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
inline
bool splitMarkdownArchive( std::vector<MdArchivePart>     &mdArchiveParts
                         , const std::vector<std::string> &mdLines
                         , bool        keepFilenameComment
                         , std::size_t partSeparatorLen=0 // по умолчанию - не используем разделение на части, считаем весь файл md-архивом
                         )
{
    std::vector<std::string> lastSignificantLines;
    std::size_t lastSeparatorLineNo = 0;

    bool readingCode = false;
    char codeMarkerChar = 0;
    std::size_t codeMarkerLen = 0;
    std::string codeLang;
    std::vector<std::string> codeLines;
    MdArchivePart archiveCurPart;

    if (partSeparatorLen!=0 && partSeparatorLen<3) // Если задан, то не менее трёх символов
        partSeparatorLen = 3;

    auto appendLastSignificanLinesToText = [&]()
    {
        lastSignificantLines = stripEmptyLeadingTrailingLines(lastSignificantLines);
        if (!archiveCurPart.textLines.empty() && !lastSignificantLines.empty())
        {
            archiveCurPart.textLines.emplace_back();
        }

        archiveCurPart.textLines.insert(archiveCurPart.textLines.end(), lastSignificantLines.begin(), lastSignificantLines.end());
    };
    // lastSignificantLines
    // textLines

    almai::MdLineType listingType = almai::MdLineType::emptyLine;

    lineNo = 0;
    for(const auto &line : mdLines)
    {
        lineNo++;

        char markerChar = 0;
        std::size_t markerLen = 0;

        MdLineType mdLineType = detectMarkdownLineType(line, &markerChar, &markerLen);

        if (readingCode)
        {
            bool unexpectedEndOfListing = false;

            if (listingType==MdLineType::codeTilda || listingType==MdLineType::codeBacktick)
            {
                // ИИ может глючить (не хватать токенов), и он может продалбываться
                // Иногда он может добавить маркер конца листинга в конец строки
                // потеряв часть содержимого листинга.
                // Надо просекать такие ситуации

                // !!! Не нужно. Просто был кривоватый документ, в нем были листинги на питоне, и внутри них были 
                // маркдаун маркеры блоков кода

                // auto blockMarker = std::string(codeMarkerLen, codeMarkerChar);
                //  
                // if (line!=blockMarker && umba::string::ends_with(line, blockMarker))
                // {
                //     unexpectedEndOfListing = true;
                //     LOG_WARN_INPUT("unexp-code-block-end") << "found unexpected code block end\n";
                // }
                
            }

            // listingType = mdLineType;
            if (listingType==MdLineType::codeIndentTab || listingType==MdLineType::codeIndentSpace)
            {
                // Идёт чтение листинга с отступами

                if (listingType==mdLineType) // тип листинга совпадает с типом текущейф строки
                {
                    // Продолжается чтение листинга с отступом
                    codeLines.push_back(line);
                }
                else // текущая строка не совпадает с типом листинга
                {
                    // Может начаться backtick или tilda листинг
                    if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
                    {
                        listingType = mdLineType;
                        readingCode = true;
                        codeMarkerChar = markerChar;
                        codeMarkerLen  = markerLen ;
                        codeLines.clear();
                        codeLang = extractCodeLangFromFencedCodeBlockMarker(line);
                    }
                    else
                    {
                        // листинг с отступом закончился
                        readingCode = false;
                        appendLastSignificanLinesToText();
                        lastSignificantLines.clear();
                        codeLines.clear();
                    }
                }
            }

            else if ( unexpectedEndOfListing // был обнаружен неожиданнй конец листинга?
                  || ((listingType==MdLineType::codeTilda || listingType==MdLineType::codeBacktick) // у нас текущий маркер листинга бэктик или тильда?
                    && listingType==mdLineType && codeMarkerChar==markerChar && codeMarkerLen==markerLen // текущий маркер листинга совпадает с типом текущей строки?
                     )
                    )
            {
                if (codeLang.empty())
                {
                    codeLang = extractCodeLangFromFencedCodeBlockMarker(line);
                }

                // Остальная обработка финализации листинга
                ListingInfo listingInfo;
                listingInfo.listingCodeLines = codeLines;
                listingInfo.foundLangName    = codeLang;

                std::vector<std::string> filenames;
                std::vector<std::string> remLines;
                bool hasEdging = findListingFilenames(lastSignificantLines, keepFilenameComment, &filenames, &remLines);
// bool findListingFilenames( std::vector<std::string>  textLines
//                          , bool                      bCommentPrependName
//                          , std::vector<std::string> *pResNames          = 0
//                          , std::vector<std::string> *pRemainedTextLines = 0
//                          , size_t                   *pNumEdgings        = 0
//                          , size_t                   *pFoundEdgingType   = 0
//                          ) 


                for(auto &name : filenames)
                {
                    name = replaceInvalidPathNameChars(name, !hasEdging);
                    name = makeNormalizedRelativePath(name);
                    listingInfo.listingFilenames.emplace_back(name);
                }

                //foundListings.emplace_back(listingInfo);
                archiveCurPart.listings.emplace_back(listingInfo);
    
                readingCode = false;
                lastSignificantLines = remLines;
                appendLastSignificanLinesToText();
                lastSignificantLines.clear();
                codeLines.clear();
            }
            else
            {
                // продолжаем чтение листинга
                codeLines.push_back(line);
            }
        }

        else // обычный режим
        {
            if ( mdLineType==MdLineType::headerSetext ) // '-'/'='
            {
                appendLastSignificanLinesToText();
                lastSignificantLines.clear();

                // Если обнаружен разделитель --- или === (или более длинный)
                if (partSeparatorLen!=0 && markerLen>=partSeparatorLen)
                {
                    //foundListings.clear(); // найденное ранее очищаем, листинги md-архива только в последней части

                    if (lastSeparatorLineNo!=lineNo) // Фрагмент текста не пустой
                    {
                        auto itB = std::next(mdLines.begin(), std::ptrdiff_t(lastSeparatorLineNo));
                        auto itE = std::next(mdLines.begin(), std::ptrdiff_t(lineNo-1));
                        archiveCurPart.allText = stripEmptyLeadingTrailingLines(std::vector<std::string>(itB, itE));
                    }
                    lastSeparatorLineNo = lineNo; // Запоминаем последний сепаратор

                    if (!archiveCurPart.empty())
                    {
                        archiveCurPart.textLines = stripEmptyLeadingTrailingLines(archiveCurPart.textLines);
                        mdArchiveParts.push_back(archiveCurPart);
                        archiveCurPart.clear();
                    }

                }
            }

            else if ( mdLineType==MdLineType::emptyLine)
            {
                // std::vector<std::string> filenames;
                // std::vector<std::string> remLines;
                size_t numEdgings        = 0;
                size_t foundEdgingType   = 0;

                //bool hasEdging = 
                findListingFilenames(lastSignificantLines, keepFilenameComment, 0, 0, &numEdgings, &foundEdgingType);

                if (foundEdgingType==(std::size_t)-1) // имен файлов не найдено
                {
                    appendLastSignificanLinesToText();
                    archiveCurPart.textLines.push_back(line);
                    lastSignificantLines.clear();
                }

// bool findListingFilenames( std::vector<std::string>  textLines
//                          , bool                      bCommentPrependName
//                          , std::vector<std::string> *pResNames          = 0
//                          , std::vector<std::string> *pRemainedTextLines = 0
//                          , size_t                   *pNumEdgings        = 0
//                          , size_t                   *pFoundEdgingType   = 0
//                          ) 
            }

            else if ( mdLineType==MdLineType::headerAtx
                   || mdLineType==MdLineType::quotation
                    )
            {
                appendLastSignificanLinesToText();
                archiveCurPart.textLines.push_back(line);
                lastSignificantLines.clear();
            }

            else if (mdLineType==MdLineType::regularLine)
            {
                lastSignificantLines.push_back(line);
            }

            else if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
            {
                listingType = mdLineType;
                readingCode = true;
                codeMarkerChar = markerChar;
                codeMarkerLen  = markerLen ;
                codeLines.clear();
                codeLang = extractCodeLangFromFencedCodeBlockMarker(line);
            }

            else if (mdLineType==MdLineType::codeIndentTab || mdLineType==MdLineType::codeIndentSpace)
            {
                LOG_WARN_INPUT("unsup-code-block-type") << (mdLineType==MdLineType::codeIndentTab ? "tab" : "space") << " indent code blocks not supported\n";
                listingType = mdLineType;
                readingCode = true;
                codeMarkerChar = markerChar;
                codeMarkerLen  = markerLen ;
                codeLines.clear();
                codeLang.clear();
                codeLines.push_back(line);
            }

            else if (mdLineType==MdLineType::unorderedList || mdLineType==MdLineType::orderedList)
            {
                //lastSignificantLines.push_back(line);
                appendLastSignificanLinesToText();
                archiveCurPart.textLines.push_back(line);
                lastSignificantLines.clear();
            }

            else
            {
                LOG_WARN_INPUT("unk-line-type") << "unknown line type found, mdLineType: " << (unsigned)mdLineType << ", line: '" << line << "'\n";
                appendLastSignificanLinesToText();
                archiveCurPart.textLines.push_back(line);
                lastSignificantLines.clear();
            }

        }

    }

    if (lastSeparatorLineNo!=lineNo) // Фрагмент текста не пустой
    {
        auto itB = std::next(mdLines.begin(), std::ptrdiff_t(lastSeparatorLineNo));
        auto itE = std::next(mdLines.begin(), std::ptrdiff_t(lineNo-1));
        archiveCurPart.allText = stripEmptyLeadingTrailingLines(std::vector<std::string>(itB, itE));
    }
    //lastSeparatorLineNo = lineNo; // Запоминаем последний сепаратор

    archiveCurPart.textLines = stripEmptyLeadingTrailingLines(archiveCurPart.textLines);

    if (!archiveCurPart.empty())
    {
        mdArchiveParts.push_back(archiveCurPart);
        archiveCurPart.clear();
    }

    return true;
}


//----------------------------------------------------------------------------

} // namespace md

// md::

//----------------------------------------------------------------------------


