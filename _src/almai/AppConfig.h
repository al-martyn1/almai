#pragma once


//
#include "AppConfigBase.h"
#include "PluralDatabase.h"
#include "Localization.h"

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


struct AppConfig : public AppConfigBase
{

    std::string                       appRoot;
    std::string                       appConfPath;
    std::string                       projectRoot; // устанавливается только через setProjectRoot
    std::string                       projectFile; // Полное имя almai.yaml, с путём
    std::string                       aiName; // Например, deepeek, qwen - используется для поиска кастомизированных препромптов

    std::unordered_set<std::string>   projectRootStopNames; //  = { ".git", ".out", ".vscode", ".build", "build" }; // пока явно инициализируем
    //std::vector<std::string>          projectRootStopNames = { ".git", ".out", ".vscode", ".build", "build" }; // пока явно инициализируем
    // almai.yaml

    std::unordered_map<almai::PrepromptPathType, std::vector<std::string> > prepromptDirs;
    almai::PrepromptPathType          curPrepromptPathType = almai::PrepromptPathType::builtinOptions;

    almai::PluralDatabase             pluralDb;
    almai::Localization               localizations;
    std::string                       curLang;


    // UMBA_RULE_OF_FIVE_COPY_MOVE(FoundFileInfo, default, default, default, default);
    // UMBA_RULE_OF_FIVE(FoundFileInfo, default, default, default, default, default);

    //------------------------------
    bool addProjectRootMarker(std::string marker)
    {
        umba::string::trim(marker);

        if (marker=="-")
        {
            projectRootStopNames.clear();
            return true;
        }

        bool bRemove = false;

        if (marker.empty())
            return false;

        if (marker.front()=='-' || marker.front()=='+')
        {
            if (marker.front()=='-')
                bRemove = true;

            marker.erase(0, 1);
            umba::string::trim(marker);

            if (marker.empty())
                return false;
        }

        if (bRemove)
            projectRootStopNames.erase(marker);
        else
            projectRootStopNames.insert(marker);

        return true;
    }

    bool addProjectRootMarkers(const std::vector<std::string> &markersList)
    {
        for(const auto &m : markersList)
        {
            if (!addProjectRootMarker(m))
                return false;
        }

        return true;
    }

    bool addProjectRootMarkers(const std::string &markersListStr)
    {
        // Можно через splitPathList(name,','); // umba::filename?
        // Можно через umba::string::split(p, ',', true /* skipEmpty */ );

        auto markersList = umba::filename::splitPathList(markersListStr, ',');
        return addProjectRootMarkers(markersList);
    }



// // std::vector<std::string> maskList = splitPathList(name,',');
// inline
// std::vector<std::string> splitNormalizedPath(const std::string &p)
// {
//     return umba::string::split(p, '/', true /* skipEmpty */ );
// }


    //------------------------------
    bool addLocalization(const std::string &langKeyTextTriplet)
    {
        return localizations.addLocalization(langKeyTextTriplet);
    }

    std::string getLocalizedText(std::string lang, const std::string &key) const
    {
        return localizations.getLocalizedText(lang, key);
    }

    std::string getLocalizedText(const std::string &key) const
    {
        return localizations.getLocalizedText(curLang, key);
    }


    //------------------------------
    std::vector<std::string> getPrepromptDirs() const
    {
        std::vector<std::string> resVec;

        unsigned pptBegin = (unsigned)almai::PrepromptPathType::begin;
        unsigned pptEnd   = (unsigned)almai::PrepromptPathType::end;
        for(unsigned i=pptBegin; i!=pptEnd; ++i)
        {
            std::unordered_map<almai::PrepromptPathType, std::vector<std::string> >::const_iterator it = prepromptDirs.find((almai::PrepromptPathType)i);
            if (it==prepromptDirs.end())
                continue;

            resVec.insert(resVec.end(), it->second.begin(), it->second.end());
        }

        return resVec;
    }

    static
    std::string getPrepromptPathTypeAnnotation(almai::PrepromptPathType ppt) 
    {
        switch(ppt)
        {
            case almai::PrepromptPathType::installDirs   : return "INST";
            case almai::PrepromptPathType::builtinOptions: return "BLTN";
            case almai::PrepromptPathType::envPaths      : return "ENVV";
            case almai::PrepromptPathType::projectDirs   : return "PRJD";
            case almai::PrepromptPathType::cliOptions    : return "CLIO";
            case almai::PrepromptPathType::unknown       : return "UNKN";
            case almai::PrepromptPathType::end           : return "END" ;
            default : return "DFLT";
        }
    }

    std::vector<std::pair<almai::PrepromptPathType, std::string> > getPrepromptDirsAnnotated() const
    {
        std::vector<std::pair<almai::PrepromptPathType, std::string> > resVec;

        unsigned pptBegin = (unsigned)almai::PrepromptPathType::begin;
        unsigned pptEnd   = (unsigned)almai::PrepromptPathType::end;
        for(unsigned i=pptBegin; i!=pptEnd; ++i)
        {
            std::unordered_map<almai::PrepromptPathType, std::vector<std::string> >::const_iterator it = prepromptDirs.find((almai::PrepromptPathType)i);
            if (it==prepromptDirs.end())
                continue;

            for(auto &&p : it->second)
            {
                resVec.emplace_back(it->first, p);
            }
            //resVec.insert(resVec.end(), it->second.begin(), it->second.end());
        }

        return resVec;
    }




