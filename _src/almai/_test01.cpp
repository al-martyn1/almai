/*!
    \file
    \brief Тесты утилитарных функций
 */

#include "utils.h"
//

#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

//----------------------------------------------------------------------------
inline 
void performMakeNormalizedRelativePathTest(const std::string &name)
{
    cout << "  [" << name << "] - [" << makeNormalizedRelativePath(name) << "]\n";
}

//----------------------------------------------------------------------------
inline 
void performMakeNormalizedRelativePathTests()
{
    cout << "**test01/makeNormalizedRelativePath.txt**\n";
    cout << "```\n";
    performMakeNormalizedRelativePathTest("C:\\Windows\\Program Files");
    performMakeNormalizedRelativePathTest("\\\\?\\C:\\Windows\\Program Files");
    performMakeNormalizedRelativePathTest("\\\\?\\share\\c$\\Windows\\Program Files");
    performMakeNormalizedRelativePathTest("/usr/share/bin");
    performMakeNormalizedRelativePathTest("~/bin");
    cout << "```\n\n";
}

//----------------------------------------------------------------------------
inline 
void performFormatFileSizeTest(std::size_t sz, bool dec)
{
    cout << sz << ": " << formatFileSize(sz, dec) << "\n";
}

//----------------------------------------------------------------------------
inline 
void performFormatFileSizeTests(bool dec)
{
    std::string decStr = dec?"dec":"bin";
    cout << "**test01/formatFileSize_" << decStr << ".txt**\n";
    cout << "```\n";
    // cout << "formatFileSize test (" << decStr << "):\n";

    std::size_t sz = 64;
    std::size_t incSz = 699;
    const std::size_t maxSize = 100*1024*1024; // Тестируем до 100 Мб размера

    while(sz<maxSize)
    {
        performFormatFileSizeTest(sz, dec);

        if (sz<12800)
            sz += incSz;
        else if (sz<128000)
            sz += incSz*9;
        else if (sz<1280000)
            sz += incSz*9*9;
        else if (sz<12800000)
            sz += incSz*9*9*9;
        else
            sz += incSz*9*9*9*9;
    }

    cout << "```\n\n";
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    performMakeNormalizedRelativePathTests();
    performFormatFileSizeTests(false);
    performFormatFileSizeTests(true );

    return 0;
}
