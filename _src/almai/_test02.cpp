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

    std::vector<FileSystemScanInfo> scanInfo;

    for(int i=1; i<argc; ++i)
    {
        scanInfo.emplace_back(FileSystemScanInfo::parse(argv[i]));
    }

    if (scanInfo.empty())
    {
        cout << "Nothing to scan\n";
    }
    else
    {
        for(auto &&s : scanInfo)
        {
            cout << s.toString() << "\n";
        }
    }

    return 0;
}

