/*! \file
    \brief Тестируем переименование файла
 */

#include "utils.h"
#include "Preprompt.h"
//
#include "marty_cpp/src_normalization.h"
#include "umba/filesys.h"
#include "umba/shellapi.h"
#include "umba/debug_helpers.h"

//

#include <iostream>

using std::cout;
using std::cerr;



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::string oldName, newName;

    if (argc>2)
    {
        oldName = argv[1];
        newName = argv[2];
    }

    if (umba::isDebuggerPresent())
    {
        // std::string cwd;
        // std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        // std::cout << "App Root Path: " << rootPath << "\n";
        // std::cout << "Working Dir  : " << cwd << "\n";
        //  
        // filename = rootPath + "/tests/test06";
    }

    if (oldName.empty())
    {
        cerr << "No ExistingFileName taken\n";
        return 1;
    }

    if (newName.empty())
    {
        cerr << "No NewFileName taken\n";
        return 1;
    }

    if (umba::filesys::moveFileOrDir(oldName, newName))
    {
        cerr << "OK: " << oldName << " -> " << newName << "\n";
    }
    else
    {
        cerr << "Failed to move (rename): " << oldName << " -> " << newName << "\n";
        return 1;
    }

    return 0;
}


