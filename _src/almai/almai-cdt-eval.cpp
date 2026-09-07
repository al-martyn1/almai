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



// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"
//
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"
//
#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//
#include "umba/utf.h"

#if defined(WIN32) || defined(_WIN32)
    #include "umba/clipboard_win32.h"
#endif

//
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>



//----------------------------------------------------------------------------
//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

// bool bOverwrite         = false;

inline
int returnReportFailedToConnect(ix::HttpErrorCode c, const std::string &endPoint, int retCode=1)
{
    std::cout << "Failed to connect to '" << endPoint << "' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(c) << " (" << (int)c << ")" << "\n";
    return retCode;
}

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

//
#include "log.h"
//
#include "CdtEvalAppConfig.h"

AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;

#include "CdtEvalArgParser.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
int unsafeMain(int argc, char* argv[]);

UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
        return 2;
    }

}

//----------------------------------------------------------------------------
int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    using std::cout;
    using std::cerr;


    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        // argsParser.args.clear();
        // argsParser.args.push_back("--overwrite");

        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        // argsParser.args.push_back("-Y");
        // argsParser.args.push_back("-o=" + rootPath + "\\tests\\almai-md-join_01.md");
        // //argsParser.args.push_back(rootPath + "/_src/almai/**/*.cpp,*.bat");
        // argsParser.args.push_back("../../../README.md");

        //argsParser.args.push_back("--help");


        argsParser.args.push_back("-Y");
        argsParser.args.push_back("-J=100");
        argsParser.args.push_back(rootPath + "/_src/almai/**/*.cpp");

    } // if (umba::isDebuggerPresent())


    // Job completed - may be, --where option found
    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        // LOG_INFO("config") << "Error found in builtin option files\n";
        return 1;
    }

    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        return 1;
    }

    if (argsParser.mustExit)
        return 0;


    if (!appConfig.quet)
    {
        std::cerr << "\n";
        std::cerr << "Runtime.evaluate options:\n";
        std::cerr << "\n";

        std::cerr << "  " << "contextId                   " << ": " << (appConfig.evalContextId.empty()   ? std::string("<EMPTY>") : appConfig.evalContextId  ) << "\n";
        std::cerr << "  " << "objectGroup                 " << ": " << (appConfig.evalObjectGroup.empty() ? std::string("<EMPTY>") : appConfig.evalObjectGroup)  << "\n";
        std::cerr << "\n";

        std::cerr << "  " << "returnByValue               " << ": " << to_string(appConfig.evalReturnType)   << "\n";
        std::cerr << "  " << "awaitPromise                " << ": " << to_string(appConfig.evalAwaitPromise) << "\n";
        std::cerr << "  " << "userGesture                 " << ": " << to_string(appConfig.evalUserGesture)  << "\n";
        std::cerr << "  " << "throwOnSideEffect           " << ": " << to_string(appConfig.evalThrowOnSideEffect) << "\n";
        std::cerr << "  " << "disableBreaks               " << ": " << to_string(appConfig.evalBreaksControl) << "\n";
        std::cerr << "  " << "replMode                    " << ": " << to_string(appConfig.evalReplMode) << "\n";
        std::cerr << "  " << "allowUnsafeEvalBlockedByCSP " << ": " << to_string(appConfig.evalCspMode) << "\n";
        //std::cerr << "  " << "" << ": " << to_string(appConfig.) << "\n";

    }

#if 0

    ix::initNetSystem();

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.initAll(chromeName, false /* !useTempDir */ ))
    {
        LOG_ERR << "Project root not found" << "\n";
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


        wsConnection->wsSetSystemEventHandler( { ix::WebSocketMessageType::Open
                                               , ix::WebSocketMessageType::Close
                                               , ix::WebSocketMessageType::Error
                                               , ix::WebSocketMessageType::Ping
                                               , ix::WebSocketMessageType::Pong
                                               , ix::WebSocketMessageType::Fragment
                                               }
                                             , [&](marty::cdt::Connection */* pCon */, const marty::cdt::WebSocketMessage& /* msg */, ix::WebSocketMessageType type)
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

        wsConnection->wsSetMethodEventHandler("*", GenericMethodHandler{"Default"});

        wsConnection->wsSetMethodEventHandler("Page.*", GenericMethodHandler{"Page"});


        std::atomic<bool> domContentEventFiredFlag = false;
        std::atomic<bool> loadEventFiredFlag       = false;

        wsConnection->wsSetMethodEventHandler("Page.domContentEventFired", marty::cdt::AtomicBoolMethodHandler{domContentEventFiredFlag, true});
        wsConnection->wsSetMethodEventHandler("Page.loadEventFired"      , marty::cdt::AtomicBoolMethodHandler{loadEventFiredFlag      , true});


        
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

        std::cerr << pageHtml;
       

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

#endif

    return 0;
}


