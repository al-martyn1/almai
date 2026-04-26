#pragma once

#include <stack>

//#include "app_config.h"
#include "umba/cmd_line.h"
#include "umba/cli_tool_helpers.h"
#include "umba/shellapi.h"
#include "umba/string_plus.h"
#include "umba/rule_of_five.h"
//
//#include "marty_cpp/marty_cpp.h"
//
#include "AppVerConfig.h"

//
//#include "marty_cpp/src_normalization.h"



// AppConfig    appConfig;

//extern umba::SimpleFormatter umbaLogStreamMsg;


template<typename StringType>
struct ArgParser
{

    umba::command_line::CommandSequenceController   cmdController;

    bool isOptionAllowed(const umba::command_line::CommandLineOption &opt) const
    {
        return cmdController.isOptionAllowed(opt);
    }

    bool isOptionAllowed(const umba::command_line::CommandLineOption &opt, std::string &errMsg) const
    {
        return cmdController.isOptionAllowed(opt, errMsg);
    }

    umba::command_line::CommandInfo& addCommand(const std::string &cmd)
    {
        return cmdController.addCommand(cmd);
    }



    // if (cmdController.canAddSubCommand())
    // {
    //     cmdController.appendSubCommandSequence(a);
    // }
    // else
    // {
    //     // cmdController.addInput(a, getBasePath()); // add as is
    //     cmdController.addInput(a, getBasePath()); // add file with base path




    // bool cmdController.canAddSubCommand() const
    // bool cmdController.isSubCommandAllowed(const std::string& cmd, std::string &errMsg) const
    // cmdController.appendSubCommandSequence(const std::string& cmd, bool throwError=true)
    // void cmdController.addOptionsToFinalCommands(const std::string &optionsStr)

    //UMBA_RULE_OF_FIVE_DEFAULT(ArgParser);
    UMBA_RULE_OF_FIVE_COPY_MOVE_DEFAULT(ArgParser);

    #define APP_ARGPARSER_CHECK_OPTION_ALLOWED()    \
            if (argsParser.hasHelpOption) return 0; \
            do                                      \
            {                                       \
                if (!cmdController.isOptionAllowed(opt, errMsg)) \
                {                                   \
                    LOG_ERR<<errMsg<<"\n";          \
                    return -1;                      \
                }                                   \
            } while(0)


