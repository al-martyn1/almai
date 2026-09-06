/*! \file
    \brief Тест запуска chrome, и получения списка доступных страниц - основная часть кода ушла в библиотеку
 */

#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
#include "marty_cdt/utils.h"
#include "marty_cdt/JsonListResponse.h"
#include "marty_cdt/JsonVersionResponse.h"

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


inline
int returnReportFailedToConnect(ix::HttpErrorCode c, const std::string &endPoint, int retCode=1)
{
    std::cout << "Failed to connect to '" << endPoint << "' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(c) << " (" << (int)c << ")" << "\n";
    return retCode;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#define CATCH_PARSE_RESPONSE()                              \
                                                            \
    catch(const std::exception &e)                          \
    {                                                       \
        cout << "Error parsing JSON: " << e.what() << "\n"; \
    }                                                       \
    catch(...)                                              \
    {                                                       \
        cout << "Error: " << "unknown error" << "\n";       \
    }





//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    ix::initNetSystem();

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.initAll(chromeName, false /* !useTempDir */ ))
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    marty::cdt::Timeouts timeouts; // default {1,1}
    auto newInstanceConnectTimeouts = timeouts.getCopy().setConnectTimeout(1).setTransferTimeout(3);

    std::string httpBaseUrl = projectDirs.generateConnectionBaseUrlString(true /* http */);

    cout << "### Run&Connect" << "\n";

    auto spawnArgs = projectDirs.generateArgsForSpawnChrome();
    cout << "Chrome args:\n";
    for(auto a: spawnArgs)
        cout << "  " << a << "\n";

    auto response = marty::cdt::utils::runConnectAndGetJsonList( httpBaseUrl
                                                              , chromeName
                                                              , spawnArgs
                                                              , timeouts
                                                              , newInstanceConnectTimeouts
                                                              );

    if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
    {
        cout << "Failed to connect to browser" << "\n";
        return 1;
    }

    cout << "HTTP code: " << response->statusCode << "\n";
    cout << "Chrome is running" << "\n";

    cout << "Response:\n" << response->body << "\n";

    std::string defaultNewTabId;

    try
    {
        auto j = nlohmann::json::parse(response->body);
        std::vector<marty::cdt::JsonListResponseEntry> list;
        from_json(j, list);
        cout << "\n" << "Total " << list.size() << " items" << "\n";

        for(const auto &e: list)
        {
            if (e.type=="page" && e.url=="chrome://newtab/")
                defaultNewTabId = e.id;
        }

    }
    CATCH_PARSE_RESPONSE()



    cout << "\n\n";
    cout << "### /json/version" << "\n";
    response = marty::cdt::utils::httpGetJsonVersion(httpBaseUrl, timeouts); // httpGet(request);

    if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
    {
        // cout << "Failed to connect to '/json/version' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(response->errorCode) << " (" << (int)response->errorCode << ")" << "\n";
        return returnReportFailedToConnect(response->errorCode, "/json/version", 1);
    }

    cout << "HTTP code: " << response->statusCode << "\n";
    cout << "Response:\n" << response->body << "\n";

    try
    {
        auto j = nlohmann::json::parse(response->body);
        marty::cdt::JsonVersionResponse versionResponse;
        from_json(j, versionResponse);
        //cout << "\n" << "Total " << list.size() << " items" << "\n";

    }
    CATCH_PARSE_RESPONSE()


    cout << "\n\n";
    cout << "### /json/new" << "\n";
    response = marty::cdt::utils::httpPutJsonNewPage(httpBaseUrl, "https://яндекс.рф", timeouts);

    if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
    {
        return returnReportFailedToConnect(response->errorCode, "/json/new", 1);
    }

    cout << "HTTP code: " << response->statusCode << "\n";
    cout << "Response:\n" << response->body << "\n";

    std::string newPageId;
    std::string newPageWsUrl;

    try
    {
        auto j = nlohmann::json::parse(response->body);
        //std::vector<marty::cdt::JsonListResponseEntry> list;
        marty::cdt::JsonNewPageResponse newPageResponse;
        from_json(j, newPageResponse);

        newPageId    = newPageResponse.id;
        newPageWsUrl = newPageResponse.webSocketDebuggerUrl;
    }
    CATCH_PARSE_RESPONSE()


    if (!defaultNewTabId.empty())
    {
        cout << "\n\n";
        cout << "### /json/close/${id}" << "\n";
        response = marty::cdt::utils::httpClosePage(httpBaseUrl, defaultNewTabId, timeouts); // httpGet(request);
        // httpClosePage(const std::string &httpBaseUrl, const std::string &pageId, Timeouts timeouts)

        if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
        {
            return returnReportFailedToConnect(response->errorCode, "/json/close/${id}", 1);
        }
    
        cout << "HTTP code: " << response->statusCode << "\n";
        cout << "Response:\n" << response->body << "\n";

    }

    ix::WebSocket webSocket;

    // webSocket.setOnBeforeHandshakeCallback([](ix::WebSocketHttpHeaders& headers)
    //                                       {
    //                                           std::cout << "=== Заголовки WebSocket-запроса ===" << std::endl;
    //                                           for (const auto& [key, value] : headers) {
    //                                               std::cout << key << ": " << value << std::endl;
    //                                           }
    //                                           std::cout << "==================================" << std::endl;
    //                                           // Можно оставить headers без изменений или добавить/исправить
    //                                       });


    auto wsConnectRes = marty::cdt::utils::wsConnect( webSocket, newPageWsUrl
                                                    , [&](const ix::WebSocketMessagePtr& pMsg)
                                                      {
                                                          UMBA_USED(pMsg);

                                                          std::cout << "\n";

                                                          std::cout << "WS Callback\n";
                                                          std::cout << "Type    : " << marty::cdt::utils::ixWebSocketMessageTypeToString(pMsg->type) << "\n";
                                                          // pMsg->str
                                                          std::cout << "WireSize: " << pMsg->wireSize << "\n";
                                                          std::cout << "Binary  : " << (pMsg->binary ? "true" : "false") << "\n";
                                                          
                                                          if (pMsg->type==ix::WebSocketMessageType::Message)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Open)
                                                          {
                                                              // WebSocketOpenInfo openInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Close)
                                                          {
                                                              // WebSocketCloseInfo closeInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Error)
                                                          {
                                                              // WebSocketErrorInfo errorInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Ping)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Pong)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Fragment)
                                                          {
                                                          }
                                                          else // unknown pMsg->type
                                                          {
                                                          }

                                                          if (!pMsg->binary)
                                                          {
                                                              cout << "Response text: [" << pMsg->str << "]" << "\n";
                                                          }

                                                          std::cout << "\n";

                                                      }
                                                    );

    // bool success;
    // int http_status;
    // std::string errorStr;
    // WebSocketHttpHeaders headers;
    // std::string uri;
    // std::string protocol;

    if (!wsConnectRes.success)
    {
        cout << "WS NOT connected" << "\n";
        cout << "http_status: " << wsConnectRes.http_status << "\n";
        cout << "errorStr   : " << wsConnectRes.errorStr    << "\n";

        return 1;
    }
    else
    {
        cout << "WS connected" << "\n";
    }

    // for(auto i=0; i!=30; ++i)
    //     webSocket.poll(100);


    // std::mutex dataMutex;
    // std::queue<json> messageQueue;
    //  
    // webSocket.setOnMessageCallback([&](const ix::WebSocketMessagePtr& msg) {
    //     std::lock_guard<std::mutex> lock(dataMutex);
    //     // Обработка сообщения
    //     messageQueue.push(json::parse(msg->str));
    // });
    //  
    // webSocket.start();


    webSocket.start();

    for(auto i=0; i!=20; ++i)
    {
        umba::sleepMs(250);
    }

    webSocket.stop();

    umba::sleepMs(100);

    return 0;
}


