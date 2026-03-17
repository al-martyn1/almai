#pragma once


#include "umba/umba.h"
//
#include "AppConfigBase.h"
//
#include "umba/string.h"
#include "umba/rule_of_five.h"
#include "umba/filename.h"
#include "umba/filesys.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>


struct AppConfig : public AppConfigBase
{

    std::string                       appRoot;
    std::string                       projectRoot; // устанавливается только через setProjectRoot
    std::string                       projectFile; // Полное имя almai.yaml, с путём
    std::string                       aiName; // Например, deepeek, qwen - используется для поиска кастомизированных препромптов

    std::vector<std::string>          prepromptFolders; // = { "preprompts.custom", "preprompts"}; // препромпты проекта потом добавляем в начало
    std::vector<std::string>          projectRootStopNames = { ".git", ".out", ".vscode", ".build", "build" }; // пока явно инициализируем
    // almai.yaml


    // UMBA_RULE_OF_FIVE_COPY_MOVE(FoundFileInfo, default, default, default, default);
    // UMBA_RULE_OF_FIVE(FoundFileInfo, default, default, default, default, default);

    void setAppRoot(const std::string &appRoot_)
    {
        appRoot = appRoot_;

        prepromptFolders.clear();

        prepromptFolders.emplace_back(umba::filename::makeAbsPath(std::string("preprompts.custom"), appRoot));
        prepromptFolders.emplace_back(umba::filename::makeAbsPath(std::string("preprompts"), appRoot));
    }

    void setProjectRoot(const std::string &projectRoot_)
    {
        // starts_with_and_strip(name, std::string("//?/"));

        // наверное, не смысла проверять, был ли он уже добавлен - всё равно это делается один раз при старте

        projectRoot = projectRoot_;

        auto tmpName = umba::filename::makeAbsPath(std::string(".preprompts.almai"), projectRoot);
        if (umba::filesys::isPathDirectory(tmpName))
        {
            prepromptFolders.insert(prepromptFolders.begin(), tmpName);
        }
        else
        {
            tmpName = umba::filename::makeAbsPath(std::string(".preprompts"), projectRoot);
            if (umba::filesys::isPathDirectory(tmpName))
            {
                prepromptFolders.insert(prepromptFolders.begin(), tmpName);
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
        return umba::filename::makeAbsPath((leadingDot?".":"") + std::string("almai.yaml"), path);
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

