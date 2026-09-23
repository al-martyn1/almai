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
    std::vector<std::string>     headerLines;
    std::vector<std::string>     footerLines;


    std::vector<std::string> makePrepromptHeader(std::vector<std::string> lines)
    {
        // add skills lines
        return stripEmptyHeadTailLines(lines);
    }

    std::vector<std::string> makePrepromptFooter(std::vector<std::string> lines)
    {
        // add skills lines
        return stripEmptyHeadTailLines(lines);
    }


    std::vector<std::string> makePrepromptHeader()
    {
        auto lines = almai::utils::simpleReplaceClipboardMarkerLine(headerLines);
        return makePrepromptHeader(lines);
    }

    std::vector<std::string> makePrepromptFooter()
    {
        auto lines = almai::utils::simpleReplaceClipboardMarkerLine(footerLines);
        return makePrepromptFooter(lines);
    }





}; // struct AppConfig

