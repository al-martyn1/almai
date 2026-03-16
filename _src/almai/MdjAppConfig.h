#pragma once

//----------------------------------------------------------------------------
#include "enum.h"
#include "utils.h"

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
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig : public AppConfigBase
{
    std::vector<FileSystemScanInfo>      scanInfos;
    std::vector<FoundFileInfo>           foundFileInfos;
    std::vector<std::string>             stripPrefixes;


    almai::FilenameDecorationType        filenameDecorationType  = almai::FilenameDecorationType::text;
    almai::FilenameDecorationStyle       filenameDecorationStyle = almai::FilenameDecorationStyle::bold;
    std::size_t                          filenameTitleLevel = 6;
    almai::ComparisonType                sortCriteria = ComparisonType::nameExt;
    almai::SortOrder                     sortOrder    = almai::SortOrder::ascending;
    almai::FenceStyle                    fenceStyle   = FenceStyle::auto_;
    almai::CodeLanguageMarker            langMarker   = almai::CodeLanguageMarker::langName;

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
        auto ext = umba::filename::getExt(displayFileName);

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
            case FilenameDecorationStyle::quot:             return "\"";
            case FilenameDecorationStyle::apos:             return "\'";
            case FilenameDecorationStyle::bold:             return "**";
            case FilenameDecorationStyle::underscoreBold:   return "__";
            case FilenameDecorationStyle::italic:           return "*";
            case FilenameDecorationStyle::underscoreItalic: return "_";
            case FilenameDecorationStyle::strikeout:        return "~~";
            case FilenameDecorationStyle::none:             return "";
            case FilenameDecorationStyle::invalid:          return "";
        }
    }

    std::string makeFilenameTextDecorated(const std::string &fileName) const
    {
        auto decoration = getFilenameTextDecoration();
        return decoration + fileName + decoration;
    }


    void generateListing(std::ostream &oss, const std::string &displayFileName, const std::vector<std::string> &fileLines) const
    {

        almai::FilenameDecorationType        filenameDecorationType  = almai::FilenameDecorationType::text;
        almai::FilenameDecorationStyle       filenameDecorationStyle = almai::FilenameDecorationStyle::bold;
        almai::FenceStyle                    fenceStyle   = FenceStyle::auto_;
        almai::CodeLanguageMarker            langMarker   = almai::CodeLanguageMarker::langName;

    }

// std::string generateFence(FenceStyle fenseStyle, const std::vector<std::string> &fileLines) const

// enum class CodeLanguageMarker : std::uint32_t
// {
//     none       = 0x0000 /*!<  */,
//     langName   = 0x0001 /*!<  */,
//     fileExt    = 0x0002 /*!<  */
//  
// }; // enum 

// enum class FenceStyle : std::uint32_t
// {
//     auto_       = 0x0000 /*!<  */,
//     backticks   = 0x0001 /*!<  */,
//     tildes      = 0x0002 /*!<  */,
//  
// }; // enum 

// enum class FilenameDecorationStyle : std::uint32_t
// {
//     none               = 0x0000 /*!<  */,
//     quot               = 0x0001 /*!< use quotation mark */,
//     apos               = 0x0002 /*!< use apos mark */,
//     bold               = 0x0003 /*!< bold using ** */,
//     underscoreBold     = 0x0004 /*!< bold using __ */,
//     strikeout          = 0x0005 /*!< ~~ */,
//     italic             = 0x0006 /*!< italic using * */,
//     underscoreItalic   = 0x0007 /*!< italic using _ */,
//  
// }; // enum 


// enum class FilenameDecorationType : std::uint32_t
// {
//     none      = 0x0000 /*!<  */,
//     text      = 0x0001 /*!<  */,
//     title     = 0x0002 /*!<  */,
//     attr      = 0x0003 /*!<  */
//  
// }; // enum 







}; // struct AppConfig

