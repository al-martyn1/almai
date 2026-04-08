/*! \file
    \brief Конфиг главного приложения
 */

#pragma once


//
#include "AppConfigBase.h"
#include "PluralDatabase.h"
#include "Localization.h"
#include "Preprompt.h"
#include "Project.h"

//
#include "umba/umba.h"
#include "umba/env.h"

//
#include "umba/string.h"
#include "umba/rule_of_five.h"
#include "umba/filename.h"
#include "umba/filesys.h"

//
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
namespace almai {

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
struct AppConfig : public AppConfigBase
{

    std::string                       appRoot;
    std::string                       appConfPath;
    std::string                       projectRoot; // устанавливается только через setProjectRoot
    std::string                       almaiDir; // 
    std::string                       projectFile; // Полное имя '.almai/project.yaml', с путём
    std::string                       aiName; // Например, deepeek, qwen - используется для поиска кастомизированных препромптов

    std::unordered_set<std::string>   projectRootStopNames; //  = { ".git", ".out", ".vscode", ".build", "build" }; // пока явно инициализируем
    // almai.yaml

    std::unordered_map<almai::PrepromptPathType, std::vector<std::string> > prepromptDirs;
    almai::PrepromptPathType          curPrepromptPathType = almai::PrepromptPathType::builtinOptions;

    almai::PluralDatabase             pluralDb;
    almai::Localization               localizations;
    std::string                       curLang;

    std::string                       curAiEngine; // deepseek, qwen

    almai::Project                    almaiProject;



    // UMBA_RULE_OF_FIVE_COPY_MOVE(FoundFileInfo, default, default, default, default);
    // UMBA_RULE_OF_FIVE(FoundFileInfo, default, default, default, default, default);

    //------------------------------
    bool addProjectRootMarker(std::string marker); // Добавляет маркер остановки поиска корневого каталога проекта
    bool addProjectRootMarkers(const std::vector<std::string> &markersList);
    bool addProjectRootMarkers(const std::string &markersListStr);

    //------------------------------
    bool addLocalization(const std::string &langKeyTextTriplet);
    std::string getLocalizedText(std::string lang, const std::string &key) const;
    std::string getLocalizedText(const std::string &key) const;

    //------------------------------
    static
    std::string getPrepromptPathTypeAnnotation(almai::PrepromptPathType ppt) ;

    std::vector<std::string> getPrepromptDirs() const;
    std::vector<std::pair<almai::PrepromptPathType, std::string> > getPrepromptDirsAnnotated() const;

    void addPrepromptPath(almai::PrepromptPathType ppt, std::string path);
    void addPrepromptPath(const std::string &path);

    //------------------------------
    void setAppRoot(const std::string &appRoot_, const std::string &appConfPath_);
    void setProjectRoot(const std::string &projectRoot_);

    //------------------------------
    void addEnvironmentPrepromptPaths();

    //------------------------------
    //! Проверка существования каталога или файла, одного из многих, по заданному пути
    static bool isPathExistOneOf(const std::string &basePath, const std::vector<std::string> &relNames);
    static bool isPathExistOneOf(const std::string &basePath, const std::unordered_set<std::string> &relNames);
    static bool isPathExistOneOf(const std::string &basePath, const std::string &relName);

    //------------------------------
    static std::vector<std::string> makeAlmaiFolderNames();
    static const std::vector<std::string>& getAlmaiFolderNames();
    static std::vector<std::string> getAlmaiFolderFullNames(const std::string &path);

    //------------------------------
    bool isProjectRootPath(const std::string &path, std::string *pAlmaiYamlName) const;
    bool findProjectRoot(std::string startPath=umba::filesys::getCurrentDirectory());


    //------------------------------
    std::string normalizeSkillName(std::string skillName) const;

