/*!
    \file
    \brief Тест задания путей поиска
 */

#include "utils.h"
#include "FileSystemScanInfo.h"
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
    }

    std::vector<std::string> foundFiles;
    for(const auto &s : scanInfos)
    {
        std::vector<std::string> tmp;
        s.scanForFiles(tmp);
        foundFiles.insert(foundFiles.end(), tmp.begin(), tmp.end());
    }

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
        }
    }


    return 0;
}

