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
void makeNormalizedRelativePathTest(const std::string &name)
{
    cout << "  [" << name << "] - [" << makeNormalizedRelativePath(name) << "]\n";
}

//----------------------------------------------------------------------------
inline 
void makeNormalizedRelativePathTests()
{
    cout << "makeNormalizedRelativePath:\n";
    makeNormalizedRelativePathTest("C:\\Windows\\Program Files");
    makeNormalizedRelativePathTest("\\\\?\\C:\\Windows\\Program Files");
    makeNormalizedRelativePathTest("\\\\?\\share\\c$\\Windows\\Program Files");
    makeNormalizedRelativePathTest("/usr/share/bin");
    makeNormalizedRelativePathTest("~/bin");
}




//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    makeNormalizedRelativePathTests();

    return 0;
}