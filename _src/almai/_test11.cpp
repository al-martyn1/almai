/*! \file
    \brief Тест регистрации EXE без добавления в PATH
 */

#include "utils.h"
//
#include "umba/shellapi.h"
#include "umba/win32_utils.h"
#include "umba/cbp/utils.h"
//
#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
void testRegisterExe(const std::string &exeName)
{
    if (umba::shellapi::regExecutable(exeName))
        std::cout << "Exe registered: " << exeName << "\n";
    else
        std::cout << "Exe registration failed: " << exeName << "\n";
}

//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    //std::string exeFullPathName = "D:/MarkdownMonster/MarkdownMonster.exe";

    std::string path = "D:/MarkdownMonster";
    if (argc>1)
        path = argv[1];


    testRegisterExe(umba::filename::appendPath(path, "MarkdownMonster.exe"));
    testRegisterExe(umba::filename::appendPath(path, "mm.exe"));
    testRegisterExe(umba::filename::appendPath(path, "mmcli.exe"));

    return 0;
}


