/*! \file
    \brief Тестируем umba::command_line::CommandSequenceController
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

#include "umba/debug_helpers.h"

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

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"

#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

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
#include "AppConfig.h"

almai::AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;


#include "Test08ArgParser.h"



int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        //std::cout << "Error: " << e.what() << "\n";
        LOG_ERR << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        LOG_ERR << "unknown error\n";
        //std::cout << "Unknown error\n";
        return 2;
    }

}



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

        argsParser.args.clear();
        argsParser.args.push_back("--overwrite");
        argsParser.args.push_back("-q");
        
        // argsParser.args.push_back("submodule");
        // argsParser.args.push_back("--init");
        // argsParser.args.push_back("--recursive");
        // argsParser.args.push_back("update");

        // argsParser.args.push_back("branch");
        // //argsParser.args.push_back("-d");

        // argsParser.args.push_back("worktree");
        // argsParser.args.push_back("remove");

        //argsParser.args.push_back("add");

        // argsParser.args.push_back("--bash");
        // argsParser.args.push_back("--clink");
        //argsParser.args.push_back("--md");

        argsParser.args.push_back("branch");
        // argsParser.args.push_back("--delete");

        argsParser.args.push_back("stash");
        //argsParser.args.push_back("list");

        argsParser.args.push_back("--help");

        // argsParser.args.push_back("../backup-branch");
        // argsParser.args.push_back("backup");
        // argsParser.args.push_back("--help");

    } // if (umba::isDebuggerPresent())


    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        return 1;
    }

    //!!! --------------

    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        return 1;
    }

    if (argsParser.mustExit)
        return 0;


    return argsParser.argParser.cmdController.executeCommand();

    // return 0;
}



