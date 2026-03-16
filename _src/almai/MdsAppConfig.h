#pragma once

//----------------------------------------------------------------------------
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
#include "ListingInfo.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig : public AppConfigBase
{

    std::vector<std::string>                         inputFiles;

    bool listOnly  = false; // list files, but don't save them
    std::size_t listLines = 5;




}; // struct AppConfig

