/*! \file
    \brief Конфиг главного приложения - реализация
 */

#include "AppConfig.h"
#include "Preprompt.h"
//
#include "umba/filename.h"

//
#include <utility>

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::addProjectRootMarker(std::string marker)
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

//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::addProjectRootMarkers(const std::vector<std::string> &markersList)
{
    for(const auto &m : markersList)
    {
        if (!addProjectRootMarker(m))
            return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::addProjectRootMarkers(const std::string &markersListStr)
{
    // Можно через splitPathList(name,','); // umba::filename?
    // Можно через umba::string::split(p, ',', true /* skipEmpty */ );

    auto markersList = umba::filename::splitPathList(markersListStr, ',');
    return addProjectRootMarkers(markersList);
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::addLocalization(const std::string &langKeyTextTriplet)
{
    return localizations.addLocalization(langKeyTextTriplet);
}

//--------------------------------------------------------------------------------------------------------------------
std::string AppConfig::getLocalizedText(std::string lang, const std::string &key) const
{
    return localizations.getLocalizedText(lang, key);
}

//--------------------------------------------------------------------------------------------------------------------
std::string AppConfig::getLocalizedText(const std::string &key) const
{
    return localizations.getLocalizedText(curLang, key);
}

//--------------------------------------------------------------------------------------------------------------------
std::vector<std::string> AppConfig::getPrepromptDirs() const
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

//--------------------------------------------------------------------------------------------------------------------
std::string AppConfig::getPrepromptPathTypeAnnotation(almai::PrepromptPathType ppt) 
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

//--------------------------------------------------------------------------------------------------------------------
std::vector<std::pair<almai::PrepromptPathType, std::string> > AppConfig::getPrepromptDirsAnnotated() const
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

//--------------------------------------------------------------------------------------------------------------------
void AppConfig::addPrepromptPath(almai::PrepromptPathType ppt, std::string path)
{
    umba::string::trim(path);

    std::vector<std::string> &ppDirsVec = prepromptDirs[ppt];
    // ppDirsVec.insert(ppDirsVec.begin(), path); // Позже добавлен - приоритет выше
    ppDirsVec.push_back(path);
}

//--------------------------------------------------------------------------------------------------------------------
void AppConfig::addPrepromptPath(const std::string &path)
{
    addPrepromptPath(curPrepromptPathType, path);
    
}

//--------------------------------------------------------------------------------------------------------------------
void AppConfig::setAppRoot(const std::string &appRoot_, const std::string &appConfPath_)
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

//--------------------------------------------------------------------------------------------------------------------
void AppConfig::setProjectRoot(const std::string &projectRoot_)
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

//--------------------------------------------------------------------------------------------------------------------
void AppConfig::addEnvironmentPrepromptPaths()
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

//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::isPathExistOneOf(const std::string &basePath, const std::vector<std::string> &relNames)
{
    for(auto &&rn : relNames)
    {
        auto fullName = umba::filename::makeAbsPath(rn, basePath);
        if (umba::filesys::isPathExist(fullName))
            return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::isPathExistOneOf(const std::string &basePath, const std::unordered_set<std::string> &relNames)
{
    for(auto &&rn : relNames)
    {
        auto fullName = umba::filename::makeAbsPath(rn, basePath);
        if (umba::filesys::isPathExist(fullName))
            return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::isPathExistOneOf(const std::string &basePath, const std::string &relName)
{
    auto fullName = umba::filename::makeAbsPath(relName, basePath);
    if (umba::filesys::isPathExist(fullName))
        return true;
    return false;
}

//--------------------------------------------------------------------------------------------------------------------
std::vector<std::string> AppConfig::makeAlmaiYamlNames()
{
    return std::vector<std::string>{ ".almai.yaml"
                                   , ".ALMAI.yaml"
                                   , ".ALMAI.YAML"
                                   , ".almai.YAML"
                                   ,  "almai.yaml"
                                   ,  "ALMAI.yaml"
                                   ,  "ALMAI.YAML"
                                   ,  "almai.YAML"
                                   };
}

//--------------------------------------------------------------------------------------------------------------------
const std::vector<std::string>& AppConfig::getAlmaiYamlNames()
{
    static std::vector<std::string> names = makeAlmaiYamlNames();
    return names;
}

//--------------------------------------------------------------------------------------------------------------------
std::vector<std::string> AppConfig::getAlmaiYamlFullNames(const std::string &path)
{
    const auto &almaiNames = getAlmaiYamlNames();

    std::vector<std::string> resVec; resVec.reserve(almaiNames.size());

    for(const auto &name : almaiNames)
    {
        resVec.push_back(umba::filename::appendPath(path, name));
    }

    return resVec;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::isProjectRootPath(const std::string &path, std::string *pAlmaiYamlName) const
{
    auto almaiNames = getAlmaiYamlFullNames(path);

    for(const auto &almaiYaml : almaiNames)
    {
        if (umba::filesys::isFileExist(almaiYaml))
        {
            if (pAlmaiYamlName)
               *pAlmaiYamlName = almaiYaml;
            return true;
        }
    }

    if (pAlmaiYamlName)
       pAlmaiYamlName->clear();

    if (isPathExistOneOf(path, projectRootStopNames))
        return true;

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfig::findProjectRoot(std::string startPath)
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

//--------------------------------------------------------------------------------------------------------------------
void AppConfig::scanForPreprompts( std::vector<std::string> *pScannedFolders
                                 , std::unordered_map< std::string, std::unordered_map<std::string, almai::PrepromptProps> > &scannedPrepromptProps
                                 , std::unordered_map< std::string, std::unordered_set<std::string> > &scannedPrepromptTypes
                                 , std::vector<std::string> prepromptTypesToScan
                                 )
{
    auto ppDirs = getPrepromptDirs();

    {
        std::vector<std::string> pptsTmp; pptsTmp.reserve(prepromptTypesToScan.size());
        for(const auto &ppt: prepromptTypesToScan)
        {
            pptsTmp.push_back(pluralDb.findPlural(ppt));
        }

        using std::swap;

        swap(prepromptTypesToScan, pptsTmp);
    }

    scanForPreprompts(pScannedFolders, ppDirs, std::string(), scannedPrepromptProps, scannedPrepromptTypes, prepromptTypesToScan);
    if (!curAiEngine.empty())
        scanForPreprompts(pScannedFolders, ppDirs, curAiEngine, scannedPrepromptProps, scannedPrepromptTypes, prepromptTypesToScan);
}

//--------------------------------------------------------------------------------------------------------------------
/*
    Умеем просканировать каталог на предмет препромптов.
    Имеем список каталогов с препромптами, добавленное раньше обрабатывается раньше, позднее добавленное перетирает раннее.
    Итого:
      - обходим каталоги с препромптами
      - для каждого каталога сканируем подкаталоги с типами препромптов
      - после этого делаем тоже самое для подкаталогов curEngine, если не пустой

    PrepromptProps складываем в:
      - мапу мап - первый ключ - тип, второй - имя препромпта
      - мапа сетов - ключ - имя препромпта, сет - типы препромптов

*/

void AppConfig::scanForPreprompts( std::vector<std::string> *pScannedFolders
                                 , const std::vector<std::string> &ppDirs
                                 , const std::string &aiEngineName
                                 , std::unordered_map< std::string, std::unordered_map<std::string, almai::PrepromptProps> > &scannedPrepromptProps
                                 , std::unordered_map< std::string, std::unordered_set<std::string> > &scannedPrepromptTypes
                                 , const std::vector<std::string> &prepromptTypesToScan
                                 )
{
    for(auto ppDir: ppDirs)
    {
        if (!aiEngineName.empty())
            ppDir = umba::filename::appendPath(ppDir, "." + aiEngineName);

        for(const auto &ppType: prepromptTypesToScan)
        {
            auto scanDir = umba::filename::appendPath(ppDir, ppType);
            if (pScannedFolders)
                pScannedFolders->push_back(scanDir);

            auto ppPropsVec = almai::PrepromptProps::scanPath(scanDir, ppType);

            for(const auto &pppItem : ppPropsVec)
            {
                scannedPrepromptProps[pppItem.type][pppItem.name] = pppItem;
                scannedPrepromptTypes[pppItem.name].insert(pppItem.type);
            }

        }
    }

    UMBA_USED(scannedPrepromptProps);
    UMBA_USED(scannedPrepromptTypes);
    UMBA_USED(prepromptTypesToScan);
}










