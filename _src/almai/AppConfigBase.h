/*!
    \file
    \brief Базовый конфиг
 */

//----------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------
#include "enums.h"
#include "utils.h"
#include "FileSystemScanInfo.h"
#include "FoundFileInfo.h"
//
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

//
#include <algorithm>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfigBase
{

    bool            overwrite = false;
    bool            quet      = false;


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
    std::vector<std::string> stripEmptyHeadLines(const std::vector<std::string> &lines)
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

    static
    std::vector<std::string> stripEmptyTailLines(std::vector<std::string> lines)
    {
        std::reverse(lines.begin(), lines.end());
        lines = stripEmptyHeadLines(lines);
        std::reverse(lines.begin(), lines.end());
        return lines;
    }

    static
    void appendLines(std::vector<std::string> &linesAppendTo, const std::vector<std::string> &lines)
    {
        linesAppendTo.insert(linesAppendTo.end(), lines.begin(), lines.end());
    }


    static
    bool readFile(const std::string &inputFilename, std::vector<std::string> &inputFileLines)
    {
        std::string text;
        if (!readFile(inputFilename, text))
             return false;

        inputFileLines = marty_cpp::splitToLinesSimple(text);

        // Убираем мусорные пробелы в концах строк
        for(auto &l : inputFileLines)
            umba::string::rtrim(l);

        return true;
    }

    // Если передавать абсолютное имя файла, то можно писать куда угодно
    // pFullName - опционально возвращаемое значение, возвращает полное имя файла, куда производилась запись (или пытались)
    bool writeFile(const std::string &filename, const std::string &filedata, std::string *pFullName=0) const
    {
        auto fullName = umba::filename::makeAbsPath(filename, output);
        if (pFullName)
           *pFullName = fullName;

        auto dir = umba::filename::getPath(fullName);
        if (!dir.empty())
            umba::filesys::createDirectoryEx(dir, true /* forceCreatePath */ );

        return umba::filesys::writeFile(fullName, filedata, overwrite);
    }

    bool writeFile(const std::string &filename, const std::vector<std::string> &lines, std::string *pFullName=0) const
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
        return writeFile(filename, text, pFullName);
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

    //------------------------------



    //------------------------------
    std::vector<almai::FileSystemScanInfo>    scanInfos;
    std::vector<almai::FoundFileInfo>         foundFileInfos;
    std::vector<std::string>                  stripPrefixes;


    almai::FilenameDecorationType             filenameDecorationType  = almai::FilenameDecorationType::text;
    almai::FilenameDecorationStyle            filenameDecorationStyle = almai::FilenameDecorationStyle::bold;
    std::size_t                               filenameTitleLevel = 6;
    almai::ComparisonType                     sortCriteria = almai::ComparisonType::nameExt;
    almai::SortOrder                          sortOrder    = almai::SortOrder::ascending;
    almai::FenceStyle                         fenceStyle   = almai::FenceStyle::auto_;
    almai::CodeLanguageMarker                 langMarker   = almai::CodeLanguageMarker::langName;

    //------------------------------

    std::string makeFilenameTitlePrefix() const
    {
        std::size_t lvl = filenameTitleLevel;
        if (lvl<1) lvl = 1;
        if (lvl>6) lvl = 6;

        return std::string(lvl, '#');
    }

    std::string makeFilenameTitle(const std::string &fileName) const
    {
        return makeFilenameTitlePrefix() + " " + fileName;
    }

    std::string makeLangMarker(const std::string &fileName) const
    {
        auto ext = umba::filename::getExt(fileName);

        if (langMarker==almai::CodeLanguageMarker::langName)
        {
            return findExtentionLang(ext);
        }
        else if (langMarker==almai::CodeLanguageMarker::fileExt)
        {
            return ext;
        }

        return std::string();
    }

    std::string getFilenameTextDecoration() const
    {
        switch(filenameDecorationStyle)
        {
            case almai::FilenameDecorationStyle::quot:             return "\"";
            case almai::FilenameDecorationStyle::apos:             return "\'";
            case almai::FilenameDecorationStyle::bold:             return "**";
            case almai::FilenameDecorationStyle::underscoreBold:   return "__";
            case almai::FilenameDecorationStyle::italic:           return "*" ;
            case almai::FilenameDecorationStyle::underscoreItalic: return "_" ;
            case almai::FilenameDecorationStyle::strikeout:        return "~~";
            case almai::FilenameDecorationStyle::none:             return ""  ;
            case almai::FilenameDecorationStyle::invalid:          return ""  ;
            default: return "";
        }
    }

    std::string makeFilenameTextDecorated(const std::string &fileName) const
    {
        auto decoration = getFilenameTextDecoration();
        return decoration + fileName + decoration;
    }


    void generateMarkdownListing(std::ostream &oss, const std::string &displayFileName, const std::vector<std::string> &fileLines) const
    {
        if (filenameDecorationType==almai::FilenameDecorationType::title)
        {
            oss << makeFilenameTitle(displayFileName) << "\n\n";
        }
        else if (filenameDecorationType==almai::FilenameDecorationType::text)
        {
            oss << makeFilenameTextDecorated(displayFileName) << "\n";
        }

        std::string fence = almai::generateFence(fenceStyle, fileLines);

        oss << fence << makeLangMarker(displayFileName);

        if (filenameDecorationType==almai::FilenameDecorationType::attr)
        {
            oss << " {filename=\"" << displayFileName << "\"}";
        }

        oss << "\n";

        // Нужно, чтобы itBegin указывал на первую не пустую строку
        // Нужно, чтобы itEnd указывал на пустую строку, следующую за последней не пустой

        std::vector<std::string>::const_iterator it = fileLines.begin();
        std::vector<std::string>::const_iterator itBegin = it;
        for(; it != fileLines.end(); ++it)
        {
            auto line = *it;
            umba::string::rtrim(line);
            if (!line.empty())
            {
                itBegin = it;
                break;
            }
        }

        it = itBegin;
        std::vector<std::string>::const_iterator itEnd = fileLines.end();
        for(; it != fileLines.end(); ++it)
        {
            auto line = *it;
            umba::string::rtrim(line);
            if (!line.empty())
            {
                itEnd = std::next(it /* , std::ptrdiff_t(1) */ );
            }
        }

        it = itBegin;
        for(; it != itEnd; ++it)
        {
            auto line = *it;
            umba::string::rtrim(line);
            oss << line << "\n";
        }

        oss << fence << "\n\n";

    }




}; // struct AppConfigBase