    ArgParser()
    {
        // cmdController.addStandardCommonGlobalOptions()
        // cmdController.addStandardAutocompleteGlobalOptions()
        // cmdController.addStandardHelpGlobalOptions()
        cmdController.addAllStandardGlobalOptions();

        //cmdController.addGlobalOptions("")

        // branch
        cmdController.addCommand("branch").setOptions("delete","force-delete","move,all") // -d delete, -D force-delete, -m move, -a all
                     // .setUsageInfo("[OPTIONS] BRANCH_NAME") // это не нужно
                     .setUsageInfo("BRANCH_NAME") // можно короче
                     .setBrief("create branch brief")
                     .setDescription("create branch full description create branch full description  create branch full description ");

        // stash
        cmdController.addCommand("stash push").setOptions("message") // -M message
                     // .setUsageInfo("[OPTIONS]") // это не нужно
                     .setBrief("push stash to stash stack")
                     .setDescription("push stash to stash stack full description  push stash to stash stack full description ");

        cmdController.addCommand("stash list")
                     .setBrief("list stashes brief");

        cmdController.addCommand("stash apply")
                     .setBrief("apply stash brief");

        cmdController.addCommand("stash pop")
                     .setBrief("pop stash brief");

        cmdController.addCommand("stash drop")
                     .setBrief("drop stash brief");

        cmdController.addCommand("stash clear")
                     .setBrief("clear stash brief");

        // config
        cmdController.addCommand("config").setOptions("global","local","list","unset")
                     .setUsageInfo("CONFIG_VALUE_NAME")
                     .setBrief("set/unset configuration option brief");

        // log
        cmdController.addCommand("log").setOptions("oneline","graph","patch","since")
                     .setBrief("show log brief");

        // reset
        cmdController.addCommand("reset").setOptions({"soft","mixed","hard"})
                     .setBrief("perform reset brief");

        // submodule
        cmdController.addCommand("submodule add")
                     .setBrief("add submodule brief");

        cmdController.addCommand("submodule update").setOptions("init,recursive")
                     .setBrief("update submodule brief");

        cmdController.addCommand("submodule foreach").setRawMode(true)
                     .setBrief("exec command for each submodule");

        // worktree
        cmdController.addCommand("worktree add")
                     .setMaxInputParams(2) // Или закоментить throw ниже в setParameterTransformHandler
                     .setParameterTransformHandler( []( const std::string & /* fullCommandStr */  // команда не нужна, игнорим
                                                      , const std::vector<std::string> &inputList // ../backup-branch backup
                                                      , const std::string &paramValue
                                                      , const std::string &cwd
                                                      )
                                                    {
                                                        if (inputList.empty())
                                                            return umba::filename::makeAbsPath( paramValue, cwd );
                                                        else if (inputList.size()<2)
                                                            return paramValue;
                                                        else
                                                            return paramValue;
                                                            // throw std::runtime_error("too many parameters for command: '" + fullCommandStr + "'");
                                                    }
                                                  )
                     .setBrief("add worktree");

        cmdController.addCommand("worktree remove all")
                     .setBrief("remove worktree all");

        cmdController.addCommand("worktree remove exact")
                     .setBrief("remove worktree exact item");

        cmdController.addCommand("worktree remove filter")
                     .setBrief("remove worktree by filter");

        cmdController.addCommand("worktree list")
                     .setBrief("list worktree");

        cmdController.findCommand("worktree")
                     .setOptions("local,remote");

        cmdController.findCommand("worktree remove")
                     .setOptions("soft,hard");

        cmdController.findCommand("worktree remove filter")
                     .setOptions("simple,regex");

        // https://chat.deepseek.com/share/m489klmmtnimvksyqz
        //  
        // git branch feature — создаёт новую ветку feature (но не переключается на неё).
        // git branch -d feature — удаляет ветку feature (только если она слита с текущей).
        // git branch -D feature — принудительно удаляет ветку feature (даже если не слита).
        // git branch -m old-name new-name — переименовывает ветку.
        // git branch -a — показывает все локальные и удалённые ветки.
        //  
        // git stash push -m "описание" — сохраняет текущие изменения с сообщением.
        // git stash list — показывает список всех сохранений.
        // git stash apply stash@{0} — применяет последнее сохранение (не удаляя его из стека).
        // git stash pop — применяет последнее сохранение и удаляет его из стека.
        // git stash drop stash@{0} — удаляет конкретное сохранение.
        // git stash clear — удаляет все сохранения.
        //  
        // git config --global user.name "Ваше Имя" — задаёт глобальное имя автора коммитов.
        // git config --local user.email "email@example.com" — задаёт email только для текущего репозитория.
        // git config --list — показывает все текущие настройки.
        // git config --unset alias.co — удаляет псевдоним co.
        //  
        // git log --oneline — компактный вывод (каждый коммит в одну строку).
        // git log --graph — отображает историю в виде ASCII-графа.
        // git log -p — показывает разницу (patch) для каждого коммита.
        // git log --since="2 days ago" — коммиты за последние 2 дня.
        //  
        // git reset --soft HEAD~1 — отменяет последний коммит, но оставляет изменения в индексе (staged).
        // git reset --mixed HEAD~1 — отменяет последний коммит и убирает изменения из индекса (остаются только в рабочей директории).
        // git reset --hard HEAD~1
        //  
        // git reflog — показывает все перемещения HEAD.
        // git reflog expire --expire=now --all — очищает старые записи рефлога.
        //  
        // git submodule add <url> — добавляет внешний репозиторий как подмодуль.
        // git submodule update --init --recursive — инициализирует и подтягивает все вложенные подмодули.
        // git submodule foreach git pull — выполняет git pull в каждом подмодуле.
        //  
        // git worktree add ../backup-branch backup — создаёт новую рабочую копию ветки backup в папке ../backup-branch.
        // git worktree remove ../backup-branch — удаляет созданную рабочую копию.
        // git worktree list — показывает все привязанные рабочие деревья.


        // cmdController.addOptionsToFinalCommands("pass");

    }


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

