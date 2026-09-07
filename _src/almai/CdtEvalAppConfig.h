#pragma once

//----------------------------------------------------------------------------
#include "umba/umba.h"
//
// #include "AppConfigBase.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//
//#include "ListingInfo.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig
{

    bool            overwrite         = false;
    bool            quet              = false;

    std::string     inputFile;
    // std::vector<std::string>                         inputFiles;
    // std::string                                      descriptionFilename = "__description__.md";
    // bool                                             keepFilenameComment = true;

}; // struct AppConfig

