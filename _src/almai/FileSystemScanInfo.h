/*! \file
    \brief Информация об указании сканирования файловой системы
 */
#pragma once

//----------------------------------------------------------------------------
#include "utils.h"
#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/filesys_scanners.h"
#include "umba/string.h"

//
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct FileSystemScanInfo
{
    std::string                  path;
    std::vector<std::string>     includeMaskList;
    std::vector<std::string>     excludeMaskList;

    bool                         recursive = false;


protected:

    struct LogDummy
    {
        template<typename T>
        LogDummy& operator<<(const T &t)
        {
            UMBA_USED(t);
            return *this;
        }
    };

    static
    bool isMask(const std::string &name)
    {
        static std::string maskChars = "*?";
        return name.find_first_of(maskChars)!=name.npos;
    }

    static
    bool isRecurseMarker(const std::string &name)
    {
        return name=="**";
    }


public:

    static
    FileSystemScanInfo parse( const std::string &fullName
                            , const std::string &rootPath = umba::filesys::internal::getCurrentDirectory<std::string>()
                            )
    {
        using namespace umba::filename;
        using namespace umba::filesys;
        using namespace umba::string;

        FileSystemScanInfo scanInfo;

        scanInfo.path = getPath(fullName);
        auto name = getFileName(fullName);

        if (isRecurseMarker(getFileName(scanInfo.path)))
        {
            scanInfo.recursive = true;
            scanInfo.path = getPath(scanInfo.path);
        }

        // Если масок нет, то у нас точное имя файла, и оно может содержать и запятые, и минус в начале
        // Его надо обработать без разбора на запчасти

        if (!isMask(name))
        {
            scanInfo.includeMaskList.emplace_back(name);
        }
        else
        {
            std::vector<std::string> maskList = splitPathList(name,',');
    
            for(auto &mask : maskList)
            {
                trim(mask);
    
                if (mask.empty())
                    continue;
    
                std::vector<std::string> *pList = &scanInfo.includeMaskList;
                if (mask[0]=='-')
                {
                    pList = &scanInfo.excludeMaskList;
                    mask.erase(0,1);
                    trim(mask);
                }
    
                if (mask.empty())
                    continue;
    
                pList->emplace_back(mask);
            }
    
            if (scanInfo.includeMaskList.empty())
                scanInfo.includeMaskList.emplace_back("*.*");
        }

        scanInfo.path = makeAbsPath(scanInfo.path, rootPath);
        scanInfo.path = makeCanonical(scanInfo.path);

        return scanInfo;
    }

    void scanForFiles(std::vector<std::string> &foundFiles) const
    {
        using namespace umba::filesys::scanners;

        LogDummy log;

        std::vector<std::string>    excludedFiles;
        std::set<std::string>       foundExtentions;
        std::vector<std::string>    excludeFoldersExact;


        umba::filesys::scanners::scanFolders( std::vector<std::string>(1, path) // rootScanPaths
                                            , includeMaskList
                                            , excludeMaskList
                                            , log
                                            , foundFiles
                                            , excludedFiles
                                            , foundExtentions
                                            , (std::vector<std::string>*)0 // pFoundFilesRootFolders
                                            , excludeFoldersExact
                                            , recursive
                                            , false // logFoundHeader
                                            , false // addFolders
                                            );
    }

    std::string toString() const
    {
        auto resStr = std::string(1, recursive ? 'R' : 'T'); // Recursive | Exact
        resStr.append(1, ' ');

        resStr.append(path);

        resStr.append(4, ' ');

        for(auto &&m : includeMaskList)
        {
            if (resStr.back()!=' ')
                resStr.append(1, ' ');
            resStr.append(m);
        }

        if (!excludeMaskList.empty())
        {
            resStr.append(4, ' ');
            resStr.append("X ");

            for(auto &&m : excludeMaskList)
            {
                if (resStr.back()!=' ')
                    resStr.append(1, ' ');
                resStr.append(m);
            }
        }

        return resStr;
    }


}; // struct FileSystemScanInfo

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai


