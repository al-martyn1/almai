/*! \file
    \brief Exception testing
 */

#include "utils.h"
//
#include "umba/shellapi.h"
#include "umba/win32_utils.h"
#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/exception.h"
#include "umba/app_main.h"
//
#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

//----------------------------------------------------------------------------



int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    bool errFlag = false;
    try
    {
        auto res = unsafeMain(argc, argv);
        return res;
    }
    UMBA_EXCEPTION_CATCH_LOG_TO_STREAM(errFlag, std::cerr)

    return errFlag ? 1 : 0;
    // catch(const std::exception& e)
    // {
    //     LOG_ERR << "Error: " << e.what() << "\n";
    //     return 1;
    // }
    // catch(...)
    // {
    //     LOG_ERR << "Unknown error\n";
    //     return 2;
    // }

}



int unsafeMain(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    // std::string fileName = "C:\\Temp\\test.tmp";
    // if (argc>1)
    //     fileName = argv[1];

    throw umba::SystemException(5);

    //return 0;
}


