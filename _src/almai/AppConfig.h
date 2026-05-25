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
#include "PrepromptDatabase.h"
//
#include "utils.h"

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
#include <memory>

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
namespace almai {

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
struct AppConfig : public AppConfigBase
{

    using PluralDatabaseSharedPtrType = std::shared_ptr<almai::PluralDatabase>;
    using PrepromptDatabaseMap        = std::unordered_map<std::string, PrepromptDatabase>;


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

    PluralDatabaseSharedPtrType       pluralDb = std::make_shared<almai::PluralDatabase>();
    almai::Localization               localizations;
    std::string                       curLang;

    //std::string                       curAiEngine; // deepseek, qwen
    PrepromptDatabaseMap              ppDBases;

    almai::Project                    almaiProject;



    // UMBA_RULE_OF_FIVE_COPY_MOVE(FoundFileInfo, default, default, default, default);
    // UMBA_RULE_OF_FIVE(FoundFileInfo, default, default, default, default, default);

    //------------------------------
    std::string normalizePrepromptId(const std::string &prepromptId) const { return almai::utils::normalizePrepromptId(*pluralDb.get(), prepromptId); }

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
    bool roleSetupFromCli(const std::string &roleSetupStr);

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
    auto makeSkillPrepareHandler() const
    {
        auto skillPrepareHandler = [&](std::string str)
        {
            umba::string::case_convert(str, umba::CaseOption::toLower);
            return normalizePrepromptId(str);
        };

        return skillPrepareHandler;
    }

    //------------------------------
    template<typename PrepromptReadingErrorHandler, typename PrepromptParsingErrorHandler>
    void readProjectFile( PrepromptReadingErrorHandler   readingErrHandler
                        , PrepromptParsingErrorHandler   parsingErrorHandler
                        )
    {
        if (projectFile.empty())
            return;

        std::string projectText;

        if (!almai::utils::readFile(projectFile, projectText))
        {
            readingErrHandler(projectFile);
            return;
        }

        try
        {
            almai::Project::parse( almaiProject, projectText
                                 , makeSkillPrepareHandler()
                                 , true /* throwErrors */
                                 );
        }
        catch(const std::exception &e)
        {
            parsingErrorHandler(projectFile, e);

            // Пробуем игнорировать ошибки
            // Но исключение всё равно может вылететь. Но мы его уже не ловим, пусть летит
            almai::Project::parse( almaiProject, projectText
                                 , makeSkillPrepareHandler()
                                 , false /* !throwErrors */
                                 );
        }

    }

    template<typename ErrorHandler, typename WarningHandler>
    bool projectCheckNormalize( const PrepromptDatabase &ppDb
                              , ErrorHandler   errorHandler
                              , WarningHandler warningHandler
                              )
    {
        return almaiProject.checkNormalize(ppDb, errorHandler, warningHandler);
    }

    //------------------------------

    // template<typename PrepromptReadingErrorHandler, typename PrepromptParsingErrorHandler>
    // void scanForPreprompts( std::vector<std::string>       *pScannedFolders
    //                       , std::vector<std::string>       aiEngines
    //                       , std::vector<std::string>       prepromptCategoriesToScan
    //                       , PrepromptReadingErrorHandler   readingErrHandler
    //                       , PrepromptParsingErrorHandler   parsingErrorHandler
    //                       )
    // {
    //     auto ppDirs = getPrepromptDirs();
    //  
    //     for(const auto &aiEngine : aiEngines)
    //     {
    //         PrepromptDatabase ppDb = almai::PrepromptDatabase(pluralDb, ppDirs);
    //     }
    // }
    //  
    //  
    //  
    // almai::PrepromptDatabase ppDb = almai::PrepromptDatabase(appConfig.pluralDb, appConfig.getPrepromptDirs());
    // ppDb.prepromptDirs = appConfig.getPrepromptDirs();
    // std::vector<std::string> scannedFolders;
    //  
    // processedFileType = "preprompt";
    //  
    // ppDb.scanForPreprompts( &scannedFolders, aiEngines
    //                       , { "skill", "instruction", "knowledge", "format", "output" }
    //                       , prepromptReadingErrorHandler, prepromptParsingErrorHandler
    //                       );
    //  
    // PrepromptDatabaseMap              ppDBases;



}; // struct AppConfig

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------

} // namespace almai