    //------------------------------
    template<typename PrepromptReadingErrorHandler, typename PrepromptParsingErrorHandler>
    void readProjectFile( PrepromptReadingErrorHandler readingErrHandler
                        , PrepromptParsingErrorHandler parsingErrorHandler
                        )
    {
        if (projectFile.empty())
            return;

        std::string projectText;

        if (!readFile(projectFile, projectText))
        {
            readingErrHandler(projectFile);
            return;
        }

        try
        {
            // marty::json parse(Project &p, const std::string &text, SkillNamePrepareHandler skillNamePrepareHandler, bool throwErrors)
            almai::Project::parse( almaiProject, projectText
                                 , [&](const std::string &str) { return normalizeSkillName(str); }
                                 , true /* throwErrors */
                                 );
        }
        catch(const std::exception &e)
        {
            parsingErrorHandler(projectFile, e);

            // Пробуем игнорировать ошибки
            // Но исключение всё равно может вылететь. Но мы его уже не ловим, пусть летит
            almai::Project::parse( almaiProject, projectText
                                 , [&](const std::string &str) { return normalizeSkillName(str); }
                                 , false /* !throwErrors */
                                 );
        }

    }

    //------------------------------
    static
    void scanForPreprompts( std::vector<std::string> *pScannedFolders
                          , const std::vector<std::string> &ppDirs
                          , const std::string &aiEngineName
                          , std::unordered_map< std::string, std::unordered_map<std::string, almai::PrepromptProps> > &scannedPrepromptProps
                          , std::unordered_map< std::string, std::unordered_set<std::string> > &scannedPrepromptTypes
                          , const std::vector<std::string> &prepromptTypesToScan // = { "skill", "instruction", "knowledge", "format", "output" }
                          );

    void scanForPreprompts( std::vector<std::string> *pScannedFolders
                          , std::unordered_map< std::string, std::unordered_map<std::string, almai::PrepromptProps> > &scannedPrepromptProps
                          , std::unordered_map< std::string, std::unordered_set<std::string> > &scannedPrepromptTypes
                          , std::vector<std::string> prepromptTypesToScan = { "skill", "instruction", "knowledge", "format", "output" }
                          );

    template<typename PrepromptReadingErrorHandler, typename PrepromptParsingErrorHandler>
    void scanForPreprompts( std::unordered_map< std::string, std::unordered_map<std::string, almai::Preprompt> > &scannedPreprompts
                          , const std::unordered_map< std::string, std::unordered_map<std::string, almai::PrepromptProps> > &scannedPrepromptProps
                          , PrepromptReadingErrorHandler readingErrHandler
                          , PrepromptParsingErrorHandler parsingErrorHandler
                          )
    {
        for(const auto &[ppTypeStr, ppNameMap] : scannedPrepromptProps)
        {
            for(const auto &[ppName, ppProps] : ppNameMap)
            {
                std::vector<std::string> inputFileLines;

                if (!readFile(ppProps.file, inputFileLines))
                {
                    readingErrHandler(ppProps.file);
                    continue;
                }

                almai::Preprompt preprompt;

                try
                {
                    preprompt = almai::Preprompt::parse(inputFileLines, true /* throwErrors */ );
                }
                catch(const std::exception &e)
                {
                    parsingErrorHandler(ppProps.file, e);

                    // Пробуем игнорировать ошибки
                    // Но исключение всё равно может вылететь. Но мы его уже не ловим, пусть летит
                    preprompt = almai::Preprompt::parse(inputFileLines, false /* !throwErrors */ );
                }

                preprompt.props = ppProps;

                // Если до сюда не вылетели, то можно добавлять в результаты

                // !!! Нужно поправить зависимости, привести всё в plural
                for(auto &require : preprompt.description.requires)
                {
                    for(auto &r: require)
                    {
                        r = normalizeSkillName(r);
                    }
                }

                scannedPreprompts[ppTypeStr][ppName] = preprompt;

            }
        }

    }

    //------------------------------




}; // struct AppConfig

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------

} // namespace almai


