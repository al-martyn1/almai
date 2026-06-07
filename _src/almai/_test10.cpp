/*! \file
    \brief Тесты запуска сторонних программ (без ожидания)
 */

#include "utils.h"
//
#include "umba/shellapi.h"
#include "umba/win32_utils.h"
#include "umba/cbp/utils.h"
//
#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
void testFindExe(const std::string &exeName)
{
    std::vector<std::string> foundExes;
    umba::shellapi::findExecutable(exeName, foundExes);

    if (foundExes.empty())
    {
        cout << exeName << " not found\n";
    }
    else
    {
        cout << "found " << exeName << " executables:\n";
        for(auto &&chExe : foundExes)
           cout << "  " << chExe << "\n";
    }
}

//----------------------------------------------------------------------------
/*
    Расширение CMake Tools не полагается исключительно на системный PATH. Оно использует собственную подсистему 
    обнаружения «китов» (Kits), которая агрегирует данные из нескольких источников и затем передаёт CMake 
    необходимые пути и CMAKE_TOOLCHAIN_FILE.

    1. Системный PATH – сканируются все директории из переменной окружения.
    2. Жёстко заданные стандартные пути (зависят от ОС):
      - Windows: C:\Program Files\LLVM, C:\MinGW, C:\Strawberry\c, C:\msys64\mingw64, C:\Qt\..., пути от vswhere (Visual Studio), реестр HKLM\SOFTWARE\Microsoft\VisualStudio
      - Linux: /usr, /usr/local, /opt, ~/.local, ~/.cargo/bin
      - macOS: /Applications/Xcode.app/Contents/Developer, /opt/homebrew, /usr/local
    3. Файлы пресетов CMake – CMakePresets.json и CMakeUserPresets.json (начиная с CMake 3.19+). Пресеты могут явно указывать toolchainFile или binaryDir.
    4. Конфигурационные файлы китов:
      - Глобальный: ~/.cmake-tools/kits/cmake-tools-kits.json (генерируется автоматически)
      - Рабочей области: .vscode/cmake-tools-kits.json или cmake-kits.json в корне проекта
    5. Пользовательские настройки VSCode: cmake.additionalKits (массив путей к JSON-файлам с китами).

    Сканер ищет известные имена исполняемых файлов компиляторов/линкеров/отладчиков: 
    gcc, g++, clang, clang++, arm-none-eabi-gcc, riscv64-unknown-elf-gcc, ninja, make и т.д.

    Если архив распакован в одну из сканируемых директорий (например, ~/tools/, /opt/, C:\tools\), 
    и внутри есть папка bin/ с этими файлами, сканер подхватит их автоматически.

    Некоторые версии расширения выполняют рекурсивный поиск в определённых префиксах, особенно если включён cmake.scanForKits: true (по умолчанию).
    Если тулсет следует стандартной иерархии (bin/, lib/, include/, share/cmake/), CMake Tools распознаёт его как валидный кит.

    Настройка,Описание
    cmake.scanForKits,true/false. Включает/отключает автопоиск.
    cmake.additionalKits,Массив путей к пользовательским JSON-файлам китов.
    cmake.preferredGenerators,Приоритетные генераторы (Ninja, Unix Makefiles, MSYS Makefiles и т.д.).
    cmake.environment,Переменные окружения, передаваемые CMake и сканеру.
*/
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/*
    В переменной среды PATHEXT в Windows указаны расширения исполняемых файлов, там есть .BAT и .JS. 
    Они запускаются через CreateProcess или Windows как-то более сложно делает для файлов, отличных от PE Executable?

    Короткий ответ: PATHEXT не имеет никакого отношения к CreateProcess и к загрузчику Windows. Это исключительно 
    механизм разрешения команд в командной оболочке (cmd.exe) и некоторых API поиска путей. Для файлов, отличных от PE-образов, 
    Windows не запускает их "напрямую", а делегирует выполнение соответствующему интерпретатору или хосту.

*/
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    // about reparse points - проверить - https://chat.deepseek.com/share/ezcvr9tgm1zxkvvl11

    // std::string pathVarValue;
    // umba::env::getVar(std::string("PATH"), pathVarValue);
    // cout << "PATH: " << pathVarValue << "\n\n";

    // bool isPythonFile = umba::filesys::isPathFile("C:\\Users\\martinov\\AppData\\Local\\Microsoft\\WindowsApps\\python.exe");
    //  
    // cout << "isPathFile(C:\\Users\\martinov\\AppData\\Local\\Microsoft\\WindowsApps\\python.exe): " 
    //      << isPythonFile
    //      << "\n\n";


    // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths
    // HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths

    std::string browserExeStr;
    LSTATUS status;

    if (!umba::win32_utils::regGetValue( HKEY_CURRENT_USER
                                       , std::string("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\browser.exe")
                                       , std::string("")
                                       , browserExeStr
                                       , 0 // samDesired
                                       , 0 // pType
                                       , &status
                                       )
       )
    {
        
        cout << "regGetValue failed: " << umba::win32::getStrErrorNameStr((unsigned)status) << "\n";
    }
    else
    {
        cout << "browserExeStr: " << browserExeStr << "\n";
    }

    testFindExe("chrome");
    testFindExe("Python");
    testFindExe("py");
    testFindExe("g++");
    testFindExe("vswhere");
    testFindExe("umba-md-pp");
    testFindExe("git");


    auto curDir = umba::filesys::getCurrentDirectory();
    auto userProfileFolderProject = umba::cdp::utils::generateChromeUserProfileFolderForProject(curDir, std::string(), std::string(".almai-working-files"));
    auto userCacheFolderProject   = umba::cdp::utils::generateChromeUserCacheFolderForProject  (curDir, std::string(), std::string(".almai-working-files"));
    auto userProfileFolderGlobal  = umba::cdp::utils::generateChromeUserProfileFolderForProject(curDir, std::string(), std::string());
    auto userCacheFolderGlobal    = umba::cdp::utils::generateChromeUserCacheFolderForProject  (curDir, std::string(), std::string());

    cout << "userProfileFolderProject: " << userProfileFolderProject << "\n";
    cout << "userCacheFolderProject  : " << userCacheFolderProject   << "\n";
    cout << "userProfileFolderGlobal : " << userProfileFolderGlobal  << "\n";
    cout << "userCacheFolderGlobal   : " << userCacheFolderGlobal    << "\n";


    std::string errMsg;
    // chrome.exe --remote-debugging-port=9222 --user-data-dir=C:\temp\chrome-profile
    // C:\Users\martinov\AppData\Local\Google\Chrome\Application\chrome.exe
    auto res = umba::shellapi::spawnProcess( "chrome" // "chrome.exe"
                                          , std::vector<std::string>{ "--new-window" // "--new-tab"
                                                                    , "--remote-debugging-port=9222"
                                                                    , "--user-data-dir=C:\\temp\\test-chrome-profile"
                                                                    , "--disk-cache-dir=C:\\temp\\test-chrome-cache"
                                                                    }
                                          , umba::shellapi::SpawnProcessFlags::default_ | umba::shellapi::SpawnProcessFlags::argvFromCmd
                                          , &errMsg
                                          );
    if (res==std::uintptr_t(-1))
        cout << "Error: " << errMsg << "\n";
    else
        cout << "Chrome PID: " << res << "\n";

    return 0;
}


