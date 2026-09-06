/*! \file
    \brief Утилитарные функции проекта
 */
#pragma once

#include "enums.h"
#include "almai_enums.h"
#include "PluralDatabase.h"
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
// almai::utils::
namespace almai {
namespace utils {

//----------------------------------------------------------------------------







//----------------------------------------------------------------------------
inline
std::string formatFileSize(std::size_t sz, bool useDecimal=true)
{
     std::stringstream oss;
     std::size_t divider = useDecimal ? 1000u : 1024u;

     if (sz < 100u*divider) // меньше 100 килобайт выводим в байтах
     {
         oss << sz /*  << " bytes" */ ;
         return oss.str();
     }

     sz /= divider; // Теперь у нас килобайты

     if (sz < 10u*divider) // меньше 10*1000 килобайт выводим в килобайтах
     {
         oss << sz << " K";
         return oss.str();
     }

     sz /= divider; // Теперь у нас мегабайты

     if (sz < 10u*divider)
     {
         oss << sz << " M";
         return oss.str();
     }

     sz /= divider;

     oss << sz << " G";
     return oss.str();
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::string autoEncodeToUtf(const std::string &text)
{
    size_t bomSize = 0;
    //const charDataPtr =
    encoding::EncodingsApi* pEncodingsApi = encoding::getEncodingsApi();
    std::string detectRes = pEncodingsApi->detect( text.data(), text.size(), bomSize );
    auto cpId = pEncodingsApi->getCodePageByName(detectRes);
    std::string utfText = pEncodingsApi->convert( text.data()+bomSize, text.size()-bomSize, cpId, encoding::EncodingsApi::cpid_UTF8 );
    return utfText;
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> splitTextToLines(const std::string &text)
{
    return marty_cpp::splitToLinesSimple(text, false /*!addEmptyLineAfterLastLf*/);
}

//----------------------------------------------------------------------------
inline
bool readFile(const std::string &inputFilename, std::string &inputFileText)
{
    std::string inputFileTextOrg;

    if (!umba::filesys::readFile(inputFilename, inputFileTextOrg))
    {
        return false;
    }

    inputFileText = autoEncodeToUtf(inputFileTextOrg);
    inputFileText = marty_cpp::normalizeCrLfToLf(inputFileText);

    return true;
}

//----------------------------------------------------------------------------
inline
bool readFile(const std::string &inputFilename, std::vector<std::string> &inputFileLines)
{
    std::string text;
    if (!readFile(inputFilename, text))
         return false;

    inputFileLines = splitTextToLines(text);

    // Убираем мусорные пробелы в концах строк
    for(auto &l : inputFileLines)
        umba::string::rtrim(l);

    return true;
}

//----------------------------------------------------------------------------
inline
std::string normalizePrepromptId(const PluralDatabase &pluralDb, std::string prepromptId)
{
    umba::string::trim(prepromptId);
    umba::string::tolower(prepromptId);

    std::string category, name;
    if (!umba::parse_utils::optionStringSplitToPair(prepromptId, category, name, "/\\"))
    {
        // Нет разделителя
        return prepromptId;
    }

    category = pluralDb.findPlural(category);

    return category + "/" + name;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::vector<std::string> simpleReplaceClipboardMarkerLine(const std::vector<std::string> &lines)
{
    std::vector<std::string> resLines; resLines.reserve(lines.size());

    bool clipboardInserted = false;
    for(const auto &line : lines)
    {
        auto lineLower = umba::string::tolower_copy(umba::string::ltrim_copy(line));
        if (lineLower=="<clipboard>" || lineLower=="<clipboard/>" || lineLower=="<clipbrd>" || lineLower=="<clipbrd/>")
        {
            if (!clipboardInserted)
            {
                clipboardInserted = true;

                #if defined(WIN32) || defined(_WIN32)

                std::wstring clpbText;
                if (umba::win32::clipboardTextGet(clpbText))
                {
                    auto clpbTextUtf = umba::toUtf8(clpbText);
                    auto clpbLines   = marty_cpp::splitToLinesSimple(clpbTextUtf);
                    resLines.insert(resLines.end(), clpbLines.begin(), clpbLines.end());
                }

                #endif

            }
        }

        else if (lineLower=="<clipboard-code>" || lineLower=="<clipboard-code/>" || lineLower=="<clipbrd-code>" || lineLower=="<clipbrd-code/>")
        {
            if (!clipboardInserted)
            {
                clipboardInserted = true;

                #if defined(WIN32) || defined(_WIN32)

                std::wstring clpbText;
                if (umba::win32::clipboardTextGet(clpbText))
                {
                    auto clpbTextUtf = umba::toUtf8(clpbText);
                    auto clpbLines   = marty_cpp::splitToLinesSimple(clpbTextUtf);
                    resLines.push_back("~~~Text");
                    resLines.insert(resLines.end(), clpbLines.begin(), clpbLines.end());
                    resLines.push_back("~~~");
                }

                #endif

            }
        }

        else
        {
            resLines.push_back(line);
        }
    }

    return resLines;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::vector<std::string> makeAlmaiFolderNames()
{
    return std::vector<std::string>{ ".almai"
                                   , ".ALMAI"
                                   // , ".almai.yaml"
                                   // , ".ALMAI.yaml"
                                   // , ".ALMAI.YAML"
                                   // , ".almai.YAML"
                                   // ,  "almai.yaml"
                                   // ,  "ALMAI.yaml"
                                   // ,  "ALMAI.YAML"
                                   // ,  "almai.YAML"
                                   };
}

//--------------------------------------------------------------------------------------------------------------------
inline
bool isPathExistOneOf(const std::string &basePath, const std::vector<std::string> &relNames)
{
    for(auto &&rn : relNames)
    {
        auto fullName = umba::filename::makeAbsPath(rn, basePath);
        if (umba::filesys::isPathExist(fullName))
            return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
inline
bool isPathExistOneOf(const std::string &basePath, const std::unordered_set<std::string> &relNames)
{
    return isPathExistOneOf(basePath, std::vector<std::string>(relNames.begin(), relNames.end()));
}

//--------------------------------------------------------------------------------------------------------------------
inline
const std::vector<std::string>& getAlmaiFolderNames()
{
    static std::vector<std::string> names = makeAlmaiFolderNames();
    return names;
}

//--------------------------------------------------------------------------------------------------------------------
inline
std::vector<std::string> getAlmaiFolderFullNames(const std::string &path, const std::vector<std::string> &almaiNames=getAlmaiFolderNames())
{
    //const auto &almaiNames = getAlmaiFolderNames();

    std::vector<std::string> resVec; resVec.reserve(almaiNames.size());

    for(const auto &name : almaiNames)
    {
        resVec.push_back(umba::filename::appendPath(path, name));
    }

    return resVec;
}

//--------------------------------------------------------------------------------------------------------------------
inline
bool isProjectRootPath( const std::string &path
                      , std::string *pAlmaiFolderName
                      , const std::unordered_set<std::string> &projectRootStopNames = { ".git", ".vscode", ".build", "build", ".out", "out" }
                      )
{
    auto almaiNames = getAlmaiFolderFullNames(path);

    for(const auto &almaiFolder : almaiNames)
    {
        if (umba::filesys::isDirExist(almaiFolder))
        {
            if (pAlmaiFolderName)
               *pAlmaiFolderName = almaiFolder;
            return true;
        }
    }

    if (pAlmaiFolderName)
       pAlmaiFolderName->clear();

    if (isPathExistOneOf(path, projectRootStopNames))
        return true;

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
inline
bool findProjectRoot( std::string startPath
                    , std::string *pFoundProjectPath=0
                    , std::string *pFoundAlmaiDir=0
                    , std::string *pProjectFile=0
                    , const std::unordered_set<std::string> &projectRootStopNames = { ".git", ".vscode", ".build", "build", ".out", "out" }
                    )
{
    std::string projectFile;
    std::string almaiDir;

    auto upLevelPath = startPath;
    auto curPath     = startPath;
    do
    {
        curPath = upLevelPath;

        if (isProjectRootPath(curPath, &almaiDir, projectRootStopNames))
        {
            projectFile = umba::filename::appendPath(almaiDir, std::string("PROJECT.yaml"));
            if (!umba::filesys::isFileExist(projectFile))
                projectFile.clear();

            if (pProjectFile)
               *pProjectFile = projectFile;

            if (pFoundProjectPath)
               *pFoundProjectPath = curPath;

            if (pFoundAlmaiDir)
               *pFoundAlmaiDir = almaiDir;

            return true;
        }

        upLevelPath = umba::filename::getPath(curPath);
    }
    while(upLevelPath!=curPath);

    return false;
}



//----------------------------------------------------------------------------

} // namespace utils
} // namespace almai

// almai::utils::

//----------------------------------------------------------------------------