    void addPrepromptPath(almai::PrepromptPathType ppt, std::string path)
    {
        umba::string::trim(path);

        std::vector<std::string> &ppDirsVec = prepromptDirs[ppt];
        // ppDirsVec.insert(ppDirsVec.begin(), path); // Позже добавлен - приоритет выше
        ppDirsVec.push_back(path);
    }

    void addPrepromptPath(const std::string &path)
    {
        addPrepromptPath(curPrepromptPathType, path);
        
    }

    void setAppRoot(const std::string &appRoot_, const std::string &appConfPath_)
    {
        appRoot = appRoot_;
        appConfPath = appConfPath_;

        auto 
        tmpPath = umba::filename::makeAbsPath(std::string("preprompts.almai"), appConfPath);
        if (umba::filesys::isPathDirectory(tmpPath))
            addPrepromptPath(almai::PrepromptPathType::installDirs, tmpPath);

        tmpPath = umba::filename::makeAbsPath(std::string("preprompts.almai.custom"), appConfPath);
        if (umba::filesys::isPathDirectory(tmpPath))
            addPrepromptPath(almai::PrepromptPathType::installDirs, tmpPath);
    }

    void setProjectRoot(const std::string &projectRoot_)
    {
        projectRoot = projectRoot_;

        auto tmpPath = umba::filename::makeAbsPath(std::string(".preprompts.almai"), projectRoot);
        if (umba::filesys::isPathDirectory(tmpPath))
        {
            addPrepromptPath(almai::PrepromptPathType::projectDirs, tmpPath);
        }
        else
        {
            // Это могут быть чужие препромпты
            tmpPath = umba::filename::makeAbsPath(std::string(".preprompts"), projectRoot);
            if (umba::filesys::isPathDirectory(tmpPath))
                addPrepromptPath(almai::PrepromptPathType::projectDirs, tmpPath);
        }
    }

    void addEnvironmentPrepromptPaths()
    {
        std::string envAlmaiOverlayPrepromtsPathList;
        if (umba::env::getVar(std::string("ALMAI_OVERLAY_PREPROMPTS"), envAlmaiOverlayPrepromtsPathList) && !envAlmaiOverlayPrepromtsPathList.empty())
        {
            auto pathList = umba::filename::splitPathList(envAlmaiOverlayPrepromtsPathList);
            for(auto &&p: pathList)
            {
                umba::string::trim(p);
                std::vector<std::string> &vec = prepromptDirs[almai::PrepromptPathType::envPaths];
                vec.insert(vec.begin(), p); // Тут приоритет выше у тех, кто впереди - обычно важное в переменных окружения добавляем раньше - чем меньше приоритет, тем ближе к началу
            }
        }
    }

    //! Проверка существования каталога или файла, одного из многих, по заданному пути
    static
    bool isPathExistOneOf(const std::string &basePath, const std::vector<std::string> &relNames)
    {
        for(auto &&rn : relNames)
        {
            auto fullName = umba::filename::makeAbsPath(rn, basePath);
            if (umba::filesys::isPathExist(fullName))
                return true;
        }

        return false;
    }

    static
    bool isPathExistOneOf(const std::string &basePath, const std::unordered_set<std::string> &relNames)
    {
        for(auto &&rn : relNames)
        {
            auto fullName = umba::filename::makeAbsPath(rn, basePath);
            if (umba::filesys::isPathExist(fullName))
                return true;
        }

        return false;
    }

    static
    bool isPathExistOneOf(const std::string &basePath, const std::string &relName)
    {
        auto fullName = umba::filename::makeAbsPath(relName, basePath);
        if (umba::filesys::isPathExist(fullName))
            return true;
        return false;
    }

    static
    std::string makeAlmaiYamlFullName(const std::string &path, bool leadingDot)
    {
        return umba::filename::makeAbsPath((leadingDot?".":"") + std::string("ALMAI.yaml"), path);
    }

    bool isProjectRootPath(const std::string &path, std::string *pAlmaiYamlName) const
    {
        std::string almaiYaml = makeAlmaiYamlFullName(path, true);
        if (umba::filesys::isFileExist(almaiYaml))
        {
            if (pAlmaiYamlName)
               *pAlmaiYamlName = almaiYaml;
            return true;
        }

        almaiYaml = makeAlmaiYamlFullName(path, false);
        if (umba::filesys::isFileExist(almaiYaml))
        {
            if (pAlmaiYamlName)
               *pAlmaiYamlName = almaiYaml;
            return true;
        }

        if (pAlmaiYamlName)
           pAlmaiYamlName->clear();

        if (isPathExistOneOf(path, projectRootStopNames))
            return true;

        return false;
    }

    bool findProjectRoot(std::string startPath=umba::filesys::getCurrentDirectory())
    {
        auto upLevelPath = startPath;
        auto curPath     = startPath;
        do
        {
            curPath = upLevelPath;

            if (isProjectRootPath(curPath, &projectFile))
            {
                setProjectRoot(curPath);
                return true;
            }

            upLevelPath = umba::filename::getPath(curPath);
        }
        while(upLevelPath!=curPath);

        return false;
    }

    // std::string                       projectRoot; // устанавливается только через setProjectRoot
    // std::string                       projectFile; // Полное имя almai.yaml, с путём

    //inline std::string getCurrentDirectory()


// inline bool isFileExist(const char *path)
// {
//     return fsysapi::isFileExist(impl_helpers::encodeToNative(path));
// }
// inline bool isPathExist(const std::wstring &path)

// template<typename StringType>
// bool isPathDirectory(const StringType &path)
// inline bool isPathDirectory(const std::wstring &path)



    // inline std::string getCurrentDirectory()

    





// custom.options

}; // struct AppConfig

