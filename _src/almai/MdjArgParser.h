#pragma once

#include <stack>

//#include "app_config.h"
#include "umba/cmd_line.h"
#include "umba/cli_tool_helpers.h"
#include "umba/shellapi.h"
#include "umba/string_plus.h"
//
#include "marty_cpp/marty_cpp.h"
//
#include "MdjAppVerConfig.h"

//
#include "marty_cpp/src_normalization.h"



// AppConfig    appConfig;

//extern umba::SimpleFormatter umbaLogStreamMsg;


template<typename StringType>
struct ArgParser
{

    static
    const std::set<std::string>& getWarnOptsSet()
    {
        const static std::set<std::string> s {}; // {"img-copy", "img-copy-exist", "same-file", "plantuml", "graphviz"};
        return s;
    }

    static
    const std::set<std::string>& getInfoOptsSet()
    {
        const static std::set<std::string> s {}; // /* = */ {"snippet-lookup", "plantuml", "graphviz", "opt-files", "config", "strip-extentions", "page-index", "meta-tags", "auto-url"};
        return s;
    }


// 0 - ok, 1 normal stop, -1 - error
template<typename ArgsParser>
int operator()( const StringType                                &a           //!< строка - текущий аргумент
              , umba::command_line::CommandLineOption           &opt         //!< Объект-опция, содержит разобранный аргумент и умеет отвечать на некоторые вопросы
              , ArgsParser                                      &argsParser  //!< Класс, который нас вызывает, содержит некоторый контекст
              , umba::command_line::ICommandLineOptionCollector *pCol        //!< Коллектор опций - собирает инфу по всем опциям и готов вывести справку
              , bool fBuiltin
              , bool ignoreInfos
              )
{
    //using namespace marty::clang::helpers;

    UMBA_USED(fBuiltin);
    UMBA_USED(a);

    std::string dppof = "Don't parse predefined options from ";

    if (opt.isOption())
    {

#include "umba/warnings/push_disable_C4189.h"

        std::string errMsg;
        std::string strVal;
        int intVal = 0;
        //unsigned uintVal = 0;
        std::size_t szVal = 0;
        bool boolVal = false;

        UMBA_USED(szVal);
        UMBA_USED(boolVal);

#include "umba/warnings/pop.h"



#include "parse_opt/basic_options.h"
#include "parse_opt/overwrite.h"
// #include "parse_opt/list.h"
#include "parse_opt/dict.h"
#include "parse_opt/output.h"
#include "parse_opt/filename_decoration.h"
#include "parse_opt/filename_title_level.h"
#include "parse_opt/sort.h"
#include "parse_opt/fence_style.h"
#include "parse_opt/strip_prefix.h"
#include "parse_opt/lang_marker.h"


        if (opt.isHelpStyleOption())
        {
            // Job is done in isHelpStyleOption
            // return 0; // !!!
        }

        else if (opt.isHelpOption()) // if (opt.infoIgnore() || opt.isOption("help") || opt.isOption('h') || opt.isOption('?') || opt.setDescription(""))
        {
            if (!ignoreInfos)
            {
                if (pCol && !pCol->isNormalPrintHelpStyle())
                    argsParser.quet = true;
                //printNameVersion();
                if (!argsParser.quet)
                {
                    umba::cli_tool_helpers::printNameVersion(std::cout);
                    //umba::cli_tool_helpers::printBuildDateTime();
                    umba::cli_tool_helpers::printCommitHash(std::cout);
                    std::cout<<"\n";
                //printHelp();
                }

                if (pCol && pCol->isNormalPrintHelpStyle() && argsParser.argsNeedHelp.empty())
                {
                    //argsParser.printHelpPage( std::cout, "[OPTIONS] input_file [output_file]", "If output_file not taken, STDOUT used", helpText );
                    auto helpText = opt.getHelpOptionsString();
                    std::cout << "Usage: " << argsParser.programLocationInfo.exeName
                              << " [OPTIONS] PATTERN [PATTERN]\n"
                              << "\nOptions:\n\n"
                              << helpText;
                              //<< " [OPTIONS] input_file [output_file]\n\nOptions:\n\n"<<helpText;
                }

                if (pCol) // argsNeedHelp
                {
                    argsParser.printHelpPage( std::cout
                                            , "[OPTIONS] input_file [output_file]"
                                            , "If output_file not taken, STDOUT used"
                                            , pCol->makeText( 78, &argsParser.argsNeedHelp )
                                            );
                    // std::cout<<pCol->makeText( 78, &argsParser.argsNeedHelp );
                }

                return 1;

            }

            return 0; // simple skip then parse builtins
        }

        else
        {
            LOG_ERR<<"unknown option: "<<opt.argOrg<<"\n";
            return -1;
        }

        return 0;

    } // if (opt.isOption())

    else if (opt.isResponseFile())
    {
        //std::string

        StringType optName;
        umba::utfToStringTypeHelper(optName, opt.name);
        auto optFileName = argsParser.makeAbsPath(optName);

        if (!argsParser.quet)
        {
            #if !defined(NDEBUG)
            std::cout << "Processing options file: " << optFileName << "\n";
            #endif
        }

        argsParser.pushOptionsFileName(optFileName);
        auto parseRes = argsParser.parseOptionsFile( optFileName );
        argsParser.popOptionsFileName();

        if (!parseRes)
            return -1;

        if (argsParser.mustExit)
            return 1;

        return 0;

    }

    // Process non-option args here

    // appConfig.inputFiles.push_back(argsParser.makeAbsPath(a));
    appConfig.scanInfos.emplace_back(almai::FileSystemScanInfo::parse(argsParser.makeAbsPath(a)));

    return 0;

}

}; // struct ArgParser



class CommandLineOptionCollector : public umba::command_line::CommandLineOptionCollectorImplBase
{
protected:
    virtual void onOptionDup( const std::string &opt ) override
    {
        LOG_ERR<<"Duplicated option key - '"<<opt<<"'\n";
        throw std::runtime_error("Duplicated option key");
    }

};



