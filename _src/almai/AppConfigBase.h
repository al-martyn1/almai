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
#include "umba/filename.h"
#include "umba/filesys.h"
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfigBase
{
    using ELinefeedType = marty_cpp::ELinefeedType;

    std::string     outputDir;
    bool            overwrite = false;
    ELinefeedType   outputLinefeedType = ELinefeedType::systemDefault;


    void checkUpdateOutputDir()
    {
        if (outputDir.empty())
        {
            outputDir = umba::filesys::getCurrentDirectory();
        }
    
        outputDir = umba::filename::makeAbsPath(outputDir);
        // if (!umba::filename::isAbsPath(outputDir))
        // {
        //     outputDir = umba::filename::appendPath(appConfig.outputDir /*, cwd*/);
        // }
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
        fullName = umba::filename::makeAbsPath(filename, outputDir);

        auto dir = umba::filename::getPath(fullName);
        if (!dir.empty())
            umba::filesys::createDirectoryEx(dir, true /* forceCreatePath */ );

        return umba::filesys::writeFile(filename, filedata, overwrite);
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


}; // struct AppConfigBase


