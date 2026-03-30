/*! \file
    \brief Тест задания путей поиска
 */

#include "utils.h"
#include "FileSystemScanInfo.h"
#include "FoundFileInfo.h"
//

#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

// //----------------------------------------------------------------------------
// inline 
// void makeNormalizedRelativePathTest(const std::string &name)
// {
//     cout << "  [" << name << "] - [" << makeNormalizedRelativePath(name) << "]\n";
// }
//  
// //----------------------------------------------------------------------------
// inline 
// void makeNormalizedRelativePathTests()
// {
//     cout << "makeNormalizedRelativePath:\n";
//     makeNormalizedRelativePathTest("C:\\Windows\\Program Files");
//     makeNormalizedRelativePathTest("\\\\?\\C:\\Windows\\Program Files");
//     makeNormalizedRelativePathTest("\\\\?\\share\\c$\\Windows\\Program Files");
//     makeNormalizedRelativePathTest("/usr/share/bin");
//     makeNormalizedRelativePathTest("~/bin");
// }




//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::vector<FileSystemScanInfo> scanInfos;

    for(int i=1; i<argc; ++i)
    {
        scanInfos.emplace_back(FileSystemScanInfo::parse(argv[i]));
    }

    if (scanInfos.empty())
    {
        cout << "Nothing to scan\n";
    }
    else
    {
        cout << "Scan paths & masks:\n";
        for(auto &&s : scanInfos)
        {
            cout << "  " << s.toString() << "\n";
        }

        cout << "\n";
    }

    std::vector<std::string> foundFiles;
    for(const auto &s : scanInfos)
    {
        std::vector<std::string> tmp;
        s.scanForFiles(tmp);
        foundFiles.insert(foundFiles.end(), tmp.begin(), tmp.end());
    }


    std::vector<FoundFileInfo>  foundFileInfos;

    if (foundFiles.empty())
    {
        cout << "Files not found\n";
    }
    else
    {
        cout << "Found files:\n";
        for( auto &&f: foundFiles)
        {
            cout << "  " << f << "\n";
            foundFileInfos.emplace_back(f);
        }

        cout << "\n";

        std::string commonPrefix = findMostCommonPathPrefix(foundFileInfos.begin(), foundFileInfos.end());
        commonPrefix = checkCorrectMostCommonPathPrefixIsPath(commonPrefix);

        if (commonPrefix.empty())
        {
            cout << "Common prefix not found\n";
            cout << "\n";
        }
        else
        {
            cout << "Found Common prefix: " << commonPrefix << "\n\n";

            for(auto &ffi: foundFileInfos)
            {
                ffi.stripPrefix(commonPrefix);
            }

        }

        cout << "Found files (stripped prefix):\n";
        for(auto &ffi: foundFileInfos)
        {
            cout << "  " << ffi.displayName << "\n";
        }

        cout << "\n";

        sortFoundFileInfos(foundFileInfos);

        cout << "Found files (sorted):\n";
        for(auto &ffi: foundFileInfos)
        {
            cout << "  " << ffi.displayName << "\n";
        }

        cout << "\n";

        // sortFoundFileInfos
    }


    return 0;
}

