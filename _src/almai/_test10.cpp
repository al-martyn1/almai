/*! \file
    \brief Тесты запуска сторонних программ (без ожидания)
 */

#include "utils.h"
//
#include "umba/shellapi.h"
//
#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::string errMsg;
    // chrome.exe --remote-debugging-port=9222 --user-data-dir=C:\temp\chrome-profile
    // C:\Users\martinov\AppData\Local\Google\Chrome\Application\chrome.exe
    int res = umba::shellapi::spawnProcess( "C:\\Users\\martinov\\AppData\\Local\\Google\\Chrome\\Application\\chrome.exe" // "chrome.exe"
                                          , std::vector<std::string>{ "chrome.exe"
                                                                    , "--new-window" // "--new-tab"
                                                                    , "--remote-debugging-port=9222"
                                                                    , "--user-data-dir=C:\\temp\\test-chrome-profile"
                                                                    , "--disk-cache-dir=C:\\temp\\test-chrome-cache"
                                                                    }
                                          , umba::shellapi::SpawnProcessFlags::useExactExeName | umba::shellapi::SpawnProcessFlags::argsExeAlready
                                          , &errMsg
                                          );
    if (res==-1)
        cout << "Error: " << errMsg << "\n";
    else
        cout << "Chrome PID: " << res << "\n";

    return 0;
}
