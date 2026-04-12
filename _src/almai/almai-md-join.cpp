/*! \file
    \brief 
 */


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

//
#include "log.h"
//
#include "MdjAppConfig.h"

AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;

#include "MdjArgParser.h"

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


    if (appConfig.scanInfos.empty())
    {
        // LOG_ERR << "no input files/masks taken" << "\n";
        // return 1;

        LOG_WARN("no-input") << "no input files/masks taken" << "\n";
    }

    std::vector<std::string> foundFiles;
    for(const auto &s : appConfig.scanInfos)
    {
        std::vector<std::string> tmp;
        s.scanForFiles(tmp);
        foundFiles.insert(foundFiles.end(), tmp.begin(), tmp.end());
    }

    if (!appConfig.scanInfos.empty() && foundFiles.empty())
    {
        // LOG_ERR << "no files found" << "\n";

        LOG_WARN("no-input") << "no files found" << "\n";

        LOG_MSG << "Scan paths & masks:\n";
        for(auto &&s : appConfig.scanInfos)
        {
            LOG_MSG << "  " << s.toString() << "\n";
        }

        LOG_MSG << "\n";

        return 1;
    }

    for( auto &&f: foundFiles)
    {
        appConfig.foundFileInfos.emplace_back(f);
    }

    for( auto &ffi: appConfig.foundFileInfos)
    {
        ffi.stripPrefix(appConfig.stripPrefixes);
    }

    //------------------------------
    // В главной тулзе это не нужно будет, там будет использоваться для стрипа каталог проекта
    std::string commonPrefix = almai::findMostCommonPathPrefix(appConfig.foundFileInfos.begin(), appConfig.foundFileInfos.end());
    commonPrefix = almai::checkCorrectMostCommonPathPrefixIsPath(commonPrefix);
    if (!commonPrefix.empty())
    {
        for(auto &ffi: appConfig.foundFileInfos)
        {
            ffi.stripPrefix(commonPrefix);
        }
    }
    //------------------------------

    for(const auto &ffi: appConfig.foundFileInfos)
    {
        if (!ffi.isPrefixStripped())
        {
            LOG_ERR << "prefixes stripped not for all files, file: '" << ffi.fullName << "'" << "\n";
            return 1;
        }
    }

    almai::sortFoundFileInfos(appConfig.foundFileInfos);

    std::vector<std::string> readedFiles;

    for(auto &ffi: appConfig.foundFileInfos)
    {
        if (!almai::utils::readFile(ffi.fullName, ffi.fileLines))
        {
            LOG_WARN("read-failed") << "failed to read file: '" << ffi.fullName << "'";
        }
        else
        {
            readedFiles.push_back(ffi.fullName);
        }
    }


    if (appConfig.listOnly)
    {
        UMBA_LOG_MSG << "\nFound files:\n";

        for(auto && ff: readedFiles)
        {
            UMBA_LOG_MSG << "  " << ff << "\n";
        }

        UMBA_LOG_MSG << "\n";

        return 0;
    }

    // Теперь надо для каждого файла найти fence
    // Вывести в конечный документ и записать результат

    std::stringstream oss;

    for(auto &ffi: appConfig.foundFileInfos)
    {
        appConfig.generateMarkdownListing(oss, ffi.displayName, ffi.fileLines);
    }

    if (appConfig.output.empty())
    {
        std::cout << oss.str() << "\n";
        return 0;
    }

    auto mdLines = marty_cpp::splitToLinesSimple(oss.str());

    auto resLines = appConfig.headerLines;

    if (!resLines.empty())
    {
        resLines.push_back(std::string());
        resLines.push_back(std::string(3u,'-'));
        resLines.push_back(std::string());
    }

    resLines.insert(resLines.end(), mdLines.begin(), mdLines.end());

    if (!appConfig.footerLines.empty())
    {
        resLines.push_back(std::string());
        resLines.push_back(std::string(3u,'-'));
        resLines.push_back(std::string());
    }

    resLines.insert(resLines.end(), appConfig.footerLines.begin(), appConfig.footerLines.end());


    std::string fullName;
    std::size_t sizeTotal = 0;

    if (!appConfig.writeFile(appConfig.output, resLines, &fullName, &sizeTotal))
    {
        LOG_ERR << "failed to write file: '" << fullName << "'\n";
        return 1;
    }

    if (!argsParser.quet)
    {
        LOG_MSG << "Result size: " << almai::utils::formatFileSize(sizeTotal) << "\n";

        LOG_MSG << "Line limit to join: ";
        if (appConfig.isSetJoinLinesLimit())
            LOG_MSG << appConfig.joinLinesLimit;
        else
            LOG_MSG << "not set";
        LOG_MSG << "\n"; 
    }


    #if defined(WIN32) || defined(_WIN32)
    if (appConfig.useClipboard)
    {
        auto allText = appConfig.mergeLines(resLines);
        if (!umba::win32::clipboardTextSet( allText, [](const std::string &t ) { return umba::fromUtf8(t); } /* fromUtfConverter */ , true /* utf */ , umba::win32::clipboardGetConsoleHwnd()))
        {
            LOG_WARN("clipbrd") << "failed to set clipboard text\n";
        }
    }
    #else
    //if (!clipboardTextSet(text, fromUtfConverter, utfSource))
    #endif


    return 0;

}

