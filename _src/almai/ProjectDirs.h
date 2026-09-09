/*! \file
    \brief Информация по проекту - каталоги, порты, кеши
 */
#pragma once

//----------------------------------------------------------------------------
#include "umba/filesys.h"
#include "marty_cdt/utils.h"
#include "utils.h"
//
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXHttpClient.h>
//
#include <string>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct ProjectDirs
{
    std::string projectPath;
    std::string almaiDir;
    std::string projectFile;

    std::string browserUserDataDir;
    std::string browserCacheDataDir;

    int port = 0;

    bool findProjectRoot(std::string curDir=std::string())
    {
        if (curDir.empty())
            curDir = umba::filesys::getCurrentDir();

        return almai::utils::findProjectRoot(curDir, &projectPath, &almaiDir, &projectFile);
    }

    void generateBrowserDirs(std::string browserName, bool useTempDir=false)
    {
        if (browserName.empty())
            browserName = "chrome";

        std::string projectSubfolder;

        if (!useTempDir)
        {
            projectSubfolder = "." + browserName;
        }

        browserUserDataDir  = marty::cdt::utils::generateChromeUserProfileFolderForProject(projectPath, browserName, projectSubfolder);
        browserCacheDataDir = marty::cdt::utils::generateChromeUserCacheFolderForProject(projectPath, browserName, projectSubfolder);

        umba::filesys::createDirectoryEx(browserUserDataDir , true /* forceCreatePath */ );
        umba::filesys::createDirectoryEx(browserCacheDataDir, true /* forceCreatePath */ );
    }

    void generateProjectConnectionPort(int startPort=9000, int range=1000)
    {
        port = marty::cdt::utils::generatePortNumberForProjectConnection(projectPath, startPort, range);
    }

    std::string generateConnectionBaseUrlString(bool bHttp) const
    {
        return marty::cdt::utils::generateConnectionBaseUrlString(port, bHttp);
    }

    std::vector<std::string> generateArgsForSpawnChrome(bool restoreLastSession=false) const
    {
        return marty::cdt::utils::generateArgsForSpawnChromeExactDirs(browserUserDataDir, browserCacheDataDir, port, restoreLastSession);
    }
    

    bool initAll( std::string chromeName=std::string()
                , bool        useTempDir=false
                , std::string curDir=std::string()
                , int startPort=9000, int range=1000
                )
    {
        if (!findProjectRoot(curDir))
            return false;

        generateBrowserDirs(chromeName, useTempDir);                 
        generateProjectConnectionPort(startPort, range);

        return true;
    }



}; // struct ProjectDirs


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------


