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

AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;


#include "ArgParser.h"



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

    } // if (umba::isDebuggerPresent())


    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        return 1;
    }

    //!!! --------------
    appConfig.setAppRoot(argsParser.getAppRoot(), argsParser.getAppConfPath()); // to find prompts

    if (!appConfig.findProjectRoot()) // Также устанавливает ProjectRoot
    {
        LOG_WARN("prj-root") << "project root not found\n";
    }
    else
    {
        if (!argsParser.quet)
        {
            LOG_MSG << "\n";

            LOG_MSG << "found project root: '" << appConfig.projectRoot << "'\n";
            if (appConfig.projectFile.empty())
                LOG_MSG << "project file not found\n";
            else
                LOG_MSG << "found project file: '" << appConfig.projectFile << "'\n";

            LOG_MSG << "\n";
        }
    }

    appConfig.addEnvironmentPrepromptPaths();
    appConfig.curPrepromptPathType = almai::PrepromptPathType::cliOptions;


    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        return 1;
    }

    if (argsParser.mustExit)
        return 0;


    //if (!argsParser.quet)
    if (1)
    {
        LOG_MSG << "Preprompt dirs:\n";

        auto ppDirsAnnotated = appConfig.getPrepromptDirsAnnotated();
        for(auto &&ppdp : ppDirsAnnotated)
        {
            LOG_MSG << "  " << appConfig.getPrepromptPathTypeAnnotation(ppdp.first) << ": " << ppdp.second << "\n";
        }

        LOG_MSG << "\n";
    }

    if (!argsParser.quet  /* && !hasHelpOption */ )
    {
    }

    if (1)
    {
        LOG_MSG << "Translations test:\n";

        auto printTranslation = [&](std::string lang, std::string key)
        {
            auto text = appConfig.getLocalizedText(lang, key);
            LOG_MSG << "  " << lang << ":" << key << ": " << text << "\n";
        };

        printTranslation("ru", "roles");
        printTranslation("en", "roles");
        printTranslation("ru", "constraints");
        printTranslation("en", "constraints");
        printTranslation("ru", "domains");
        printTranslation("en", "domains");

        LOG_MSG << "\n";
    }


    appConfig.curAiEngine = "deepseek"; // !!! Должно вычитываться из настроек проекта

    std::vector<std::string> scannedFolders;
    std::unordered_map< std::string, std::unordered_map<std::string, almai::PrepromptProps> > scannedPrepromptProps;
    std::unordered_map< std::string, std::unordered_set<std::string> > scannedPrepromptTypes;

    appConfig.scanForPreprompts(&scannedFolders, scannedPrepromptProps, scannedPrepromptTypes);

    auto prepromptReadingErrorHandler = [&](const std::string &ppFilename)
    {
        LOG_WARN("read-error") << "failed to read preprompt file: '" << ppFilename << "'\n";
    };

    auto prepromptParsingErrorHandler = [&](const std::string &ppFilename, const std::exception &e)
    {
        LOG_WARN("parsing-error") << "failed to parsing preprompt file. Error: '" << e.what() << "', file: '" << ppFilename << "'\n";
    };

    std::unordered_map< std::string, std::unordered_map<std::string, almai::Preprompt> > scannedPreprompts;

    appConfig.scanForPreprompts(scannedPreprompts, scannedPrepromptProps, prepromptReadingErrorHandler, prepromptParsingErrorHandler);


    if (1)
    {

        LOG_MSG << "Scanned folders:\n";

        for(const auto &fldr: scannedFolders)
        {
            LOG_MSG << "  " << fldr << "\n";
        }

        LOG_MSG << "\n";


        LOG_MSG << "Found preprompts:\n";

        for(const auto &[ppTypeStr, ppNameMap] : scannedPrepromptProps)
        {
            LOG_MSG << "  " << ppTypeStr << ":\n";

            for(const auto &[ppName, ppProps] : ppNameMap)
            {
                LOG_MSG << "    " << ppProps << "\n";
            }
        }

        LOG_MSG << "\n";

        
        LOG_MSG << "Found preprompt types:\n";

        for(const auto &[ppId, ppTypeSet] : scannedPrepromptTypes)
        {
            std::size_t cnt = 0;
            LOG_MSG << "  " << ppId; // << ""
            for(const auto ppType: ppTypeSet)
            {
                LOG_MSG << (cnt ? ", " : ": ") << ppType;
                ++cnt;
            }
            LOG_MSG << "\n";
        }

    }

    


    return 0;
}



