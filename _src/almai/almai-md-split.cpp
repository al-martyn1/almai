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
#include "MdsAppConfig.h"

AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;

#include "MdsArgParser.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
bool splitLinesAndSaveContent(std::vector<std::string> mdLines)
{
    std::vector<std::string> lastSignificantLines;

    bool readingCode = false;
    char codeMarkerChar = 0;
    std::size_t codeMarkerLen = 0;
    std::string codeLang;
    std::vector<std::string> codeLines;

    std::vector<almai::ListingInfo> foundListings;

    using almai::MdLineType;

    almai::MdLineType listingType = almai::MdLineType::emptyLine;

    lineNo = 0;
    for(const auto &line : mdLines)
    {
        lineNo++;

        char markerChar = 0;
        std::size_t markerLen = 0;

        MdLineType mdLineType = almai::detectMarkdownLineType(line, &markerChar, &markerLen);

        if (readingCode)
        {
            bool unexpectedEndOfListing = false;

            if (listingType==MdLineType::codeTilda || listingType==MdLineType::codeBacktick)
            {
                // ИИ может глючить (не хватать токенов), и он может продалбываться
                // Иногда он может добавить маркер конца листинга в конец строки
                // потеряв часть содержимого листинга.
                // Надо просекать такие ситуации

                // !!! Не нужно. Просто был кривоватый документ, в нем были листинги на питоне, и внутри них были 
                // маркдаун маркеры блоков кода

                // auto blockMarker = std::string(codeMarkerLen, codeMarkerChar);
                //  
                // if (line!=blockMarker && umba::string::ends_with(line, blockMarker))
                // {
                //     unexpectedEndOfListing = true;
                //     LOG_WARN_INPUT("unexp-code-block-end") << "found unexpected code block end\n";
                // }
                
            }

            // listingType = mdLineType;
            if (listingType==MdLineType::codeIndentTab || listingType==MdLineType::codeIndentSpace)
            {
                if (listingType==mdLineType)
                {
                    // Продолжается чтение листинга с отступом
                    codeLines.push_back(line);
                }
                else
                {
                    // Может начаться backtick или tilda листинг
                    if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
                    {
                        listingType = mdLineType;
                        readingCode = true;
                        codeMarkerChar = markerChar;
                        codeMarkerLen  = markerLen ;
                        codeLines.clear();
                        codeLang = almai::extractCodeLangFromFencedCodeBlockMarker(line);
                    }
                    else
                    {
                        // листинг с отступом закончился
                        readingCode = false;
                        lastSignificantLines.clear();
                        codeLines.clear();
                    }
                }
            }

            else if ( unexpectedEndOfListing
                  || ((listingType==MdLineType::codeTilda || listingType==MdLineType::codeBacktick) 
                    && listingType==mdLineType && codeMarkerChar==markerChar && codeMarkerLen==markerLen
                     )
                    )
            {
                if (codeLang.empty())
                {
                    codeLang = almai::extractCodeLangFromFencedCodeBlockMarker(line);
                }

                // Остальная обработка финализации листинга
                almai::ListingInfo listingInfo;
                listingInfo.listingCodeLines = codeLines;
                listingInfo.foundLangName    = codeLang;

                std::vector<std::string> filenames;
                bool hasEdging = almai::findListingFilenames(lastSignificantLines, filenames);

                for(auto &name : filenames)
                {
                    name = almai::replaceInvalidPathNameChars(name, !hasEdging);
                    name = almai::makeNormalizedRelativePath(name);
                    listingInfo.listingFilenames.emplace_back(name);
                }

                foundListings.emplace_back(listingInfo);
    
                readingCode = false;
                lastSignificantLines.clear();
                codeLines.clear();
            }
            else
            {
                // продолжаем чтение листинга
                codeLines.push_back(line);
            }
        }

        else // обычный режим
        {
            if ( mdLineType==MdLineType::emptyLine
              || mdLineType==MdLineType::headerArx
              || mdLineType==MdLineType::headerSetext
              || mdLineType==MdLineType::quotation
               )
            {
                lastSignificantLines.clear();
            }
            else if (mdLineType==MdLineType::regularLine)
            {
                lastSignificantLines.push_back(line);
            }
            else if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
            {
                listingType = mdLineType;
                readingCode = true;
                codeMarkerChar = markerChar;
                codeMarkerLen  = markerLen ;
                codeLines.clear();
                codeLang = almai::extractCodeLangFromFencedCodeBlockMarker(line);
            }
            else if (mdLineType==MdLineType::codeIndentTab || mdLineType==MdLineType::codeIndentSpace)
            {
                LOG_WARN_INPUT("unsup-code-block-type") << (mdLineType==MdLineType::codeIndentTab ? "tab" : "space") << " indent code blocks not supported\n";
                listingType = mdLineType;
                readingCode = true;
                codeMarkerChar = markerChar;
                codeMarkerLen  = markerLen ;
                codeLines.clear();
                codeLang.clear();
            }
            else
            {
                LOG_WARN_INPUT("unk-line-type") << "unknown line type found\n";
            }
        }

    }

    std::unordered_map<std::string, std::size_t>  filenameCounters;

    for(auto &listingInfo : foundListings)
    {
        auto ext = appConfig.findLangExtention(listingInfo.foundLangName);
        if (ext.empty())
            ext = "txt";

        if (listingInfo.listingFilenames.empty())
            listingInfo.listingFilenames.emplace_back("autosave");
        
        listingInfo.checkAddExtention(ext);
        listingInfo.checkAutoEnumerate(filenameCounters);
    }

    if (appConfig.listOnly)
    {
        // std::cout 
        UMBA_LOG_MSG << "\nFound listings:\n";

        for(const auto &listingInfo : foundListings)
        {
            for(const auto &name : listingInfo.listingFilenames)
            {
                // std::cout 
                UMBA_LOG_MSG << "  " << name << "\n";
            }

            std::size_t printLines = listingInfo.listingCodeLines.size();
            if (printLines>appConfig.listLines)
                printLines = appConfig.listLines;

            for(std::size_t i=0u; i!=printLines; ++i)
            {
                //std::cout 
                UMBA_LOG_MSG << "    >" << listingInfo.listingCodeLines[i] << "\n";
            }

            // std::cout 
            UMBA_LOG_MSG << "\n";
        }

        return true;
    }

    bool hasErrors = false;

    for(const auto &listingInfo : foundListings)
    {
        for(const auto &name : listingInfo.listingFilenames)
        {
            std::string fullName;
            if (!appConfig.writeFile(name, listingInfo.listingCodeLines, &fullName))
            {
                 hasErrors = true;
                 LOG_ERR << "failed to write file: '" << fullName << "'\n";
            }
        }
    }

    return !hasErrors;
}

