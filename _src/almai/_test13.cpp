/*! \file
    \brief Тест запуска chrome, и получения списка доступных страниц - тестируем almai::ProjectDirs
 */


/*
LSP - тоже JSON-RPC - надо оттуда всё HTTP-шное вытащить в отдельную либу. Или не надо?

Порт для подключения генерировать примерно аналогично имени временных папок - 
использовать хэш std::hash по имени проекта, а дальше его редюсить до заданного диапазона.

https://chat.deepseek.com/share/hipbevscouw97pijpm


*/


#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
#include "marty_cdt/utils.h"
#include "marty_cdt/JsonListResponse.h"
//
#include <iostream>
#include <stdexcept>
//
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXHttpClient.h>


using std::cout;
using std::cerr;
using namespace almai;


//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.findProjectRoot())
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    projectDirs.generateBrowserDirs(chromeName, false /* !useTempDir */);
    projectDirs.generateProjectConnectionPort( /* int startPort=9000, int range=1000 */ );
    auto httpBaseUrl = projectDirs.generateConnectionBaseUrlString(true /* http */);
    auto jsonListUrl = httpBaseUrl + "/json/list";

    cout << "projectPath        : " << projectDirs.projectPath << "\n";
    cout << "almaiDir           : " << projectDirs.almaiDir    << "\n";
    cout << "projectFile        : " << projectDirs.projectFile      << "\n";
    cout << "browserUserDataDir : " << projectDirs.browserUserDataDir  << "\n";
    cout << "browserCacheDataDir: " << projectDirs.browserCacheDataDir << "\n";
    cout << "httpBaseUrl        : " << httpBaseUrl << "\n";
    cout << "jsonListUrl        : " << jsonListUrl << "\n";

    ix::initNetSystem();

    ix::HttpClient httpClient;
    auto args = httpClient.createRequest(jsonListUrl);
    args->connectTimeout  = 2; // 2 секунды на подключение
    args->transferTimeout = 5; // 5 секунды на получение данных

    auto response = httpClient.get(args->url, args);

    // chrome://settings/onStartup

    cout << "response->errorCode : " << marty::cdt::utils::ixHttpErrorCodeToString(response->errorCode) << " (" << (int)response->errorCode << ")" << "\n";
    cout << "response->errorMsg  : " << response->errorMsg   << "\n";
    cout << "response->statusCode: " << response->statusCode << "\n";

    if ( response->errorCode!=ix::HttpErrorCode::Ok            // 0
      && response->errorCode!=ix::HttpErrorCode::CannotConnect // 1
      && response->errorCode!=ix::HttpErrorCode::Timeout       // 2
       )
    {
        cout << "Something goes wrong" << "\n";
        return 1;
    }


    if (response->errorCode!=ix::HttpErrorCode::Ok)
    {
        // args->connectTimeout  = 2; 
        args->transferTimeout = 10; // даём время прочухаться

        std::vector<std::string> foundExes;
        umba::shellapi::findExecutable(chromeName, foundExes);

        std::vector<std::string> spawnArgs = projectDirs.generateArgsForSpawnChrome();

        cout << "\n" << "Args: \n";
        for(auto a: spawnArgs)
            cout << "  " << a << "\n";


        for(auto chromeExeFullName : foundExes)
        {

            auto spawnRes = umba::shellapi::spawnProcess( chromeExeFullName
                                                        , spawnArgs
                                                        , umba::shellapi::SpawnProcessFlags::default_ // | umba::shellapi::SpawnProcessFlags::argvFromCmd
                                                        );
            if (spawnRes==std::uintptr_t(-1))
                continue;

            for(auto n=0; n!=10; ++n)
            {
                umba::sleepMs(1000);

                response = httpClient.get(args->url, args);
    
                if (response->errorCode==ix::HttpErrorCode::Ok)
                    break;
            }
        
        }
    }

    if (response->errorCode!=ix::HttpErrorCode::Ok)
    {
        cout << "Can't run chrome" << "\n";
        return 1;
    }



    cout << "Chrome is running" << "\n";

    cout << "Response:\n" << response->body << "\n";

    try
    {
        auto j = nlohmann::json::parse(response->body);
        std::vector<marty::cdt::JsonListResponseEntry> list;
        from_json(j, list);
        cout << "\n" << "Total " << list.size() << " items" << "\n";

    }
    catch(const std::exception &e)
    {
        cout << "Error parsing JSON: " << e.what() << "\n";
    }
    catch(...)
    {
        cout << "Error: " << "unknown error" << "\n";
    }

    //void from_json(const json& j, CommonPreset& p)
    // JsonListResponse
    // marty::cdt::

    //response->statusCode

    return 0;
}