    if (!cmdController.isRawMode() && opt.isOption())
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

        // !!! setDescription обязателеь, он "финализирует" опцию. Может быть пустым.
        // if (opt.isOption("recursive") || opt.setDescription(""))

#include "cli_opt_parsers/empty_option_seal.h"
#include "cli_opt_parsers/basic_options.h"
#include "cli_opt_parsers/overwrite.h"
// #include "cli_opt_parsers/list.h"
//#include "cli_opt_parsers/dict.h"
//#include "cli_opt_parsers/output.h"
//#include "cli_opt_parsers/filename_decoration.h"
//#include "cli_opt_parsers/filename_title_level.h"
//#include "cli_opt_parsers/sort.h"
//#include "cli_opt_parsers/fence_style.h"
//#include "cli_opt_parsers/strip_prefix.h"
//#include "cli_opt_parsers/lang_marker.h"
//#include "cli_opt_parsers/add_plural_pair.h"
//#include "cli_opt_parsers/add_translation.h"
//#include "cli_opt_parsers/add_project_root_marker.h"

#include "cli_opt_parsers/role_setup.h"


        if (opt.isOption("delete") || opt.isOption('d') || opt.setDescription("Delete something"))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("force-delete") || opt.isOption('D') || opt.setDescription("Force delete something"))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("move")  || opt.isOption('m') || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("all") || opt.isOption('a') || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("message") || opt.setDescription("Commit message"))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("global") || opt.setDescription("Global configuration"))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("local") || opt.setDescription("Local configuration"))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("list") || opt.isOption('l') || opt.setDescription("List something"))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("unset") || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("oneline") || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("graph") || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("patch") || opt.isOption('p') || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("since") || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("soft") || opt.isOption('S') || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("mixed") || opt.isOption('X') || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }

        if (opt.isOption("hard") || opt.isOption('H') || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }


        if (opt.isOption("init") || opt.setDescription(""))
        {
            // if (argsParser.hasHelpOption) return 0;
            //  
            // if (!opt.getParamValue(intVal,errMsg))
            // {
            //     LOG_ERR<<errMsg<<"\n";
            //     return -1;
            // }

            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }


        if (opt.isOption("recursive") || opt.isOption('R') || opt.setDescription(""))
        {
            APP_ARGPARSER_CHECK_OPTION_ALLOWED();
            // do nothing
            return 0;
        }


        // if (opt.isOption(""))
        // {
        //     APP_ARGPARSER_CHECK_OPTION_ALLOWED();
        //     // do nothing
        //     return 0;
        // }



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

                if (!argsParser.quet)
                {
                    //umba::cli_tool_helpers::printNameVersion(std::cout);
                    //umba::cli_tool_helpers::printBuildDateTime();
                    //umba::cli_tool_helpers::printCommitHash(std::cout);
                    //std::cout<<"\n";
                //printHelp();
                }

                // virtual void setPrintHelpStyle( PrintHelpStyle phs ) override
                // virtual PrintHelpStyle getPrintHelpStyle() const override
                // argsNeedHelp - похоже, что справка по отдельной опции (опциям)
                // isNormalPrintHelpStyle - похоже, это вариант, когда вывод не для документации, а на консоль

                // Если есть коллектор опций
                // и вывод online, на консоль, для оперативной подсказки
                // и вывод не по отдельным опциям, а общий

                // std::cout << opt.getHelpOptionsString();

                #if 0
                if (pCol && pCol->isNormalPrintHelpStyle() && argsParser.argsNeedHelp.empty())
                {
                    auto helpText = opt.getHelpOptionsString();
                    std::cout << "Usage: " << argsParser.programLocationInfo.exeName
                              << " [OPTIONS] PATTERN [PATTERN]\n"
                              << "\nOptions:\n\n"
                              << helpText;
                              //<< " [OPTIONS] input_file [output_file]\n\nOptions:\n\n"<<helpText;
                }
                #endif

                // argsNeedHelp - справка по отдельной опции
                // Тут выводится полная справка, по одной опции или по всем

                // 1) Режим хелпа детектится раньше всего
                // 2) В режиме хелпа никакие проверки не будут срабатывать - чек на help работает раньше
                // 3) В режиме хелпа можно задать несколько опций, и хелп будет только по ним
                // 4) Поддерживается вроде в итоге только режим markdown и обычный. Всё, что не обычный - это markdown. На остальное положен больт. Но вроде только местами
                // 5) pCol->makeText( 78, &argsParser.argsNeedHelp ) - генерит текст справки по опциям, вроде во всех форматах (normal/md/wiki)

                // pCol->getPrintHelpStyle() - возвращает стиль форматирования
                // umba::::textAddIndent(umba::text_utils::formatTextParas( descr, width, umba::text_utils::TextAlignment::left ), "    " );
                // if (style==PrintHelpStyle::wiki || style==PrintHelpStyle::md)
                // umba::command_line::ICommandLineOptionCollector *pCol
                // std::set<StringType>      argsNeedHelp

                /*
                    Итого.

                    1) argsParser.argsNeedHelp - это set опций, по которым запрашиваем справку.
                       но если попадается неизвестная опция - будет ошибка - это гут
                       Но нет, неизвестная опция обнаруживается только на этапе обработки аргументов, а не при выдаче справки, так что не гут
                    2) pCol->makeText - делает всю магию документации по опциям
                    3) Если задана команда - то выдаём справку только по ней, и, 
                       используя argsParser.argsNeedHelp - справку по её опциям - заодно проверяется соответствие, нигде ли не накосячили с опциями.
                       (на самом деле это работает на уровне разбора аргументов, а не унутре, унутре проверок нет)

                    Ещё раз:

                      (no command) --help - выдаёт справку по всем опциям (после краткого описания команд)
                      (no command) --option1 --option2 --help - выдаёт справку по опциям --option1/--option2, не важно, чьи они (без описания команд).
                      command [subcommand] --help - выдаёт справку по команде/сабкоманде - brief/detailed, и краткий список всех опций сабкоманды, без описаний
                      command [subcommand] --option1 --option2 --help - выдаёт справку по команде/сабкоманде и детальную справку по option1 и option2
                
                */

                auto helpText = cmdController.makeHelp( 78   // textWidth
                                                      , pCol // ICommandLineOptionCollector
                                                      , argsParser.argsNeedHelp
                                                      //, argsParser.programLocationInfo.exeName
                                                      );

                std::cout << helpText; // << "\n"; Там хватает переводов строки

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

    else if (!cmdController.isRawMode() && opt.isResponseFile())
    {
        //std::string

        StringType optName;
        umba::utfToStringTypeHelper(optName, opt.name);
        auto optFileName = argsParser.makeAbsPath(optName);

        if (!argsParser.quet)
        {
            #if !defined(NDEBUG)
            // std::cout << "Processing options file: " << optFileName << "\n";
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
    if (cmdController.canAddSubCommand())
    {
        cmdController.appendSubCommandSequence(a);
    }
    else
    {
        // cmdController.addInput(a, getBasePath()); // add as is
        cmdController.addInput(a, argsParser.getBasePath()); // add file with base path // учитывается "сырой" режим
    }

    // StringType getBasePath() const
    // appConfig.inputFiles.push_back(argsParser.makeAbsPath(a));
    //appConfig.scanInfos.emplace_back(almai::FileSystemScanInfo::parse(argsParser.makeAbsPath(a)));

    return 0;

}

}; // struct ArgParser



class CommandLineOptionCollector : public umba::command_line::CommandLineOptionCollectorImplBase
{
protected:
    virtual void onOptionDup( const std::string &opt ) override
    {
        LOG_ERR<<"Duplicated option key - '"<<opt<<"'\n";
        throw std::runtime_error("Duplicated option key - '" + opt + "'");
    }

};