inline
bool splitFileAndSaveContent(const std::string &fileName)
{
    std::vector<std::string> mdLines;

    if (!appConfig.readFile(fileName, mdLines))
    {
        LOG_ERR << "failed to read input file: '" << fileName << "'" << "\n";
        return false;
    }

    curFile = fileName;

    return splitLinesAndSaveContent(mdLines);
}
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

        // argsParser.args.push_back("--list");
        argsParser.args.push_back("-o=" + rootPath + "\\tests\\almai-md-split");
        argsParser.args.push_back(rootPath + "\\tests\\almai-md-split.md");

    } // if (umba::isDebuggerPresent())


    // try
    // {
        // Job completed - may be, --where option found
        if (argsParser.mustExit)
            return 0;

        // if (!argsParser.quet)
        // {
        //     printNameVersion();
        // }

        // LOG_INFO("config") << "-----------------------------------------" << "\n";
        // LOG_INFO("config") << "Processing builtin option files\n";
        if (!argsParser.parseStdBuiltins())
        {
            // LOG_INFO("config") << "Error found in builtin option files\n";
            return 1;
        }
        // LOG_INFO("config") << "-----------------------------------------" << "\n";

        if (argsParser.mustExit)
            return 0;

        // LOG_INFO("config") << "-----------------------------------------" << "\n";
        // LOG_INFO("config") << "Processing command line arguments\n";
        if (!argsParser.parse())
        {
            // LOG_INFO("config") << "Error found while parsing command line arguments\n";
            return 1;
        }
        // LOG_INFO("config") << "-----------------------------------------" << "\n";

        if (argsParser.mustExit)
            return 0;
    // }
    // catch(const std::exception &e)
    // {
    //     LOG_ERR << e.what() << "\n";
    //     return -1;
    // }
    // catch(const std::exception &e)
    // {
    //     LOG_ERR << "command line arguments parsing error" << "\n";
    //     return -1;
    // }

    // if (!argsParser.quet  /* && !hasHelpOption */ )
    // {
    //     //printNameVersion();
    //     //LOG_MSG<<"\n";
    //     umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
    // }

    
    if ( appConfig.inputFiles.empty()
      #if defined(WIN32) || defined(_WIN32)
      && !appConfig.useClipboard
      #endif
       )
    {
        LOG_ERR << "no input files taken" << "\n";
        return 1;
    }


// template<typename ToUtfConverter> inline
// bool clipboardTextGet(std::string &text, const ToUtfConverter &toUtfConverter, bool *pUtf, HWND hWndNewOwner=0)


    appConfig.checkUpdateOutputDir();


    bool hasErrors = false;

#if defined(WIN32) || defined(_WIN32)
    if (appConfig.useClipboard)
    {
        std::string clpbText;
        if (!umba::win32::clipboardTextGet( clpbText, [](const std::wstring &t ) { return umba::toUtf8(t); } /* toUtfConverter */ , 0 /* pUtf */ , umba::win32::clipboardGetConsoleHwnd()))
        {
            LOG_WARN("clipbrd") << "failed to get clipboard text\n";
        }
        else
        {
            std::vector<std::string> lines = appConfig.splitTextToLines(clpbText);
            if (!splitLinesAndSaveContent(lines))
            {
                hasErrors = true;
            }
        }
    
    }
#endif


    for(const auto &inputFileName : appConfig.inputFiles)
    {
        if (!splitFileAndSaveContent(inputFileName))
        {
            hasErrors = true;
        }
    }


    return hasErrors ? 1 : 0;
}

