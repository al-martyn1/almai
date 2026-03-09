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

bool bOverwrite         = false;

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
std::string extractCodeLangFromFencedCodeBlockMarker(std::string line)
{
    char markerChar = 0;
    std::size_t markerLen = 0;
    MdLineType mdLineType = almai::detectMarkdownLineType(line, &markerChar, &markerLen);
    if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
    {
        line.erase(0, markerLen);
        umba::string::trim(line);
        return line;
    }

    return std::string();
}

//----------------------------------------------------------------------------
inline
bool splitFileAndSaveContent(const std::string &fileName)
{
    std::string inputFileText;

    if (!appConfig.readInputFile(fileName, inputFileText))
    {
        LOG_ERR << "failed to read input file: '" << fileName << "'" << "\n";
        return false;
    }

    curFile = fileName;

    auto mdLines = marty_cpp::splitToLinesSimple(inputFileText);

    std::vector<std::string> lastSignificantLines;

    bool readingCode = false;
    char codeMarkerChar = 0;
    std::size_t codeMarkerLen = 0;
    std::string codeLang;

    using almai::MdLineType;

    lineNo = 0;
    for(const auto &line : mdLines)
    {
        lineNo++;

        char markerChar = 0;
        std::size_t markerLen = 0;

        MdLineType mdLineType = almai::detectMarkdownLineType(line, &markerChar, &markerLen);

        if (readingCode)
        {
            if ((mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick) && codeMarkerChar==markerChar && codeMarkerLen==markerLen)
            {
                if (codeLang.empty())
                {
                    codeLang = extractCodeLangFromFencedCodeBlockMarker(line);
                }

                // Остальная обработка финализации листинга
    
                // 
                readingCode = false;
                lastSignificantLines.clear();
            }
            else
            {
                // листинг, да не тот - продолжаем чтение листинга
            }
        }

        else // обычный 
        {

        }


        if ( mdLineType==MdLineType::emptyLine
          || mdLineType==MdLineType::regularLine
          || mdLineType==MdLineType::headerArx
          || mdLineType==MdLineType::headerSetext
          || mdLineType==MdLineType::quotation
           )
        {
            lastSignificantLines = 
        }


    // emptyLine   = 0,
    // regularLine
    // headerArx                 // # - Atx - word processor on Amiga, min 1 char
    // headerSetext              // ---- / ==== Setext (Structure Enhanced Text), min 1 char
    // codeTilda
    // codeBacktick
    // codeIndentTab
    // codeIndentSpace
    // quotation                 // >



    }


            // auto dictFileLines = marty_cpp::splitToLinesSimple(dictFileText);
            //  
            // std::size_t lineNum = 0;
            // for(const auto &l : dictFileLines)
            // {
            //     lineNum++;
            //  
            //     auto line = l;
            //     umba::string::trim(line);
            //     if (line.empty())
            //         continue;
            //  
            //     if (line[0]=='#')
            //         continue;
            //  
            //     if (!appConfig.addLangExtention(line))
            //     {
            //         curFile = dictFile;
            //         lineNo = (unsigned)lineNum;
            //         LOG_ERR_INPUT << "failed to add extention for language\n";
            //         return -1;
            //     }
            //  
            // }




    return true;
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

    if (!argsParser.quet  /* && !hasHelpOption */ )
    {
        //printNameVersion();
        //LOG_MSG<<"\n";
        umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
    }


    if (appConfig.inputFiles.empty())
    {
        LOG_ERR << "no input files taken" << "\n";
        return 1;
    }

    if (appConfig.outputDir.empty())
    {
        appConfig.outputDir = umba::filesys::getCurrentDirectory();
    }

    if (!umba::filename::isAbsPath(appConfig.outputDir))
    {
        appConfig.outputDir = umba::filename::appendPath(appConfig.outputDir, appConfig.outputDir);
    }


    for(const auto &inputFileName : appConfig.inputFiles)
    {
        splitFileAndSaveContent(inputFileName);
    }
    // std::vector<std::string>                         inputFiles;



    return 0;
}
