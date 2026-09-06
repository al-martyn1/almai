/*! \file
    \brief Тест запуска chrome, отработка открытия страницы, события Page, Network, Runtime, DOM

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
    // cout << "Chrome args:\n";
    // for(auto a: spawnArgs)
    //     cout << "  " << a << "\n";

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
    std::string defaultNewTabPageWsUrl;

    try
    {
        auto j = nlohmann::json::parse(response->body);
        std::vector<marty::cdt::JsonListResponseEntry> list;
        from_json(j, list);
        cout << "\n" << "Total " << list.size() << " items" << "\n";

        for(const auto &e: list)
        {
            if (e.type=="page" && (e.url=="chrome://newtab/" || e.url=="chrome://newtab"))
            {
                defaultNewTabId = e.id;
                defaultNewTabPageWsUrl = e.webSocketDebuggerUrl;
            }
        }

    }
    CATCH_PARSE_RESPONSE()


    if (defaultNewTabPageWsUrl.empty())
    {
        cout << "\n\n";
        cout << "### /json/new" << "\n";
        response = marty::cdt::utils::httpPutJsonNewPage(httpBaseUrl, timeouts); // Создаём новую вкладку без задания адреса
    
        if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
        {
            return returnReportFailedToConnect(response->errorCode, "/json/new", 1);
        }
    
        cout << "HTTP code: " << response->statusCode << "\n";
        cout << "Response:\n" << response->body << "\n";
    
        try
        {
            auto j = nlohmann::json::parse(response->body);
            //std::vector<marty::cdt::JsonListResponseEntry> list;
            marty::cdt::JsonNewPageResponse newPageResponse;
            from_json(j, newPageResponse);
    
            defaultNewTabId        = newPageResponse.id;
            defaultNewTabPageWsUrl = newPageResponse.webSocketDebuggerUrl;
        }
        CATCH_PARSE_RESPONSE()
    
    }



    if (defaultNewTabId.empty())
    {
        return 1;
    }

    ix::WebSocket webSocket;

    auto wsConnectRes = marty::cdt::utils::wsConnect( webSocket, defaultNewTabPageWsUrl // newPageWsUrl
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
                                                              std::string msgBody;
                                                              try
                                                              {
                                                                  if (pMsg->str.empty())
                                                                  {
                                                                      msgBody = "<EMPTY>";
                                                                  }
                                                                  else
                                                                  {
                                                                      auto j = nlohmann::json::parse(pMsg->str);
                                                                      msgBody = j.dump(2);
                                                                  }
                                                              }
                                                              CATCH_PARSE_RESPONSE()

                                                              // cout << "Response text: [" << pMsg->str << "]" << "\n";
                                                              cout << "Response text:\n" << msgBody << "\n";
                                                          }

                                                          std::cout << "\n";

                                                      }
                                                    );

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

    webSocket.start();

    unsigned cmdId = 0;

    auto
    sendRes = marty::cdt::utils::wsSendEventSubscription(webSocket, cmdId++, "Page.enable");
    if (!sendRes.success)
        cout << "Send 'Page.enable' subscription failed" << "\n";

    sendRes = marty::cdt::utils::wsSendEventSubscription(webSocket, cmdId++, "Network.enable");
    if (!sendRes.success)
        cout << "Send 'Network.enable' subscription failed" << "\n";

    sendRes = marty::cdt::utils::wsSendEventSubscription(webSocket, cmdId++, "Runtime.enable");
    if (!sendRes.success)
        cout << "Send 'Runtime.enable' subscription failed" << "\n";

    sendRes = marty::cdt::utils::wsSendEventSubscription(webSocket, cmdId++, "DOM.enable");
    if (!sendRes.success)
        cout << "Send 'DOM.enable' subscription failed" << "\n";


    sendRes = marty::cdt::utils::wsSendCommand(webSocket, cmdId++, "Page.navigate", { {"url", "https://яндекс.рф"} });
    if (!sendRes.success)
        cout << "Send 'Page.navigate' command failed" << "\n";


    for(auto i=0; i!=20; ++i)
    {
        umba::sleepMs(250);
    }

    webSocket.stop();

    umba::sleepMs(100);

    return 0;
}


