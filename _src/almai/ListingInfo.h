/*! \file
    \brief Информация о листинге (сохраняемом из markdown)
 */
#pragma once

//----------------------------------------------------------------------------
#include "utils.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct ListingInfo
{
    std::vector<std::string>  listingFilenames;
    std::vector<std::string>  listingCodeLines;
    std::string               foundLangName   ;

    void checkAddExtention(std::string ext)
    {
        static std::string seps = "./";

        if (ext.empty())
            ext = "txt";

        for(auto &name : listingFilenames)
        {
            auto pos = name.find_last_of(seps);

            if (pos==name.npos || name[pos]=='/')
            {
                name += ".";
                name += ext;
            }
        }
    }

    void checkAutoEnumerate(std::unordered_map<std::string, std::size_t> &filenameCounters)
    {
        for(auto &name : listingFilenames)
        {
            auto lowerCaseName = umba::string::tolower_copy(name);
            std::size_t counter = filenameCounters[lowerCaseName];
            if (!counter)
            {
                filenameCounters[lowerCaseName] = 1;
            }
            else
            {
                name = insertCounterIntoFilename(name, counter);
                ++filenameCounters[lowerCaseName];
            }
        }

    }


}; // struct ListingInfo

//----------------------------------------------------------------------------


} // namespace almai


