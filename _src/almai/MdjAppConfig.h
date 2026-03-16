#pragma once

//----------------------------------------------------------------------------
#include "enums.h"
#include "utils.h"
#include "FileSystemScanInfo.h"
#include "FoundFileInfo.h"
//
#include "umba/umba.h"
//
#include "AppConfigBase.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//

#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig : public AppConfigBase
{
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


}; // struct AppConfig

