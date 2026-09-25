/*! \file
    \brief Конфиг главного приложения - реализация
 */

#include "AppConfig.h"
#include "Preprompt.h"
//
#include "umba/filename.h"
#include "umba/parse_utils.h"

//
#include <algorithm>
#include <utility>

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
bool AppConfigBase::roleSetupFromCli(const std::string &roleSetupStr)
{
    std::string role, roleDef;

    if (!umba::parse_utils::optionStringSplitToPair(roleSetupStr, role, roleDef /* , const std::string &seps=":=" */ ))
        return false;

    return almaiProject.updateRoleFromRoleStringList(role, roleDef, makeSkillPrepareHandler());
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfigBase::addProjectRootMarker(std::string marker)
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
bool AppConfigBase::addProjectRootMarkers(const std::vector<std::string> &markersList)
{
    for(const auto &m : markersList)
    {
        if (!addProjectRootMarker(m))
            return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfigBase::addProjectRootMarkers(const std::string &markersListStr)
{
    // Можно через splitPathList(name,','); // umba::filename?
    // Можно через umba::string::split(p, ',', true /* skipEmpty */ );

    auto markersList = umba::filename::splitPathList(markersListStr, ',');
    return addProjectRootMarkers(markersList);
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfigBase::addLocalization(const std::string &langKeyTextTriplet)
{
    return localizations.addLocalization(langKeyTextTriplet);
}

//--------------------------------------------------------------------------------------------------------------------
std::string AppConfigBase::getLocalizedText(std::string lang, const std::string &key) const
{
    return localizations.getLocalizedText(lang, key);
}

//--------------------------------------------------------------------------------------------------------------------
std::string AppConfigBase::getLocalizedText(const std::string &key) const
{
    return localizations.getLocalizedText(curLang, key);
}

//--------------------------------------------------------------------------------------------------------------------
std::vector<std::string> AppConfigBase::getPrepromptDirs() const
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
std::string AppConfigBase::getPrepromptPathTypeAnnotation(almai::PrepromptPathType ppt) 
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
std::vector<std::pair<almai::PrepromptPathType, std::string> > AppConfigBase::getPrepromptDirsAnnotated() const
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
void AppConfigBase::addPrepromptPath(almai::PrepromptPathType ppt, std::string path)
{
    umba::string::trim(path);

    std::vector<std::string> &ppDirsVec = prepromptDirs[ppt];
    // ppDirsVec.insert(ppDirsVec.begin(), path); // Позже добавлен - приоритет выше
    ppDirsVec.push_back(path);
}

//--------------------------------------------------------------------------------------------------------------------
void AppConfigBase::addPrepromptPath(const std::string &path)
{
    addPrepromptPath(curPrepromptPathType, path);
    
}

//--------------------------------------------------------------------------------------------------------------------
void AppConfigBase::setAppRoot(const std::string &appRoot_, const std::string &appConfPath_)
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
void AppConfigBase::setProjectRoot(const std::string &projectRoot_)
{
    projectRoot = projectRoot_;

    auto tmpPath = umba::filename::appendPath(almaiDir, std::string(".preprompts"));
    // umba::filename::makeAbsPath(std::string(".preprompts.almai"), projectRoot);
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
void AppConfigBase::addEnvironmentPrepromptPaths()
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
bool AppConfigBase::isPathExistOneOf(const std::string &basePath, const std::vector<std::string> &relNames)
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
bool AppConfigBase::isPathExistOneOf(const std::string &basePath, const std::unordered_set<std::string> &relNames)
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
bool AppConfigBase::isPathExistOneOf(const std::string &basePath, const std::string &relName)
{
    auto fullName = umba::filename::makeAbsPath(relName, basePath);
    if (umba::filesys::isPathExist(fullName))
        return true;
    return false;
}

//--------------------------------------------------------------------------------------------------------------------
std::vector<std::string> AppConfigBase::makeAlmaiFolderNames()
{
    return std::vector<std::string>{ ".almai"
                                   , ".ALMAI"
                                   // , ".almai.yaml"
                                   // , ".ALMAI.yaml"
                                   // , ".ALMAI.YAML"
                                   // , ".almai.YAML"
                                   // ,  "almai.yaml"
                                   // ,  "ALMAI.yaml"
                                   // ,  "ALMAI.YAML"
                                   // ,  "almai.YAML"
                                   };
}

//--------------------------------------------------------------------------------------------------------------------
const std::vector<std::string>& AppConfigBase::getAlmaiFolderNames()
{
    static std::vector<std::string> names = makeAlmaiFolderNames();
    return names;
}

//--------------------------------------------------------------------------------------------------------------------
std::vector<std::string> AppConfigBase::getAlmaiFolderFullNames(const std::string &path)
{
    const auto &almaiNames = getAlmaiFolderNames();

    std::vector<std::string> resVec; resVec.reserve(almaiNames.size());

    for(const auto &name : almaiNames)
    {
        resVec.push_back(umba::filename::appendPath(path, name));
    }

    return resVec;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfigBase::isProjectRootPath(const std::string &path, std::string *pAlmaiFolderName) const
{
    auto almaiNames = getAlmaiFolderFullNames(path);

    for(const auto &almaiFolder : almaiNames)
    {
        if (umba::filesys::isDirExist(almaiFolder))
        {
            if (pAlmaiFolderName)
               *pAlmaiFolderName = almaiFolder;
            return true;
        }
    }

    if (pAlmaiFolderName)
       pAlmaiFolderName->clear();

    if (isPathExistOneOf(path, projectRootStopNames))
        return true;

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfigBase::findProjectRoot(std::string startPath)
{
    auto upLevelPath = startPath;
    auto curPath     = startPath;
    do
    {
        curPath = upLevelPath;

        if (isProjectRootPath(curPath, &almaiDir))
        {
            projectFile = umba::filename::appendPath(almaiDir, std::string("PROJECT.yaml"));
            if (!umba::filesys::isFileExist(projectFile))
                projectFile.clear();
            setProjectRoot(curPath);
            return true;
        }

        upLevelPath = umba::filename::getPath(curPath);
    }
    while(upLevelPath!=curPath);

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
// template<>
static
void addUniqueLowerStrToVec(std::vector<std::string> &vec, std::string s)
{
    umba::string::trim(s);
    if (s.empty())
        return;

    auto lowerS = umba::string::tolower_copy(s);

    auto it = std::find_if( vec.begin(), vec.end()
                          , [&](auto cmpTo)
                            {
                                umba::string::tolower(cmpTo);
                                return lowerS==cmpTo;
                            }
                          );
    if (it!=vec.end())
        return;

    vec.push_back(s);
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfigBase::addRoles(const std::string &str)
{
    auto list = umba::filename::splitPathList(str, ' ');
    for(auto s: list)
        addUniqueLowerStrToVec(roles, s);
    
    return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool AppConfigBase::addSkills(const std::string &str)
{
    auto list = umba::filename::splitPathList(str, ' ');
    for(auto s: list)
        addUniqueLowerStrToVec(skills, s);
    
    return true;
}

//--------------------------------------------------------------------------------------------------------------------
std::vector<std::string> AppConfigBase::resolveSkillList(const almai::PrepromptDatabase &ppDb) const
{
    std::vector<std::string> result;

    auto findInResult = [&](const std::string &id) -> std::vector<std::string>::iterator
    {
        return std::find(result.begin(), result.end(), id);
    };

    // --- Шаг 1. Скилы по ролям ---
    for(const auto &roleRaw : roles)
    {
        std::string role = roleRaw;
        umba::string::tolower(role);

        auto itRole = almaiProject.roles.find(role);
        if (itRole==almaiProject.roles.end())
        {
            // Роль не найдена — игнорируем молча (или логировать через warning-хендлер)
            continue;
        }

        for(const auto &skillRaw : itRole->second)
        {
            std::string id = resolveSingleSkillId(ppDb, skillRaw);

            // Добавляем без дубликатов, сохраняя порядок
            if (findInResult(id)==result.end())
                result.push_back(id);
        }
    }

    // --- Шаг 2. Модификаторы из массива skills ---
    for(const auto &mod : skills)
    {
        if (mod.empty())
            continue;

        char        prefix = '+';
        std::string name   = mod;

        if (mod[0]=='+' || mod[0]=='-')
        {
            prefix = mod[0];
            name   = mod.substr(1);
        }

        std::string id = resolveSingleSkillId(ppDb, name);

        auto it = findInResult(id);

        if (prefix=='-')
        {
            if (it!=result.end())
                result.erase(it);
        }
        else
        {
            if (it==result.end())
                result.push_back(id);
        }
    }

    return result;
}

//--------------------------------------------------------------------------------------------------------------------
std::string AppConfigBase::resolveSingleSkillId(const almai::PrepromptDatabase &ppDb, const std::string &skillId) const
{
    PrepromptCategorySetType ppCatSet;

    std::string complete = ppDb.makeCompletePpId(aiName, skillId, &ppCatSet);
    if (!complete.empty())
        return complete;

    // Не нашли — возвращаем как есть (пригодится для диагностики выше по стеку)
    return skillId;
}
