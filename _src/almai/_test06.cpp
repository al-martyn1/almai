/*! \file
    \brief Тестируем сканирование препромптов (пока только в props и только в одном каталоге)
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

    std::string filename;

    if (argc>1)
        filename = argv[1];

    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        filename = rootPath + "/tests/test06";
    }

    if (filename.empty())
    {
        cerr << "No input path name taken\n";
        return 1;
    }

    auto ppVec = almai::PrepromptProps::scanPath(filename, "skills");

    cout << "Scan path: " << filename << "\n";
    cout << ppVec << "\n";
    if (ppVec.empty())
        cout << "Nothing was found\n";

    return 0;
}


