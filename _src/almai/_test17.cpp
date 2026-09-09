/*! \file
    \brief Тест запуска chrome, отработка открытия страницы, события Page, Network, Runtime, DOM - используем marty::cdt::Connection - специализированные функции

 */

#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
//
#include "marty_cdt/Connection.h"
//
#include "marty_cdt/AtomicBoolIdHandler.h"
#include "marty_cdt/AtomicBoolMethodHandler.h"
//
#include "marty_cdt/Dom.h"


//
#include <iostream>
#include <stdexcept>
#include <memory>
#include <variant>
//

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
#define CATCH_PARSE_RESPONSE()                        \
                                                      \
    catch(const std::exception &e)                    \
    {                                                 \
        cout << "Error: " << e.what() << "\n";        \
    }                                                 \
    catch(...)                                        \
    {                                                 \
        cout << "Error: " << "unknown error" << "\n"; \
    }





//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);


    {
        std::vector<std::string> patterns = marty::cdt::utils::generatePatterns("page.navigation.navigate");
        cout << "Patterns:\n";
        for(const auto &p: patterns)
        {
            cout << "  " << p << "\n";
        }
    }

    ix::initNetSystem();

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.initAll(chromeName, false /* !useTempDir */ ))
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    marty::cdt::Timeouts httpProbeTimeouts   = {1,1};
    marty::cdt::Timeouts httpConnectTimeouts = {1,3};

    //std::string httpBaseUrl = projectDirs.generateConnectionBaseUrlString(true /* http */);

    auto wsConnection = std::make_shared<marty::cdt::Connection>();

    wsConnection->setHttpBaseUrl(projectDirs.generateConnectionBaseUrlString(true /* http */));
    wsConnection->setHttpTimeouts(httpConnectTimeouts);

    cout << "### Run&Connect" << "\n";

    auto spawnArgs = projectDirs.generateArgsForSpawnChrome();
    // cout << "Chrome args:\n";
    // for(auto a: spawnArgs)
    //     cout << "  " << a << "\n";

    std::vector<marty::cdt::JsonListResponseEntry> jsonListResponse;
    auto httpResponse = wsConnection->checkRunAndGetJsonList( jsonListResponse, chromeName, spawnArgs, httpProbeTimeouts);

    if (!wsConnection->httpIsResponseOk(httpResponse))
        throw std::runtime_error("Failed to connect to browser");

    cout << "HTTP code: " << httpResponse->statusCode << "\n";
    cout << "Chrome is running" << "\n";


    marty::cdt::JsonVersionResponse versionResponse;
    httpResponse = wsConnection->httpGetJsonVersion(versionResponse);
    if (!wsConnection->httpIsResponseOk(httpResponse))
        throw std::runtime_error("Failed to browser version");

    cout << "\n" << "Version Info:" << "\n";

    cout << "  " << "browser        : " << versionResponse.browser         << "\n";
    cout << "  " << "protocolVersion: " << versionResponse.protocolVersion << "\n";
    cout << "  " << "userAgent      : " << versionResponse.userAgent       << "\n";
    cout << "  " << "v8Version      : " << versionResponse.v8Version       << "\n";
    cout << "  " << "webkitVersion  : " << versionResponse.webkitVersion   << "\n";
    cout << "  " << "debuggerVersion: " << versionResponse.debuggerVersion << "\n";
    //cout << "  " << ": " << versionResponse. << "\n";


    //cout << "Response:\n" << response->body << "\n";

    std::string defaultNewTabId;
    std::string defaultNewTabPageWsUrl;

    for(const auto &e: jsonListResponse)
    {
        if (e.type=="page" && (e.url=="chrome://newtab/" || e.url=="chrome://newtab"))
        {
            defaultNewTabId = e.id;
            defaultNewTabPageWsUrl = e.webSocketDebuggerUrl;
        }
    }


    if (defaultNewTabPageWsUrl.empty())
    {
        cout << "\n\n";
        cout << "### /json/new" << "\n";

        marty::cdt::JsonNewPageResponse newPageResponse;
        httpResponse = wsConnection->httpNewPage(newPageResponse);
        if (!wsConnection->httpIsResponseOk(httpResponse))
            return returnReportFailedToConnect(httpResponse->errorCode, "/json/new", 1);

        cout << "HTTP code: " << httpResponse->statusCode << "\n";
        cout << "Response:\n" << httpResponse->body << "\n";

        defaultNewTabId        = newPageResponse.id;
        defaultNewTabPageWsUrl = newPageResponse.webSocketDebuggerUrl;
        
    }


    try
    {
        if (defaultNewTabId.empty())
        {
            throw std::runtime_error("defaultNewTabId is empty");
            //cout << "Error: defaultNewTabId is empty\n";
        }

        wsConnection->wsSetUrl(defaultNewTabPageWsUrl);


        wsConnection->wsSetSystemEventHandler( [&](marty::cdt::Connection */* pCon */, const marty::cdt::WebSocketMessage& /* msg */, ix::WebSocketMessageType type)
                                               {
                                                   std::cout << "Message, type: " << marty::cdt::utils::ixWebSocketMessageTypeToString(type) << "\n" << "\n";
                                               }
                                             );

        struct GenericIdHandler
        {
            std::string handlerName;

            void operator()(marty::cdt::Connection *pCon, const marty::cdt::WebSocketMessage& /* msg */, marty::cdt::MessageIdVariant idVariant, marty::cdt::json j) const
            {
                using std::cout;
                std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                cout << "'" << handlerName << "'" << " ID Handler" << "\n";
                std::visit( [&](auto&& arg)
                            {
                                using T = std::decay_t<decltype(arg)>;
    
                                if constexpr (std::is_same_v<T, unsigned>)
                                {
                                    cout << "  ID: " << arg << "\n";
                                    cout << "  JSON:" << "\n";
                                    cout << j.dump(2) << "\n";
                                }
                                else if constexpr (std::is_same_v<T, std::string>)
                                {
                                    cout << "  Method: " << arg << "\n";
                                }
                                else if constexpr (std::is_same_v<T, marty::cdt::ResponseError>)
                                {
                                    cout << "  ResponseError:" << "\n";
                                    cout << "    ID     : " << arg.id << "\n";
                                    cout << "    Code   : " << arg.errorInfo.code << "\n";
                                    cout << "    Message: " << arg.errorInfo.message << "\n";
                                }    
                                // else
                                //     static_assert(false, "non-exhaustive visitor!");
                            }
                          , idVariant
                          );
    
                cout << "\n";
            
            }

        }; // struct GenericIdHandler


        struct GenericMethodHandler
        {
            std::string handlerName;

            void operator()(marty::cdt::Connection *pCon, const marty::cdt::WebSocketMessage& /* msg */, marty::cdt::MessageIdVariant idVariant, marty::cdt::json j) const
            {
                using std::cout;
                std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                cout << "'" << handlerName << "'" << " Method Handler" << "\n";
                std::visit( [&](auto&& arg)
                            {
                                using T = std::decay_t<decltype(arg)>;
    
                                if constexpr (std::is_same_v<T, unsigned>)
                                {
                                    cout << "  ID: " << arg << "\n";
                                    cout << "  JSON:" << "\n";
                                    cout << j.dump(2) << "\n";
                                }
                                else if constexpr (std::is_same_v<T, std::string>)
                                {
                                    cout << "  Method: " << arg << "\n";
                                }
                                else if constexpr (std::is_same_v<T, marty::cdt::ResponseError>)
                                {
                                    cout << "  ResponseError:" << "\n";
                                    cout << "    ID     : " << arg.id << "\n";
                                    cout << "    Code   : " << arg.errorInfo.code << "\n";
                                    cout << "    Message: " << arg.errorInfo.message << "\n";
                                }    
                                else
                                    static_assert(false, "non-exhaustive visitor!");
                            }
                          , idVariant
                          );
    
                cout << "\n";
            }

        }; // struct GenericMethodHandler



        wsConnection->wsSetDefaultIdHandler(GenericIdHandler{"DefaultIdHandler"});

        //wsConnection->wsSetMethodEventHandler("*", GenericMethodHandler{"Default"});
        //wsConnection->wsSetMethodEventHandler("Page.*", GenericMethodHandler{"Page"});


        // std::atomic<bool> domContentEventFiredFlag = false;
        // std::atomic<bool> loadEventFiredFlag       = false;
        //  
        // wsConnection->wsSetMethodEventHandler("Page.domContentEventFired", marty::cdt::AtomicBoolMethodHandler{domContentEventFiredFlag, true});
        // wsConnection->wsSetMethodEventHandler("Page.loadEventFired"      , marty::cdt::AtomicBoolMethodHandler{loadEventFiredFlag      , true});

        
        auto wsConnectRes = wsConnection->wsConnect();
        if (!wsConnectRes.success)
        {
            cout << "WS NOT connected" << "\n";
            cout << "http_status: " << wsConnectRes.http_status << "\n";
            cout << "errorStr   : " << wsConnectRes.errorStr    << "\n";
         
            throw std::runtime_error("WS NOT connected");
        }
        else
        {
            cout << "WS connected" << "\n" << "\n";
        }
    
        wsConnection->wsStart();
    
        // wsConnection->wsSetDefaultIdHandler( GenericIdHandler{"Default"}
        auto
        sendRes = wsConnection->wsEventSubscribe("Page", GenericIdHandler{"PageEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Page.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();

         
        sendRes = wsConnection->wsEventSubscribe("Network", GenericIdHandler{"NetworkEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Network.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();

         
        sendRes = wsConnection->wsEventSubscribe("Runtime", GenericIdHandler{"RuntimeEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Runtime.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();

         
        sendRes = wsConnection->wsEventSubscribe("DOM", GenericIdHandler{"DomEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'DOM.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        marty::cdt::PageNavigateResponse pageNavigateResponse;

        if (!wsConnection->cdtPageNavigate( pageNavigateResponse, "https://яндекс.рф", 30000))
            throw std::runtime_error("Couldn't wait for the page to load");

        if (pageNavigateResponse.frameId.empty())
            throw std::runtime_error("Frame ID not returned");


        cout << "'Page.navigate' done\n" << "\n";
        cout << "Page loaded\n" << "\n";



        cout << "Execute DOM.getDocument\n" << "\n";


        marty::cdt::DomDocument domDocument;
        if (!wsConnection->cdtDomGetDocument(domDocument))
            throw std::runtime_error("No reply for 'DOM.getDocument'");

        cout << "'DOM.getDocument' done\n" << "\n";


        std::string pageHtml;

        {
            marty::cdt::json jOuterHTML;
            if (!wsConnection->cdtRuntimeEvaluate(jOuterHTML, "document.documentElement.outerHTML"))
                throw std::runtime_error("No reply for 'Runtime.evaluate' with 'document.documentElement.outerHTML'");

            pageHtml = jOuterHTML["result"]["value"].get<std::string>();

            std::cout << "document.documentElement.outerHTML JSON:\n";
            std::cout << jOuterHTML.dump(2) << "\n\n";
        }

        //std::cerr << "Page.HTML\n";
        std::cerr << pageHtml << "\n";

        if (!wsConnection->cdtGetHtml(pageHtml))
            std::cout << "cdtGetHtml timedout\n";
        else
            std::cout << "cdtGetHtml: " << pageHtml << "\n";


        // 
        for(auto i=0; i!=20; ++i)
        {
            if (!wsConnection->wsDispatchMessages())
                umba::sleepMs(100);
        }
         
        wsConnection->wsStop();
         
        umba::sleepMs(100);

        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Normal exit\n";

    }
    catch(const std::exception &e)
    {
        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Error: " << e.what() << "\n";
    }
    catch(...)
    {
        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Error: " << "unknown error" << "\n";
    }

    cout << "\n";
    std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
    cout << "Exiting\n";

    return 0;
}


