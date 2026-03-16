/*!
    \file
    \brief Базовый конфиг
 */

//----------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------
#include "umba/umba.h"
#include "encoding/encoding.h"
//
#include "marty_cpp/src_normalization.h"
//
#include "umba/string.h"
//
#include "utils.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfigBase
{

    bool            overwrite = false;

    //------------------------------
    using ELinefeedType = marty_cpp::ELinefeedType;

    std::string     output;
    ELinefeedType   outputLinefeedType = ELinefeedType::systemDefault;


    void checkUpdateOutputDir()
    {
        if (output.empty())
        {
            output = umba::filesys::getCurrentDirectory();
        }
    
        output = umba::filename::makeAbsPath(output);
    }

    void checkUpdateOutput()
    {
        if (!output.empty())
        {
            // output = umba::filesys::getCurrentDirectory();
            output = umba::filename::makeAbsPath(output);
        }
    }


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

    bool writeFile(const std::string &filename, const std::string &filedata, std::string &fullName) const
    {
        fullName = umba::filename::makeAbsPath(filename, output);

        auto dir = umba::filename::getPath(fullName);
        if (!dir.empty())
            umba::filesys::createDirectoryEx(dir, true /* forceCreatePath */ );

        return umba::filesys::writeFile(fullName, filedata, overwrite);
    }

    bool writeFile(const std::string &filename, const std::string &filedata) const
    {
        std::string fullName;
        return writeFile(filename, filedata, fullName);
    }

    bool writeFile(const std::string &filename, const std::vector<std::string> &lines, std::string &fullName) const
    {
        bool addTrailingNewLine = true;
        if (lines.empty())
        {
            addTrailingNewLine = false;
        }
        else
        {
            if (lines.back().empty())
                addTrailingNewLine = false;
        }

        std::string text = marty_cpp::mergeLines(lines, outputLinefeedType,  /* true */ addTrailingNewLine);
        return writeFile(filename, text, fullName);
    }

    bool writeFile(const std::string &filename, const std::vector<std::string> &lines) const
    {
        std::string fullName;
        return writeFile(filename, lines, fullName);
    }


    //------------------------------

    std::unordered_map<std::string, std::string>     langExtDict;
    std::unordered_map<std::string, std::string>     extLangDict;


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


        umba::string::tolower(langExtention);

        extLangDict[langExtention] = langName;


        umba::string::tolower(langName);

        langExtDict[langName] = langExtention;

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

    std::string findLangExtention(std::string langName) const
    {
        umba::string::trim(langName);
        umba::string::tolower(langName);

        std::unordered_map<std::string, std::string>::const_iterator it = langExtDict.find(langName);
        if (it==langExtDict.end())
        {
            return almai::replaceInvalidFileNameChars(langName, true /* replaceSpaceAlso */ );
        }

        return it->second;
    }

    std::string findExtentionLang(std::string ext) const
    {
        umba::string::trim(ext);
        umba::string::tolower(ext);

        std::unordered_map<std::string, std::string>::const_iterator it = extLangDict.find(ext);
        if (it==extLangDict.end())
        {
            return ext;
        }

        return it->second;
    }




}; // struct AppConfigBase


