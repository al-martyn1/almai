#pragma once

//----------------------------------------------------------------------------
#include "umba/umba.h"
#include "encoding/encoding.h"
//
#include "umba/string.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "utils.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig
{

    std::vector<std::string>                         inputFiles;
    std::string                                      outputDir;
    std::unordered_map<std::string, std::string>     langExtentionDict;

    bool overwrite = false;
    bool listOnly  = false; // list files, but don't save them
    std::size_t listLines = 5;


    static
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

    static
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

    static
    bool readFile(const std::string &inputFilename, std::vector<std::string> &inputFileLines)
    {
        std::string text;
        if (!readFile(inputFilename, text))
             return false;

        inputFileLines = marty_cpp::splitToLinesSimple(text);

        return true;
    }

    bool writeFile(const std::string &filename, const std::string &filedata) const
    {
        return umba::filesys::writeFile(filename, filedata, overwrite);
    }

    bool writeFile(const std::string &filename, const std::vector<std::string> &lines) const
    {
        bool addTrailingNewLine = true;
        if (lines.empty() || !lines.back().empty())
            addTrailingNewLine = false;

        std::string text = marty_cpp::mergeLines(lines, marty_cpp::ELinefeedType::systemDefault,  /* true */ addTrailingNewLine);
        return writeFile(filename, text);
    }


    bool addLangExtention(std::string langName, std::string langExtention)
    {
        umba::string::trim(langName);
        if (langName.empty())
            return false;

        umba::string::trim(langExtention);
        if (langExtention.empty())
            return false;

        if (langExtention.front()=='.')
            langExtention.erase(0,1);

        umba::string::trim(langExtention);
        if (langExtention.empty())
            return false;

        umba::string::tolower(langName);

        langExtentionDict[langName] = langExtention;

        return true;
    }

    bool addLangExtention(const std::string &langNameExtention)
    {
        auto pos = langNameExtention.find_first_of(" :");
        if (pos==langNameExtention.npos)
            return false;

        auto ext  = std::string(langNameExtention, 0, pos);
        auto lang = std::string(langNameExtention, pos+1 );

        return addLangExtention(lang, ext);
    }

    std::string findLangExtention(std::string &langName)
    {
        umba::string::trim(langName);
        umba::string::tolower(langName);

        std::unordered_map<std::string, std::string>::const_iterator it = langExtentionDict.find(langName);
        if (it==langExtentionDict.end())
        {
            return almai::replaceInvalidFileNameChars(langName, true /* replaceSpaceAlso */ );
        }

        return it->second;
    }


// StringType mergeLines(const std::vector<StringType> &v, ELinefeedType lfType, bool addTrailingNewLine=false)
// {
//     StringType resText; resText.reserve(v.size()*16u);
//  
//     StringType lfStr;
//  
//     switch(lfType)
//     {
//         case ELinefeedType::lf             :  lfStr = make_string<StringType>("\n"  ); break;
//         case ELinefeedType::cr             :  lfStr = make_string<StringType>("\r"  ); break;
//         case ELinefeedType::lfcr           :  lfStr = make_string<StringType>("\n\r"); break;
//         case ELinefeedType::crlf           :  lfStr = make_string<StringType>("\r\n"); break;
//         case ELinefeedType::linefeedRemove :  lfStr = make_string<StringType>(""    ); break;




}; // struct AppConfig

