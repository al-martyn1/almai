**\_src/almai/\_test01.cpp**
~~~C++
/*! \file
    \brief Тесты утилитарных функций
 */

#include "utils.h"
#include "md_utils.h"
//
#include "marty_url/percent_encoding.h"

#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

//----------------------------------------------------------------------------
inline
void performMakeNormalizedRelativePathTest(const std::string &name)
{
    cout << "  [" << name << "] - [" << md::makeNormalizedRelativePath(name) << "]\n";
}

//----------------------------------------------------------------------------
inline
void performMakeNormalizedRelativePathTests()
{
    cout << "**test01/makeNormalizedRelativePath.txt**\n";
    cout << "```\n";
    performMakeNormalizedRelativePathTest("C:\\Windows\\Program Files");
    performMakeNormalizedRelativePathTest("\\\\?\\C:\\Windows\\Program Files");
    performMakeNormalizedRelativePathTest("\\\\?\\share\\c$\\Windows\\Program Files");
    performMakeNormalizedRelativePathTest("/usr/share/bin");
    performMakeNormalizedRelativePathTest("~/bin");
    cout << "```\n\n";
}

//----------------------------------------------------------------------------
inline
void performFormatFileSizeTest(std::size_t sz, bool dec)
{
    cout << sz << ": " << almai::utils::formatFileSize(sz, dec) << "\n";
}

//----------------------------------------------------------------------------
inline
void performFormatFileSizeTests(bool dec)
{
    std::string decStr = dec?"dec":"bin";
    cout << "**test01/formatFileSize_" << decStr << ".txt**\n";
    cout << "```\n";
    // cout << "formatFileSize test (" << decStr << "):\n";

    std::size_t sz = 64;
    std::size_t incSz = 699;
    const std::size_t maxSize = 100*1024*1024; // Тестируем до 100 Мб размера

    while(sz<maxSize)
    {
        performFormatFileSizeTest(sz, dec);

        if (sz<12800)
            sz += incSz;
        else if (sz<128000)
            sz += incSz*9;
        else if (sz<1280000)
            sz += incSz*9*9;
        else if (sz<12800000)
            sz += incSz*9*9*9;
        else
            sz += incSz*9*9*9*9;
    }

    cout << "```\n\n";
}

//----------------------------------------------------------------------------
inline
void performGetPathNameFileExtTest(const std::string &pathIn)
{
    cout << "Full name: '" << pathIn << "'\n";

// getFileName - Извлекает из полного пути имя файла + расширение
// getPathFile - Извлекает из полного пути путь и имя файла без расширения
// getFileExtention - Извлекает из имени расширение
// getName - Извлекает из имени имя файла без пути и расширения

    auto path     = umba::filename::getPath(pathIn)         ;
    auto fileName = umba::filename::getFileName(pathIn)     ;
    auto pathFile = umba::filename::getPathFile(pathIn)     ;
    auto fileExt  = umba::filename::getFileExtention(pathIn);
    auto nme      = umba::filename::getName(pathIn)         ;

    cout << "  path     : '" << path     << "'\n";
    cout << "  file name: '" << fileName << "'\n";
    cout << "  path file: '" << pathFile << "'\n";
    cout << "  ext      : '" << fileExt  << "'\n";
    cout << "  name     : '" << nme      << "'\n";

    cout << "\n";
}

inline
void performGetPathNameFileExtTests()
{
    std::vector<std::string> localNames =
    { "//"
    , "c:tmp.txt"
    , "c:\\windows\\temp\\tmp.txt"
    , "c:\\windows\\temp\\\\tmp.txt"
    , "c:\\windows\\temp\\\\tmp.txt"
    , "c:\\windows\\temp/tmp.txt"
    , "c:\\windows\\temp//tmp.txt"
    , "c:\\windows\\temp///tmp.txt"
    , "c:\\tmp.txt"
    , "c:\\\\tmp.txt"
    , "c:/tmp.txt"
    , "c://tmp.txt"
    , "/"
    };

    std::vector<std::string> networkNames =
    { "c\\tmp.txt"
    , "c\\windows\\temp\\tmp.txt"
    , "c\\windows\\temp\\\\tmp.txt"
    , "c\\windows\\temp\\\\tmp.txt"
    , "c\\windows\\temp/tmp.txt"
    , "c\\windows\\temp//tmp.txt"
    , "c\\windows\\temp///tmp.txt"
    , "c\\tmp.txt"
    , "c\\\\tmp.txt"
    , "c/tmp.txt"
    , "c//tmp.txt"
    , "/"
    , "//"
    };

    std::vector<std::string> testNames;

    for(const auto &n : localNames)
    {
        testNames.push_back(n);
        testNames.push_back(n + ".zip");
        testNames.push_back(n + ":stream");
        testNames.push_back(n + ":stream.txt");
        testNames.push_back(n + ".zip:stream");
        testNames.push_back(n + ".zip:stream.txt");

        testNames.push_back("\\\\?\\" + n);
        testNames.push_back("\\\\?\\" + n + ".zip");
        testNames.push_back("\\\\?\\" + n + ":stream");
        testNames.push_back("\\\\?\\" + n + ":stream.txt");
        testNames.push_back("\\\\?\\" + n + ".zip:stream");
        testNames.push_back("\\\\?\\" + n + ".zip:stream.txt");

        testNames.push_back("\\\\.\\" + n);
        testNames.push_back("\\\\.\\" + n + ".zip");
        testNames.push_back("\\\\.\\" + n + ":stream");
        testNames.push_back("\\\\.\\" + n + ":stream.txt");
        testNames.push_back("\\\\.\\" + n + ".zip:stream");
        testNames.push_back("\\\\.\\" + n + ".zip:stream.txt");
    }


    std::string serverShare       = "server\\share";
    std::string dotServerShare    = "ser.ver\\share";
    std::string serverDotShare    = "server\\sh.are";
    std::string dotServerDotShare = "ser.ver\\sh.are";

    for(const auto &n : networkNames)
    {
        testNames.push_back(serverShare       + n);
        testNames.push_back(dotServerShare    + n);
        testNames.push_back(serverDotShare    + n);
        testNames.push_back(dotServerDotShare + n);

        testNames.push_back(serverShare       + n + ".zip");
        testNames.push_back(dotServerShare    + n + ".zip");
        testNames.push_back(serverDotShare    + n + ".zip");
        testNames.push_back(dotServerDotShare + n + ".zip");
    }



    cout << "**test01/getPathNameFileExt.txt**\n";
    cout << "```\n";

    for(const auto n: testNames)
        performGetPathNameFileExtTest(n);

    cout << "```\n";

}

//----------------------------------------------------------------------------
void performExtractListingFilenamesTest(const std::string &lines)
{
    std::cout << "\n";

    auto lastSignificantLines = almai::utils::splitTextToLines(lines);

    std::vector<std::string> filenames;
    bool hasEdging = md::findListingFilenames(lastSignificantLines, true /*bCommentPrependName*/, &filenames);

    for(auto &name : filenames)
    {
        name = md::replaceInvalidPathNameChars(name, !hasEdging);
        name = md::makeNormalizedRelativePath(name);
        std::cout << name << "\n";
    }

}

void performExtractListingFilenamesTests()
{
    std::cout << "\n";

    cout << "**test01/ExtractListingFilenamesTests**" << "\n";

    performExtractListingFilenamesTest("Some text\n"
                                       "**\\_src/lsp\\_tests/lsp\\_test\\_server2.cpp (новый тестовый файл)**\n"
                                       "**\\_src/lsp\\_tests/lsp\\_test\\_server.cpp** (новый тестовый файл)\n"
                                      );

    std::cout << "\n";
}

//----------------------------------------------------------------------------
void performUrlEncodeTest(const std::string &url)
{
    auto urlEncoded   = marty::url::urlEncode(url);
    auto urlComponent = marty::url::urlEncodeComponent(url);
    auto urlDecoded1  = marty::url::urlDecode(urlEncoded);
    auto urlDecoded2  = marty::url::urlDecode(urlComponent);

    cout << "URL      : " << url          << "\n";
    cout << "Encoded  : " << urlEncoded   << "\n";
    cout << "Component: " << urlComponent << "\n";
    cout << "Decoded1 : " << urlDecoded1  << " ("<< (urlDecoded1==url ? "+" : "-") << ")" << "\n";
    cout << "Decoded2 : " << urlDecoded2  << " ("<< (urlDecoded2==url ? "+" : "-") << ")" << "\n";

    std::cout << "\n";
}

//----------------------------------------------------------------------------
void performUrlEncodeTests()
{
    std::cout << "\n";

    cout << "**test01/UrlEncodeTests**" << "\n";

    //performUrlEncodeTest("https://www.google.com/search?q=спрей+10%25+можно+купить+без+рецепта&oq=спрей+10%25+можно+купить+без+рецепта&gs_lcrp=EgZjaHJvbWUyBggAEEUYOTIHCAEQIRiSAzIHCAIQIRiSAzIHCAMQIRiSAzIHCAQQIRiSAzIHCAUQIRiSAzIHCAYQIRiSA9IBCTE1NDcxajBqN6gCCLACAfEFDmndytbEcKQ&sourceid=chrome&source=chrome.ob&ie=UTF-8");
    performUrlEncodeTest("https://www.google.com/search?q=спрей+10%+можно+купить+без+рецепта&sourceid=chrome&source=chrome.ob&ie=UTF-8");
    performUrlEncodeTest("http://аптека.рф");
    performUrlEncodeTest("http://аптека.рф/search?q=спрей 10%&location=Москва");
    // performUrlEncodeTest("");

    std::cout << "\n";
}

//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    performExtractListingFilenamesTests();

    performGetPathNameFileExtTests();

    performMakeNormalizedRelativePathTests();
    performFormatFileSizeTests(false);
    performFormatFileSizeTests(true );
    performUrlEncodeTests();

    return 0;
}
~~~

**\_src/almai/\_test02.cpp**
~~~C++
/*! \file
    \brief Тест задания путей поиска
 */

#include "utils.h"
#include "FileSystemScanInfo.h"
#include "FoundFileInfo.h"
//

#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

// //----------------------------------------------------------------------------
// inline
// void makeNormalizedRelativePathTest(const std::string &name)
// {
//     cout << "  [" << name << "] - [" << makeNormalizedRelativePath(name) << "]\n";
// }
//
// //----------------------------------------------------------------------------
// inline
// void makeNormalizedRelativePathTests()
// {
//     cout << "makeNormalizedRelativePath:\n";
//     makeNormalizedRelativePathTest("C:\\Windows\\Program Files");
//     makeNormalizedRelativePathTest("\\\\?\\C:\\Windows\\Program Files");
//     makeNormalizedRelativePathTest("\\\\?\\share\\c$\\Windows\\Program Files");
//     makeNormalizedRelativePathTest("/usr/share/bin");
//     makeNormalizedRelativePathTest("~/bin");
// }




//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::vector<FileSystemScanInfo> scanInfos;

    for(int i=1; i<argc; ++i)
    {
        scanInfos.emplace_back(FileSystemScanInfo::parse(argv[i]));
    }

    if (scanInfos.empty())
    {
        cout << "Nothing to scan\n";
    }
    else
    {
        cout << "Scan paths & masks:\n";
        for(auto &&s : scanInfos)
        {
            cout << "  " << s.toString() << "\n";
        }

        cout << "\n";
    }

    std::vector<std::string> foundFiles;
    for(const auto &s : scanInfos)
    {
        std::vector<std::string> tmp;
        s.scanForFiles(tmp);
        foundFiles.insert(foundFiles.end(), tmp.begin(), tmp.end());
    }


    std::vector<FoundFileInfo>  foundFileInfos;

    if (foundFiles.empty())
    {
        cout << "Files not found\n";
    }
    else
    {
        cout << "Found files:\n";
        for( auto &&f: foundFiles)
        {
            cout << "  " << f << "\n";
            foundFileInfos.emplace_back(f);
        }

        cout << "\n";

        std::string commonPrefix = findMostCommonPathPrefix(foundFileInfos.begin(), foundFileInfos.end());
        commonPrefix = checkCorrectMostCommonPathPrefixIsPath(commonPrefix);

        if (commonPrefix.empty())
        {
            cout << "Common prefix not found\n";
            cout << "\n";
        }
        else
        {
            cout << "Found Common prefix: " << commonPrefix << "\n\n";

            for(auto &ffi: foundFileInfos)
            {
                ffi.stripPrefix(commonPrefix);
            }

        }

        cout << "Found files (stripped prefix):\n";
        for(auto &ffi: foundFileInfos)
        {
            cout << "  " << ffi.displayName << "\n";
        }

        cout << "\n";

        sortFoundFileInfos(foundFileInfos);

        cout << "Found files (sorted):\n";
        for(auto &ffi: foundFileInfos)
        {
            cout << "  " << ffi.displayName << "\n";
        }

        cout << "\n";

        // sortFoundFileInfos
    }


    return 0;
}
~~~

**\_src/almai/\_test03.cpp**
~~~C++
/*! \file
    \brief Тестируем разбор "MdXml"
 */

#include "utils.h"
#include "MdXml.h"
#include "marty_cpp/src_normalization.h"
#include "umba/filesys.h"
#include "umba/shellapi.h"
//

#include <iostream>

using std::cout;
using std::cerr;



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::string filename;

    if (argc>1)
        filename = argv[1];

    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        filename = rootPath + "/tests/_test03_01.md";
    }

    if (filename.empty())
    {
        cerr << "No input file name taken\n";
        return 1;
    }


    std::string text;

    if (!umba::filesys::readFile(filename, text))
    {
        cerr << "Failed to read input file: '" << filename << "'\n";
        return 1;
    }

    std::vector<std::string> inputLines = marty_cpp::splitToLinesSimple(text);

    almai::mdxml::XmlTag mdXml;

    try
    {
        mdXml = almai::mdxml::parseMarkdownXml(inputLines.begin(), inputLines.end(), true /* throwErrors */ );
    }
    catch(const std::exception &e)
    {
        cerr << "Error parsing MdXml: " << e.what() << "\n";
        mdXml = almai::mdxml::parseMarkdownXml(inputLines.begin(), inputLines.end(), false /* throwErrors */ );
    }

    auto mdXmlAllText = mdXml.getTextAll();

    for(auto &&l : mdXmlAllText)
    {
        cout << l << "\n";
    }

    return 0;
}
~~~

**\_src/almai/\_test04.cpp**
~~~C++
/*! \file
    \brief Тестируем разбор frontmatter
 */

#include "utils.h"
#include "Descriptions.h"
//
#include "marty_cpp/src_normalization.h"
#include "umba/filesys.h"
#include "umba/shellapi.h"
//

#include <iostream>

using std::cout;
using std::cerr;



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::string filename;

    if (argc>1)
        filename = argv[1];

    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        filename = rootPath + "/tests/_test04_02.yaml";
    }

    if (filename.empty())
    {
        cerr << "No input file name taken\n";
        return 1;
    }


    std::string text;

    if (!umba::filesys::readFile(filename, text))
    {
        cerr << "Failed to read input file: '" << filename << "'\n";
        return 1;
    }

    std::vector<std::string> inputLines = marty_cpp::splitToLinesSimple(text);

    almai::PrepromptDescription ppd;

    try
    {
        ppd = almai::PrepromptDescription::parse(inputLines);
        cout << "Description parsed well";
    }
    catch(const std::exception &e)
    {
        cerr << "Error while parsing YAML: " << e.what() << "\n";
    }

#if 0

    almai::mdxml::XmlTag mdXml;

    try
    {
        mdXml = almai::mdxml::parseMarkdownXml(inputLines.begin(), inputLines.end(), true /* throwErrors */ );
    }
    catch(const std::exception &e)
    {
        cerr << "Error parsing MdXml: " << e.what() << "\n";
        mdXml = almai::mdxml::parseMarkdownXml(inputLines.begin(), inputLines.end(), false /* throwErrors */ );
    }

    auto mdXmlAllText = mdXml.getTextAll();

    for(auto &&l : mdXmlAllText)
    {
        cout << l << "\n";
    }
#endif

    return 0;
}
~~~

**\_src/almai/\_test05.cpp**
~~~C++
/*! \file
    \brief Тестируем чтение препромпта
 */

#include "utils.h"
#include "Preprompt.h"
//
#include "marty_cpp/src_normalization.h"
#include "umba/filesys.h"
#include "umba/shellapi.h"
//

#include <iostream>

using std::cout;
using std::cerr;



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::string filename;

    if (argc>1)
        filename = argv[1];

    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        filename = rootPath + "/tests/_test05_01.md";
    }

    if (filename.empty())
    {
        cerr << "No input file name taken\n";
        return 1;
    }


    std::string text;

    if (!umba::filesys::readFile(filename, text))
    {
        cerr << "Failed to read input file: '" << filename << "'\n";
        return 1;
    }


    almai::Preprompt pp;

    try
    {
        pp = almai::Preprompt::parse(text, true /* throwErrors */ );
        cout << "Preprompt parsed well";
    }
    catch(const std::exception &e)
    {
        cerr << "Error parsing preprompt: " << e.what() << "\n";
        pp = almai::Preprompt::parse(text, false /* throwErrors */ );
    }

    auto ppSects = pp.findSections();

    cout << "Preprompt: " << pp.description.name << "\n";
    if (!pp.description.description.empty())
       cout << "  (" << pp.description.description << ")\n";

    cout << "Sections:";
    if (!ppSects.empty())
        cout << "\n";
    else
        cout << " not found\n";

    for(auto sect : ppSects)
    {
        cout << "  " << sect << "\n";
    }

    cout << "\n";


#if 0

    almai::mdxml::XmlTag mdXml;

    try
    {
        mdXml = almai::mdxml::parseMarkdownXml(inputLines.begin(), inputLines.end(), true /* throwErrors */ );
    }
    catch(const std::exception &e)
    {
        cerr << "Error parsing MdXml: " << e.what() << "\n";
        mdXml = almai::mdxml::parseMarkdownXml(inputLines.begin(), inputLines.end(), false /* throwErrors */ );
    }

    auto mdXmlAllText = mdXml.getTextAll();

    for(auto &&l : mdXmlAllText)
    {
        cout << l << "\n";
    }
#endif

    return 0;
}
~~~

**\_src/almai/\_test06.cpp**
~~~C++
/*! \file
    \brief Тестируем сканирование препромптов (пока только в props и только в одном каталоге)
 */

#include "utils.h"
#include "Preprompt.h"
//
#include "marty_cpp/src_normalization.h"
#include "umba/filesys.h"
#include "umba/shellapi.h"
#include "umba/debug_helpers.h"

//

#include <iostream>

using std::cout;
using std::cerr;



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::string filename;

    if (argc>1)
        filename = argv[1];

    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        filename = rootPath + "/tests/test06";
    }

    if (filename.empty())
    {
        cerr << "No input path name taken\n";
        return 1;
    }

    auto ppVec = almai::PrepromptProps::scanPath(filename, "skills");

    cout << "Scan path: " << filename << "\n";
    cout << ppVec << "\n";
    if (ppVec.empty())
        cout << "Nothing was found\n";

    return 0;
}
~~~

**\_src/almai/\_test07.cpp**
~~~C++
/*! \file
    \brief Тестируем переименование файла
 */

#include "utils.h"
#include "Preprompt.h"
//
#include "marty_cpp/src_normalization.h"
#include "umba/filesys.h"
#include "umba/shellapi.h"
#include "umba/debug_helpers.h"

//

#include <iostream>

using std::cout;
using std::cerr;



//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    std::string oldName, newName;

    if (argc>2)
    {
        oldName = argv[1];
        newName = argv[2];
    }

    if (umba::isDebuggerPresent())
    {
        // std::string cwd;
        // std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        // std::cout << "App Root Path: " << rootPath << "\n";
        // std::cout << "Working Dir  : " << cwd << "\n";
        //
        // filename = rootPath + "/tests/test06";
    }

    if (oldName.empty())
    {
        cerr << "No ExistingFileName taken\n";
        return 1;
    }

    if (newName.empty())
    {
        cerr << "No NewFileName taken\n";
        return 1;
    }

    if (umba::filesys::moveFileOrDir(oldName, newName))
    {
        cerr << "OK: " << oldName << " -> " << newName << "\n";
    }
    else
    {
        cerr << "Failed to move (rename): " << oldName << " -> " << newName << "\n";
        return 1;
    }

    return 0;
}
~~~

**\_src/almai/\_test08.cpp**
~~~C++
/*! \file
    \brief Тестируем umba::command_line::CommandSequenceController
 */


// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/debug_helpers.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>

#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"

#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

// bool bOverwrite         = false;

//
#include "log.h"
//
#include "AppConfig.h"

//almai::
AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;


#include "Test08ArgParser.h"



int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        //std::cout << "Error: " << e.what() << "\n";
        LOG_ERR << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        LOG_ERR << "unknown error\n";
        //std::cout << "Unknown error\n";
        return 2;
    }

}



int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    #if 0
    if (umba::isDebuggerPresent())
    {
        // argsParser.args.clear();
        // argsParser.args.push_back("--overwrite");

        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        argsParser.args.clear();
        argsParser.args.push_back("--overwrite");
        argsParser.args.push_back("-q");

        // argsParser.args.push_back("submodule");
        // argsParser.args.push_back("--init");
        // argsParser.args.push_back("--recursive");
        // argsParser.args.push_back("update");

        // argsParser.args.push_back("branch");
        // //argsParser.args.push_back("-d");

        // argsParser.args.push_back("worktree");
        // argsParser.args.push_back("remove");

        //argsParser.args.push_back("add");

        // argsParser.args.push_back("--bash");
        // argsParser.args.push_back("--clink");
        //argsParser.args.push_back("--md");

        // argsParser.args.push_back("branch");
        // argsParser.args.push_back("--delete");

        // argsParser.args.push_back("stash");
        //argsParser.args.push_back("list");

        argsParser.args.push_back("--help");

        // argsParser.args.push_back("../backup-branch");
        // argsParser.args.push_back("backup");
        // argsParser.args.push_back("--help");

    } // if (umba::isDebuggerPresent())
    #endif

    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        return 1;
    }

    //!!! --------------

    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        return 1;
    }

    if (argsParser.mustExit)
        return 0;


    return argsParser.argParser.cmdController.executeCommand();

    // return 0;
}
~~~

**\_src/almai/\_test09.cpp**
~~~C++
/*! \file
    \brief Тестируем фичи almai, не захламляя проект almai
 */


// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/debug_helpers.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>

#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"

#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//

#include "PrepromptDatabase.h"

//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

// bool bOverwrite         = false;

//
#include "log.h"
//
#include "AppConfig.h"

//almai::
AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;


#include "ArgParser.h"



int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
        return 2;
    }

}



int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            , "almai" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        // argsParser.args.clear();
        // argsParser.args.push_back("--overwrite");

        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

    } // if (umba::isDebuggerPresent())


    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        return 1;
    }

    //!!! --------------
    appConfig.setAppRoot(argsParser.getAppRoot(), argsParser.getAppConfPath()); // to find prompts

    if (!appConfig.findProjectRoot()) // Также устанавливает ProjectRoot
    {
        LOG_WARN("prj-root") << "project root not found\n";
    }
    else
    {
        if (!argsParser.quet)
        {
            LOG_MSG << "\n";

            LOG_MSG << "found project root: '" << appConfig.projectRoot << "'\n";
            if (appConfig.projectFile.empty())
                LOG_MSG << "project file not found\n";
            else
                LOG_MSG << "found project file: '" << appConfig.projectFile << "'\n";

            LOG_MSG << "\n";
        }
    }

    appConfig.addEnvironmentPrepromptPaths();
    appConfig.curPrepromptPathType = almai::PrepromptPathType::cliOptions;


    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        return 1;
    }

    if (argsParser.mustExit)
        return 0;


    //if (!argsParser.quet)
    if (1)
    {
        LOG_MSG << "Preprompt dirs:\n";

        auto ppDirsAnnotated = appConfig.getPrepromptDirsAnnotated();
        for(auto &&ppdp : ppDirsAnnotated)
        {
            LOG_MSG << "  " << appConfig.getPrepromptPathTypeAnnotation(ppdp.first) << ": " << ppdp.second << "\n";
        }

        LOG_MSG << "\n";
    }

    if (!argsParser.quet  /* && !hasHelpOption */ )
    {
    }

    if (1)
    {
        LOG_MSG << "Translations test:\n";

        auto printTranslation = [&](std::string lang, std::string key)
        {
            auto text = appConfig.getLocalizedText(lang, key);
            LOG_MSG << "  " << lang << ":" << key << ": " << text << "\n";
        };

        printTranslation("ru", "roles");
        printTranslation("en", "roles");
        printTranslation("ru", "constraints");
        printTranslation("en", "constraints");
        printTranslation("ru", "domains");
        printTranslation("en", "domains");

        LOG_MSG << "\n";
    }


    std::string
    processedFileType = "project";

    auto prepromptReadingErrorHandler = [&](const std::string &ppFilename)
    {
        LOG_WARN("read-error") << "failed to read " << processedFileType << " file: '" << ppFilename << "'\n";
    };

    auto prepromptParsingErrorHandler = [&](const std::string &ppFilename, const std::exception &e)
    {
        LOG_WARN("parsing-error") << "failed to parsing " << processedFileType << " file: " << e.what() << ", file: '" << ppFilename << "'\n";
    };

    auto warningHandler = [&](const std::string &msg)
    {
        LOG_WARN("checking-error") << "checking error: " << msg << "\n";
    };

    appConfig.readProjectFile(prepromptReadingErrorHandler, prepromptParsingErrorHandler);

    // appConfig.curAiEngine = "deepseek"; // !!! Должно вычитываться из настроек проекта
    std::vector<std::string> aiEngines = {"deepseek", "qwen"};

    almai::PrepromptDatabase ppDb = almai::PrepromptDatabase(appConfig.pluralDb, appConfig.getPrepromptDirs());
    ppDb.prepromptDirs = appConfig.getPrepromptDirs();
    std::vector<std::string> scannedFolders;

    processedFileType = "preprompt";

    ppDb.scanForPreprompts( &scannedFolders, aiEngines
                          , { "skill", "instruction", "knowledge", "format", "output" }
                          , prepromptReadingErrorHandler, prepromptParsingErrorHandler
                          );

    appConfig.projectCheckNormalize(ppDb, warningHandler, warningHandler);
    std::cout << appConfig.almaiProject <<"\n";


    if (1)
    {

        LOG_MSG << "Scanned folders:\n";

        for(const auto &fldr: scannedFolders)
        {
            LOG_MSG << "  " << fldr << "\n";
        }

        LOG_MSG << "\n";


        LOG_MSG << "Found preprompts:\n";

        for(const auto &[aiEngine, aiPreprompts] : ppDb.preprompts)
        {
            LOG_MSG << "\n";
            LOG_MSG << "AiEngine: " << (!aiEngine.empty() ? aiEngine : std::string("<NONAME>")) << "\n";

            LOG_MSG << "\n  Found preprompts:\n";

            for(const auto &[ppTypeStr, ppNameMap] : aiPreprompts.prepromptProps)
            {
                LOG_MSG << "    " << ppTypeStr << ":\n";

                for(const auto &[ppName, ppProps] : ppNameMap)
                {
                    LOG_MSG << "      " << ppProps << "\n";
                }
            }

            LOG_MSG << "\n";


            LOG_MSG << "  Found preprompt types:\n";

            for(const auto &[ppId, ppTypeSet] : aiPreprompts.prepromptCategories)
            {
                std::size_t cnt = 0;
                LOG_MSG << "    " << ppId; // << ""
                for(const auto ppType: ppTypeSet)
                {
                    LOG_MSG << (cnt ? ", " : ": ") << ppType;
                    ++cnt;
                }
                LOG_MSG << "\n";
            }

            LOG_MSG << "\n";
        }

        LOG_MSG << "\n";

    }


    auto checkPreprompt = [&](auto ppName)
    {
        almai::PrepromptCategorySetType ppCatSet;

        auto completePpName = ppDb.makeCompletePpId("", ppName, &ppCatSet);

        if (!completePpName.empty())
        {
            LOG_MSG << "+ Complete name of '" << ppName << "': " << completePpName << "\n";
        }
        else
        {
            UMBA_ASSERT(ppCatSet.size()!=1); // Не может быть равно 1

            if (ppCatSet.empty())
            {
                LOG_MSG << "- Preprompt '" << ppName << "' not found\n";
            }
            else
            {
                LOG_MSG << "- Preprompt '" << ppName << "' found in multiple categories:\n";
                for(auto && cat: ppCatSet)
                    LOG_MSG << "  - " << cat << "\n";
            }
        }

        LOG_MSG << "\n";

    }; // auto checkPreprompt

    LOG_MSG << "\n\n";
    LOG_MSG << "Test preprompt names completion:\n\n";
    checkPreprompt("tester");
    checkPreprompt("bugster"); // bug buster/bug hunter (QA) // Ghostbusters - https://chat.deepseek.com/share/qa5vqd0dkprxc1yss2
    checkPreprompt("instruction/tester");
    checkPreprompt("skills/tester");
    checkPreprompt("auto-tester");
    checkPreprompt("c-dev");
    checkPreprompt("jni-master");
    checkPreprompt("ui-tester");

    // checkPreprompt("");


    auto checkPreprompt2 = [&](auto ppName)
    {
        almai::PrepromptCategorySetType ppCatSet;

        auto completePpName = ppDb.makeCompletePpId("", ppName, &ppCatSet);

        bool bGood = false;

        auto msg = ppDb.makeCompletePpIdErrorMsg("", ppName, completePpName, ppCatSet, &bGood);

        LOG_MSG << (bGood ? "+" : "-" ) << " " << msg << "\n";
    };


    LOG_MSG << "\n\n";
    LOG_MSG << "Test preprompt names completion (2):\n\n";
    checkPreprompt2("tester");
    checkPreprompt2("bugster"); // bug buster/bug hunter (QA) // Ghostbusters - https://chat.deepseek.com/share/qa5vqd0dkprxc1yss2
    checkPreprompt2("instruction/tester");
    checkPreprompt2("skills/tester");
    checkPreprompt2("auto-tester");
    checkPreprompt2("c-dev");
    checkPreprompt2("jni-master");
    checkPreprompt2("ui-tester");



    return 0;
}
~~~

**\_src/almai/\_test10.cpp**
~~~C++
/*! \file
    \brief Тесты запуска сторонних программ (без ожидания)
 */

#include "utils.h"
//
#include "umba/shellapi.h"
#include "umba/win32_utils.h"
#include "marty_cdt/utils.h"
//
#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

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
    testFindExe("powershell");


    auto curDir = umba::filesys::getCurrentDirectory();
    auto userProfileFolderProject = marty::cdt::utils::generateChromeUserProfileDirForProject(curDir, std::string(), std::string(".almai-working-files"));
    auto userCacheFolderProject   = marty::cdt::utils::generateChromeUserCacheDirForProject  (curDir, std::string(), std::string(".almai-working-files"));
    auto userProfileFolderGlobal  = marty::cdt::utils::generateChromeUserProfileDirForProject(curDir, std::string(), std::string());
    auto userCacheFolderGlobal    = marty::cdt::utils::generateChromeUserCacheDirForProject  (curDir, std::string(), std::string());

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
~~~

**\_src/almai/\_test11.cpp**
~~~C++
/*! \file
    \brief Тест регистрации EXE без добавления в PATH
 */

#include "utils.h"
//
#include "umba/shellapi.h"
#include "umba/win32_utils.h"
#include "marty_cdt/utils.h"
//
#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
void testRegisterExe(const std::string &exeName)
{
    if (umba::shellapi::regExecutable(exeName))
        std::cout << "Exe registered: " << exeName << "\n";
    else
        std::cout << "Exe registration failed: " << exeName << "\n";
}

//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    //std::string exeFullPathName = "D:/MarkdownMonster/MarkdownMonster.exe";

    std::string path = "D:/MarkdownMonster";
    if (argc>1)
        path = argv[1];


    testRegisterExe(umba::filename::appendPath(path, "MarkdownMonster.exe"));
    testRegisterExe(umba::filename::appendPath(path, "mm.exe"));
    testRegisterExe(umba::filename::appendPath(path, "mmcli.exe"));

    return 0;
}
~~~

**\_src/almai/\_test12.cpp**
~~~C++
/*! \file
    \brief Тест запуска chrome, и получения списка доступных страниц
 */


/*
LSP - тоже JSON-RPC - надо оттуда всё HTTP-шное вытащить в отдельную либу. Или не надо?

Порт для подключения генерировать примерно аналогично имени временных папок -
использовать хэш std::hash по имени проекта, а дальше его редюсить до заданного диапазона.

https://chat.deepseek.com/share/hipbevscouw97pijpm


*/


#include "utils.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
#include "marty_cdt/utils.h"
//
#include <iostream>
//
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXHttpClient.h>


using std::cout;
using std::cerr;
using namespace almai;


//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    const std::string chromeName = "chrome";

    std::string foundProjectPath;
    std::string foundAlmaiDir;
    std::string projectFile;

    if (!almai::utils::findProjectRoot( umba::filesys::getCurrentDir()
                                      , &foundProjectPath
                                      , &foundAlmaiDir
                                      , &projectFile
                                      )
       )
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    cout << "foundProjectPath: " << foundProjectPath << "\n";
    cout << "foundAlmaiDir   : " << foundAlmaiDir    << "\n";
    cout << "projectFile     : " << projectFile      << "\n";


    // 1) Надо подключится к TCP порту по имени каталога проекта

    int projectPort = marty::cdt::utils::generatePortNumberForProjectConnection(foundProjectPath);
    cout << "projectPort     : " << projectPort      << "\n";

    std::string httpBaseUrl = marty::cdt::utils::generateConnectionBaseUrlString(projectPort, true  /* http */ );
    cout << "httpBaseUrl     : " << httpBaseUrl      << "\n";


    ix::initNetSystem();

    ix::HttpClient httpClient;
    auto args = httpClient.createRequest(httpBaseUrl + "/json/list");
    args->connectTimeout  = 2; // 2 секунды на подключение
    args->transferTimeout = 5; // 5 секунды на получение данных

    // auto response = httpClient.get(httpBaseUrl + "/json/list", args);
    auto response = httpClient.get(args->url, args);


    cout << "response->errorCode : " << marty::cdt::utils::ixHttpErrorCodeToString(response->errorCode) << " (" << (int)response->errorCode << ")" << "\n";
    cout << "response->errorMsg  : " << response->errorMsg   << "\n";
    cout << "response->statusCode: " << response->statusCode << "\n";
    //cout << "" <<  << "\n";

    if ( response->errorCode!=ix::HttpErrorCode::Ok            // 0
      && response->errorCode!=ix::HttpErrorCode::CannotConnect // 1
      && response->errorCode!=ix::HttpErrorCode::Timeout       // 2
       )
    {
        cout << "Something goes wrong" << "\n";
        return 1;
    }


    if (response->errorCode!=ix::HttpErrorCode::Ok)
    {
        // args->connectTimeout  = 2;
        args->transferTimeout = 10; // даём время прочухаться

        std::vector<std::string> foundExes;
        umba::shellapi::findExecutable(chromeName, foundExes);

        std::vector<std::string> spawnArgs = marty::cdt::utils::generateArgsForSpawnChrome(foundProjectPath, "chrome", std::string(), projectPort);

        for(auto chromeExeFullName : foundExes)
        {

            auto spawnRes = umba::shellapi::spawnProcess( chromeExeFullName
                                                        , spawnArgs
                                                        , umba::shellapi::SpawnProcessFlags::default_ // | umba::shellapi::SpawnProcessFlags::argvFromCmd
                                                        );
            if (spawnRes==std::uintptr_t(-1))
                continue;

            for(auto n=0; n!=10; ++n)
            {
                umba::sleepMs(1000);

                response = httpClient.get(args->url, args);

                if (response->errorCode==ix::HttpErrorCode::Ok)
                    break;
            }

        }
    }

    if (response->errorCode!=ix::HttpErrorCode::Ok)
    {
        cout << "Can't run chrome" << "\n";
        return 1;
    }

    cout << "Chrome is running" << "\n";

    return 0;
}
~~~

**\_src/almai/\_test13.cpp**
~~~C++
/*! \file
    \brief Тест запуска chrome, и получения списка доступных страниц - тестируем almai::ProjectDirs
 */


/*
LSP - тоже JSON-RPC - надо оттуда всё HTTP-шное вытащить в отдельную либу. Или не надо?

Порт для подключения генерировать примерно аналогично имени временных папок -
использовать хэш std::hash по имени проекта, а дальше его редюсить до заданного диапазона.

https://chat.deepseek.com/share/hipbevscouw97pijpm


*/


#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
#include "marty_cdt/utils.h"
#include "marty_cdt/JsonListResponse.h"
//
#include <iostream>
#include <stdexcept>
//
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXHttpClient.h>


using std::cout;
using std::cerr;
using namespace almai;


//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.findProjectRoot())
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    projectDirs.generateBrowserDirs(chromeName, false /* !useTempDir */);
    projectDirs.generateProjectConnectionPort( /* int startPort=9000, int range=1000 */ );
    auto httpBaseUrl = projectDirs.generateConnectionBaseUrlString(true /* http */);
    auto jsonListUrl = httpBaseUrl + "/json/list";

    cout << "projectPath        : " << projectDirs.projectPath << "\n";
    cout << "almaiDir           : " << projectDirs.almaiDir    << "\n";
    cout << "projectFile        : " << projectDirs.projectFile      << "\n";
    cout << "browserUserDataDir : " << projectDirs.browserDirs.userDataDir  << "\n";
    cout << "browserCacheDataDir: " << projectDirs.browserDirs.cacheDataDir << "\n";
    cout << "httpBaseUrl        : " << httpBaseUrl << "\n";
    cout << "jsonListUrl        : " << jsonListUrl << "\n";

    ix::initNetSystem();

    ix::HttpClient httpClient;
    auto args = httpClient.createRequest(jsonListUrl);
    args->connectTimeout  = 2; // 2 секунды на подключение
    args->transferTimeout = 5; // 5 секунды на получение данных

    auto response = httpClient.get(args->url, args);

    // chrome://settings/onStartup

    cout << "response->errorCode : " << marty::cdt::utils::ixHttpErrorCodeToString(response->errorCode) << " (" << (int)response->errorCode << ")" << "\n";
    cout << "response->errorMsg  : " << response->errorMsg   << "\n";
    cout << "response->statusCode: " << response->statusCode << "\n";

    if ( response->errorCode!=ix::HttpErrorCode::Ok            // 0
      && response->errorCode!=ix::HttpErrorCode::CannotConnect // 1
      && response->errorCode!=ix::HttpErrorCode::Timeout       // 2
       )
    {
        cout << "Something goes wrong" << "\n";
        return 1;
    }


    if (response->errorCode!=ix::HttpErrorCode::Ok)
    {
        // args->connectTimeout  = 2;
        args->transferTimeout = 10; // даём время прочухаться

        std::vector<std::string> foundExes;
        umba::shellapi::findExecutable(chromeName, foundExes);

        std::vector<std::string> spawnArgs = projectDirs.generateArgsForSpawnChrome();

        cout << "\n" << "Args: \n";
        for(auto a: spawnArgs)
            cout << "  " << a << "\n";


        for(auto chromeExeFullName : foundExes)
        {

            auto spawnRes = umba::shellapi::spawnProcess( chromeExeFullName
                                                        , spawnArgs
                                                        , umba::shellapi::SpawnProcessFlags::default_ // | umba::shellapi::SpawnProcessFlags::argvFromCmd
                                                        );
            if (spawnRes==std::uintptr_t(-1))
                continue;

            for(auto n=0; n!=10; ++n)
            {
                umba::sleepMs(1000);

                response = httpClient.get(args->url, args);

                if (response->errorCode==ix::HttpErrorCode::Ok)
                    break;
            }

        }
    }

    if (response->errorCode!=ix::HttpErrorCode::Ok)
    {
        cout << "Can't run chrome" << "\n";
        return 1;
    }



    cout << "Chrome is running" << "\n";

    cout << "Response:\n" << response->body << "\n";

    try
    {
        auto j = nlohmann::json::parse(response->body);
        std::vector<marty::cdt::JsonListResponseEntry> list;
        from_json(j, list);
        cout << "\n" << "Total " << list.size() << " items" << "\n";

    }
    catch(const std::exception &e)
    {
        cout << "Error parsing JSON: " << e.what() << "\n";
    }
    catch(...)
    {
        cout << "Error: " << "unknown error" << "\n";
    }

    //void from_json(const json& j, CommonPreset& p)
    // JsonListResponse
    // marty::cdt::

    //response->statusCode

    return 0;
}
~~~

**\_src/almai/\_test14.cpp**
~~~C++
/*! \file
    \brief Тест запуска chrome, и получения списка доступных страниц - основная часть кода ушла в библиотеку
 */

#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
#include "marty_cdt/utils.h"
#include "marty_cdt/JsonListResponse.h"
#include "marty_cdt/JsonVersionResponse.h"

//
#include <iostream>
#include <stdexcept>
//
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXHttpClient.h>


using std::cout;
using std::cerr;
using namespace almai;


inline
int returnReportFailedToConnect(ix::HttpErrorCode c, const std::string &endPoint, int retCode=1)
{
    std::cout << "Failed to connect to '" << endPoint << "' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(c) << " (" << (int)c << ")" << "\n";
    return retCode;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#define CATCH_PARSE_RESPONSE()                              \
                                                            \
    catch(const std::exception &e)                          \
    {                                                       \
        cout << "Error parsing JSON: " << e.what() << "\n"; \
    }                                                       \
    catch(...)                                              \
    {                                                       \
        cout << "Error: " << "unknown error" << "\n";       \
    }





//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    ix::initNetSystem();

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.initAll(chromeName, false /* !useTempDir */ ))
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    marty::cdt::Timeouts timeouts; // default {1,1}
    auto newInstanceConnectTimeouts = timeouts.getCopy().setConnectTimeout(1).setTransferTimeout(3);

    std::string httpBaseUrl = projectDirs.generateConnectionBaseUrlString(true /* http */);

    cout << "### Run&Connect" << "\n";

    auto spawnArgs = projectDirs.generateArgsForSpawnChrome();
    cout << "Chrome args:\n";
    for(auto a: spawnArgs)
        cout << "  " << a << "\n";

    auto response = marty::cdt::utils::runConnectAndGetJsonList( httpBaseUrl
                                                              , chromeName
                                                              , spawnArgs
                                                              , timeouts
                                                              , newInstanceConnectTimeouts
                                                              );

    if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
    {
        cout << "Failed to connect to browser" << "\n";
        return 1;
    }

    cout << "HTTP code: " << response->statusCode << "\n";
    cout << "Chrome is running" << "\n";

    cout << "Response:\n" << response->body << "\n";

    std::string defaultNewTabId;

    try
    {
        auto j = nlohmann::json::parse(response->body);
        std::vector<marty::cdt::JsonListResponseEntry> list;
        from_json(j, list);
        cout << "\n" << "Total " << list.size() << " items" << "\n";

        for(const auto &e: list)
        {
            if (e.type=="page" && e.url=="chrome://newtab/")
                defaultNewTabId = e.id;
        }

    }
    CATCH_PARSE_RESPONSE()



    cout << "\n\n";
    cout << "### /json/version" << "\n";
    response = marty::cdt::utils::httpGetJsonVersion(httpBaseUrl, timeouts); // httpGet(request);

    if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
    {
        // cout << "Failed to connect to '/json/version' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(response->errorCode) << " (" << (int)response->errorCode << ")" << "\n";
        return returnReportFailedToConnect(response->errorCode, "/json/version", 1);
    }

    cout << "HTTP code: " << response->statusCode << "\n";
    cout << "Response:\n" << response->body << "\n";

    try
    {
        auto j = nlohmann::json::parse(response->body);
        marty::cdt::JsonVersionResponse versionResponse;
        from_json(j, versionResponse);
        //cout << "\n" << "Total " << list.size() << " items" << "\n";

    }
    CATCH_PARSE_RESPONSE()


    cout << "\n\n";
    cout << "### /json/new" << "\n";
    response = marty::cdt::utils::httpPutJsonNewPage(httpBaseUrl, "https://яндекс.рф", timeouts);

    if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
    {
        return returnReportFailedToConnect(response->errorCode, "/json/new", 1);
    }

    cout << "HTTP code: " << response->statusCode << "\n";
    cout << "Response:\n" << response->body << "\n";

    std::string newPageId;
    std::string newPageWsUrl;

    try
    {
        auto j = nlohmann::json::parse(response->body);
        //std::vector<marty::cdt::JsonListResponseEntry> list;
        marty::cdt::JsonNewPageResponse newPageResponse;
        from_json(j, newPageResponse);

        newPageId    = newPageResponse.id;
        newPageWsUrl = newPageResponse.webSocketDebuggerUrl;
    }
    CATCH_PARSE_RESPONSE()


    if (!defaultNewTabId.empty())
    {
        cout << "\n\n";
        cout << "### /json/close/${id}" << "\n";
        response = marty::cdt::utils::httpClosePage(httpBaseUrl, defaultNewTabId, timeouts); // httpGet(request);
        // httpClosePage(const std::string &httpBaseUrl, const std::string &pageId, Timeouts timeouts)

        if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
        {
            return returnReportFailedToConnect(response->errorCode, "/json/close/${id}", 1);
        }

        cout << "HTTP code: " << response->statusCode << "\n";
        cout << "Response:\n" << response->body << "\n";

    }

    ix::WebSocket webSocket;

    // webSocket.setOnBeforeHandshakeCallback([](ix::WebSocketHttpHeaders& headers)
    //                                       {
    //                                           std::cout << "=== Заголовки WebSocket-запроса ===" << std::endl;
    //                                           for (const auto& [key, value] : headers) {
    //                                               std::cout << key << ": " << value << std::endl;
    //                                           }
    //                                           std::cout << "==================================" << std::endl;
    //                                           // Можно оставить headers без изменений или добавить/исправить
    //                                       });


    auto wsConnectRes = marty::cdt::utils::wsConnect( webSocket, newPageWsUrl
                                                    , [&](const ix::WebSocketMessagePtr& pMsg)
                                                      {
                                                          UMBA_USED(pMsg);

                                                          std::cout << "\n";

                                                          std::cout << "WS Callback\n";
                                                          std::cout << "Type    : " << marty::cdt::utils::ixWebSocketMessageTypeToString(pMsg->type) << "\n";
                                                          // pMsg->str
                                                          std::cout << "WireSize: " << pMsg->wireSize << "\n";
                                                          std::cout << "Binary  : " << (pMsg->binary ? "true" : "false") << "\n";

                                                          if (pMsg->type==ix::WebSocketMessageType::Message)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Open)
                                                          {
                                                              // WebSocketOpenInfo openInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Close)
                                                          {
                                                              // WebSocketCloseInfo closeInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Error)
                                                          {
                                                              // WebSocketErrorInfo errorInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Ping)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Pong)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Fragment)
                                                          {
                                                          }
                                                          else // unknown pMsg->type
                                                          {
                                                          }

                                                          if (!pMsg->binary)
                                                          {
                                                              cout << "Response text: [" << pMsg->str << "]" << "\n";
                                                          }

                                                          std::cout << "\n";

                                                      }
                                                    );

    // bool success;
    // int http_status;
    // std::string errorStr;
    // WebSocketHttpHeaders headers;
    // std::string uri;
    // std::string protocol;

    if (!wsConnectRes.success)
    {
        cout << "WS NOT connected" << "\n";
        cout << "http_status: " << wsConnectRes.http_status << "\n";
        cout << "errorStr   : " << wsConnectRes.errorStr    << "\n";

        return 1;
    }
    else
    {
        cout << "WS connected" << "\n";
    }

    // for(auto i=0; i!=30; ++i)
    //     webSocket.poll(100);


    // std::mutex dataMutex;
    // std::queue<json> messageQueue;
    //
    // webSocket.setOnMessageCallback([&](const ix::WebSocketMessagePtr& msg) {
    //     std::lock_guard<std::mutex> lock(dataMutex);
    //     // Обработка сообщения
    //     messageQueue.push(json::parse(msg->str));
    // });
    //
    // webSocket.start();


    webSocket.start();

    for(auto i=0; i!=20; ++i)
    {
        umba::sleepMs(250);
    }

    webSocket.stop();

    umba::sleepMs(100);

    return 0;
}
~~~

**\_src/almai/\_test15.cpp**
~~~C++
/*! \file
    \brief Тест запуска chrome, отработка открытия страницы, события Page, Network, Runtime, DOM

 */

#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
#include "marty_cdt/utils.h"
#include "marty_cdt/JsonListResponse.h"
#include "marty_cdt/JsonVersionResponse.h"

//
#include <iostream>
#include <stdexcept>
//
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXHttpClient.h>


using std::cout;
using std::cerr;
using namespace almai;


inline
int returnReportFailedToConnect(ix::HttpErrorCode c, const std::string &endPoint, int retCode=1)
{
    std::cout << "Failed to connect to '" << endPoint << "' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(c) << " (" << (int)c << ")" << "\n";
    return retCode;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#define CATCH_PARSE_RESPONSE()                              \
                                                            \
    catch(const std::exception &e)                          \
    {                                                       \
        cout << "Error parsing JSON: " << e.what() << "\n"; \
    }                                                       \
    catch(...)                                              \
    {                                                       \
        cout << "Error: " << "unknown error" << "\n";       \
    }





//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);

    ix::initNetSystem();

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.initAll(chromeName, false /* !useTempDir */ ))
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    marty::cdt::Timeouts timeouts; // default {1,1}
    auto newInstanceConnectTimeouts = timeouts.getCopy().setConnectTimeout(1).setTransferTimeout(3);

    std::string httpBaseUrl = projectDirs.generateConnectionBaseUrlString(true /* http */);

    cout << "### Run&Connect" << "\n";

    auto spawnArgs = projectDirs.generateArgsForSpawnChrome();
    // cout << "Chrome args:\n";
    // for(auto a: spawnArgs)
    //     cout << "  " << a << "\n";

    auto response = marty::cdt::utils::runConnectAndGetJsonList( httpBaseUrl
                                                              , chromeName
                                                              , spawnArgs
                                                              , timeouts
                                                              , newInstanceConnectTimeouts
                                                              );

    if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
    {
        cout << "Failed to connect to browser" << "\n";
        return 1;
    }

    cout << "HTTP code: " << response->statusCode << "\n";
    cout << "Chrome is running" << "\n";

    cout << "Response:\n" << response->body << "\n";

    std::string defaultNewTabId;
    std::string defaultNewTabPageWsUrl;

    try
    {
        auto j = nlohmann::json::parse(response->body);
        std::vector<marty::cdt::JsonListResponseEntry> list;
        from_json(j, list);
        cout << "\n" << "Total " << list.size() << " items" << "\n";

        for(const auto &e: list)
        {
            if (e.type=="page" && (e.url=="chrome://newtab/" || e.url=="chrome://newtab"))
            {
                defaultNewTabId = e.id;
                defaultNewTabPageWsUrl = e.webSocketDebuggerUrl;
            }
        }

    }
    CATCH_PARSE_RESPONSE()


    if (defaultNewTabPageWsUrl.empty())
    {
        cout << "\n\n";
        cout << "### /json/new" << "\n";
        response = marty::cdt::utils::httpPutJsonNewPage(httpBaseUrl, timeouts); // Создаём новую вкладку без задания адреса

        if (!marty::cdt::utils::ixHttpErrorCodeIsOk(response->errorCode))
        {
            return returnReportFailedToConnect(response->errorCode, "/json/new", 1);
        }

        cout << "HTTP code: " << response->statusCode << "\n";
        cout << "Response:\n" << response->body << "\n";

        try
        {
            auto j = nlohmann::json::parse(response->body);
            //std::vector<marty::cdt::JsonListResponseEntry> list;
            marty::cdt::JsonNewPageResponse newPageResponse;
            from_json(j, newPageResponse);

            defaultNewTabId        = newPageResponse.id;
            defaultNewTabPageWsUrl = newPageResponse.webSocketDebuggerUrl;
        }
        CATCH_PARSE_RESPONSE()

    }



    if (defaultNewTabId.empty())
    {
        return 1;
    }

    ix::WebSocket webSocket;

    auto wsConnectRes = marty::cdt::utils::wsConnect( webSocket, defaultNewTabPageWsUrl // newPageWsUrl
                                                    , [&](const ix::WebSocketMessagePtr& pMsg)
                                                      {
                                                          UMBA_USED(pMsg);

                                                          std::cout << "\n";

                                                          std::cout << "WS Callback\n";
                                                          std::cout << "Type    : " << marty::cdt::utils::ixWebSocketMessageTypeToString(pMsg->type) << "\n";
                                                          // pMsg->str
                                                          std::cout << "WireSize: " << pMsg->wireSize << "\n";
                                                          std::cout << "Binary  : " << (pMsg->binary ? "true" : "false") << "\n";

                                                          if (pMsg->type==ix::WebSocketMessageType::Message)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Open)
                                                          {
                                                              // WebSocketOpenInfo openInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Close)
                                                          {
                                                              // WebSocketCloseInfo closeInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Error)
                                                          {
                                                              // WebSocketErrorInfo errorInfo;
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Ping)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Pong)
                                                          {
                                                          }
                                                          else if (pMsg->type==ix::WebSocketMessageType::Fragment)
                                                          {
                                                          }
                                                          else // unknown pMsg->type
                                                          {
                                                          }

                                                          if (!pMsg->binary)
                                                          {
                                                              std::string msgBody;
                                                              try
                                                              {
                                                                  if (pMsg->str.empty())
                                                                  {
                                                                      msgBody = "<EMPTY>";
                                                                  }
                                                                  else
                                                                  {
                                                                      auto j = nlohmann::json::parse(pMsg->str);
                                                                      msgBody = j.dump(2);
                                                                  }
                                                              }
                                                              CATCH_PARSE_RESPONSE()

                                                              // cout << "Response text: [" << pMsg->str << "]" << "\n";
                                                              cout << "Response text:\n" << msgBody << "\n";
                                                          }

                                                          std::cout << "\n";

                                                      }
                                                    );

    if (!wsConnectRes.success)
    {
        cout << "WS NOT connected" << "\n";
        cout << "http_status: " << wsConnectRes.http_status << "\n";
        cout << "errorStr   : " << wsConnectRes.errorStr    << "\n";

        return 1;
    }
    else
    {
        cout << "WS connected" << "\n";
    }

    webSocket.start();

    unsigned cmdId = 0;

    auto
    sendRes = marty::cdt::utils::wsSendEventSubscription(webSocket, cmdId++, "Page.enable");
    if (!sendRes.success)
        cout << "Send 'Page.enable' subscription failed" << "\n";

    sendRes = marty::cdt::utils::wsSendEventSubscription(webSocket, cmdId++, "Network.enable");
    if (!sendRes.success)
        cout << "Send 'Network.enable' subscription failed" << "\n";

    sendRes = marty::cdt::utils::wsSendEventSubscription(webSocket, cmdId++, "Runtime.enable");
    if (!sendRes.success)
        cout << "Send 'Runtime.enable' subscription failed" << "\n";

    sendRes = marty::cdt::utils::wsSendEventSubscription(webSocket, cmdId++, "DOM.enable");
    if (!sendRes.success)
        cout << "Send 'DOM.enable' subscription failed" << "\n";


    sendRes = marty::cdt::utils::wsSendCommand(webSocket, cmdId++, "Page.navigate", { {"url", "https://яндекс.рф"} });
    if (!sendRes.success)
        cout << "Send 'Page.navigate' command failed" << "\n";


    for(auto i=0; i!=20; ++i)
    {
        umba::sleepMs(250);
    }

    webSocket.stop();

    umba::sleepMs(100);

    return 0;
}
~~~

**\_src/almai/\_test16.cpp**
~~~C++
/*! \file
    \brief Тест запуска chrome, отработка открытия страницы, события Page, Network, Runtime, DOM - используем marty::cdt::Connection

 */

#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
//
#include "marty_cdt/Connection.h"
//
#include "marty_cdt/AtomicBoolIdHandler.h"
#include "marty_cdt/AtomicBoolMethodHandler.h"
//
#include "marty_cdt/Dom.h"


//
#include <iostream>
#include <stdexcept>
#include <memory>
#include <variant>
//

using std::cout;
using std::cerr;
using namespace almai;


inline
int returnReportFailedToConnect(ix::HttpErrorCode c, const std::string &endPoint, int retCode=1)
{
    std::cout << "Failed to connect to '" << endPoint << "' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(c) << " (" << (int)c << ")" << "\n";
    return retCode;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#define CATCH_PARSE_RESPONSE()                        \
                                                      \
    catch(const std::exception &e)                    \
    {                                                 \
        cout << "Error: " << e.what() << "\n";        \
    }                                                 \
    catch(...)                                        \
    {                                                 \
        cout << "Error: " << "unknown error" << "\n"; \
    }





//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);


    {
        std::vector<std::string> patterns = marty::cdt::utils::generatePatterns("page.navigation.navigate");
        cout << "Patterns:\n";
        for(const auto &p: patterns)
        {
            cout << "  " << p << "\n";
        }
    }

    ix::initNetSystem();

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.initAll(chromeName, false /* !useTempDir */ ))
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    marty::cdt::Timeouts httpProbeTimeouts   = {1,1};
    marty::cdt::Timeouts httpConnectTimeouts = {1,3};

    //std::string httpBaseUrl = projectDirs.generateConnectionBaseUrlString(true /* http */);

    auto wsConnection = std::make_shared<marty::cdt::Connection>();

    wsConnection->setHttpBaseUrl(projectDirs.generateConnectionBaseUrlString(true /* http */));
    wsConnection->setHttpTimeouts(httpConnectTimeouts);

    cout << "### Run&Connect" << "\n";

    auto spawnArgs = projectDirs.generateArgsForSpawnChrome();
    // cout << "Chrome args:\n";
    // for(auto a: spawnArgs)
    //     cout << "  " << a << "\n";

    std::vector<marty::cdt::JsonListResponseEntry> jsonListResponse;
    auto httpResponse = wsConnection->checkRunAndGetJsonList( jsonListResponse, chromeName, spawnArgs, httpProbeTimeouts);

    if (!wsConnection->httpIsResponseOk(httpResponse))
        throw std::runtime_error("Failed to connect to browser");

    cout << "HTTP code: " << httpResponse->statusCode << "\n";
    cout << "Chrome is running" << "\n";


    marty::cdt::JsonVersionResponse versionResponse;
    httpResponse = wsConnection->httpGetJsonVersion(versionResponse);
    if (!wsConnection->httpIsResponseOk(httpResponse))
        throw std::runtime_error("Failed to browser version");

    cout << "\n" << "Version Info:" << "\n";

    cout << "  " << "browser        : " << versionResponse.browser         << "\n";
    cout << "  " << "protocolVersion: " << versionResponse.protocolVersion << "\n";
    cout << "  " << "userAgent      : " << versionResponse.userAgent       << "\n";
    cout << "  " << "v8Version      : " << versionResponse.v8Version       << "\n";
    cout << "  " << "webkitVersion  : " << versionResponse.webkitVersion   << "\n";
    cout << "  " << "debuggerVersion: " << versionResponse.debuggerVersion << "\n";
    //cout << "  " << ": " << versionResponse. << "\n";


    //cout << "Response:\n" << response->body << "\n";

    std::string defaultNewTabId;
    std::string defaultNewTabPageWsUrl;

    for(const auto &e: jsonListResponse)
    {
        if (e.type=="page" && (e.url=="chrome://newtab/" || e.url=="chrome://newtab"))
        {
            defaultNewTabId = e.id;
            defaultNewTabPageWsUrl = e.webSocketDebuggerUrl;
        }
    }


    if (defaultNewTabPageWsUrl.empty())
    {
        cout << "\n\n";
        cout << "### /json/new" << "\n";

        marty::cdt::JsonNewPageResponse newPageResponse;
        httpResponse = wsConnection->httpNewPage(newPageResponse);
        if (!wsConnection->httpIsResponseOk(httpResponse))
            return returnReportFailedToConnect(httpResponse->errorCode, "/json/new", 1);

        cout << "HTTP code: " << httpResponse->statusCode << "\n";
        cout << "Response:\n" << httpResponse->body << "\n";

        defaultNewTabId        = newPageResponse.id;
        defaultNewTabPageWsUrl = newPageResponse.webSocketDebuggerUrl;

    }


    try
    {
        if (defaultNewTabId.empty())
        {
            throw std::runtime_error("defaultNewTabId is empty");
            //cout << "Error: defaultNewTabId is empty\n";
        }

        wsConnection->wsSetUrl(defaultNewTabPageWsUrl);


        wsConnection->wsSetSystemEventHandler( { ix::WebSocketMessageType::Open
                                               , ix::WebSocketMessageType::Close
                                               , ix::WebSocketMessageType::Error
                                               , ix::WebSocketMessageType::Ping
                                               , ix::WebSocketMessageType::Pong
                                               , ix::WebSocketMessageType::Fragment
                                               }
                                             , [&](marty::cdt::Connection */* pCon */, const marty::cdt::WebSocketMessage& /* msg */, ix::WebSocketMessageType type)
                                               {
                                                   std::cout << "Message, type: " << marty::cdt::utils::ixWebSocketMessageTypeToString(type) << "\n" << "\n";
                                               }
                                             );

        struct GenericIdHandler
        {
            std::string handlerName;

            void operator()(marty::cdt::Connection *pCon, const marty::cdt::WebSocketMessage& /* msg */, marty::cdt::MessageIdVariant idVariant, marty::cdt::json j) const
            {
                using std::cout;
                std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                cout << "'" << handlerName << "'" << " ID Handler" << "\n";
                std::visit( [&](auto&& arg)
                            {
                                using T = std::decay_t<decltype(arg)>;

                                if constexpr (std::is_same_v<T, unsigned>)
                                {
                                    cout << "  ID: " << arg << "\n";
                                    cout << "  JSON:" << "\n";
                                    cout << j.dump(2) << "\n";
                                }
                                else if constexpr (std::is_same_v<T, std::string>)
                                {
                                    cout << "  Method: " << arg << "\n";
                                }
                                else if constexpr (std::is_same_v<T, marty::cdt::ResponseError>)
                                {
                                    cout << "  ResponseError:" << "\n";
                                    cout << "    ID     : " << arg.id << "\n";
                                    cout << "    Code   : " << arg.errorInfo.code << "\n";
                                    cout << "    Message: " << arg.errorInfo.message << "\n";
                                }
                                // else
                                //     static_assert(false, "non-exhaustive visitor!");
                            }
                          , idVariant
                          );

                cout << "\n";

            }

        }; // struct GenericIdHandler


        struct GenericMethodHandler
        {
            std::string handlerName;

            void operator()(marty::cdt::Connection *pCon, const marty::cdt::WebSocketMessage& /* msg */, marty::cdt::MessageIdVariant idVariant, marty::cdt::json j) const
            {
                using std::cout;
                std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                cout << "'" << handlerName << "'" << " Method Handler" << "\n";
                std::visit( [&](auto&& arg)
                            {
                                using T = std::decay_t<decltype(arg)>;

                                if constexpr (std::is_same_v<T, unsigned>)
                                {
                                    cout << "  ID: " << arg << "\n";
                                    cout << "  JSON:" << "\n";
                                    cout << j.dump(2) << "\n";
                                }
                                else if constexpr (std::is_same_v<T, std::string>)
                                {
                                    cout << "  Method: " << arg << "\n";
                                }
                                else if constexpr (std::is_same_v<T, marty::cdt::ResponseError>)
                                {
                                    cout << "  ResponseError:" << "\n";
                                    cout << "    ID     : " << arg.id << "\n";
                                    cout << "    Code   : " << arg.errorInfo.code << "\n";
                                    cout << "    Message: " << arg.errorInfo.message << "\n";
                                }
                                else
                                    static_assert(false, "non-exhaustive visitor!");
                            }
                          , idVariant
                          );

                cout << "\n";
            }

        }; // struct GenericMethodHandler



        wsConnection->wsSetDefaultIdHandler(GenericIdHandler{"DefaultIdHandler"});

        wsConnection->wsSetMethodEventHandler("*", GenericMethodHandler{"Default"});

        wsConnection->wsSetMethodEventHandler("Page.*", GenericMethodHandler{"Page"});


        std::atomic<bool> domContentEventFiredFlag = false;
        std::atomic<bool> loadEventFiredFlag       = false;

        wsConnection->wsSetMethodEventHandler("Page.domContentEventFired", marty::cdt::AtomicBoolMethodHandler{domContentEventFiredFlag, true});
        wsConnection->wsSetMethodEventHandler("Page.loadEventFired"      , marty::cdt::AtomicBoolMethodHandler{loadEventFiredFlag      , true});



        auto wsConnectRes = wsConnection->wsConnect();
        if (!wsConnectRes.success)
        {
            cout << "WS NOT connected" << "\n";
            cout << "http_status: " << wsConnectRes.http_status << "\n";
            cout << "errorStr   : " << wsConnectRes.errorStr    << "\n";

            throw std::runtime_error("WS NOT connected");
        }
        else
        {
            cout << "WS connected" << "\n" << "\n";
        }

        wsConnection->wsStart();

        // wsConnection->wsSetDefaultIdHandler( GenericIdHandler{"Default"}
        auto
        sendRes = wsConnection->wsEventSubscribe("Page", GenericIdHandler{"PageEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Page.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        sendRes = wsConnection->wsEventSubscribe("Network", GenericIdHandler{"NetworkEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Network.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        sendRes = wsConnection->wsEventSubscribe("Runtime", GenericIdHandler{"RuntimeEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Runtime.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        sendRes = wsConnection->wsEventSubscribe("DOM", GenericIdHandler{"DomEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'DOM.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        std::atomic<bool> atomicBoolFlag = false;
        std::string frameId;
        std::string loaderId;
        sendRes = wsConnection->wsSendCommand( "Page.navigate", { {"url", "https://яндекс.рф"} }
                                             // , GenericIdHandler{"Page.navigate"}
                                             , [&](marty::cdt::Connection *pCon, const marty::cdt::WebSocketMessage& /* msg */, marty::cdt::MessageIdVariant idVariant, marty::cdt::json j)
                                               {
                                                   try
                                                   {
                                                       auto id = std::get<unsigned>(idVariant); // Просто убедиться, что там ID лежит, и всё корректно
                                                       UMBA_USED(id);
                                                       std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                                                       std::cout << "Page.navigate to ya.ru\n";
                                                       std::cout << j.dump(2) << "\n";

                                                       if (j.contains("frameId"))
                                                           frameId = j["frameId"].get<std::string>();

                                                       if (j.contains("loaderId"))
                                                           loaderId = j["loaderId"].get<std::string>();

                                                       atomicBoolFlag = true;

                                                   }
                                                   catch(...)
                                                   {}
                                                   std::cout << "\n";
                                               }
                                             );
        if (!sendRes.success)
            cout << "Send 'Page.navigate' command failed" << "\n";


        cout << "Waiting for 'Page.navigate' reply" << "\n";
        if (!wsConnection->wsWaitAndDispatchMessagesAndReset(10000 /* ms */, atomicBoolFlag))
            throw std::runtime_error("No reply for 'Page.navigate'");

        cout << "'Page.navigate' done\n" << "\n";

        if (frameId.empty())
            throw std::runtime_error("Frame ID not returned");


        // Не надо так делать - события уже могли произойти
        // domContentEventFiredFlag = false;
        // loadEventFiredFlag       = false;
        if (!wsConnection->wsWaitAndDispatchMessages( 60000 /* 30000 */ /* ms */
                                                    , [&]() -> bool
                                                      {
                                                          // bool dFlag = domContentEventFiredFlag;
                                                          // bool lFlag = loadEventFiredFlag;

                                                          // std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
                                                          // std::cout << "Check two flags\n";
                                                          // std::cout << "  domContentEventFiredFlag: " << (domContentEventFiredFlag? "true" : "false") << "\n";
                                                          // std::cout << "  loadEventFiredFlag      : " << (loadEventFiredFlag      ? "true" : "false") << "\n";
                                                          // std::cout << "  fired                   : " << (dFlag && lFlag          ? "true" : "false") << "\n";
                                                          // std::cout << "\n";

                                                          // std::this_thread::sleep_for(std::chrono::milliseconds(50));
                                                          // return dFlag && lFlag;

                                                          return bool(domContentEventFiredFlag) && bool(loadEventFiredFlag);
                                                      }
                                                    )
           )
            throw std::runtime_error("Couldn't wait for the page to load");

        cout << "Page loaded\n" << "\n";

        cout << "Execute DOM.getDocument\n" << "\n";


        atomicBoolFlag = false;
        marty::cdt::DomDocument domDocument;

        sendRes =
        wsConnection->wsSendCommand( "DOM.getDocument"
                                   // , marty::cdt::json::object() // Возвращает только корневой элемент
                                   , {
                                         {"depth", -1},          // -1 означает "вся глубина"
                                         {"pierce", true}        // проникать через shadow DOM
                                     }
                                   , [&](marty::cdt::Connection *pCon, const marty::cdt::WebSocketMessage&, marty::cdt::MessageIdVariant /* idVariant */, marty::cdt::json j)
                                     {
                                         std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                                         std::cout << "DOM.getDocument" << "\n";
                                         std::cout << j.dump(2) << "\n";
                                         // std::cerr << j.dump(2) << "\n";

                                         // from_json(j, domDocument);
                                         domDocument.from_json(j);

                                         atomicBoolFlag = true;
                                     }
                                   );
        if (!sendRes.success)
            cout << "Send 'DOM.getDocument' command failed" << "\n";

        if (!wsConnection->wsWaitAndDispatchMessagesAndReset(10000 /* ms */, atomicBoolFlag))
            throw std::runtime_error("No reply for 'DOM.getDocument'");

        cout << "'DOM.getDocument' done\n" << "\n";


        atomicBoolFlag = false;
        std::string pageHtml;

        wsConnection->wsSendCommand( "Runtime.evaluate"
                                   , {
                                       {"expression", "document.documentElement.outerHTML"},
                                       {"returnByValue", true}   // чтобы получить строку напрямую, а не objectId
                                     }
                                   , [&](marty::cdt::Connection* pCon, const marty::cdt::WebSocketMessage&, marty::cdt::MessageIdVariant, marty::cdt::json j)
                                     {
                                         // Ответ: { "result": { "type": "string", "value": "<html>...</html>" } }
                                         // if (j.contains("result") && j["result"].contains("value"))
                                         // {
                                         //     pageHtml = j["result"]["value"].get<std::string>();
                                         // }

                                         std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                                         std::cout << "Runtime.evaluate" << "\n";
                                         std::cout << j.dump(2) << "\n";
                                         // std::cerr << j.dump(2) << "\n";

                                         atomicBoolFlag = true;
                                     }
                                   );

        if (!wsConnection->wsWaitAndDispatchMessagesAndReset(10000, atomicBoolFlag))
            throw std::runtime_error("Failed to get HTML");

// <textarea rows="1"
//           placeholder="Найдётся всё"
//           spellcheck="false"
//           accesskey="s"
//           aria-label="Запрос"
//           aria-autocomplete="list"
//           autocapitalize="off"
//           autocomplete="off"
//           autocorrect="off"
//           maxlength="4000"
//           class="search3__input mini-suggest__input"
//           id="text"
//           name="text"
//           autofocus=""
//           role="combobox"
//           aria-expanded="false"
//           aria-controls="suggest-list-45672486"
// >
// </textarea>
        for(auto i=0; i!=20; ++i)
        {
            if (!wsConnection->wsDispatchMessages())
                umba::sleepMs(100);
        }

        wsConnection->wsStop();

        umba::sleepMs(100);

        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Normal exit\n";

    }
    catch(const std::exception &e)
    {
        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Error: " << e.what() << "\n";
    }
    catch(...)
    {
        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Error: " << "unknown error" << "\n";
    }

    cout << "\n";
    std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
    cout << "Exiting\n";

    return 0;
}
~~~

**\_src/almai/\_test17.cpp**
~~~C++
/*! \file
    \brief Тест запуска chrome, отработка открытия страницы, события Page, Network, Runtime, DOM - используем marty::cdt::Connection - специализированные функции

 */

#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
//
#include "marty_cdt/Connection.h"
//
#include "marty_cdt/AtomicBoolIdHandler.h"
#include "marty_cdt/AtomicBoolMethodHandler.h"
//
#include "marty_cdt/Dom.h"


//
#include <iostream>
#include <stdexcept>
#include <memory>
#include <variant>
//

using std::cout;
using std::cerr;
using namespace almai;


inline
int returnReportFailedToConnect(ix::HttpErrorCode c, const std::string &endPoint, int retCode=1)
{
    std::cout << "Failed to connect to '" << endPoint << "' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(c) << " (" << (int)c << ")" << "\n";
    return retCode;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#define CATCH_PARSE_RESPONSE()                        \
                                                      \
    catch(const std::exception &e)                    \
    {                                                 \
        cout << "Error: " << e.what() << "\n";        \
    }                                                 \
    catch(...)                                        \
    {                                                 \
        cout << "Error: " << "unknown error" << "\n"; \
    }





//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    UMBA_USED(argc);
    UMBA_USED(argv);


    {
        std::vector<std::string> patterns = marty::cdt::utils::generatePatterns("page.navigation.navigate");
        cout << "Patterns:\n";
        for(const auto &p: patterns)
        {
            cout << "  " << p << "\n";
        }
    }

    {
        std::vector<std::string> values = {"string", "number", "boolean", "object"};
        std::string merged = marty::cdt::utils::mergeValsToString(values.begin(), values.end(), ",", "or", [](auto s) { return '\'' + s + '\''; });
        cout << "Merged values: " << merged << "\n";
    }

// marty::cdt::utils::
// template<typename IteratorType, typename StringifierType>
// std::string mergeValsToString(IteratorType b, IteratorType e, std::string sepStr, std::string lastSepStr, StringifierType stringifier)


    ix::initNetSystem();

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.initAll(chromeName, false /* !useTempDir */ ))
    {
        cout << "Project root not found" << "\n";
        return 1;
    }

    marty::cdt::Timeouts httpProbeTimeouts   = {1,1};
    marty::cdt::Timeouts httpConnectTimeouts = {1,3};

    //std::string httpBaseUrl = projectDirs.generateConnectionBaseUrlString(true /* http */);

    auto wsConnection = std::make_shared<marty::cdt::Connection>();

    wsConnection->setHttpBaseUrl(projectDirs.generateConnectionBaseUrlString(true /* http */));
    wsConnection->setHttpTimeouts(httpConnectTimeouts);

    cout << "### Run&Connect" << "\n";

    auto spawnArgs = projectDirs.generateArgsForSpawnChrome();
    // cout << "Chrome args:\n";
    // for(auto a: spawnArgs)
    //     cout << "  " << a << "\n";

    std::vector<marty::cdt::JsonListResponseEntry> jsonListResponse;
    auto httpResponse = wsConnection->checkRunAndGetJsonList( jsonListResponse, chromeName, spawnArgs, httpProbeTimeouts);

    if (!wsConnection->httpIsResponseOk(httpResponse))
        throw std::runtime_error("Failed to connect to browser");

    cout << "HTTP code: " << httpResponse->statusCode << "\n";
    cout << "Chrome is running" << "\n";


    marty::cdt::JsonVersionResponse versionResponse;
    httpResponse = wsConnection->httpGetJsonVersion(versionResponse);
    if (!wsConnection->httpIsResponseOk(httpResponse))
        throw std::runtime_error("Failed to browser version");

    cout << "\n" << "Version Info:" << "\n";

    cout << "  " << "browser        : " << versionResponse.browser         << "\n";
    cout << "  " << "protocolVersion: " << versionResponse.protocolVersion << "\n";
    cout << "  " << "userAgent      : " << versionResponse.userAgent       << "\n";
    cout << "  " << "v8Version      : " << versionResponse.v8Version       << "\n";
    cout << "  " << "webkitVersion  : " << versionResponse.webkitVersion   << "\n";
    cout << "  " << "debuggerVersion: " << versionResponse.debuggerVersion << "\n";
    //cout << "  " << ": " << versionResponse. << "\n";


    //cout << "Response:\n" << response->body << "\n";

    std::string defaultNewTabId;
    std::string defaultNewTabPageWsUrl;

    for(const auto &e: jsonListResponse)
    {
        if (e.type=="page" && (e.url=="chrome://newtab/" || e.url=="chrome://newtab"))
        {
            defaultNewTabId = e.id;
            defaultNewTabPageWsUrl = e.webSocketDebuggerUrl;
        }
    }


    if (defaultNewTabPageWsUrl.empty())
    {
        cout << "\n\n";
        cout << "### /json/new" << "\n";

        marty::cdt::JsonNewPageResponse newPageResponse;
        httpResponse = wsConnection->httpNewPage(newPageResponse);
        if (!wsConnection->httpIsResponseOk(httpResponse))
            return returnReportFailedToConnect(httpResponse->errorCode, "/json/new", 1);

        cout << "HTTP code: " << httpResponse->statusCode << "\n";
        cout << "Response:\n" << httpResponse->body << "\n";

        defaultNewTabId        = newPageResponse.id;
        defaultNewTabPageWsUrl = newPageResponse.webSocketDebuggerUrl;

    }


    try
    {
        if (defaultNewTabId.empty())
        {
            throw std::runtime_error("defaultNewTabId is empty");
            //cout << "Error: defaultNewTabId is empty\n";
        }

        wsConnection->wsSetUrl(defaultNewTabPageWsUrl);


        wsConnection->wsSetSystemEventHandler( [&](marty::cdt::Connection */* pCon */, const marty::cdt::WebSocketMessage& /* msg */, ix::WebSocketMessageType type)
                                               {
                                                   std::cout << "Message, type: " << marty::cdt::utils::ixWebSocketMessageTypeToString(type) << "\n" << "\n";
                                               }
                                             );

        struct GenericIdHandler
        {
            std::string handlerName;

            void operator()(marty::cdt::Connection *pCon, const marty::cdt::WebSocketMessage& /* msg */, marty::cdt::MessageIdVariant idVariant, marty::cdt::json j) const
            {
                using std::cout;
                std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                cout << "'" << handlerName << "'" << " ID Handler" << "\n";
                std::visit( [&](auto&& arg)
                            {
                                using T = std::decay_t<decltype(arg)>;

                                if constexpr (std::is_same_v<T, unsigned>)
                                {
                                    cout << "  ID: " << arg << "\n";
                                    cout << "  JSON:" << "\n";
                                    cout << j.dump(2) << "\n";
                                }
                                else if constexpr (std::is_same_v<T, std::string>)
                                {
                                    cout << "  Method: " << arg << "\n";
                                }
                                else if constexpr (std::is_same_v<T, marty::cdt::ResponseError>)
                                {
                                    cout << "  ResponseError:" << "\n";
                                    cout << "    ID     : " << arg.id << "\n";
                                    cout << "    Code   : " << arg.errorInfo.code << "\n";
                                    cout << "    Message: " << arg.errorInfo.message << "\n";
                                }
                                // else
                                //     static_assert(false, "non-exhaustive visitor!");
                            }
                          , idVariant
                          );

                cout << "\n";

            }

        }; // struct GenericIdHandler


        struct GenericMethodHandler
        {
            std::string handlerName;

            void operator()(marty::cdt::Connection *pCon, const marty::cdt::WebSocketMessage& /* msg */, marty::cdt::MessageIdVariant idVariant, marty::cdt::json j) const
            {
                using std::cout;
                std::cout << "timestamp: " << pCon->getTimestamp() << "\n";
                cout << "'" << handlerName << "'" << " Method Handler" << "\n";
                std::visit( [&](auto&& arg)
                            {
                                using T = std::decay_t<decltype(arg)>;

                                if constexpr (std::is_same_v<T, unsigned>)
                                {
                                    cout << "  ID: " << arg << "\n";
                                    cout << "  JSON:" << "\n";
                                    cout << j.dump(2) << "\n";
                                }
                                else if constexpr (std::is_same_v<T, std::string>)
                                {
                                    cout << "  Method: " << arg << "\n";
                                }
                                else if constexpr (std::is_same_v<T, marty::cdt::ResponseError>)
                                {
                                    cout << "  ResponseError:" << "\n";
                                    cout << "    ID     : " << arg.id << "\n";
                                    cout << "    Code   : " << arg.errorInfo.code << "\n";
                                    cout << "    Message: " << arg.errorInfo.message << "\n";
                                }
                                else
                                    static_assert(false, "non-exhaustive visitor!");
                            }
                          , idVariant
                          );

                cout << "\n";
            }

        }; // struct GenericMethodHandler



        wsConnection->wsSetDefaultIdHandler(GenericIdHandler{"DefaultIdHandler"});

        //wsConnection->wsSetMethodEventHandler("*", GenericMethodHandler{"Default"});
        //wsConnection->wsSetMethodEventHandler("Page.*", GenericMethodHandler{"Page"});


        // std::atomic<bool> domContentEventFiredFlag = false;
        // std::atomic<bool> loadEventFiredFlag       = false;
        //
        // wsConnection->wsSetMethodEventHandler("Page.domContentEventFired", marty::cdt::AtomicBoolMethodHandler{domContentEventFiredFlag, true});
        // wsConnection->wsSetMethodEventHandler("Page.loadEventFired"      , marty::cdt::AtomicBoolMethodHandler{loadEventFiredFlag      , true});


        auto wsConnectRes = wsConnection->wsConnect();
        if (!wsConnectRes.success)
        {
            cout << "WS NOT connected" << "\n";
            cout << "http_status: " << wsConnectRes.http_status << "\n";
            cout << "errorStr   : " << wsConnectRes.errorStr    << "\n";

            throw std::runtime_error("WS NOT connected");
        }
        else
        {
            cout << "WS connected" << "\n" << "\n";
        }

        wsConnection->wsStart();

        // wsConnection->wsSetDefaultIdHandler( GenericIdHandler{"Default"}
        auto
        sendRes = wsConnection->wsEventSubscribe("Page", GenericIdHandler{"PageEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Page.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        sendRes = wsConnection->wsEventSubscribe("Network", GenericIdHandler{"NetworkEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Network.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        sendRes = wsConnection->wsEventSubscribe("Runtime", GenericIdHandler{"RuntimeEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'Runtime.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        sendRes = wsConnection->wsEventSubscribe("DOM", GenericIdHandler{"DomEventSubsribe"});
        if (!sendRes.success)
            cout << "Send 'DOM.enable' subscription failed" << "\n";
        wsConnection->wsDispatchMessages();


        marty::cdt::PageNavigateResponse pageNavigateResponse;

        if (!wsConnection->cdtPageNavigate( pageNavigateResponse, "https://яндекс.рф", 30000))
            throw std::runtime_error("Couldn't wait for the page to load");

        if (pageNavigateResponse.frameId.empty())
            throw std::runtime_error("Frame ID not returned");


        cout << "'Page.navigate' done\n" << "\n";
        cout << "Page loaded\n" << "\n";



        cout << "Execute DOM.getDocument\n" << "\n";


        marty::cdt::DomDocument domDocument;
        if (!wsConnection->cdtDomGetDocument(domDocument))
            throw std::runtime_error("No reply for 'DOM.getDocument'");

        cout << "'DOM.getDocument' done\n" << "\n";


        std::string pageHtml;

        {
            marty::cdt::json jOuterHTML;
            if (!wsConnection->cdtRuntimeEvaluate(jOuterHTML, "document.documentElement.outerHTML"))
                throw std::runtime_error("No reply for 'Runtime.evaluate' with 'document.documentElement.outerHTML'");

            pageHtml = jOuterHTML["result"]["value"].get<std::string>();

            std::cout << "document.documentElement.outerHTML JSON:\n";
            std::cout << jOuterHTML.dump(2) << "\n\n";
        }

        //std::cerr << "Page.HTML\n";
        std::cerr << pageHtml << "\n";

        if (!wsConnection->cdtGetHtml(pageHtml))
            std::cout << "cdtGetHtml timedout\n";
        else
            std::cout << "cdtGetHtml: " << pageHtml << "\n";


        // Как связать свои данные (идентификатор) с конкретной страницей персистентно -  https://chat.deepseek.com/share/23l9e1yae94620x4jh
        // сохраняем - window.sessionStorage.setItem
        // получаем - window.sessionStorage.getItem
        // При запуске хрома, если мы хотим подклюсится ко вкладке, которую ранее пометили
        // нужно подключится по ws ко всем вкладкам, и получить свой идентификатор у каждой страницы
        // идентификатор можно задавать по id страницы, который был получен при первом запуске
        // (при восстановлении сессии та же страница получит новый ID, поэтому надо ориентироваться только на тот, который мы сами задали)
        {
            marty::cdt::json jRes;
            if (!wsConnection->cdtRuntimeEvaluate(jRes, "window.sessionStorage.setItem(\'my_tab_identifier\', \'TYRNIYTFTYHJ\')"))
                throw std::runtime_error("No reply for 'Runtime.evaluate' with 'window.sessionStorage.setItem'");

            std::cout << "window.sessionStorage.setItem JSON:\n";
            std::cout << jRes.dump(2) << "\n\n";
        }

        {
            marty::cdt::json jRes;
            if (!wsConnection->cdtRuntimeEvaluate(jRes, "window.sessionStorage.getItem(\'my_tab_identifier\')"))
                throw std::runtime_error("No reply for 'Runtime.evaluate' with 'window.sessionStorage.getItem'");

            std::cout << "window.sessionStorage.getItem JSON:\n";
            std::cout << jRes.dump(2) << "\n\n";

            if (!wsConnection->cdtRuntimeEvaluateGetValue(jRes, "window.sessionStorage.getItem(\'my_tab_identifier\')", "string"))
                throw std::runtime_error("No reply for 'Runtime.evaluate' with 'window.sessionStorage.getItem'");

            std::string str = jRes;
            std::cout << "cdtRuntimeEvaluateGetValue returned: " << str << "\n\n";
            std::cout << "As JSON: " << jRes.dump() << "\n\n";

        }



        // window.sessionStorage.setItem('my_tab_identifier', 'TYRNIYTFTYHJ');
        // window.sessionStorage.getItem('my_tab_identifier');


        //
        for(auto i=0; i!=20; ++i)
        {
            if (!wsConnection->wsDispatchMessages())
                umba::sleepMs(100);
        }

        wsConnection->wsStop();

        umba::sleepMs(100);

        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Normal exit\n";

    }
    catch(const std::exception &e)
    {
        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Error: " << e.what() << "\n";
    }
    catch(...)
    {
        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Error: " << "unknown error" << "\n";
    }

    cout << "\n";
    std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
    cout << "Exiting\n";

    return 0;
}
~~~

**\_src/almai/almai.cpp**
~~~C++
/*! \file
    \brief
 */


// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/debug_helpers.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>

#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"

#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//

#include "PrepromptDatabase.h"

//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

// bool bOverwrite         = false;

//
#include "log.h"
//
#include "AppConfig.h"

//almai::
AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;


#include "ArgParser.h"



int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        LOG_ERR << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        LOG_ERR << "Unknown error\n";
        return 2;
    }

}



int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        // argsParser.args.clear();
        // argsParser.args.push_back("--overwrite");

        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        LOG_MSG << "App Root Path: " << rootPath << "\n";
        LOG_MSG << "Working Dir  : " << cwd << "\n";

    } // if (umba::isDebuggerPresent())


    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        return 1;
    }

    //!!! --------------
    appConfig.setAppRoot(argsParser.getAppRoot(), argsParser.getAppConfPath()); // to find prompts

    if (!appConfig.findProjectRoot()) // Также устанавливает ProjectRoot
    {
        LOG_WARN("prj-root") << "project root not found\n";
    }
    else
    {
        if (!argsParser.quet)
        {
            LOG_MSG << "\n";

            LOG_MSG << "found project root: '" << appConfig.projectRoot << "'\n";
            if (appConfig.projectFile.empty())
                LOG_MSG << "project file not found\n";
            else
                LOG_MSG << "found project file: '" << appConfig.projectFile << "'\n";

            LOG_MSG << "\n";
        }
    }

    appConfig.addEnvironmentPrepromptPaths();
    appConfig.curPrepromptPathType = almai::PrepromptPathType::cliOptions;


    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        return 1;
    }

    if (argsParser.mustExit)
        return 0;


    //if (!argsParser.quet)
    if (1)
    {
        LOG_MSG << "Preprompt dirs:\n";

        auto ppDirsAnnotated = appConfig.getPrepromptDirsAnnotated();
        for(auto &&ppdp : ppDirsAnnotated)
        {
            LOG_MSG << "  " << appConfig.getPrepromptPathTypeAnnotation(ppdp.first) << ": " << ppdp.second << "\n";
        }

        LOG_MSG << "\n";
    }

    if (!argsParser.quet  /* && !hasHelpOption */ )
    {
    }

    if (1)
    {
        LOG_MSG << "Translations test:\n";

        auto printTranslation = [&](std::string lang, std::string key)
        {
            auto text = appConfig.getLocalizedText(lang, key);
            LOG_MSG << "  " << lang << ":" << key << ": " << text << "\n";
        };

        printTranslation("ru", "roles");
        printTranslation("en", "roles");
        printTranslation("ru", "constraints");
        printTranslation("en", "constraints");
        printTranslation("ru", "domains");
        printTranslation("en", "domains");

        LOG_MSG << "\n";
    }


    std::string processedFileType = "project";

    auto prepromptReadingErrorHandler = [&](const std::string &ppFilename)
    {
        LOG_WARN("read-error") << "failed to read " << processedFileType << " file: '" << ppFilename << "'\n";
    };

    auto prepromptParsingErrorHandler = [&](const std::string &ppFilename, const std::exception &e)
    {
        LOG_WARN("parsing-error") << "failed to parsing " << processedFileType << " file: " << e.what() << ", file: '" << ppFilename << "'\n";
    };

    appConfig.readProjectFile(prepromptReadingErrorHandler, prepromptParsingErrorHandler);

    LOG_MSG << appConfig.almaiProject <<"\n";

    // appConfig.curAiEngine = "deepseek"; // !!! Должно вычитываться из настроек проекта
    std::vector<std::string> aiEngines = {"deepseek", "qwen"};

    almai::PrepromptDatabase ppDb = almai::PrepromptDatabase(appConfig.pluralDb, appConfig.getPrepromptDirs());
    //ppDb.prepromptDirs = appConfig.getPrepromptDirs();
    std::vector<std::string> scannedFolders;

    processedFileType = "preprompt";

    ppDb.scanForPreprompts( &scannedFolders, aiEngines
                          , { "skill", "instruction", "knowledge", "format", "output" }
                          , prepromptReadingErrorHandler, prepromptParsingErrorHandler
                          );

    // std::unordered_map< std::string, std::unordered_map<std::string, almai::PrepromptProps> > scannedPrepromptProps;
    // std::unordered_map< std::string, std::unordered_set<std::string> > scannedPrepromptTypes;
    //
    // appConfig.scanForPreprompts(&scannedFolders, scannedPrepromptProps, scannedPrepromptTypes);
    //
    // processedFileType = "preprompt";
    // std::unordered_map< std::string, std::unordered_map<std::string, almai::Preprompt> > scannedPreprompts;
    // appConfig.scanForPreprompts(scannedPreprompts, scannedPrepromptProps, prepromptReadingErrorHandler, prepromptParsingErrorHandler);



    if (1)
    {

        LOG_MSG << "Scanned folders:\n";

        for(const auto &fldr: scannedFolders)
        {
            LOG_MSG << "  " << fldr << "\n";
        }

        LOG_MSG << "\n";


    // struct AiPreprompts
    // {
    //     std::unordered_map< std::string, PrepromptMapType >            preprompts;
    //     std::unordered_map< std::string, PrepromptPropsMapType >       prepromptProps;
    //     std::unordered_map< std::string, PrepromptCategorySetType >    prepromptCategories;
    // };
    // std::unordered_map< std::string, AiPreprompts>    preprompts;  // aiEngine -> aiPreprompts

        for(const auto &[aiEngine, aiPreprompts] : ppDb.preprompts)
        {
            LOG_MSG << "\n";
            LOG_MSG << "AiEngine: " << (!aiEngine.empty() ? aiEngine : std::string("<NONAME>")) << "\n";

            LOG_MSG << "\n  Found preprompts:\n";

            for(const auto &[ppTypeStr, ppNameMap] : aiPreprompts.prepromptProps)
            {
                LOG_MSG << "    " << ppTypeStr << ":\n";

                for(const auto &[ppName, ppProps] : ppNameMap)
                {
                    LOG_MSG << "      " << ppProps << "\n";
                }
            }

            LOG_MSG << "\n";


            LOG_MSG << "  Found preprompt types:\n";

            for(const auto &[ppId, ppTypeSet] : aiPreprompts.prepromptCategories)
            {
                std::size_t cnt = 0;
                LOG_MSG << "    " << ppId; // << ""
                for(const auto ppType: ppTypeSet)
                {
                    LOG_MSG << (cnt ? ", " : ": ") << ppType;
                    ++cnt;
                }
                LOG_MSG << "\n";
            }

            LOG_MSG << "\n";
        }

    }




    return 0;
}
~~~

**\_src/almai/almai-cdt-eval.cpp**
~~~C++
/*! \file
    \brief Тест запуска chrome, отработка открытия страницы, события Page, Network, Runtime, DOM - используем marty::cdt::Connection - специализированные функции

 */

#include "utils.h"
#include "ProjectDirs.h"
//
#include "umba/shellapi.h"
#include "umba/sleep.h"
#include "umba/win32_utils.h"
//
#include "marty_cdt/Connection.h"
//
#include "marty_cdt/AtomicBoolIdHandler.h"
#include "marty_cdt/AtomicBoolMethodHandler.h"
//
#include "marty_cdt/Dom.h"



// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"
//
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"
//
#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//
#include "umba/utf.h"

#if defined(WIN32) || defined(_WIN32)
    #include "umba/clipboard_win32.h"
#endif

//
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>



//----------------------------------------------------------------------------
//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

// bool bOverwrite         = false;

inline
int returnReportFailedToConnect(ix::HttpErrorCode c, const std::string &endPoint, int retCode=1)
{
    std::cout << "Failed to connect to '" << endPoint << "' endpoint, code: " << marty::cdt::utils::ixHttpErrorCodeToString(c) << " (" << (int)c << ")" << "\n";
    return retCode;
}

#define CATCH_PARSE_RESPONSE()                        \
                                                      \
    catch(const std::exception &e)                    \
    {                                                 \
        cout << "Error: " << e.what() << "\n";        \
    }                                                 \
    catch(...)                                        \
    {                                                 \
        cout << "Error: " << "unknown error" << "\n"; \
    }

//
#include "log.h"
//
#include "CdtEvalAppConfig.h"

AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;

#include "CdtEvalArgParser.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
int unsafeMain(int argc, char* argv[]);

UMBA_APP_MAIN()
{
    try
    {
        auto res = unsafeMain(argc, argv);
        return res;
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
        return 2;
    }

}

//----------------------------------------------------------------------------
int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    using std::cout;
    using std::cerr;


    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        // argsParser.args.clear();
        // argsParser.args.push_back("--overwrite");

        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        // argsParser.args.push_back("-Y");
        // argsParser.args.push_back("-o=" + rootPath + "\\tests\\almai-md-join_01.md");
        // //argsParser.args.push_back(rootPath + "/_src/almai/**/*.cpp,*.bat");
        // argsParser.args.push_back("../../../README.md");

        //argsParser.args.push_back("--help");

    } // if (umba::isDebuggerPresent())


    // Job completed - may be, --where option found
    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        // LOG_INFO("config") << "Error found in builtin option files\n";
        return 1;
    }

    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        return 1;
    }

    if (argsParser.mustExit)
        return 0;


    if (!appConfig.quet)
    {
        std::cerr << "\n";
        std::cerr << "Runtime.evaluate options:\n";
        std::cerr << "\n";

        std::cerr << "  " << "contextId                   " << ": " << (appConfig.evalContextId.empty()   ? std::string("<EMPTY>") : appConfig.evalContextId  ) << "\n";
        std::cerr << "  " << "objectGroup                 " << ": " << (appConfig.evalObjectGroup.empty() ? std::string("<EMPTY>") : appConfig.evalObjectGroup)  << "\n";
        std::cerr << "\n";

        std::cerr << "  " << "returnByValue               " << ": " << to_string(appConfig.evalReturnType)   << "\n";
        std::cerr << "  " << "awaitPromise                " << ": " << to_string(appConfig.evalAwaitPromise) << "\n";
        std::cerr << "  " << "userGesture                 " << ": " << to_string(appConfig.evalUserGesture)  << "\n";
        std::cerr << "  " << "throwOnSideEffect           " << ": " << to_string(appConfig.evalThrowOnSideEffect) << "\n";
        std::cerr << "  " << "disableBreaks               " << ": " << to_string(appConfig.evalBreaksControl) << "\n";
        std::cerr << "  " << "replMode                    " << ": " << to_string(appConfig.evalReplMode) << "\n";
        std::cerr << "  " << "allowUnsafeEvalBlockedByCSP " << ": " << to_string(appConfig.evalCspMode) << "\n";
        //std::cerr << "  " << "" << ": " << to_string(appConfig.) << "\n";

        std::cerr << "\n";
    }

    if (!appConfig.expression.empty() && !appConfig.inputFile.empty())
    {
        LOG_ERR << "'--expression' option taken, but input file also taken\n";
        return 2;
    }

    if (appConfig.expression.empty() && appConfig.inputFile.empty())
    {
        LOG_ERR << "'--expression' option nor input file not taken\n";
        return 2;
    }

    std::string expression = appConfig.expression;

    if (!appConfig.inputFile.empty())
    {
        if (!almai::utils::readFile(appConfig.inputFile, expression))
        {
            LOG_ERR << "failed to read input file: '" << appConfig.inputFile << "'" << "\n";
            return 2;
        }
    }

    if (expression.empty())
    {
        LOG_ERR << "expression is empty\n";
        return 2;
    }

    // Возможно будет как-то подключаться к открытой уже странице по её frameId, но пока так
    if (appConfig.openUrl.empty())
    {
        LOG_ERR << "URL not taken or empty ('--url')\n";
        return 2;
    }




    ix::initNetSystem();

    const std::string chromeName = "chrome";


    almai::ProjectDirs projectDirs;

    if (!projectDirs.initAll(chromeName, false /* !useTempDir */ ))
    {
        LOG_ERR << "Project root not found" << "\n";
        return 1;
    }

    marty::cdt::Timeouts httpProbeTimeouts   = {3,3};
    marty::cdt::Timeouts httpConnectTimeouts = {5,5};


    auto wsConnection = std::make_shared<marty::cdt::Connection>();

    wsConnection->setHttpBaseUrl(projectDirs.generateConnectionBaseUrlString(true /* http */));
    wsConnection->setHttpTimeouts(httpConnectTimeouts);

    cout << "### Run&Connect" << "\n";

    auto spawnArgs = projectDirs.generateArgsForSpawnChrome(appConfig.restoreLastSession);
    // cout << "Chrome args:\n";
    // for(auto a: spawnArgs)
    //     cout << "  " << a << "\n";

    std::vector<marty::cdt::JsonListResponseEntry> jsonListResponse;
    marty::cdt::json jsonListResponseJson;
    auto httpResponse = wsConnection->checkRunAndGetJsonList( jsonListResponseJson, chromeName, spawnArgs, httpProbeTimeouts);


    if (!wsConnection->httpIsResponseOk(httpResponse))
        throw std::runtime_error("Failed to connect to browser");

    cout << "HTTP code: " << httpResponse->statusCode << "\n";
    cout << "Chrome is running" << "\n";
    cout << "JsonList JSON:" << "\n";
    cout << jsonListResponseJson.dump(2) << "\n";

    from_json(jsonListResponseJson, jsonListResponse);
    // std::vector<marty::cdt::JsonListResponseEntry> jsonListResponse;
    // marty::cdt::json jsonListResponseJson;


    marty::cdt::JsonVersionResponse versionResponse;
    httpResponse = wsConnection->httpGetJsonVersion(versionResponse);
    if (!wsConnection->httpIsResponseOk(httpResponse))
        throw std::runtime_error("Failed to browser version");

    cout << "\n" << "Version Info:" << "\n";

    cout << "  " << "browser        : " << versionResponse.browser         << "\n";
    cout << "  " << "protocolVersion: " << versionResponse.protocolVersion << "\n";
    cout << "  " << "userAgent      : " << versionResponse.userAgent       << "\n";
    cout << "  " << "v8Version      : " << versionResponse.v8Version       << "\n";
    cout << "  " << "webkitVersion  : " << versionResponse.webkitVersion   << "\n";
    cout << "  " << "debuggerVersion: " << versionResponse.debuggerVersion << "\n";
    //cout << "  " << ": " << versionResponse. << "\n";


    //cout << "Response:\n" << response->body << "\n";

    std::string defaultNewTabId;
    std::string defaultNewTabPageWsUrl;

    for(const auto &e: jsonListResponse)
    {
        if (e.type=="page" && (e.url=="chrome://newtab/" || e.url=="chrome://newtab"))
        {
            defaultNewTabId = e.id;
            defaultNewTabPageWsUrl = e.webSocketDebuggerUrl;
        }
    }


    if (defaultNewTabPageWsUrl.empty())
    {
        cout << "\n\n";
        cout << "### /json/new" << "\n";

        marty::cdt::JsonNewPageResponse newPageResponse;
        httpResponse = wsConnection->httpNewPage(newPageResponse);
        if (!wsConnection->httpIsResponseOk(httpResponse))
            return returnReportFailedToConnect(httpResponse->errorCode, "/json/new", 1);

        cout << "HTTP code: " << httpResponse->statusCode << "\n";
        cout << "Response:\n" << httpResponse->body << "\n";

        defaultNewTabId        = newPageResponse.id;
        defaultNewTabPageWsUrl = newPageResponse.webSocketDebuggerUrl;

    }


    try
    {
        if (defaultNewTabId.empty())
        {
            throw std::runtime_error("defaultNewTabId is empty");
            //cout << "Error: defaultNewTabId is empty\n";
        }

        wsConnection->wsSetUrl(defaultNewTabPageWsUrl);


        // wsConnection->wsSetSystemEventHandler( [&](marty::cdt::Connection */* pCon */, const marty::cdt::WebSocketMessage& /* msg */, ix::WebSocketMessageType  /* type */ )
        //                                        {
        //                                            //std::cout << "Message, type: " << marty::cdt::utils::ixWebSocketMessageTypeToString(type) << "\n" << "\n";
        //                                        }
        //                                      );


        auto wsConnectRes = wsConnection->wsConnect();
        if (!wsConnectRes.success)
        {
            cout << "WS NOT connected" << "\n";
            cout << "http_status: " << wsConnectRes.http_status << "\n";
            cout << "errorStr   : " << wsConnectRes.errorStr    << "\n";

            throw std::runtime_error("WS NOT connected");
        }
        else
        {
            cout << "WS connected" << "\n" << "\n";
        }

        wsConnection->wsStart();


        marty::cdt::PageNavigateResponse pageNavigateResponse;

        if (!wsConnection->cdtPageNavigate( pageNavigateResponse, appConfig.openUrl, appConfig.evalTimeout, appConfig.waitFullCompletion))
        {
            // if (appConfig.waitFullCompletion)
                throw std::runtime_error("Couldn't wait for the page to load");
            // else
            //     wsConnection->wsSleepAndDispatchMessages(appConfig.pageLoadPause);
        }

        if (!appConfig.waitFullCompletion)
             wsConnection->wsSleepAndDispatchMessages(appConfig.pageLoadPause);

        cout << "PageNavigateResponse:\n";
        cout << "  frameId   : " << pageNavigateResponse.frameId    << "\n";
        cout << "  loaderId  : " << pageNavigateResponse.loaderId   << "\n";
        cout << "  isDownload: " << (pageNavigateResponse.isDownload ? "true" : "false") << "\n";
        cout << "\n";


        if (pageNavigateResponse.frameId.empty())
            throw std::runtime_error("Frame ID not returned");


        marty::cdt::json jEvalResult;
        if (!wsConnection->cdtRuntimeEvaluate( jEvalResult, expression
                                             , appConfig.evalTimeout
                                             , appConfig.evalReturnType
                                             , appConfig.evalContextId
                                             , appConfig.evalObjectGroup
                                             , appConfig.evalAwaitPromise
                                             , appConfig.evalUserGesture
                                             , appConfig.evalThrowOnSideEffect
                                             , appConfig.evalBreaksControl
                                             , appConfig.evalReplMode
                                             , appConfig.evalCspMode
                                             //, appConfig.
                                             )
           )
        {
            throw std::runtime_error("No reply for 'Runtime.evaluate'");
        }


        //pageHtml = jOuterHTML["result"]["value"].get<std::string>();

        // std::cout << "document.documentElement.outerHTML JSON:\n";
        std::cout << "------- Reply JSON -------\n";
        std::cout << jEvalResult.dump(2) << "\n\n";


        // ix::HttpResponsePtr httpClosePage(const std::string &pageId) const
        // Где взять pageId?
        wsConnection->wsStop();

    }
    catch(const std::exception &e)
    {
        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Error: " << e.what() << "\n";
    }
    catch(...)
    {
        cout << "\n";
        std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
        cout << "Error: " << "unknown error" << "\n";
    }

    cout << "\n";
    std::cout << "timestamp: " << wsConnection->getTimestamp() << "\n";
    cout << "Exiting\n";


    return 0;
}
~~~

**\_src/almai/almai-md-join.cpp**
~~~C++
/*! \file
    \brief
 */


// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"
//
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"
//
#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//
#include "umba/utf.h"

#if defined(WIN32) || defined(_WIN32)
    #include "umba/clipboard_win32.h"
#endif

//
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>



//----------------------------------------------------------------------------
//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

// bool bOverwrite         = false;

//
#include "log.h"
//
#include "MdjAppConfig.h"

AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;

#include "MdjArgParser.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
int unsafeMain(int argc, char* argv[]);

UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
        return 2;
    }

}

//----------------------------------------------------------------------------
int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        // argsParser.args.clear();
        // argsParser.args.push_back("--overwrite");

        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        // argsParser.args.push_back("-Y");
        // argsParser.args.push_back("-o=" + rootPath + "\\tests\\almai-md-join_01.md");
        // //argsParser.args.push_back(rootPath + "/_src/almai/**/*.cpp,*.bat");
        // argsParser.args.push_back("../../../README.md");

        //argsParser.args.push_back("--help");


        argsParser.args.push_back("-Y");
        argsParser.args.push_back("-J=100");
        argsParser.args.push_back(rootPath + "/_src/almai/**/*.cpp");

    } // if (umba::isDebuggerPresent())


    // Job completed - may be, --where option found
    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        // LOG_INFO("config") << "Error found in builtin option files\n";
        return 1;
    }

    //!!! --------------
    appConfig.setAppRoot(argsParser.getAppRoot(), argsParser.getAppConfPath()); // to find prompts

    if (!appConfig.findProjectRoot()) // Также устанавливает ProjectRoot
    {
        LOG_WARN("prj-root") << "project root not found\n";
    }
    else
    {
        if (!argsParser.quet)
        {
            LOG_MSG << "\n";

            LOG_MSG << "found project root: '" << appConfig.projectRoot << "'\n";
            if (appConfig.projectFile.empty())
                LOG_MSG << "project file not found\n";
            else
                LOG_MSG << "found project file: '" << appConfig.projectFile << "'\n";

            LOG_MSG << "\n";
        }
    }

    appConfig.addEnvironmentPrepromptPaths();
    appConfig.curPrepromptPathType = almai::PrepromptPathType::cliOptions;

    //!!! --------------

    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        return 1;
    }

    if (argsParser.mustExit)
        return 0;


    appConfig.checkUpdateOutput();

    if (!argsParser.quet)
    //if (1)
    {
        LOG_MSG << "Preprompt dirs:\n";

        auto ppDirsAnnotated = appConfig.getPrepromptDirsAnnotated();
        for(auto &&ppdp : ppDirsAnnotated)
        {
            LOG_MSG << "  " << appConfig.getPrepromptPathTypeAnnotation(ppdp.first) << ": " << ppdp.second << "\n";
        }

        LOG_MSG << "\n";
    }


    std::string processedFileType = "project";

    auto prepromptReadingErrorHandler = [&](const std::string &ppFilename)
    {
        LOG_WARN("read-error") << "failed to read " << processedFileType << " file: '" << ppFilename << "'\n";
    };

    auto prepromptParsingErrorHandler = [&](const std::string &ppFilename, const std::exception &e)
    {
        LOG_WARN("parsing-error") << "failed to parsing " << processedFileType << " file: " << e.what() << ", file: '" << ppFilename << "'\n";
    };

    appConfig.readProjectFile(prepromptReadingErrorHandler, prepromptParsingErrorHandler);

    LOG_MSG << "Project: " << appConfig.almaiProject <<"\n";

    if (appConfig.aiName.empty())
        appConfig.aiName = "deepseek";






    //--------------------------------------------------------------------------------------------------------------------
    appConfig.setMacro("CWD", umba::filesys::getCurrentDirectory(), false, true);


    if (appConfig.scanInfos.empty())
    {
        // LOG_ERR << "no input files/masks taken" << "\n";
        // return 1;

        LOG_WARN("no-input") << "no input files/masks taken" << "\n";
    }

    std::size_t notFoundScanMasksCount = 0;
    std::vector<std::string> foundFiles;
    for(const auto &s : appConfig.scanInfos)
    {
        std::vector<std::string> tmp;
        s.scanForFiles(tmp);

        if (tmp.empty())
        {
            LOG_WARN("not-found") << "no files found for scan info" << "\n";
            LOG_MSG << "  " << s.toString() << "\n";
            ++notFoundScanMasksCount;
        }
        else
        {
            foundFiles.insert(foundFiles.end(), tmp.begin(), tmp.end());
        }
    }

    if (!appConfig.scanInfos.empty() && foundFiles.empty())
    {
        // LOG_ERR << "no files found" << "\n";

        LOG_WARN("no-input") << "no files found" << "\n";

        LOG_MSG << "Scan paths & masks:\n";
        for(auto &&s : appConfig.scanInfos)
        {
            LOG_MSG << "  " << s.toString() << "\n";
        }

        LOG_MSG << "\n";

        return 1;
    }

    if (notFoundScanMasksCount)
    {
        LOG_MSG << "Macros:\n";
        auto macros = appConfig.getSortedMacros();
        for(const auto& [k, v] : macros)
            LOG_MSG << "  " << k << ": " << v << "\n";
    }


    for( auto &&f: foundFiles)
    {
        appConfig.foundFileInfos.emplace_back(f);
    }

    for( auto &ffi: appConfig.foundFileInfos)
    {
        ffi.stripPrefix(appConfig.stripPrefixes);
    }


    //--------------------------------------------------------------------------------------------------------------------
    // В главной тулзе это не нужно будет, там будет использоваться для стрипа каталог проекта
    std::string commonPrefix = almai::findMostCommonPathPrefix(appConfig.foundFileInfos.begin(), appConfig.foundFileInfos.end());
    commonPrefix = almai::checkCorrectMostCommonPathPrefixIsPath(commonPrefix);
    if (!commonPrefix.empty())
    {
        for(auto &ffi: appConfig.foundFileInfos)
        {
            ffi.stripPrefix(commonPrefix);
        }
    }


    //--------------------------------------------------------------------------------------------------------------------

    for(const auto &ffi: appConfig.foundFileInfos)
    {
        if (!ffi.isPrefixStripped())
        {
            LOG_ERR << "prefixes stripped not for all files, file: '" << ffi.fullName << "'" << "\n";
            return 1;
        }
    }

    almai::sortFoundFileInfos(appConfig.foundFileInfos);

    std::vector<std::string> readedFiles;

    //std::cerr << "Send files list\n";
    for(auto &ffi: appConfig.foundFileInfos)
    {
        if (!almai::utils::readFile(ffi.fullName, ffi.fileLines))
        {
            LOG_WARN("read-failed") << "failed to read file: '" << ffi.fullName << "'";
        }
        else
        {
            readedFiles.push_back(ffi.fullName);
            //std::cerr << "  " << ffi.fullName << "\n";
        }
    }


    //if (appConfig.listOnly)
    {
        UMBA_LOG_MSG << "\nFound files:\n";

        for(auto && ff: readedFiles)
        {
            UMBA_LOG_MSG << "  " << ff << "\n";
        }

        UMBA_LOG_MSG << "\n";

        if (appConfig.listOnly)
            return 0;
    }

    // Теперь надо для каждого файла найти fence
    // Вывести в конечный документ и записать результат


    std::vector<std::string> resLines;

    {
        auto lines = appConfig.makePrepromptHeader();
        if (!lines.empty())
            resLines = lines;
    }


    if (appConfig.isSourcesInline())
    {
        std::stringstream oss;

        for(auto &ffi: appConfig.foundFileInfos)
        {
            appConfig.generateMarkdownListing(oss, ffi.displayName, ffi.fileLines);
        }

        auto mdArcLines = appConfig.stripEmptyHeadTailLines(marty_cpp::splitToLinesSimple(oss.str()));

        if (!mdArcLines.empty())
        {
            appConfig.addMdPartSeparator(resLines);
            resLines.insert(resLines.end(), mdArcLines.begin(), mdArcLines.end());
        }
    }
    else
    {
        std::string errStr;
        if (!appConfig.saveAttach(errStr))
        {
            if (errStr.empty())
                LOG_ERR << "failed to write attach file: '" << appConfig.getAttachName() << "'" << "\n";
            else
                LOG_ERR << "failed to write attach file: '" << appConfig.getAttachName() << "': " << errStr << "\n";
            return 1;
        }
    }



    {
        auto lines = appConfig.makePrepromptFooter();
        if (!lines.empty())
        {
            appConfig.addMdPartSeparator(resLines);
            resLines.insert(resLines.end(), lines.begin(), lines.end());
        }
    }



    bool printToStdOut = appConfig.output.empty();
    if (appConfig.useClipboard)
        printToStdOut = false;

    bool writeToFile = !appConfig.output.empty();


    std::string fullName;
    std::size_t sizeTotal = 0;

    if (writeToFile)
    {
        if (!appConfig.writeFile(appConfig.output, resLines, &fullName, &sizeTotal))
        {
            LOG_ERR << "failed to write file: '" << fullName << "'\n";
            return 1;
        }
    }

    auto allText = appConfig.mergeLines(resLines);

    if (printToStdOut)
    {
        std::cout << allText;
    }

    if (!argsParser.quet)
    {
        LOG_MSG << "Result size: " << almai::utils::formatFileSize(sizeTotal) << "\n";

        LOG_MSG << "Line limit to join: ";
        if (appConfig.isSetJoinLinesLimit())
            LOG_MSG << appConfig.joinLinesLimit;
        else
            LOG_MSG << "not set";
        LOG_MSG << "\n";
    }

    if (appConfig.useClipboard)
    {
        #if defined(WIN32) || defined(_WIN32)
        //auto allText = appConfig.mergeLines(resLines);
        if (!umba::win32::clipboardTextSet( allText, [](const std::string &t ) { return umba::fromUtf8(t); } /* fromUtfConverter */ , true /* utf */ , umba::win32::clipboardGetConsoleHwnd()))
        {
            LOG_WARN("clipbrd") << "failed to set clipboard text\n";
        }
        #else
        #endif
    }


    return 0;

}
~~~

**\_src/almai/almai-md-split.cpp**
~~~C++
/*! \file
    \brief
 */


// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"
//
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"
//
#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

#include "md_utils.h"

//
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>



//----------------------------------------------------------------------------
//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

// bool bOverwrite         = false;

std::size_t mdArchivePartSeparatorLen = 5;

//
#include "log.h"
//
#include "MdsAppConfig.h"

AppConfig appConfig;

std::string curFile;
unsigned lineNo = 0;

#include "MdsArgParser.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
bool splitLinesAndSaveContent( std::vector<std::string> mdLines
                             , std::size_t partSeparatorLen=0 // по умолчанию - не используем разделение на части, считаем весь файл просто md-архивом, иначе - md-архив - только последняя часть
                             )
{
    std::vector<md::MdArchivePart> mdArchiveParts;
    if (!md::splitMarkdownArchive(mdArchiveParts, mdLines, appConfig.keepFilenameComment, partSeparatorLen))
        return false;

    std::vector<std::size_t> autosaveCounters; // по счётчику на часть
    autosaveCounters.resize(mdArchiveParts.size(), 0);

    // std::vector<std::size_t> fileArchPartMarkers; // по счётчику на часть
    // fileArchPartMarkers.resize(mdArchiveParts.size(), 0);


// struct MdArchivePart
// {
//     std::vector<std::string> textLines;
//     std::vector<ListingInfo> listings ;

    std::unordered_map<std::string, std::size_t>  filenameCounters;

    md::MdArchivePart collectedData;

    std::size_t partNo = 0;
    for(auto& archivePart: mdArchiveParts)
    {
        for(auto &listingInfo : archivePart.listings)
        {
            auto ext = appConfig.findLangExtention(listingInfo.foundLangName);
            if (ext.empty())
                ext = "txt";

            if (listingInfo.listingFilenames.empty())
            {
                listingInfo.listingFilenames.emplace_back("autosave");
                ++autosaveCounters[partNo];
            }

            listingInfo.checkAddExtention(ext);
            listingInfo.checkAutoEnumerate(filenameCounters);
        }


        std::size_t autosavePercent = 100;
        if (!archivePart.listings.empty())
        {
            // Только если листинги присутствуют

            // Если большинство листингов в части - autosave, то весьма вероятно
            // что это текстовая часть с примерами кода
            // Если autosave мало, то это - часть с кодом

            autosavePercent = 100*autosaveCounters[partNo]/archivePart.listings.size();
        }

        if (autosavePercent<20)
        {
            // Эта часть - с листингами
        //    fileArchPartMarkers[partNo] = 1;
            collectedData.listings.insert(collectedData.listings.end(), archivePart.listings.begin(), archivePart.listings.end());

            if (!collectedData.allText.empty() && archivePart.textLines.empty())
                collectedData.allText.emplace_back();

            collectedData.allText.insert(collectedData.allText.end(), archivePart.textLines.begin(), archivePart.textLines.end());
        }
        else // без листингов
        {
            if (!collectedData.allText.empty() && archivePart.allText.empty())
                collectedData.allText.emplace_back();

            // тут мы просто весь текст добавлем, е не только выкусанную часть без кода
            collectedData.allText.insert(collectedData.allText.end(), archivePart.allText.begin(), archivePart.allText.end());
        }

        ++partNo;
    }



    if (appConfig.listOnly)
    {
        // std::cout
        if (collectedData.listings.empty())
        {
            UMBA_LOG_MSG << "\nNo listings found\n";
        }
        else
        {
            UMBA_LOG_MSG << "\nFound listings:\n";

            for(const auto &listingInfo : collectedData.listings)
            {
                for(const auto &name : listingInfo.listingFilenames)
                {
                    // std::cout
                    UMBA_LOG_MSG << "  " << name << "\n";
                }

                std::size_t printLines = listingInfo.listingCodeLines.size();
                if (printLines>appConfig.listLines)
                    printLines = appConfig.listLines;

                for(std::size_t i=0u; i!=printLines; ++i)
                {
                    //std::cout
                    UMBA_LOG_MSG << "    >" << listingInfo.listingCodeLines[i] << "\n";
                }

                // std::cout
                UMBA_LOG_MSG << "\n";
            }
        }

        return true;
    }


    bool hasErrors = false;

    for(const auto &listingInfo : collectedData.listings)
    {
        for(const auto &name : listingInfo.listingFilenames)
        {
            std::string fullName;
            if (!appConfig.writeFile(name, listingInfo.listingCodeLines, &fullName))
            {
                 hasErrors = true;
                 LOG_ERR << "failed to write file: '" << fullName << "'\n";
            }
        }
    }


#if 0

    if (lastSeparatorLineNo!=(unsigned)-1 && lastSeparatorLineNo<(unsigned)mdLines.size())
    {
        almai::MdLineType mdLineType = md::detectMarkdownLineType(mdLines[lastSeparatorLineNo], 0, 0 /* &markerChar, &markerLen */ );
        while(lastSeparatorLineNo && (mdLineType== almai::MdLineType::emptyLine || mdLineType== almai::MdLineType::headerSetext))
        {
            --lastSeparatorLineNo;
            mdLineType = md::detectMarkdownLineType(mdLines[lastSeparatorLineNo], 0, 0 /* &markerChar, &markerLen */ );
        }

        // if (lastSeparatorLineNo)
        //     --lastSeparatorLineNo;

        //auto descriptionLines = std::vector<std::string>(&mdLines[0], &mdLines[std::size_t(lastSeparatorLineNo)]);
        auto descriptionLines = collectedData.allText;
        if (!descriptionLines.empty())
        {
            std::string fullName;
            if (!appConfig.writeFile(appConfig.descriptionFilename, descriptionLines, &fullName))
            {
                 hasErrors = true;
                 LOG_ERR << "failed to write file: '" << fullName << "'\n";
            }
        }
    }

#else

    if (!collectedData.allText.empty())
    {
        std::string fullName;
        if (!appConfig.writeFile(appConfig.descriptionFilename, collectedData.allText, &fullName))
        {
             hasErrors = true;
             LOG_ERR << "failed to write file: '" << fullName << "'\n";
        }
    }


#endif




    return !hasErrors;
}

inline
bool splitFileAndSaveContent( const std::string &fileName
                            , std::size_t partSeparatorLen=0 // по умолчанию - не используем разделение на части, считаем весь файл просто md-архивом, иначе - md-fh[bd - только последняя часть
                            )
{
    std::vector<std::string> mdLines;

    if (!almai::utils::readFile(fileName, mdLines))
    {
        LOG_ERR << "failed to read input file: '" << fileName << "'" << "\n";
        return false;
    }

    curFile = fileName;

    return splitLinesAndSaveContent(mdLines, partSeparatorLen);
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
int unsafeMain(int argc, char* argv[]);

UMBA_APP_MAIN()
{
    try
    {
        auto res = unsafeMain(argc, argv);
        return res;
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
        return 2;
    }

}

//----------------------------------------------------------------------------
int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        // argsParser.args.clear();
        // argsParser.args.push_back("--overwrite");

        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        // argsParser.args.push_back("-B");
        // argsParser.args.push_back("-Y");
        //
        // // argsParser.args.push_back("--list");
        // argsParser.args.push_back("-o=" + rootPath + "\\tests\\almai-md-split");
        // //argsParser.args.push_back(rootPath + "\\tests\\almai-md-split.md");


    } // if (umba::isDebuggerPresent())


    // Job completed - may be, --where option found
    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parseStdBuiltins())
    {
        // LOG_INFO("config") << "Error found in builtin option files\n";
        return 1;
    }

    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
    {
        // LOG_INFO("config") << "Error found while parsing command line arguments\n";
        return 1;
    }
    // LOG_INFO("config") << "-----------------------------------------" << "\n";

    if (argsParser.mustExit)
        return 0;


    if ( appConfig.inputFiles.empty()
      #if defined(WIN32) || defined(_WIN32)
      && !appConfig.useClipboard
      #endif
       )
    {
        LOG_ERR << "no input files taken" << "\n";
        return 1;
    }


    appConfig.checkUpdateOutputDir();


    bool hasErrors = false;

#if defined(WIN32) || defined(_WIN32)
    if (appConfig.useClipboard)
    {
        std::string clpbText;
        if (!umba::win32::clipboardTextGet( clpbText, [](const std::wstring &t ) { return umba::toUtf8(t); } /* toUtfConverter */ , 0 /* pUtf */ , umba::win32::clipboardGetConsoleHwnd()))
        {
            LOG_WARN("clipbrd") << "failed to get clipboard text\n";
        }
        else
        {
            std::vector<std::string> clpbLines = almai::utils::splitTextToLines(clpbText);
            if (!splitLinesAndSaveContent(clpbLines, mdArchivePartSeparatorLen))
            {
                hasErrors = true;
            }

            if (!appConfig.saveClipboard.empty())
            {
                std::string fullName;
                if (!appConfig.writeFile(appConfig.saveClipboard, clpbLines, &fullName))
                {
                     hasErrors = true;
                     LOG_ERR << "failed to write file: '" << fullName << "'\n";
                }
            }
        }
    }
#endif


    for(const auto &inputFileName : appConfig.inputFiles)
    {
        if (!splitFileAndSaveContent(inputFileName, mdArchivePartSeparatorLen))
        {
            hasErrors = true;
        }
    }


    return hasErrors ? 1 : 0;
}
~~~

**\_src/almai/almai\_enums.h**
~~~C/C++ Header
#pragma once

#include "enums.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai{

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
enum class MdLineType
{
    emptyLine   = 0,
    regularLine    ,
    headerAtx      ,           // # - Atx - word processor on Amiga, min 1 char
    headerSetext   ,           // ---- / ==== Setext (Structure Enhanced Text), min 1 char
    codeTilda      ,
    codeBacktick   ,
    codeIndentTab  ,
    codeIndentSpace,
    unorderedList  ,
    orderedList    ,
    quotation                 // >

}; // enum class MdLineType

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------
~~~

**\_src/almai/AppConfig.cpp**
~~~C++
/*! \file
    \brief Конфиг главного приложения - реализация
 */

#include "AppConfig.h"
//
#include "umba/filename.h"
#include "umba/parse_utils.h"

//
#include <utility>

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
namespace almai {

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------

} // namespace almai
~~~

**\_src/almai/AppConfig.h**
~~~C/C++ Header
/*! \file
    \brief Конфиг главного приложения
 */

#pragma once


//
#include "AppConfigBase.h"
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
struct AppConfig : public AppConfigBase
{




}; // struct AppConfig

//--------------------------------------------------------------------------------------------------------------------
~~~

**\_src/almai/AppConfigBase.cpp**
~~~C++
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
~~~

**\_src/almai/AppConfigBase.h**
~~~C/C++ Header
/*! \file
    \brief Базовый конфиг
 */

//----------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------
#include "enums.h"
#include "utils.h"
#include "md_utils.h"
#include "FileSystemScanInfo.h"
#include "FoundFileInfo.h"
#include "Preprompt.h"
#include "PluralDatabase.h"
#include "Localization.h"
#include "Project.h"
#include "PrepromptDatabase.h"

//
#include "umba/umba.h"
#include "encoding/encoding.h"
//
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_cpp.h"
//
#include "umba/string.h"
//
#include "utils.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "umba/macro_helpers.h"
#include "umba/macros.h"


//
#include <algorithm>
#include <iterator>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfigBase
{

    bool                         overwrite         = false;
    bool                         quet              = false;
    bool                         useClipboard      = false;
    std::string                  saveClipboard;

    bool                         listOnly          = false; // list files, but don't save them
    std::size_t                  listLines         = 5;

    std::size_t                  joinLinesLimit    = (std::size_t)-1;

    almai::FileNameSortOrder     dirTreeSortOrder  = almai::FileNameSortOrder::ascDirsType; // asc | dirsFirst | sortByType
    almai::AttachFormat          attachFormat      = almai::AttachFormat::none;
    std::string                  attachFileName;


    //------------------------------
    using ELinefeedType = marty_cpp::ELinefeedType;

    std::string                  output;
    ELinefeedType                outputLinefeedType = ELinefeedType::systemDefault;


    bool isSourcesInline() const
    {
        return attachFormat == almai::AttachFormat::none;
    }


    std::string getAttachName() const
    {
        if (!attachFileName.empty())
            return attachFileName;

        auto name = umba::filename::getName(output);
        auto path = umba::filename::getPath(output);

        if (!name.empty())
            name += "_sources";
        else
            name = "sources_attach";

        if (path.empty())
            path = umba::filesys::getCurrentDirectory();

        switch(attachFormat)
        {
            case almai::AttachFormat::invalid : throw std::runtime_error("saveAttach: invalid attachFormat"); // break;

            case almai::AttachFormat::no : throw std::runtime_error("getAttachName: no attacment"); // break;
            case almai::AttachFormat::md : name = umba::filename::appendExtention(name, std::string("md") ); break;
            case almai::AttachFormat::zip: name = umba::filename::appendExtention(name, std::string("zip")); break;
            case almai::AttachFormat::_7z: name = umba::filename::appendExtention(name, std::string("7z") ); break;
        }

        return umba::filename::appendPath(path, name);
    }

    bool saveAttach(std::string & /* errMsg */ ) const
    {
        std::vector<std::uint8_t> attachData;

        switch(attachFormat)
        {
            case almai::AttachFormat::invalid : throw std::runtime_error("saveAttach: invalid attachFormat"); // break;

            case almai::AttachFormat::no : throw std::runtime_error("saveAttach: no attacment"); // break;

            case almai::AttachFormat::md :
            {
                std::stringstream oss;

                for(const auto &ffi: foundFileInfos)
                {
                    generateMarkdownListing(oss, ffi.displayName, ffi.fileLines);
                }

                auto mdSourcesLines = marty_cpp::splitToLinesSimple(oss.str());
                std::string mdSources = mergeLines(mdSourcesLines);

                const std::uint8_t *pBegin = (const std::uint8_t*)mdSources.data();
                const std::uint8_t *pEnd   = std::next(pBegin, std::ptrdiff_t(mdSources.size()));
                attachData = std::vector<std::uint8_t>(pBegin, pEnd);

                break;
            }

            case almai::AttachFormat::zip: throw std::runtime_error("saveAttach: zip attachments not implemented"); break;
            case almai::AttachFormat::_7z: throw std::runtime_error("saveAttach: zip attachments not implemented"); break;
        }

        return umba::filesys::writeFile(getAttachName(), attachData, overwrite);
    }

    //
    // no         = 0x0000 /*!<  */,
    // md         = 0x0001 /*!<  */,
    // markdown   = 0x0001 /*!<  */,
    // zip        = 0x0002 /*!<  */,
    // _7z        = 0x0003 /*!<  */
    //

        //std::string mergeLines(const std::vector<std::string> &lines) const

    // if (appConfig.isSourcesInline())
    // {
    //     std::stringstream oss;
    //
    //     for(auto &ffi: appConfig.foundFileInfos)
    //     {
    //         appConfig.generateMarkdownListing(oss, ffi.displayName, ffi.fileLines);
    //     }
    //
    //     auto mdLines = marty_cpp::splitToLinesSimple(oss.str());
    //
    //     resLines.insert(resLines.end(), mdLines.begin(), mdLines.end());
    // }
    // else
    // {
    //     std::string errStr;
    //     if (!appConfig.saveAttach(errStr))
    //     {
    //         LOG_ERR << "failed to write attach file: '" << appConfig.getAttachName() << "': " << errStr << "\n";
    //         return 1;
    //     }
    // }


    void checkUpdateOutputDir()
    {
        if (output.empty())
        {
            output = umba::filesys::getCurrentDirectory();
        }

        output = umba::filename::makeAbsPath(output);
    }

    void checkUpdateOutput()
    {
        if (!output.empty())
        {
            // output = umba::filesys::getCurrentDirectory();
            output = umba::filename::makeAbsPath(output);
        }
    }


    static std::vector<std::string> stripEmptyHeadLines(const std::vector<std::string> &lines) { return md::stripEmptyLeadingLines(lines); }
    static std::vector<std::string> stripEmptyTailLines(std::vector<std::string> lines)        { return md::stripEmptyTrailingLines(lines); }
    static std::vector<std::string> stripEmptyHeadTailLines(std::vector<std::string> lines)    { return md::stripEmptyLeadingTrailingLines(lines); }

    static
    void appendLines(std::vector<std::string> &linesAppendTo, const std::vector<std::string> &lines)
    {
        linesAppendTo.insert(linesAppendTo.end(), lines.begin(), lines.end());
    }

    std::string mergeLines(const std::vector<std::string> &lines) const
    {
        bool addTrailingNewLine = true;
        if (lines.empty())
        {
            addTrailingNewLine = false;
        }
        else
        {
            if (lines.back().empty())
                addTrailingNewLine = false;
        }

        //std::string text =
        return marty_cpp::mergeLines(lines, outputLinefeedType,  /* true */ addTrailingNewLine);
    }

    static
    void splitHeaderFooter(const std::vector<std::string> &hefooter, std::vector<std::string> &header, std::vector<std::string> &footer)
    {
        // header.clear();
        // footer.clear();

        std::vector<std::string> *pResVec = &header;

        for(auto line : hefooter)
        {
            umba::string::rtrim(line);

            bool foundSep = false;

            if (!line.empty())
            {
                std::size_t nChars = md::getNumberOfFirstSameChars(line);
                if (line[0]=='-' && nChars>=3)
                {
                    foundSep = true;
                }
                else if (line[0]==';')
                {
                    continue; // строка закоментированна
                }
            }

            if (foundSep)
            {
                pResVec = &footer;
                continue;
            }

            pResVec->push_back(line);
        }

        header = stripEmptyHeadTailLines(header);
        footer = stripEmptyHeadTailLines(footer);
    }

    // Если передавать абсолютное имя файла, то можно писать куда угодно
    // pFullName - опционально возвращаемое значение, возвращает полное имя файла, куда производилась запись (или пытались)
    bool writeFile(const std::string &filename, const std::string &filedata, std::string *pFullName=0) const
    {
        auto fullName = umba::filename::makeAbsPath(filename, output);
        if (pFullName)
           *pFullName = fullName;

        auto dir = umba::filename::getPath(fullName);
        if (!dir.empty())
            umba::filesys::createDirectoryEx(dir, true /* forceCreatePath */ );

        return umba::filesys::writeFile(fullName, filedata, overwrite);
    }

    bool writeFile(const std::string &filename, const std::vector<std::string> &lines, std::string *pFullName=0, std::size_t *pSizeTotal=0) const
    {
        // bool addTrailingNewLine = true;
        // if (lines.empty())
        // {
        //     addTrailingNewLine = false;
        // }
        // else
        // {
        //     if (lines.back().empty())
        //         addTrailingNewLine = false;
        // }
        //
        // std::string text = marty_cpp::mergeLines(lines, outputLinefeedType,  /* true */ addTrailingNewLine);

        std::string text = mergeLines(lines);

        if (pSizeTotal)
           *pSizeTotal = text.size();

        return writeFile(filename, text, pFullName);
    }


    //------------------------------

    std::unordered_map<std::string, std::string>     langExtDict;
    std::unordered_map<std::string, std::string>     extLangDict;


    bool addLangExtention(std::string langName, std::string langExtention)
    {
        umba::string::trim(langName);
        if (langName.empty())
            return false;

        umba::string::trim(langExtention);
        if (langExtention.empty())
            return false;

        if (langExtention.front()=='.')
            langExtention.erase(0,1);

        umba::string::trim(langExtention);
        if (langExtention.empty())
            return false;


        umba::string::tolower(langExtention);

        extLangDict[langExtention] = langName;


        umba::string::tolower(langName);

        langExtDict[langName] = langExtention;

        return true;
    }

    bool addLangExtention(const std::string &langNameExtention)
    {
        auto pos = langNameExtention.find_first_of(" :");
        if (pos==langNameExtention.npos)
            return false;

        auto ext  = std::string(langNameExtention, 0, pos);
        auto lang = std::string(langNameExtention, pos+1 );

        return addLangExtention(lang, ext);
    }

    std::string findLangExtention(std::string langName) const
    {
        umba::string::trim(langName);
        umba::string::tolower(langName);

        std::unordered_map<std::string, std::string>::const_iterator it = langExtDict.find(langName);
        if (it==langExtDict.end())
        {
            return md::replaceInvalidFileNameChars(langName, true /* replaceSpaceAlso */ );
        }

        return it->second;
    }

    std::string findExtentionLang(std::string ext) const
    {
        umba::string::trim(ext);
        umba::string::tolower(ext);

        std::unordered_map<std::string, std::string>::const_iterator it = extLangDict.find(ext);
        if (it==extLangDict.end())
        {
            return ext;
        }

        return it->second;
    }

    //------------------------------



    //------------------------------
    using StringStringMap = umba::macros::StringStringMap<std::string>;
    StringStringMap         macros;

    std::map<std::string, std::string> getSortedMacros() const
    {
        std::map<std::string, std::string> resMap;
        for(const auto& [k, v] : macros)
            resMap[k] = v;
        return resMap;
    }

    std::string substMacros(const std::string &str) const
    {
        using namespace umba::macros;
        return umba::macros::substMacros( str, MacroTextFromMapRef(macros)
                                        , smf_uppercaseNames | smf_KeepUnknownVars | smf_uppercaseNames
                                        );
    }

    // return true if added
    bool setMacro(std::string name, std::string value, bool bSubst, bool allowOverwrite)
    {
        umba::string::trim(name);
        umba::string::trim(value);

        umba::string::toupper(name);

        auto it = macros.find(name);

        if (it!=macros.end())
        {
            if (!allowOverwrite)
                return false;
        }

        if (bSubst)
            value = substMacros(value);

        macros[name] = value;

        return true;
    }

    bool setMacro(std::string nameValue, bool bSubst, bool allowOverwrite)
    {
        std::string name, value;
        if (!umba::string::split_to_pair(nameValue, name, value, '='))
            return false;

        return setMacro(name, value, bSubst, allowOverwrite);
    }

    bool setMacroFromEnv(std::string name, bool bSubst, bool allowOverwrite)
    {
        umba::string::trim(name);

        std::string value;
        umba::env::getVar(name, value);

        return setMacro(name, value, bSubst, allowOverwrite);
    }

    //------------------------------



    //------------------------------
    std::vector<almai::FileSystemScanInfo>    scanInfos;
    std::vector<almai::FoundFileInfo>         foundFileInfos;
    std::vector<std::string>                  stripPrefixes;


    almai::FilenameDecorationType             filenameDecorationType  = almai::FilenameDecorationType::text;
    almai::FilenameDecorationStyle            filenameDecorationStyle = almai::FilenameDecorationStyle::bold;
    std::size_t                               filenameTitleLevel = 6;
    almai::ComparisonType                     sortCriteria = almai::ComparisonType::nameExt;
    almai::SortOrder                          sortOrder    = almai::SortOrder::ascending;
    almai::FenceStyle                         fenceStyle   = almai::FenceStyle::auto_;
    almai::CodeLanguageMarker                 langMarker   = almai::CodeLanguageMarker::langName;

    //------------------------------

    // Extract "name: value" pairs from lines
    template<typename NameValueHandlerType>
    std::vector<std::string> parseLinesExtractValues(const std::vector<std::string> &lines, const std::string &basePath, NameValueHandlerType handler)
    {
        // auto basePath = umba::filename::getPath(linesFromFile);

        std::vector<std::string> resLines;

        for(const auto &l : lines)
        {
            auto lCopy = umba::string::trim_copy(l);

            if (lCopy.empty())
            {
                resLines.push_back(l);
                continue;
            }

            if (lCopy.front()==';')
            {
                continue;
            }

            if (lCopy.front()!='!')
            {
                resLines.push_back(l);
                continue;
            }

            lCopy.erase(0, 1);

            std::string name, value;
            if (umba::string::split_to_pair(lCopy, name, value, ':'))
            {
                umba::string::trim(name);
                umba::string::trim(value);
                value = marty_cpp::unquoteSimpleQuoted(value);
                if (!name.empty() && handler(basePath, name, value))
                    continue;
            }

            resLines.push_back(l);
        }

        return resLines;
    }

    std::vector<std::string> parseLinesExtractValues(const std::vector<std::string> &lines, const std::string &basePath)
    {
        auto handler = [&](const auto &basePath, const auto &name, auto value) -> bool
        {
            auto nameEnum = enum_deserialize( name, almai::PrepromptTextCommands::invalid );

            if (nameEnum==almai::PrepromptTextCommands::invalid)
                return false;

            if (nameEnum==almai::PrepromptTextCommands::setVar)
            {
                return setMacro(value, false /* !bSubst */, true /* allowOverwrite */);
            }

            if (nameEnum==almai::PrepromptTextCommands::importVar)
            {
                return setMacroFromEnv(value, false /* !bSubst */, true /* allowOverwrite */);
            }

            if (nameEnum==almai::PrepromptTextCommands::expandSetVar)
            {
                return setMacro(value, true /* bSubst */, true /* allowOverwrite */);
            }

            if (nameEnum==almai::PrepromptTextCommands::expandImportVar)
            {
                return setMacroFromEnv(value, true /* bSubst */, true /* allowOverwrite */);
            }

            if (nameEnum==almai::PrepromptTextCommands::roles)
            {
                addRoles(value);
                return true;
            }

            if (nameEnum==almai::PrepromptTextCommands::skills)
            {
                addSkills(value);
                return true;
            }



            value = substMacros(value);


            if (nameEnum==almai::PrepromptTextCommands::scanPath)
            {
                value = umba::filename::makeAbsPath(value, basePath);
                // return setMacro(name, value, true /* allowOverwrite */);
                scanInfos.emplace_back(almai::FileSystemScanInfo::parse(value));
                return true;
            }

            return false; // Неизвестная команда
        };

        return parseLinesExtractValues(lines, basePath, handler);
    }

    //template<typename ReadFileErrorHandler>
    bool addScanPathsFromFileLines(std::vector<std::string> &fileLines, const std::string linesFromFile /* , ReadFileErrorHandler errHandler */ )
    {
        auto basePath = umba::filename::getPath(linesFromFile);

        for(auto line: fileLines)
        {
            umba::string::ltrim(line);

            if (line.empty())
                continue;

            if (line.front()=='#' || line.front()==';')
                continue;

            auto fullFileName = umba::filename::makeAbsPath(line, basePath);

            scanInfos.emplace_back(almai::FileSystemScanInfo::parse(fullFileName));

        }

        return true;
        // appConfig.scanInfos.emplace_back(almai::FileSystemScanInfo::parse(argsParser.makeAbsPath(a)));

        // if (!almai::utils::readFile(ffi.fullName, ffi.fileLines))
        // {
        //     LOG_WARN("read-failed") << "failed to read file: '" << ffi.fullName << "'";
        // }
        // else
        // {
        //     readedFiles.push_back(ffi.fullName);
        //     //std::cerr << "  " << ffi.fullName << "\n";
        // }

    }

    std::string makeFilenameTitlePrefix() const
    {
        std::size_t lvl = filenameTitleLevel;
        if (lvl<1) lvl = 1;
        if (lvl>6) lvl = 6;

        return std::string(lvl, '#');
    }

    std::string makeFilenameTitle(const std::string &fileName) const
    {
        return makeFilenameTitlePrefix() + " " + fileName;
    }

    std::string makeLangMarker(const std::string &fileName) const
    {
        auto ext = umba::filename::getExt(fileName);

        if (langMarker==almai::CodeLanguageMarker::langName)
        {
            return findExtentionLang(ext);
        }
        else if (langMarker==almai::CodeLanguageMarker::fileExt)
        {
            return ext;
        }

        return std::string();
    }

    std::string getFilenameTextDecoration() const
    {
        switch(filenameDecorationStyle)
        {
            case almai::FilenameDecorationStyle::quot:             return "\"";
            case almai::FilenameDecorationStyle::apos:             return "\'";
            case almai::FilenameDecorationStyle::bold:             return "**";
            case almai::FilenameDecorationStyle::underscoreBold:   return "__";
            case almai::FilenameDecorationStyle::italic:           return "*" ;
            case almai::FilenameDecorationStyle::underscoreItalic: return "_" ;
            case almai::FilenameDecorationStyle::strikeout:        return "~~";
            case almai::FilenameDecorationStyle::none:             return ""  ;
            case almai::FilenameDecorationStyle::invalid:          return ""  ;
            default: return "";
        }
    }

    std::string makeFilenameTextDecorated(const std::string &fileName_) const
    {
        // Все символы декорирования надо предварить обратным слэшем
        // Все обратные слэши надо заменить на прямой
        std::string fileName;
        for(auto ch: fileName_)
        {
            if ( ch=='\"'
              || ch=='\''
              || ch=='*'
              || ch=='_'
              || ch=='*'
              || ch=='_'
              || ch=='~'
               )
            {
                fileName.append(1, '\\');
                fileName.append(1, ch);
                continue;
            }

            if ( ch=='\\')
            {
                fileName.append(1, '/');
                continue;
            }

            fileName.append(1, ch);
        }

        auto decoration = getFilenameTextDecoration();
        return decoration + fileName + decoration;
    }


    void generateMarkdownListing(std::ostream &oss, const std::string &displayFileName, std::vector<std::string> fileLines) const
    {
        //displayFileName = umba::filename::makeCanonical(displayFileName, '/');

        if (filenameDecorationType==almai::FilenameDecorationType::title)
        {
            oss << makeFilenameTitle(displayFileName) << "\n\n";
        }
        else if (filenameDecorationType==almai::FilenameDecorationType::text)
        {
            oss << makeFilenameTextDecorated(displayFileName) << "\n";
        }

        std::string fence = md::generateFence(fenceStyle, fileLines);

        oss << fence << makeLangMarker(displayFileName);

        if (filenameDecorationType==almai::FilenameDecorationType::attr)
        {
            oss << " {filename=\"" << displayFileName << "\"}";
        }

        oss << "\n";

        fileLines = stripEmptyHeadTailLines(fileLines);

        std::size_t fileLinesSize = fileLines.size();
        if (isSetJoinLinesLimit())
        {
            fileLinesSize = joinLinesLimit;
        }

        if (fileLinesSize>fileLines.size())
        {
            fileLinesSize = fileLines.size();
        }

        for(std::size_t i=0; i!=fileLinesSize; ++i)
        {
            auto line = fileLines[i];
            umba::string::rtrim(line);
            oss << line << "\n";
        }

        oss << fence << "\n\n";

    }

    std::string generateMarkdownListing(const std::string &displayFileName, const std::vector<std::string> fileLines) const
    {
        std::stringstream oss;
        generateMarkdownListing(oss, displayFileName, fileLines);
        return oss.str();
    }

    bool isSetJoinLinesLimit() const
    {
        return joinLinesLimit!=0 && joinLinesLimit!=(std::size_t)-1;
    }


    //--------------------------------------------------------------------------------------------------------------------

    using PluralDatabaseSharedPtrType = std::shared_ptr<almai::PluralDatabase>;
    using PrepromptDatabaseMap        = std::unordered_map<std::string, almai::PrepromptDatabase>;


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
    bool projectCheckNormalize( const almai::PrepromptDatabase &ppDb
                              , ErrorHandler   errorHandler
                              , WarningHandler warningHandler
                              )
    {
        return almaiProject.checkNormalize(ppDb, errorHandler, warningHandler);
    }

    //------------------------------



    //------------------------------
    static
    void addMdPartSeparator(std::vector<std::string> &lines, std::size_t sepLen=10u)
    {
        if (!lines.empty())
        {
            lines.push_back(std::string());
            lines.push_back(std::string());
            lines.push_back(std::string(sepLen, '-'));
            lines.push_back(std::string());
            lines.push_back(std::string());
        }
    }

    //------------------------------



    //------------------------------
    std::vector<std::string>          roles;
    std::vector<std::string>          skills;

    bool addRoles(const std::string &rolesStr);
    bool addSkills(const std::string &rolesStr);


}; // struct AppConfigBase
~~~

**\_src/almai/AppVerConfig.h**
~~~C/C++ Header
std::string appFullName   = "al-martyn1 AI tool";

std::string appVersion    = "1.0";
std::string appCommitHash;  //UNDONE
std::string appBuildDate  = __DATE__;
std::string appBuildTime  = __TIME__;

const char *appHomeUrl    = "https://github.com/al-martyn1/almai";
const char *appistrPath   = "";

const char *appSubPath    = "bin/almai" UMBA_FILESYS_EXE_EXT;
~~~

**\_src/almai/ArgParser.h**
~~~C/C++ Header
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
#include "AppVerConfig.h"

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



#include "cli_opt_parsers/empty_option_error.h"
#include "cli_opt_parsers/basic_options.h"
#include "cli_opt_parsers/overwrite.h"
// #include "cli_opt_parsers/list.h"
#include "cli_opt_parsers/dict.h"
#include "cli_opt_parsers/output.h"
#include "cli_opt_parsers/filename_decoration.h"
#include "cli_opt_parsers/filename_title_level.h"
#include "cli_opt_parsers/sort.h"
#include "cli_opt_parsers/fence_style.h"
#include "cli_opt_parsers/strip_prefix.h"
#include "cli_opt_parsers/lang_marker.h"
#include "cli_opt_parsers/add_plural_pair.h"
#include "cli_opt_parsers/add_translation.h"
#include "cli_opt_parsers/add_project_root_marker.h"
#include "cli_opt_parsers/role_setup.h"


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
            LOG_MSG << "Processing options file: " << optFileName << "\n";
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
        throw std::runtime_error("Duplicated option key - '" + opt + "'");
    }

};
~~~

**\_src/almai/CdtEvalAppConfig.h**
~~~C/C++ Header
#pragma once

//----------------------------------------------------------------------------
#include "umba/umba.h"
//
// #include "AppConfigBase.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "marty_cdt/types.h"
//#include "ListingInfo.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig
{
    using RuntimeEvaluateReturnType        = marty::cdt::RuntimeEvaluateReturnType       ;
    using RuntimeEvaluateAwaitPromise      = marty::cdt::RuntimeEvaluateAwaitPromise     ;
    using RuntimeEvaluateUserGesture       = marty::cdt::RuntimeEvaluateUserGesture      ;
    using RuntimeEvaluateThrowOnSideEffect = marty::cdt::RuntimeEvaluateThrowOnSideEffect;
    using RuntimeEvaluateBreaksControl     = marty::cdt::RuntimeEvaluateBreaksControl    ;
    using RuntimeEvaluateReplMode          = marty::cdt::RuntimeEvaluateReplMode         ;
    using RuntimeEvaluateCspMode           = marty::cdt::RuntimeEvaluateCspMode          ;


    bool                              overwrite         = false;
    bool                              quet              = false;

    unsigned                          evalTimeout       = 10000; // ms
    unsigned                          pageLoadPause     = 5000; // ms
    bool                              waitFullCompletion = true;

    std::string                       inputFile;
    std::string                       output;

    bool                              restoreLastSession = false;
    std::string                       openUrl;

    std::string                       expression;       // либо inputFile, либо expression

    std::string                       evalContextId     = std::string();  // integer as string or empty string
    std::string                       evalObjectGroup   = std::string();  // object group name string


    RuntimeEvaluateReturnType         evalReturnType        = RuntimeEvaluateReturnType::returnByValue     ;
    RuntimeEvaluateAwaitPromise       evalAwaitPromise      = RuntimeEvaluateAwaitPromise::unspecified     ;
    RuntimeEvaluateUserGesture        evalUserGesture       = RuntimeEvaluateUserGesture::unspecified      ;
    RuntimeEvaluateThrowOnSideEffect  evalThrowOnSideEffect = RuntimeEvaluateThrowOnSideEffect::unspecified;
    RuntimeEvaluateBreaksControl      evalBreaksControl     = RuntimeEvaluateBreaksControl::unspecified    ;
    RuntimeEvaluateReplMode           evalReplMode          = RuntimeEvaluateReplMode::unspecified         ;
    RuntimeEvaluateCspMode            evalCspMode           = RuntimeEvaluateCspMode::unspecified          ;

}; // struct AppConfig
~~~

**\_src/almai/CdtEvalAppVerConfig.h**
~~~C/C++ Header
std::string appFullName   = "al-martyn1 AI CDT Eval test tool";

std::string appVersion    = "1.0";
std::string appCommitHash;  //UNDONE
std::string appBuildDate  = __DATE__;
std::string appBuildTime  = __TIME__;

const char *appHomeUrl    = "https://github.com/al-martyn1/almai";
const char *appistrPath   = "";

const char *appSubPath    = "bin/almai-cdt-eval" UMBA_FILESYS_EXE_EXT;
~~~

**\_src/almai/CdtEvalArgParser.h**
~~~C/C++ Header
#pragma once

#include <stack>

//#include "app_config.h"
#include "umba/cmd_line.h"
#include "umba/cli_tool_helpers.h"
#include "umba/shellapi.h"
#include "umba/string_plus.h"
//
#include "utils.h"
//
#include "marty_cpp/marty_cpp.h"
//
#include "CdtEvalAppVerConfig.h"

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
        int      intVal = 0;
        //unsigned uintVal = 0;
        std::size_t szVal = 0;
        bool boolVal = false;

        UMBA_USED(szVal);
        UMBA_USED(boolVal);

#include "umba/warnings/pop.h"


#include "cli_opt_parsers/basic_options.h"
#include "cli_opt_parsers/overwrite.h"
// #include "cli_opt_parsers/list.h"
// #include "cli_opt_parsers/dict.h"
#include "cli_opt_parsers/output.h"
// #include "cli_opt_parsers/clipboard.h"
// #include "cli_opt_parsers/description_filename.h"
#include "cli_opt_parsers/timeout.h"
#include "cli_opt_parsers/open-url.h"
#include "cli_opt_parsers/expression.h"
#include "cli_opt_parsers/context-id.h"
#include "cli_opt_parsers/object-group.h"
#include "cli_opt_parsers/cdt-eval-opts.h"
#include "cli_opt_parsers/cdt-opts.h"


        //--------------------------------------------------------------------------------------------------------------------
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
                              << " [OPTIONS] [input_file]\n"
                              << "\nOptions:\n\n"
                              << helpText;
                              //<< " [OPTIONS] input_file [output_file]\n\nOptions:\n\n"<<helpText;
                }

                if (pCol) // argsNeedHelp
                {
                    argsParser.printHelpPage( std::cout
                                            , "[OPTIONS] [input_file]"
                                            , "" // Usage comment ???
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
            LOG_MSG << "Processing options file: " << optFileName << "\n";
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

    if (!appConfig.inputFile.empty())
    {
        LOG_ERR << "Input file already taken: " << appConfig.inputFile << "\n";
    }

    appConfig.inputFile = argsParser.makeAbsPath(a);

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
~~~

**\_src/almai/Descriptions.h**
~~~C/C++ Header
/*! @file
    @brief Types which hold descriptions
 */

#pragma once

#include "utils.h"
#include "yaml_json.h"

//
#include "umba/umba.h"
#include "umba/filename.h"
#include "umba/string.h"

//
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// almai::mdxml::
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct CommonDescription
{

    std::string       name;
    std::string       description;


protected:

    // static
    // std::string mergeLines(const std::vector<std::string> &lines)
    // {
    //     std::string res;
    //     for(const auto &l : lines)
    //     {
    //         res.append(l);
    //         res.append(1, '\n');
    //     }
    //
    //     return res;
    // }

    static
    std::vector<std::string> splitString(const std::string &str, char ch)
    {
        std::vector<std::string> tmp = umba::filename::splitPathList(str, ch);

        for(auto &v : tmp)
            umba::string::trim(v);

        auto e =
        std::remove_if( tmp.begin(), tmp.end()
                      , [](const auto &s)
                        {
                            return s.empty();
                        }
                      );

        return std::vector<std::string>(tmp.begin(), e);
    }



public:

    static
    marty::json parse(CommonDescription &d, const std::string &text)
    {
        auto j = yaml_json::parseToJson(text);

        if (j.find("name")!=j.end())
        {
            if (!j["name"].is_null())
                d.name = j["name"].get<std::string>();
        }

        if (j.find("description")!=j.end())
        {
            if (!j["description"].is_null())
                d.description = j["description"].get<std::string>();
        }

        return j;
    }

    static
    marty::json parse(CommonDescription &d, const std::vector<std::string> &lines)
    {
        return parse(d, umba::string::merge<std::string>(lines.begin(), lines.end(), '\n'));
    }

    static
    CommonDescription parse(const std::string &text)
    {
        CommonDescription d;
        parse(d, text);
        return d;
    }

    static
    CommonDescription parse(const std::vector<std::string> &lines)
    {
        CommonDescription d;
        parse(d, lines);
        return d;
    }

}; // struct CommonDescription

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct PrepromptDescription : public CommonDescription
{
    std::vector< std::vector<std::string> >     requiresList;


    static
    marty::json parse(PrepromptDescription &d, const std::string &text)
    {
        marty::json j = CommonDescription::parse(d, text);

        if (j.find("requires")!=j.end())
        {
            auto r = j["requires"];

            if (r.is_string())
            {
                // parse string here

                std::string requiresListStr = r.get<std::string>();
                std::vector<std::string> requiresList = CommonDescription::splitString(requiresListStr, ',');
                for(const auto &altersListStr : requiresList)
                {
                    std::vector<std::string> altersList = CommonDescription::splitString(altersListStr, '|');
                    d.requiresList.emplace_back(altersList);
                }

            }
            else if (r.is_array())
            {
                // parse array here
                for (nlohmann::json::iterator it = r.begin(); it!=r.end(); ++it)
                {
                    std::string altersListStr = it->get<std::string>();
                    std::vector<std::string> altersList = splitString(altersListStr, '|');
                    d.requiresList.emplace_back(altersList);
                }
            }
            else if (r.is_null())
            {
                // simple ignore null fields
            }
            else
            {
                throw std::runtime_error("'requires' is in '" + marty::json_utils::nodeTypeName(r) + "' format. Only string/array formats allowed");
            }

        }

        return j;
    }

    static
    marty::json parse(PrepromptDescription &d, const std::vector<std::string> &lines)
    {
        return parse(d, umba::string::merge<std::string>(lines.begin(), lines.end(), '\n'));
    }

    static
    PrepromptDescription parse(const std::string &text)
    {
        PrepromptDescription d;
        parse(d, text);
        return d;
    }

    static
    PrepromptDescription parse(const std::vector<std::string> &lines)
    {
        PrepromptDescription d;
        parse(d, lines);
        return d;
    }



}; // struct PrepromptDescription

//----------------------------------------------------------------------------

// // other stuff
// j.size();     // 4 entries
// j.empty();    // false
// j.type();     // json::value_t::array
// j.clear();    // the array is empty again
//
// // convenience type checkers
// j.is_null();
// j.is_boolean();
// j.is_number();
// j.is_object();
// j.is_array();
// j.is_string();

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai
~~~

**\_src/almai/enums.h**
~~~C/C++ Header
/*! \file
    \brief Autogenerated enumerations (Umba Enum Gen)
 */

#pragma once


#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flag_ops.h"
#include "marty_cpp/marty_flags.h"

#include <exception>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>



namespace almai{

//#!AttachFormat
enum class AttachFormat : std::uint32_t
{
    invalid    = (std::uint32_t)(-1) /*!<  */,
    unknown    = (std::uint32_t)(-1) /*!<  */,
    none       = 0x0000 /*!<  */,
    no         = 0x0000 /*!<  */,
    md         = 0x0001 /*!<  */,
    markdown   = 0x0001 /*!<  */,
    zip        = 0x0002 /*!<  */,
    _7z        = 0x0003 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(AttachFormat)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( AttachFormat, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( AttachFormat::invalid   , "Invalid" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( AttachFormat::_7z       , "7Z"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( AttachFormat::none      , "None"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( AttachFormat::zip       , "Zip"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( AttachFormat::md        , "Md"      );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( AttachFormat, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( AttachFormat, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( AttachFormat::invalid   , "invalid"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( AttachFormat::invalid   , "unknown"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( AttachFormat::_7z       , "7z"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( AttachFormat::none      , "none"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( AttachFormat::none      , "no"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( AttachFormat::zip       , "zip"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( AttachFormat::md        , "markdown" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( AttachFormat::md        , "md"       );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( AttachFormat, std::map, 1 )


//#!CodeLanguageMarker
enum class CodeLanguageMarker : std::uint32_t
{
    invalid    = (std::uint32_t)(-1) /*!<  */,
    unknown    = (std::uint32_t)(-1) /*!<  */,
    none       = 0x0000 /*!<  */,
    name       = 0x0001 /*!<  */,
    langName   = 0x0001 /*!<  */,
    ext        = 0x0002 /*!<  */,
    fileExt    = 0x0002 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(CodeLanguageMarker)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( CodeLanguageMarker, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( CodeLanguageMarker::invalid   , "Invalid" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( CodeLanguageMarker::none      , "None"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( CodeLanguageMarker::name      , "Name"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( CodeLanguageMarker::ext       , "Ext"     );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( CodeLanguageMarker, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( CodeLanguageMarker, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::invalid   , "invalid"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::invalid   , "unknown"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::none      , "none"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::name      , "langname"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::name      , "name"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::name      , "lang-name" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::name      , "lang_name" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::ext       , "ext"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::ext       , "file-ext"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::ext       , "file_ext"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( CodeLanguageMarker::ext       , "fileext"   );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( CodeLanguageMarker, std::map, 1 )


//#!ComparisonType
enum class ComparisonType : std::uint32_t
{
    invalid    = (std::uint32_t)(-1) /*!<  */,
    unknown    = (std::uint32_t)(-1) /*!<  */,
    none       = 0x0000 /*!<  */,
    name       = 0x0001 /*!< Compare by name part, ignore extention */,
    nameExt    = 0x0002 /*!< Compare by name part first, than ext part */,
    fullName   = 0x0003 /*!< Compare by name and extention as single string */,
    full       = 0x0003 /*!< Compare by name and extention as single string */,
    ext        = 0x0004 /*!< Compare extention part first, than name part */,
    extName    = 0x0004 /*!< Compare extention part first, than name part */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(ComparisonType)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( ComparisonType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ComparisonType::invalid    , "Invalid"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ComparisonType::none       , "None"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ComparisonType::name       , "Name"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ComparisonType::nameExt    , "NameExt"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ComparisonType::fullName   , "FullName" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ComparisonType::ext        , "Ext"      );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( ComparisonType, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( ComparisonType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::invalid    , "invalid"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::invalid    , "unknown"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::none       , "none"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::name       , "name"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::nameExt    , "name-ext"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::nameExt    , "name_ext"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::nameExt    , "nameext"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::fullName   , "full-name" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::fullName   , "full_name" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::fullName   , "fullname"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::fullName   , "full"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::ext        , "ext"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::ext        , "ext-name"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::ext        , "ext_name"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ComparisonType::ext        , "extname"   );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( ComparisonType, std::map, 1 )


//#!FenceStyle
enum class FenceStyle : std::uint32_t
{
    invalid     = (std::uint32_t)(-1) /*!<  */,
    unknown     = (std::uint32_t)(-1) /*!<  */,
    auto_       = 0x0000 /*!<  */,
    backticks   = 0x0001 /*!<  */,
    backtick    = 0x0001 /*!<  */,
    tildes      = 0x0002 /*!<  */,
    tilde       = 0x0002 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(FenceStyle)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( FenceStyle, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FenceStyle::invalid     , "Invalid"   );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FenceStyle::auto_       , "Auto"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FenceStyle::backticks   , "Backticks" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FenceStyle::tildes      , "Tildes"    );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( FenceStyle, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( FenceStyle, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FenceStyle::invalid     , "invalid"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FenceStyle::invalid     , "unknown"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FenceStyle::auto_       , "auto"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FenceStyle::backticks   , "backticks" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FenceStyle::backticks   , "backtick"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FenceStyle::tildes      , "tildes"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FenceStyle::tildes      , "tilde"     );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( FenceStyle, std::map, 1 )


//#!FilenameDecorationStyle
enum class FilenameDecorationStyle : std::uint32_t
{
    invalid            = (std::uint32_t)(-1) /*!<  */,
    unknown            = (std::uint32_t)(-1) /*!<  */,
    none               = 0x0000 /*!<  */,
    quot               = 0x0001 /*!< use quotation mark */,
    apos               = 0x0002 /*!< use apos mark */,
    bold               = 0x0003 /*!< bold using ** */,
    underscoreBold     = 0x0004 /*!< bold using __ */,
    ubold              = 0x0004 /*!< bold using __ */,
    strikeout          = 0x0005 /*!< ~~ */,
    italic             = 0x0006 /*!< italic using * */,
    underscoreItalic   = 0x0007 /*!< italic using _ */,
    uitalic            = 0x0007 /*!< italic using _ */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(FilenameDecorationStyle)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( FilenameDecorationStyle, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::quot               , "Quot"             );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::invalid            , "Invalid"          );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::none               , "None"             );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::underscoreBold     , "UnderscoreBold"   );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::bold               , "Bold"             );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::apos               , "Apos"             );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::strikeout          , "Strikeout"        );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::italic             , "Italic"           );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationStyle::underscoreItalic   , "UnderscoreItalic" );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( FilenameDecorationStyle, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( FilenameDecorationStyle, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::quot               , "quot"              );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::invalid            , "invalid"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::invalid            , "unknown"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::none               , "none"              );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::underscoreBold     , "ubold"             );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::underscoreBold     , "underscore-bold"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::underscoreBold     , "underscore_bold"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::underscoreBold     , "underscorebold"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::bold               , "bold"              );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::apos               , "apos"              );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::strikeout          , "strikeout"         );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::italic             , "italic"            );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::underscoreItalic   , "uitalic"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::underscoreItalic   , "underscore-italic" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::underscoreItalic   , "underscore_italic" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationStyle::underscoreItalic   , "underscoreitalic"  );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( FilenameDecorationStyle, std::map, 1 )


//#!FilenameDecorationType
enum class FilenameDecorationType : std::uint32_t
{
    invalid   = (std::uint32_t)(-1) /*!<  */,
    unknown   = (std::uint32_t)(-1) /*!<  */,
    none      = 0x0000 /*!<  */,
    text      = 0x0001 /*!<  */,
    title     = 0x0002 /*!<  */,
    attr      = 0x0003 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(FilenameDecorationType)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( FilenameDecorationType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationType::invalid   , "Invalid" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationType::none      , "None"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationType::text      , "Text"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationType::title     , "Title"   );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FilenameDecorationType::attr      , "Attr"    );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( FilenameDecorationType, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( FilenameDecorationType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationType::invalid   , "invalid" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationType::invalid   , "unknown" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationType::none      , "none"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationType::text      , "text"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationType::title     , "title"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FilenameDecorationType::attr      , "attr"    );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( FilenameDecorationType, std::map, 1 )


//#!PrepromptPathType
enum class PrepromptPathType : std::uint32_t
{
    invalid          = (std::uint32_t)(-1) /*!<  */,
    unknown          = (std::uint32_t)(-1) /*!<  */,
    installDirs      = 0x0000 /*!< Наивысший проритет */,
    begin            = 0x0000 /*!< Наивысший проритет */,
    builtinOptions   = 0x0001 /*!<  */,
    envPaths         = 0x0002 /*!<  */,
    projectDirs      = 0x0003 /*!<  */,
    cliOptions       = 0x0004 /*!<  */,
    end              = 0x0005 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(PrepromptPathType)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( PrepromptPathType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptPathType::envPaths         , "EnvPaths"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptPathType::invalid          , "Invalid"        );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptPathType::installDirs      , "InstallDirs"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptPathType::builtinOptions   , "BuiltinOptions" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptPathType::end              , "End"            );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptPathType::projectDirs      , "ProjectDirs"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptPathType::cliOptions       , "CliOptions"     );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( PrepromptPathType, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( PrepromptPathType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::envPaths         , "env-paths"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::envPaths         , "env_paths"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::envPaths         , "envpaths"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::invalid          , "invalid"         );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::invalid          , "unknown"         );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::installDirs      , "install-dirs"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::installDirs      , "install_dirs"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::installDirs      , "installdirs"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::installDirs      , "begin"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::builtinOptions   , "builtin-options" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::builtinOptions   , "builtin_options" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::builtinOptions   , "builtinoptions"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::end              , "end"             );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::projectDirs      , "project-dirs"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::projectDirs      , "project_dirs"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::projectDirs      , "projectdirs"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::cliOptions       , "cli-options"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::cliOptions       , "cli_options"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptPathType::cliOptions       , "clioptions"      );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( PrepromptPathType, std::map, 1 )


//#!PrepromptTextCommands
enum class PrepromptTextCommands : std::uint32_t
{
    invalid               = (std::uint32_t)(-1) /*!<  */,
    unknown               = (std::uint32_t)(-1) /*!<  */,
    scanPath              = 0x0000 /*!<  */,
    setVar                = 0x0001 /*!<  */,
    importVar             = 0x0002 /*!<  */,
    setVarFromEnv         = 0x0002 /*!<  */,
    expandSetVar          = 0x0003 /*!<  */,
    expandImportVar       = 0x0004 /*!<  */,
    expandSetVarFromEnv   = 0x0004 /*!<  */,
    role                  = 0x0005 /*!<  */,
    roles                 = 0x0005 /*!<  */,
    skills                = 0x0006 /*!<  */,
    skill                 = 0x0006 /*!<  */,
    end                   = 0x0007 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(PrepromptTextCommands)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( PrepromptTextCommands, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::setVar            , "SetVar"          );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::invalid           , "Invalid"         );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::skills            , "Skills"          );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::scanPath          , "ScanPath"        );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::importVar         , "ImportVar"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::expandSetVar      , "ExpandSetVar"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::expandImportVar   , "ExpandImportVar" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::role              , "Role"            );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrepromptTextCommands::end               , "End"             );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( PrepromptTextCommands, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( PrepromptTextCommands, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::setVar            , "set-var"                 );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::setVar            , "set_var"                 );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::setVar            , "setvar"                  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::invalid           , "invalid"                 );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::invalid           , "unknown"                 );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::skills            , "skills"                  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::skills            , "skill"                   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::scanPath          , "scan-path"               );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::scanPath          , "scan_path"               );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::scanPath          , "scanpath"                );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::importVar         , "import-var"              );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::importVar         , "set_var_from_env"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::importVar         , "import_var"              );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::importVar         , "importvar"               );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::importVar         , "set-var-from-env"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::importVar         , "setvarfromenv"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandSetVar      , "expand-set-var"          );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandSetVar      , "expand_set_var"          );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandSetVar      , "expandsetvar"            );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandImportVar   , "expand-set-var-from-env" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandImportVar   , "expand_set_var_from_env" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandImportVar   , "expand-import-var"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandImportVar   , "expand_import_var"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandImportVar   , "expandimportvar"         );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::expandImportVar   , "expandsetvarfromenv"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::role              , "role"                    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::role              , "roles"                   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrepromptTextCommands::end               , "end"                     );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( PrepromptTextCommands, std::map, 1 )


//#!ResultCode
enum class ResultCode : std::uint32_t
{
    invalid            = (std::uint32_t)(-1) /*!<  */,
    unknown            = (std::uint32_t)(-1) /*!<  */,
    ok                 = 0x0000 /*!< Okay */,
    foundAlternative   = 0x0001 /*!< Alternative was found */,
    error              = 0x8000 /*!<  */,
    genericError       = 0x8000 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(ResultCode)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( ResultCode, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ResultCode::invalid            , "Invalid"          );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ResultCode::ok                 , "Ok"               );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ResultCode::foundAlternative   , "FoundAlternative" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ResultCode::error              , "Error"            );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( ResultCode, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( ResultCode, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::invalid            , "invalid"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::invalid            , "unknown"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::ok                 , "ok"                );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::foundAlternative   , "found-alternative" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::foundAlternative   , "found_alternative" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::foundAlternative   , "foundalternative"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::error              , "generic-error"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::error              , "error"             );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::error              , "generic_error"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ResultCode::error              , "genericerror"      );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( ResultCode, std::map, 1 )


//#!SortOrder
enum class SortOrder : std::uint32_t
{
    invalid      = (std::uint32_t)(-1) /*!<  */,
    unknown      = (std::uint32_t)(-1) /*!<  */,
    asc          = 0x0000 /*!<  */,
    ascending    = 0x0000 /*!<  */,
    desc         = 0x0001 /*!<  */,
    descending   = 0x0001 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(SortOrder)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( SortOrder, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( SortOrder::desc      , "Desc"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( SortOrder::invalid   , "Invalid" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( SortOrder::asc       , "Asc"     );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( SortOrder, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( SortOrder, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( SortOrder::desc      , "desc"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( SortOrder::desc      , "descending" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( SortOrder::invalid   , "invalid"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( SortOrder::invalid   , "unknown"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( SortOrder::asc       , "asc"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( SortOrder::asc       , "ascending"  );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( SortOrder, std::map, 1 )


//#!FileNameSortOrder
enum class FileNameSortOrder : std::uint32_t
{
    invalid                    = (std::uint32_t)(-1) /*!<  */,
    unknown                    = (std::uint32_t)(-1) /*!<  */,
    asc                        = 0x0000 /*!<  */,
    ascending                  = 0x0000 /*!<  */,
    desc                       = 0x0001 /*!<  */,
    descending                 = 0x0001 /*!<  */,
    type                       = 0x0002 /*!<  */,
    byType                     = 0x0002 /*!<  */,
    sortByType                 = 0x0002 /*!<  */,
    dirs                       = 0x0004 /*!<  */,
    dirsFirst                  = 0x0004 /*!<  */,
    ascDirs                    = 0x0004 /*!<  */,
    ascendingDirs              = 0x0004 /*!<  */,
    ascDirsFirst               = 0x0004 /*!<  */,
    ascendingDirsFirst         = 0x0004 /*!<  */,
    ascDirsType                = 0x0006 /*!<  */,
    ascDirsByType              = 0x0006 /*!<  */,
    ascendingDirsType          = 0x0006 /*!<  */,
    ascendingDirsByType        = 0x0006 /*!<  */,
    ascDirsFirstType           = 0x0006 /*!<  */,
    ascDirsFirstByType         = 0x0006 /*!<  */,
    ascendingDirsFirstType     = 0x0006 /*!<  */,
    ascendingDirsFirstByType   = 0x0006 /*!<  */,
    no                         = 0x0080 /*!<  */,
    noSort                     = 0x0080 /*!<  */

}; // enum
//#!

MARTY_CPP_MAKE_ENUM_FLAGS(FileNameSortOrder)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( FileNameSortOrder, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileNameSortOrder::desc          , "Desc"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileNameSortOrder::invalid       , "Invalid"     );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileNameSortOrder::no            , "No"          );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileNameSortOrder::asc           , "Asc"         );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileNameSortOrder::dirs          , "Dirs"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileNameSortOrder::type          , "Type"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "AscDirsType" );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( FileNameSortOrder, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( FileNameSortOrder, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::desc          , "desc"                         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::desc          , "descending"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::invalid       , "invalid"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::invalid       , "unknown"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::no            , "nosort"                       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::no            , "no"                           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::no            , "no-sort"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::no            , "no_sort"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::asc           , "asc"                          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::asc           , "ascending"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "dirs"                         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "asc-dirs"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "asc-dirs-first"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "dirs-first"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "ascdirs"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "dirs_first"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "dirsfirst"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "asc_dirs"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "ascending-dirs"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "ascending_dirs"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "ascendingdirs"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "asc_dirs_first"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "ascdirsfirst"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "ascending-dirs-first"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "ascending_dirs_first"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::dirs          , "ascendingdirsfirst"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::type          , "type"                         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::type          , "by-type"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::type          , "by_type"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::type          , "sort-by-type"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::type          , "sort_by_type"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::type          , "bytype"                       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::type          , "sortbytype"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascendingdirsbytype"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "asc-dirs-by-type"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "asc-dirs-type"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "asc_dirs_type"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "asc_dirs_by_type"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascdirstype"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascdirsbytype"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascending-dirs-type"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascending_dirs_by_type"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascending_dirs_type"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascendingdirstype"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascending-dirs-by-type"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "asc-dirs-first-type"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "asc_dirs_first_by_type"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "asc_dirs_first_type"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascdirsfirsttype"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "asc-dirs-first-by-type"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascdirsfirstbytype"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascending-dirs-first-type"    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascending_dirs_first_type"    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascendingdirsfirsttype"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascending-dirs-first-by-type" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascending_dirs_first_by_type" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileNameSortOrder::ascDirsType   , "ascendingdirsfirstbytype"     );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( FileNameSortOrder, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(FileNameSortOrder, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(FileNameSortOrder, std::set)

} // namespace almai
~~~

**\_src/almai/FileSystemScanInfo.h**
~~~C/C++ Header
/*! \file
    \brief Информация об указании сканирования файловой системы
 */
#pragma once

//----------------------------------------------------------------------------
#include "utils.h"
#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/filesys_scanners.h"
#include "umba/string.h"
#include "umba/nul_ostream.h"

//
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct FileSystemScanInfo
{
    std::string                  path;
    std::vector<std::string>     includeMaskList;
    std::vector<std::string>     excludeMaskList;

    bool                         recursive = false;


protected:

    // struct LogDummy
    // {
    //     template<typename T>
    //     LogDummy& operator<<(const T &t)
    //     {
    //         UMBA_USED(t);
    //         return *this;
    //     }
    // };

    static
    bool isMask(const std::string &name)
    {
        static std::string maskChars = "*?";
        return name.find_first_of(maskChars)!=name.npos;
    }

    static
    bool isRecurseMarker(const std::string &name)
    {
        return name=="**";
    }


public:

    static
    FileSystemScanInfo parse( const std::string &fullName
                            , const std::string &rootPath = umba::filesys:: /* internal:: */ getCurrentDirectory /* <std::string> */ ()
                            , const std::string &dotPath  = umba::filesys:: /* internal:: */ getCurrentDirectory /* <std::string> */ ()
                            )
    {
        using namespace umba::filename;
        using namespace umba::filesys;
        using namespace umba::string;

        FileSystemScanInfo scanInfo;

        scanInfo.path = getPath(fullName);
        auto name = getFileName(fullName);

        // Проверяем последнее имя каталога в пути
        // Если оно `**` (путь поиска вида "path/to/**/file*.ext")
        // то это рекурсивный поиск
        // Если путь вида "path/to/file*.ext", то в каталоге "path/to" будут
        // искаться файлы по маске "file*.ext"
        if (isRecurseMarker(getFileName(scanInfo.path)))
        {
            scanInfo.recursive = true;
            scanInfo.path = getPath(scanInfo.path);
        }

        // Если масок нет, то у нас точное имя файла, и оно может содержать и запятые, и минус в начале
        // Его надо обработать без разбора на запчасти

        if (!isMask(name))
        {
            scanInfo.includeMaskList.emplace_back(name);
        }
        else
        {
            std::vector<std::string> maskList = splitPathList(name,',');

            for(auto &mask : maskList)
            {
                trim(mask);

                if (mask.empty())
                    continue;

                std::vector<std::string> *pList = &scanInfo.includeMaskList;
                if (mask[0]=='-')
                {
                    pList = &scanInfo.excludeMaskList;
                    mask.erase(0,1);
                    trim(mask);
                }

                if (mask.empty())
                    continue;

                pList->emplace_back(mask);
            }

            // if (scanInfo.includeMaskList.empty())
            //     scanInfo.includeMaskList.emplace_back("*.*");
        }

        if (!scanInfo.path.empty() && scanInfo.path[0]=='.') // '..' or '.'
            scanInfo.path = makeAbsPath(scanInfo.path, dotPath);
        else
            scanInfo.path = makeAbsPath(scanInfo.path, rootPath);

        scanInfo.path = makeCanonical(scanInfo.path);

        return scanInfo;
    }

    void scanForFiles(std::vector<std::string> &foundFiles) const
    {
        using namespace umba::filesys::scanners;

        //LogDummy log;
        umba::NulOstream log;

        std::vector<std::string>    excludedFiles;
        std::set<std::string>       foundExtentions;
        std::vector<std::string>    excludeFoldersExact;


        umba::filesys::scanners::scanFolders( std::vector<std::string>(1, path) // rootScanPaths
                                            , includeMaskList
                                            , excludeMaskList
                                            , log
                                            , foundFiles
                                            , excludedFiles
                                            , foundExtentions
                                            , (std::vector<std::string>*)0 // pFoundFilesRootFolders
                                            , excludeFoldersExact
                                            , recursive
                                            , false // logFoundHeader
                                            , false // addFolders
                                            , true  // compareOnlyFilenames
                                            );
    }

    std::string toString() const
    {
        auto resStr = std::string(1, recursive ? 'R' : 'T'); // Recursive | Exact
        resStr.append(1, ' ');

        resStr.append(path);

        resStr.append(4, ' ');

        for(auto &&m : includeMaskList)
        {
            if (resStr.back()!=' ')
                resStr.append(1, ' ');
            resStr.append(m);
        }

        if (!excludeMaskList.empty())
        {
            resStr.append(4, ' ');
            resStr.append("X ");

            for(auto &&m : excludeMaskList)
            {
                if (resStr.back()!=' ')
                    resStr.append(1, ' ');
                resStr.append(m);
            }
        }

        return resStr;
    }


}; // struct FileSystemScanInfo

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai
~~~

**\_src/almai/FoundFileInfo.h**
~~~C/C++ Header
/*! \file
    \brief Информация о найденных файлах
 */
#pragma once

//----------------------------------------------------------------------------
#include "enums.h"
#include "utils.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/filesys_scanners.h"
#include "umba/string.h"
#include "umba/rule_of_five.h"

//
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct FoundFileInfo
{
    std::string                  fullName;     // Оигинальное имя файла, используется для чтения файла
    std::string                  displayName;  // "Отображаемое" имя файла, будет вставляться в результат и используется для сравнения

    std::vector<std::string>     fileLines;

    // Для сортировки, надо предварительно сделать splitDisplayFileName
    mutable std::string          path;
    mutable std::string          pathFile;
    mutable std::string          fileName;
    mutable std::string          name;
    mutable std::string          ext;


    UMBA_RULE_OF_FIVE_COPY_MOVE(FoundFileInfo, default, default, default, default);

    FoundFileInfo(const std::string &name)
    {
        setFullName(name);
    }

    void setFullName(const std::string &n)
    {
        fullName    = n;
        displayName = n;
        splitDisplayFileName();
    }

    bool isPrefixStripped() const
    {
        return fullName!=displayName;
    }

    bool stripPrefix(std::string prefix)
    {
        using namespace umba::filename;
        using namespace umba::filesys;
        using namespace umba::string;

        if (prefix.empty())
            return false;

        if (isPrefixStripped())
            return false;

        appendPathSepInline(prefix);

        std::string restName;
        if (!isSubPathName(prefix, displayName, &restName))
            return false;

        displayName = restName;

        splitDisplayFileName();

        return true;
    }

    bool stripPrefix(const std::vector<std::string> &stripPrefixes)
    {
        for(const auto &p : stripPrefixes)
        {
            if (stripPrefix(p))
                return true;
        }

        return false;
    }

    void splitDisplayFileName() const
    {
        using namespace umba::filename;

        auto cmpName = makeCanonicalForCompare(displayName);

        path     = getPath    (cmpName);
        pathFile = getPathFile(cmpName);
        fileName = getFileName(cmpName);
        name     = getName    (cmpName);
        ext      = getExt     (cmpName);
    }

    void checkSplitDisplayFileName() const
    {
        if (name.empty() && ext.empty())
            splitDisplayFileName();
    }

    int compare(const FoundFileInfo &other, ComparisonType cmpType) const
    {
        checkSplitDisplayFileName();
        other.checkSplitDisplayFileName();


        // Путь всегда учитываем - файлы группируются по путям
        int cmpRes = path.compare(other.path);
        if (cmpRes!=0)
            return cmpRes;


        switch(cmpType)
        {
            case ComparisonType::nameExt:  // Compare by name part first, than ext part
                cmpRes = name.compare(other.name);
                if (cmpRes!=0)
                    return cmpRes;

                return ext.compare(other.ext);

            case ComparisonType::ext:      // Compare extention part first, than name part
                cmpRes = ext.compare(other.ext);
                if (cmpRes!=0)
                    return cmpRes;

                return name.compare(other.name);


            case ComparisonType::fullName: // Compare by name and extention as single string
                return fileName.compare(other.fileName);


            case ComparisonType::name:     // Compare by name part, ignore extention
                return name.compare(other.name);


            case ComparisonType::none   : return 0;
            case ComparisonType::invalid: return 0;
            default                     : return 0;

        } // switch(cmpType)

    }


    //------------------------------
    struct Less
    {
        ComparisonType cmpType;

        Less(ComparisonType cmpType_) : cmpType(cmpType_) {}
        UMBA_RULE_OF_FIVE_COPY_MOVE(Less, default, default, default, default);

        bool operator()(const FoundFileInfo &ffi1, const FoundFileInfo ffi2) const
        {
            return ffi1.compare(ffi2, cmpType)<0;
        }

    }; // struct Less


    //------------------------------
    struct Greater
    {
        ComparisonType cmpType;

        Greater(ComparisonType cmpType_) : cmpType(cmpType_) {}
        UMBA_RULE_OF_FIVE_COPY_MOVE(Greater, default, default, default, default);

        bool operator()(const FoundFileInfo &ffi1, const FoundFileInfo ffi2) const
        {
            return ffi1.compare(ffi2, cmpType)>0;
        }

    }; // struct Greater


    //------------------------------
    struct Compare
    {
        ComparisonType cmpType;
        SortOrder      sortOrder;

        Compare(ComparisonType cmpType_, SortOrder sortOrder_) : cmpType(cmpType_), sortOrder(sortOrder_) {}
        UMBA_RULE_OF_FIVE_COPY_MOVE(Compare, default, default, default, default);

        bool operator()(const FoundFileInfo &ffi1, const FoundFileInfo ffi2) const
        {
            return sortOrder==SortOrder::asc
                 ? ffi1.compare(ffi2, cmpType)<0
                 : ffi1.compare(ffi2, cmpType)>0
                 ;
        }

    }; // struct Compare


}; // struct FoundFileInfo

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
void sortFoundFileInfos(std::vector<FoundFileInfo> &ffiVec, ComparisonType cmpType=ComparisonType::nameExt, SortOrder sortOrder=SortOrder::asc)
{
    if (cmpType==ComparisonType::none || cmpType==ComparisonType::invalid)
        return;

    std::stable_sort(ffiVec.begin(), ffiVec.end(), FoundFileInfo::Compare(cmpType, sortOrder));
}

//----------------------------------------------------------------------------
inline
std::size_t findCommonPrefixLen(const std::string &s1, const std::string &s2)
{
    std::size_t commonSize = s1.size() < s2.size() ? s1.size() : s2.size();
    std::size_t i = 0;

    for(; i!=commonSize; ++i)
    {
        if (s1[i]!=s2[i])
            return i;
    }

    return commonSize;
}

//----------------------------------------------------------------------------
template<typename IteratorType>
std::string findMostCommonPathPrefix(IteratorType b, IteratorType e)
{
    if (b==e)
        return std::string();

    b->checkSplitDisplayFileName();

    std::string prefix = b->path;

    for(; b!=e; ++b)
    {
        b->checkSplitDisplayFileName();
        std::size_t commonPrefixLen = findCommonPrefixLen(prefix, b->path);
        prefix.erase(commonPrefixLen);
    }

    return prefix;
}

//----------------------------------------------------------------------------
inline
std::string checkCorrectMostCommonPathPrefixIsPath(const std::string &p)
{
    if (p.empty())
        return p;

    if (p.back()=='\\' || p.back()=='/')
        return p;

    return umba::filename::getPath(p);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai
~~~

**\_src/almai/Localization.h**
~~~C/C++ Header
/*! \file
    \brief Локализация (заголовком и строк)
 */
#pragma once

/*
    --translation=ЯЗЫК:КЛЮЧ:ТЕКСТ
    --translation=ru:roles-title:Твоя роль
*/

//----------------------------------------------------------------------------
#include "utils.h"

//
#include "umba/string.h"

//
#include <string>
#include <unordered_map>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct LocalizationStrings
{

    std::unordered_map<std::string, std::string>   strings;


    bool addLocalization(std::string key, std::string text)
    {
        umba::string::trim(key);
        umba::string::trim(text);

        if (key.empty() || text.empty())
            return false;

        umba::string::tolower(key);

        strings[key] = text;

        return true;
    }

    bool addLocalization(const std::string &keyTextPair)
    {
        std::string f;
        std::string s;

        if (!umba::string::split_to_pair(keyTextPair, f, s, ':'))
            return false;

        return addLocalization(f, s);
    }


    std::string getLocalizedText(std::string key) const
    {
        umba::string::trim(key);
        umba::string::tolower(key);

        std::unordered_map<std::string, std::string>::const_iterator it = strings.find(key);
        if (it==strings.end())
            return std::string();

        return it->second;
    }


}; // struct LocalizationStrings

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct Localization
{

    std::unordered_map<std::string, LocalizationStrings> langStrings;


    bool addLocalization(std::string lang, std::string keyTextPair)
    {
        umba::string::trim(lang);
        umba::string::trim(keyTextPair);

        if ( /* lang.empty() || */ keyTextPair.empty())
            return false;

        umba::string::tolower(lang);

        return langStrings[lang].addLocalization(keyTextPair);
    }

    bool addLocalization(const std::string &langKeyTextTriplet)
    {
        std::string f;
        std::string s;

        if (!umba::string::split_to_pair(langKeyTextTriplet, f, s, ':'))
            return false;

        return addLocalization(f, s);
    }

    std::string getLocalizedTextImpl(std::string lang, const std::string &key) const
    {
        umba::string::trim(lang);
        umba::string::tolower(lang);

        std::unordered_map<std::string, LocalizationStrings>::const_iterator it = langStrings.find(lang);
        if (it==langStrings.end())
            return std::string();

        return it->second.getLocalizedText(key);
    }

    std::string getLocalizedText(std::string lang, const std::string &key) const
    {
        std::string
        resText = getLocalizedTextImpl(lang, key);
        if (!resText.empty())
            return resText;

        resText = getLocalizedTextImpl("en", key);
        if (!resText.empty())
            return resText;

        resText = getLocalizedTextImpl("", key);
        if (!resText.empty())
            return resText;

        resText = key;

        if (!resText.empty() && (resText[0]>='a' && resText[0]<='z' ))
            resText[0] = resText[0] - 'a' + 'A';

        return resText;
    }


}; // struct Localization


//----------------------------------------------------------------------------


} // namespace almai
~~~

**\_src/almai/log.h**
~~~C/C++ Header
#pragma once

#include "umba/log.h"


// source parsing errors
// requires std::stringr curFile, unsigned lineNo in log scope
#define LOG_ERR_INPUT                UMBA_LOG_ERR_INPUT
#define LOG_WARN_INPUT(warnType)     UMBA_LOG_WARN_INPUT(warnType)
#define LOG_INFO_INPUT(infoType)     UMBA_LOG_INFO_INPUT(infoType)

// options and other errors
#define LOG_ERR                      UMBA_LOG_ERR
#define LOG_WARN(warnType)           UMBA_LOG_WARN(warnType)
#define LOG_INFO(infoType)           UMBA_LOG_INFO(infoType)

#define LOG_MSG                      UMBA_LOG_MSG
~~~

**\_src/almai/md\_utils.h**
~~~C/C++ Header
/*! \file
    \brief Утилитарные функции проекта
 */
#pragma once

#include "enums.h"
#include "almai_enums.h"
#include "log.h"
//
#include "umba/umba.h"
#include "umba/parse_utils.h"
#include "umba/filename.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
#include "umba/filename.h"
#include "umba/filesys.h"

#if defined(WIN32) || defined(_WIN32)
    #include "umba/clipboard_win32.h"
#endif

//
#include "marty_cpp/src_normalization.h"
//
#include "marty_yaml_toml_json/marty_yaml_toml_json.h"

//
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
extern unsigned lineNo;
extern std::string curFile;


//----------------------------------------------------------------------------
namespace md {

//----------------------------------------------------------------------------
using almai::MdLineType;
using almai::FenceStyle;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
const char* getInvalidPathNameChars()
{
    static char chars[] =
    { '?', '*', '|', '<', '>', '\"'
    , 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10
    , 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
    };

    return &chars[0];
}

//----------------------------------------------------------------------------
inline
const char* getInvalidFileNameChars()
{
    static char chars[] =
    { '?', '*', '|', '<', '>', '\"', '/', '\\', ':'
    , 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10
    , 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
    };

    return &chars[0];
}

//----------------------------------------------------------------------------
inline
const char* getPossiblePathNameChars()
{
    static char chars[] =
    { '.', '/', '\\'
    , 0x00
    };

    return &chars[0];
}

//----------------------------------------------------------------------------
inline
bool isValidPathName(const std::string &pathName)
{
    auto pos = pathName.find_first_of(getInvalidPathNameChars());
    return pos==pathName.npos; // Если недопустимых символов не найдено, то это хорошо
}

//----------------------------------------------------------------------------
inline
bool isValidPathPart(const std::string &pathName)
{
    auto pos = pathName.find_first_of(getInvalidFileNameChars());
    return pos==pathName.npos; // Если недопустимых символов не найдено, то это хорошо
}

//----------------------------------------------------------------------------
inline
bool isPossiblePath(const std::string &pathName)
{
    auto pos = pathName.find_first_of(getPossiblePathNameChars());
    return pos!=pathName.npos; // Если подходящих символов не найдено, то это хорошо
}

//----------------------------------------------------------------------------
inline
std::string normalizePathSeps(std::string str)
{
    for(auto &ch : str)
    {
        if (ch=='\\')
            ch = '/';
    }

    return str;
}

//----------------------------------------------------------------------------
inline
std::string stripLeadingChar(const std::string &name, char ch)
{
    char s[] = { ch, 0 };
    auto pos = name.find_first_not_of(s);
    return pos==name.npos ? std::string() : std::string(name, pos, name.size()-pos);
}

//----------------------------------------------------------------------------
inline
std::string stripLeadingPathSeps(const std::string &name)
{
    return stripLeadingChar(name, '/');
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> splitNormalizedPath(const std::string &p)
{
    return umba::string::split(p, '/', true /* skipEmpty */ );
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> reducePath(const std::vector<std::string> &pathParts)
{
    std::vector<std::string> res;

    for(const auto &part : pathParts)
    {
        if (part==".")
            continue;

        if (part=="..")
        {
            if (!res.empty())
                res.pop_back();
            continue;
        }

        res.push_back(part);
    }

    return res;
}

//----------------------------------------------------------------------------
inline
std::string mergePath(const std::vector<std::string> &pathParts)
{
     return umba::string::merge<std::string>(pathParts.begin(), pathParts.end(), std::string("/"), [](const auto &s){ return s; });
}

//----------------------------------------------------------------------------
inline
std::string makeNormalizedRelativePath(std::string name)
{
    name = normalizePathSeps(name);

    using umba::string::starts_with_and_strip;
    // umba::string::starts_with_and_strip(name, std::string("//?/UNC/")); // "\\?\UNC\"
    starts_with_and_strip(name, std::string("//?/"));
    name = stripLeadingChar(name, '~');
    name = stripLeadingChar(name, '/');

    auto pos = name.find_first_of("/:");
    if (pos!=name.npos && name[pos]==':')
    {
        pos = name.find_first_of("/");
        if (pos==name.npos) // У нас тут путь вида C:
            name.clear();
        else
            name.erase(0, pos);
    }

    name = stripLeadingChar(name, '/');

    auto
    parts = splitNormalizedPath(name);
    parts = reducePath(parts);
    name  = mergePath(parts);

    return name;
}

//----------------------------------------------------------------------------
inline
std::string replaceInvalidFileNameChars(std::string name, bool replaceSpaceAlso)
{
    auto pos = name.find_first_of(getInvalidFileNameChars());
    while(pos!=name.npos)
    {
        name[pos] = '_';
        pos = name.find_first_of(getInvalidFileNameChars(), pos+1);
    }

    if (replaceSpaceAlso)
    {
        pos = name.find(' ');
        while(pos!=name.npos)
        {
            name[pos] = '_';
            pos = name.find(' ', pos+1);
        }
    }

    return name;

}

//----------------------------------------------------------------------------
inline
std::string replaceInvalidPathNameChars(std::string name, bool replaceSpaceAlso)
{
    auto pos = name.find_first_of(getInvalidPathNameChars());
    while(pos!=name.npos)
    {
        name[pos] = '_';
        pos = name.find_first_of(getInvalidPathNameChars(), pos+1);
    }

    if (replaceSpaceAlso)
    {
        pos = name.find(' ');
        while(pos!=name.npos)
        {
            name[pos] = '_';
            pos = name.find(' ', pos+1);
        }
    }

    return name;

}

//----------------------------------------------------------------------------
inline
std::string insertCounterIntoFilename(std::string name, std::size_t cnt, std::size_t numDigits=3)
{
    //std::ostringstream oss;
    auto cntStr = std::to_string(cnt);
    if (cntStr.size()<numDigits)
    {
        cntStr = std::string(numDigits-cntStr.size(), '0') + cntStr;
    }

    cntStr = "_" + cntStr;

    static std::string seps = "./";
    auto pos = name.find_last_of(seps);
    if (pos!=name.npos && name[pos]=='.')
        name.insert(pos, cntStr);
    else
        name += cntStr;

    return name;
}

    // void checkAddExtention(std::string ext)
    // {
    //     static std::string seps = "./";
    //
    //     if (ext.empty())
    //         ext = "txt";
    //
    //     for(auto &name : listingFilenames)
    //     {
    //         auto pos = name.find_last_of(seps);
    //         if (pos!=name.npos && name[pos]=='/')
    //             continue;
    //
    //         if (pos==name.npos)
    //             name += ".";
    //
    //         name += ext;
    //     }
    // }


//----------------------------------------------------------------------------
inline
const std::vector<std::string>& getAllowedFilenameEdgings()
{
    static std::vector<std::string> edgings =
    { "***" // жирный курсив
    , "**" // жирный
    , "__" // жирный
    , "~~" // зачеркнутый
    , "*"  // курсив
    , "_"  // курсив
    , "\"" //
    , "\'" //
    , "`"  //
    };

    return edgings;
}

//----------------------------------------------------------------------------
inline
std::string unescapeMarkdownString(const std::string &str)
{
    std::string resStr; resStr.reserve(str.size());

    bool prevSlash = false;
    for(auto ch: str)
    {
        if (prevSlash)
        {
            resStr.append(1, ch);
            prevSlash = false;
        }
        else
        {
            if (ch=='\\')
                prevSlash = true;
            else
                resStr.append(1, ch);
        }
    }

    return resStr;
}

//----------------------------------------------------------------------------
// Возвращает имя файла, очищенное от коментариев тупого болванчика
inline
std::string tryRemoveCommentsFromFilename(std::string name, std::string *pCommentStr=0)
{
    auto ext = umba::filename::getExt(name);

    auto bracePos = ext.rfind('('); // тупой болванчик обычно заключает свои коментарии в круглые скобки
    auto spacePos = ext.rfind(' '); // но если нет скобок, то болванчик просто отделил свой коментарий пробелом - ну не может же он лепить без пробела же
    // Так или иначе, но в расширениях пробел недопустим (но болванчик может прилепить в скобках без пробела)

    if (bracePos==ext.npos && spacePos==ext.npos) // ничего не нашлось
    {
        if (pCommentStr)
            pCommentStr->clear();
        return name;
    }

    std::size_t extCommentStartPos = std::size_t(-1);

    if (bracePos!=ext.npos && spacePos!=ext.npos)
    {
        // найдены оба
        extCommentStartPos = spacePos<bracePos ? spacePos : bracePos;
    }
    else
    {
        // нашлось только одно, выбираем найденное
        extCommentStartPos = spacePos!=ext.npos ? spacePos : bracePos;
    }

    // if (extCommentStartPos==std::size_t(-1)) // на всякий случай
    // {
    //     if (pCommentStr)
    //         pCommentStr->clear();
    //     return name;
    // }

    auto commentStr = std::string(ext, extCommentStartPos); // от найденной позиции в расширении имени файла до конца

    auto commentPos = name.rfind(commentStr);
    if (commentPos==name.npos)
    {
        if (pCommentStr)
            pCommentStr->clear();
        return name;
    }

    auto commentText = std::string(name, commentPos);
    umba::string::trim(commentText);

    name.erase(commentPos);
    umba::string::trim(name);

    if (pCommentStr)
        *pCommentStr = commentText;

    return name;
}

//----------------------------------------------------------------------------
inline
std::size_t stripFilenameEdging( std::string &name
                               , std::string *pEdging=0 // Возвращает индекс обрамления, если было или std::size_t(-1)
                               , bool bCommentPrependName=true // поместить коментарий в начало имени файла, если коментарий есть
                               )
{
    umba::string::trim(name);

    const auto& edgings = getAllowedFilenameEdgings();

    std::string commentString;

    auto prepentComment = [&](const auto &str) -> std::string
    {
        if (!bCommentPrependName)
           return str;

        if (commentString.empty())
           return str;

        auto p = umba::filename::getPath(str);
        auto n = umba::filename::getFileName(str);

        return umba::filename::appendPath(p, commentString + " " + n);
    };

    for(auto i=0u; i!=edgings.size(); ++i)
    {
        const auto &e = edgings[i];
        if (e.empty())
            continue;

        //if (umba::string::starts_with(name, e) && umba::string::ends_with(name, e))
        if (umba::string::starts_with(name, e))
        {
            // Если короткий, то должно заканчиваться строго на него же, и сразу и вырезаем, если заканчивается
            if (e.size()<2 && umba::string::ends_with_and_strip(name, e))
            {
                umba::string::starts_with_and_strip(name, e);
                umba::string::trim(name);
                name = tryRemoveCommentsFromFilename(name, &commentString);
                name = unescapeMarkdownString(name);
                name = prepentComment(name);
                if (pEdging)
                   *pEdging = e;
                return i;
            }

            // ищем последнее вхождение эджинга
            auto lastEdgingPos = name.rfind(e);
            if (lastEdgingPos!=name.npos && lastEdgingPos!=0) // Позиция найдена, и она не нулевая - не позиция стартового эджинга
            {
                name.erase(lastEdgingPos); // до конца
                umba::string::starts_with_and_strip(name, e);
                umba::string::trim(name);
                name = tryRemoveCommentsFromFilename(name, &commentString);
                name = unescapeMarkdownString(name);
                name = prepentComment(name);
                if (pEdging)
                   *pEdging = e;
                return i;
            }

            // umba::string::starts_with_and_strip(name, e);
            // umba::string::ends_with_and_strip(name, e);
            // umba::string::trim(name);
            // return i;
        }
    }

    return std::size_t(-1);
}

//----------------------------------------------------------------------------
inline
std::size_t testFilenameEdging(std::string name) // Возвращает true, если было есть обрамление
{
    umba::string::trim(name);

    const auto& edgings = getAllowedFilenameEdgings();

    for(auto i=0u; i!=edgings.size(); ++i)
    {
        const auto &e = edgings[i];

        if (umba::string::starts_with(name, e) && umba::string::ends_with(name, e))
        {
            return i;
        }
    }

    return std::size_t(-1);

}

//----------------------------------------------------------------------------
// возвращает true, если найдено обрамление, иначе false
inline
bool findListingFilenames( std::vector<std::string>::const_iterator b
                         , std::vector<std::string>::const_iterator e
                         , bool                      bCommentPrependName
                         , std::vector<std::string> *pResNames          = 0
                         , std::vector<std::string> *pRemainedTextLines = 0
                         , size_t                   *pNumEdgings        = 0
                         , size_t                   *pFoundEdgingType   = 0
                         )
{
/*
    Непустые строки перед листингом надо проверить, являются ли они именами файлов.
    Получаем в прямом порядке, отсекать надо начало, так как после текста может по ошибке
    без разрыва вставлено имя файла, а затем листинг.

    Для всех элементов получим тип эджинга. Начиная с конца, если эджинг был, выцепляем
    элементы с одинаковым эджингом.

    Если эджинга не было, выцепляем строки, которые:
    - не содержат запрещённых символов
    - похожи на имя файла (содержат точку или разделители пути)

    Если таких строк нет вообще, считаем последнюю строку не именем файла, а описанием (но это делаем на уровне выше).
*/
    // if (pResNames)
    //     pResNames->clear();

    std::vector<std::string> resNames;

    if (b==e)
        return false;


    std::vector<std::size_t> edgings;

    for(auto it=b; it!=e; ++it)
    {
        std::string str = *it;
        edgings.emplace_back(stripFilenameEdging(str, 0, bCommentPrependName));
        resNames.emplace_back(str);
    }

    // лень возится с обратным порядком, поэтому обратим порядок
    std::reverse(resNames.begin(), resNames.end());
    std::reverse(edgings.begin() , edgings.end() );

    std::size_t edgingType = edgings[0];

    // Ловим количество одинаковых обрамлений
    std::size_t numEqualEdgings = 0;
    for(; numEqualEdgings!=edgings.size(); ++numEqualEdgings)
    {
        if (edgingType!=edgings[numEqualEdgings])
            break;
    }

    if (edgingType==std::size_t(-1))
    {
        // Обрамления нет

        // Надо проверить валидность имен (если это имена)
        // и надо проверить на вероятность того, что это путь ()

        std::size_t i = 0;

        for(; i!=numEqualEdgings; ++i)
        {
            if (!isValidPathName(resNames[i]))
                break;
            if (!isPossiblePath(resNames[i]))
                break;
        }

        numEqualEdgings = i;
    }

    if (!numEqualEdgings)
        numEqualEdgings = 1;

    if (pRemainedTextLines)
    {
        pRemainedTextLines->insert( pRemainedTextLines->end(), std::next(resNames.begin(), std::ptrdiff_t(numEqualEdgings)), resNames.end() );
        std::reverse(pRemainedTextLines->begin(), pRemainedTextLines->end());
    }

    resNames.erase( std::next(resNames.begin(), std::ptrdiff_t(numEqualEdgings)), resNames.end() );
    // Реверсим обратно
    std::reverse(resNames.begin(), resNames.end());

    if (pFoundEdgingType)
       *pFoundEdgingType = edgingType;

    if (pNumEdgings)
       *pNumEdgings = numEqualEdgings;

    if (pResNames)
       *pResNames = resNames;

    return edgingType!=std::size_t(-1);
}

//----------------------------------------------------------------------------
inline
bool findListingFilenames( std::vector<std::string>  textLines
                         , bool                      bCommentPrependName
                         , std::vector<std::string> *pResNames          = 0
                         , std::vector<std::string> *pRemainedTextLines = 0
                         , size_t                   *pNumEdgings        = 0
                         , size_t                   *pFoundEdgingType   = 0
                         )
{
    return findListingFilenames(textLines.begin(), textLines.end(), bCommentPrependName, pResNames, pRemainedTextLines, pNumEdgings, pFoundEdgingType);
}

//----------------------------------------------------------------------------
inline
std::size_t getNumberOfFirstSameChars(const std::string &str)
{
    if (str.empty())
        return 0;

    char ch = str[0];

    std::size_t i = 0;
    for(; i!=str.size(); ++i)
    {
        if (ch!=str[i])
            break;
    }

    return i;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
MdLineType detectMarkdownLineType(const std::string &str, char *pChar=0, std::size_t *pNumChars=0)
{
    if (str.empty() || umba::string::trim_copy(str).empty())
        return MdLineType::emptyLine;

    std::size_t nChars = getNumberOfFirstSameChars(str);

    if (pNumChars)
       *pNumChars = nChars;

    char ch = str[0];

    if (pChar)
       *pChar = ch;

    if (ch==' ' && nChars<str.size())
    {
        if (str[nChars]=='-')
            return MdLineType::unorderedList;


        if (umba::string::is_digit(str[nChars]))
        {
            while(nChars<str.size() && umba::string::is_digit(str[nChars]))
                ++nChars;
        }

        if (nChars<str.size() && ch=='.')
            return MdLineType::orderedList;
    }


    switch(ch)
    {
        case '#' : return MdLineType::headerAtx;
        case '-' : return MdLineType::headerSetext;
        case '=' : return MdLineType::headerSetext;
        case '~' : return nChars>=3 ? MdLineType::codeTilda       : MdLineType::regularLine;
        case '`' : return nChars>=3 ? MdLineType::codeBacktick    : MdLineType::regularLine;
        case '\t': return nChars>=1 ? MdLineType::codeIndentTab   : MdLineType::regularLine;
        case ' ' : return nChars>=4 ? MdLineType::codeIndentSpace : MdLineType::regularLine;
        case '>' : return nChars>=1 ? MdLineType::quotation       : MdLineType::regularLine;
        default  : return MdLineType::regularLine;
    }
}

//----------------------------------------------------------------------------
inline
std::string extractCodeLangFromFencedCodeBlockMarker(std::string line)
{
    char markerChar = 0;
    std::size_t markerLen = 0;
    MdLineType mdLineType = detectMarkdownLineType(line, &markerChar, &markerLen);
    if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
    {
        line.erase(0, markerLen);
        umba::string::trim(line);
        return line;
    }

    return std::string();
}

//----------------------------------------------------------------------------
inline
std::string generateFence(FenceStyle fenseStyle, const std::vector<std::string> &fileLines)
{
    std::unordered_set<std::size_t> foundBackticks;
    std::unordered_set<std::size_t> foundTildes;

    for(const auto &l : fileLines)
    {
        char mdChar = 0;
        std::size_t mdNum = 0;
        MdLineType mdLineType = detectMarkdownLineType(l, &mdChar, &mdNum);

        if (mdLineType==MdLineType::codeBacktick)
        {
            foundBackticks.insert(mdNum);
        }
        else if (mdLineType==MdLineType::codeTilda)
        {
            foundTildes.insert(mdNum);
        }

    } // for(const auto &l : fileLines)

    std::size_t fenceLen = 0;

    if (fenseStyle==FenceStyle::auto_)
    {
        for(std::size_t i=3; i!=(std::size_t)-1; ++i)
        {
            if (foundTildes.find(i)==foundTildes.end())
            {
                fenceLen = i;
                fenseStyle = FenceStyle::tildes;
                break;
            }
            else if (foundBackticks.find(i)==foundBackticks.end())
            {
                fenceLen = i;
                fenseStyle = FenceStyle::backticks;
                break;
            }
        }
    }
    else
    {
        const std::unordered_set<std::size_t> &foundCounters = fenseStyle==FenceStyle::backticks ? foundBackticks : foundTildes;
        for(std::size_t i=3; i!=(std::size_t)-1; ++i)
        {
            if (foundCounters.find(i)==foundCounters.end())
            {
                fenceLen = i;
                break;
            }
        }
    }

    return std::string(fenceLen, fenseStyle==FenceStyle::backticks ? '`' : '~');

}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::vector<std::string> stripEmptyLeadingLines(const std::vector<std::string> &lines)
{
    std::vector<std::string> resLines; resLines.reserve(lines.size());
    for(auto l : lines)
    {
        umba::string::rtrim(l);

        if (!l.empty())
        {
            resLines.emplace_back(l);
            continue;
        }

        if (resLines.empty())
            continue;

        resLines.emplace_back(l);
    }

    return resLines;
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> stripEmptyTrailingLines(std::vector<std::string> lines)
{
    std::reverse(lines.begin(), lines.end());
    lines = stripEmptyLeadingLines(lines);
    std::reverse(lines.begin(), lines.end());
    return lines;
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> stripEmptyLeadingTrailingLines(std::vector<std::string> lines)
{
    return stripEmptyLeadingLines(stripEmptyTrailingLines(lines));
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct ListingInfo
{
    std::vector<std::string>  listingFilenames;
    std::vector<std::string>  listingCodeLines;
    std::string               foundLangName   ;

    void checkAddExtention(std::string ext)
    {
        static std::string seps = "./";

        if (ext.empty())
            ext = "txt";

        for(auto &name : listingFilenames)
        {
            auto pos = name.find_last_of(seps);

            if (pos==name.npos || name[pos]=='/')
            {
                name += ".";
                name += ext;
            }
        }
    }

    bool checkAutoEnumerate(std::unordered_map<std::string, std::size_t> &filenameCounters)
    {
        bool foundCollisions = false;

        for(auto &name : listingFilenames)
        {
            auto lowerCaseName = umba::string::tolower_copy(name);
            std::size_t counter = filenameCounters[lowerCaseName];
            if (!counter)
            {
                filenameCounters[lowerCaseName] = 1;
            }
            else
            {
                foundCollisions = true;
                name = insertCounterIntoFilename(name, counter);
                ++filenameCounters[lowerCaseName];
            }
        }

        return foundCollisions;
    }


}; // struct ListingInfo

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct MdArchivePart
{
    std::vector<std::string> allText  ; // Текст с листингами
    std::vector<std::string> textLines; // Только текст, без листингов
    std::vector<ListingInfo> listings ;

    //std::size_t              aitosaveCounter;

    void clear()
    {
        allText  .clear();
        textLines.clear();
        listings .clear();
    }

    bool empty() const
    {
        return allText.empty() && textLines.empty() && listings.empty();
    }

}; // struct MdArchivePart
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
inline
bool splitMarkdownArchive( std::vector<MdArchivePart>     &mdArchiveParts
                         , const std::vector<std::string> &mdLines
                         , bool        keepFilenameComment
                         , std::size_t partSeparatorLen=0 // по умолчанию - не используем разделение на части, считаем весь файл md-архивом
                         )
{
    std::vector<std::string> lastSignificantLines;
    std::size_t lastSeparatorLineNo = 0;

    bool readingCode = false;
    char codeMarkerChar = 0;
    std::size_t codeMarkerLen = 0;
    std::string codeLang;
    std::vector<std::string> codeLines;
    MdArchivePart archiveCurPart;

    if (partSeparatorLen!=0 && partSeparatorLen<3) // Если задан, то не менее трёх символов
        partSeparatorLen = 3;

    auto appendLastSignificanLinesToText = [&]()
    {
        lastSignificantLines = stripEmptyLeadingTrailingLines(lastSignificantLines);
        if (!archiveCurPart.textLines.empty() && !lastSignificantLines.empty())
        {
            archiveCurPart.textLines.emplace_back();
        }

        archiveCurPart.textLines.insert(archiveCurPart.textLines.end(), lastSignificantLines.begin(), lastSignificantLines.end());
    };
    // lastSignificantLines
    // textLines

    almai::MdLineType listingType = almai::MdLineType::emptyLine;

    lineNo = 0;
    for(const auto &line : mdLines)
    {
        lineNo++;

        char markerChar = 0;
        std::size_t markerLen = 0;

        MdLineType mdLineType = detectMarkdownLineType(line, &markerChar, &markerLen);

        if (readingCode)
        {
            bool unexpectedEndOfListing = false;

            if (listingType==MdLineType::codeTilda || listingType==MdLineType::codeBacktick)
            {
                // ИИ может глючить (не хватать токенов), и он может продалбываться
                // Иногда он может добавить маркер конца листинга в конец строки
                // потеряв часть содержимого листинга.
                // Надо просекать такие ситуации

                // !!! Не нужно. Просто был кривоватый документ, в нем были листинги на питоне, и внутри них были
                // маркдаун маркеры блоков кода

                // auto blockMarker = std::string(codeMarkerLen, codeMarkerChar);
                //
                // if (line!=blockMarker && umba::string::ends_with(line, blockMarker))
                // {
                //     unexpectedEndOfListing = true;
                //     LOG_WARN_INPUT("unexp-code-block-end") << "found unexpected code block end\n";
                // }

            }

            // listingType = mdLineType;
            if (listingType==MdLineType::codeIndentTab || listingType==MdLineType::codeIndentSpace)
            {
                // Идёт чтение листинга с отступами

                if (listingType==mdLineType) // тип листинга совпадает с типом текущейф строки
                {
                    // Продолжается чтение листинга с отступом
                    codeLines.push_back(line);
                }
                else // текущая строка не совпадает с типом листинга
                {
                    // Может начаться backtick или tilda листинг
                    if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
                    {
                        listingType = mdLineType;
                        readingCode = true;
                        codeMarkerChar = markerChar;
                        codeMarkerLen  = markerLen ;
                        codeLines.clear();
                        codeLang = extractCodeLangFromFencedCodeBlockMarker(line);
                    }
                    else
                    {
                        // листинг с отступом закончился
                        readingCode = false;
                        appendLastSignificanLinesToText();
                        lastSignificantLines.clear();
                        codeLines.clear();
                    }
                }
            }

            else if ( unexpectedEndOfListing // был обнаружен неожиданнй конец листинга?
                  || ((listingType==MdLineType::codeTilda || listingType==MdLineType::codeBacktick) // у нас текущий маркер листинга бэктик или тильда?
                    && listingType==mdLineType && codeMarkerChar==markerChar && codeMarkerLen==markerLen // текущий маркер листинга совпадает с типом текущей строки?
                     )
                    )
            {
                if (codeLang.empty())
                {
                    codeLang = extractCodeLangFromFencedCodeBlockMarker(line);
                }

                // Остальная обработка финализации листинга
                ListingInfo listingInfo;
                listingInfo.listingCodeLines = codeLines;
                listingInfo.foundLangName    = codeLang;

                std::vector<std::string> filenames;
                std::vector<std::string> remLines;
                bool hasEdging = findListingFilenames(lastSignificantLines, keepFilenameComment, &filenames, &remLines);
// bool findListingFilenames( std::vector<std::string>  textLines
//                          , bool                      bCommentPrependName
//                          , std::vector<std::string> *pResNames          = 0
//                          , std::vector<std::string> *pRemainedTextLines = 0
//                          , size_t                   *pNumEdgings        = 0
//                          , size_t                   *pFoundEdgingType   = 0
//                          )


                for(auto &name : filenames)
                {
                    name = replaceInvalidPathNameChars(name, !hasEdging);
                    name = makeNormalizedRelativePath(name);
                    listingInfo.listingFilenames.emplace_back(name);
                }

                //foundListings.emplace_back(listingInfo);
                archiveCurPart.listings.emplace_back(listingInfo);

                readingCode = false;
                lastSignificantLines = remLines;
                appendLastSignificanLinesToText();
                lastSignificantLines.clear();
                codeLines.clear();
            }
            else
            {
                // продолжаем чтение листинга
                codeLines.push_back(line);
            }
        }

        else // обычный режим
        {
            if ( mdLineType==MdLineType::headerSetext ) // '-'/'='
            {
                appendLastSignificanLinesToText();
                lastSignificantLines.clear();

                // Если обнаружен разделитель --- или === (или более длинный)
                if (partSeparatorLen!=0 && markerLen>=partSeparatorLen)
                {
                    //foundListings.clear(); // найденное ранее очищаем, листинги md-архива только в последней части

                    if (lastSeparatorLineNo!=lineNo) // Фрагмент текста не пустой
                    {
                        auto itB = std::next(mdLines.begin(), std::ptrdiff_t(lastSeparatorLineNo));
                        auto itE = std::next(mdLines.begin(), std::ptrdiff_t(lineNo-1));
                        archiveCurPart.allText = stripEmptyLeadingTrailingLines(std::vector<std::string>(itB, itE));
                    }
                    lastSeparatorLineNo = lineNo; // Запоминаем последний сепаратор

                    if (!archiveCurPart.empty())
                    {
                        archiveCurPart.textLines = stripEmptyLeadingTrailingLines(archiveCurPart.textLines);
                        mdArchiveParts.push_back(archiveCurPart);
                        archiveCurPart.clear();
                    }

                }
            }

            else if ( mdLineType==MdLineType::emptyLine)
            {
                // std::vector<std::string> filenames;
                // std::vector<std::string> remLines;
                size_t numEdgings        = 0;
                size_t foundEdgingType   = 0;

                //bool hasEdging =
                findListingFilenames(lastSignificantLines, keepFilenameComment, 0, 0, &numEdgings, &foundEdgingType);

                if (foundEdgingType==(std::size_t)-1) // имен файлов не найдено
                {
                    appendLastSignificanLinesToText();
                    archiveCurPart.textLines.push_back(line);
                    lastSignificantLines.clear();
                }

// bool findListingFilenames( std::vector<std::string>  textLines
//                          , bool                      bCommentPrependName
//                          , std::vector<std::string> *pResNames          = 0
//                          , std::vector<std::string> *pRemainedTextLines = 0
//                          , size_t                   *pNumEdgings        = 0
//                          , size_t                   *pFoundEdgingType   = 0
//                          )
            }

            else if ( mdLineType==MdLineType::headerAtx
                   || mdLineType==MdLineType::quotation
                    )
            {
                appendLastSignificanLinesToText();
                archiveCurPart.textLines.push_back(line);
                lastSignificantLines.clear();
            }

            else if (mdLineType==MdLineType::regularLine)
            {
                lastSignificantLines.push_back(line);
            }

            else if (mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick)
            {
                listingType = mdLineType;
                readingCode = true;
                codeMarkerChar = markerChar;
                codeMarkerLen  = markerLen ;
                codeLines.clear();
                codeLang = extractCodeLangFromFencedCodeBlockMarker(line);
            }

            else if (mdLineType==MdLineType::codeIndentTab || mdLineType==MdLineType::codeIndentSpace)
            {
                LOG_WARN_INPUT("unsup-code-block-type") << (mdLineType==MdLineType::codeIndentTab ? "tab" : "space") << " indent code blocks not supported\n";
                listingType = mdLineType;
                readingCode = true;
                codeMarkerChar = markerChar;
                codeMarkerLen  = markerLen ;
                codeLines.clear();
                codeLang.clear();
                codeLines.push_back(line);
            }

            else if (mdLineType==MdLineType::unorderedList || mdLineType==MdLineType::orderedList)
            {
                //lastSignificantLines.push_back(line);
                appendLastSignificanLinesToText();
                archiveCurPart.textLines.push_back(line);
                lastSignificantLines.clear();
            }

            else
            {
                LOG_WARN_INPUT("unk-line-type") << "unknown line type found, mdLineType: " << (unsigned)mdLineType << ", line: '" << line << "'\n";
                appendLastSignificanLinesToText();
                archiveCurPart.textLines.push_back(line);
                lastSignificantLines.clear();
            }

        }

    }

    if (lastSeparatorLineNo!=lineNo) // Фрагмент текста не пустой
    {
        auto itB = std::next(mdLines.begin(), std::ptrdiff_t(lastSeparatorLineNo));
        auto itE = std::next(mdLines.begin(), std::ptrdiff_t(lineNo-1));
        archiveCurPart.allText = stripEmptyLeadingTrailingLines(std::vector<std::string>(itB, itE));
    }
    //lastSeparatorLineNo = lineNo; // Запоминаем последний сепаратор

    archiveCurPart.textLines = stripEmptyLeadingTrailingLines(archiveCurPart.textLines);

    if (!archiveCurPart.empty())
    {
        mdArchiveParts.push_back(archiveCurPart);
        archiveCurPart.clear();
    }

    return true;
}


//----------------------------------------------------------------------------

} // namespace md

// md::

//----------------------------------------------------------------------------
~~~

**\_src/almai/MdjAppConfig.h**
~~~C/C++ Header
#pragma once

//----------------------------------------------------------------------------
#include "enums.h"
#include "utils.h"
#include "FileSystemScanInfo.h"
#include "FoundFileInfo.h"
//
#include "umba/umba.h"
//
#include "AppConfigBase.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//

#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig : public AppConfigBase
{
    std::vector<std::string>     headerLines;
    std::vector<std::string>     footerLines;


    std::vector<std::string> makePrepromptHeader(std::vector<std::string> lines)
    {
        // add skills lines
        return stripEmptyHeadTailLines(lines);
    }

    std::vector<std::string> makePrepromptFooter(std::vector<std::string> lines)
    {
        // add skills lines
        return stripEmptyHeadTailLines(lines);
    }


    std::vector<std::string> makePrepromptHeader()
    {
        auto lines = almai::utils::simpleReplaceClipboardMarkerLine(headerLines);
        return makePrepromptHeader(lines);
    }

    std::vector<std::string> makePrepromptFooter()
    {
        auto lines = almai::utils::simpleReplaceClipboardMarkerLine(footerLines);
        return makePrepromptFooter(lines);
    }





}; // struct AppConfig
~~~

**\_src/almai/MdjAppVerConfig.h**
~~~C/C++ Header
std::string appFullName   = "al-martyn1 AI markdown join tool";

std::string appVersion    = "1.0";
std::string appCommitHash;  //UNDONE
std::string appBuildDate  = __DATE__;
std::string appBuildTime  = __TIME__;

const char *appHomeUrl    = "https://github.com/al-martyn1/almai";
const char *appistrPath   = "";

const char *appSubPath    = "bin/almai-md-join" UMBA_FILESYS_EXE_EXT;
~~~

**\_src/almai/MdjArgParser.h**
~~~C/C++ Header
#pragma once

#include <stack>

//#include "app_config.h"
#include "umba/cmd_line.h"
#include "umba/cli_tool_helpers.h"
#include "umba/shellapi.h"
#include "umba/string_plus.h"
//
#include "utils.h"
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



#include "cli_opt_parsers/basic_options.h"
#include "cli_opt_parsers/overwrite.h"

#include "cli_opt_parsers/dict.h"
#include "cli_opt_parsers/output.h"
#include "cli_opt_parsers/filename_decoration.h"
#include "cli_opt_parsers/filename_title_level.h"
#include "cli_opt_parsers/sort.h"
#include "cli_opt_parsers/fence_style.h"
#include "cli_opt_parsers/strip_prefix.h"
#include "cli_opt_parsers/lang_marker.h"
#include "cli_opt_parsers/add_plural_pair.h"
#include "cli_opt_parsers/add_translation.h"
#include "cli_opt_parsers/add_project_root_marker.h"
#include "cli_opt_parsers/role_setup.h"

#include "cli_opt_parsers/list.h"
#include "cli_opt_parsers/add_header_footer.h"
#include "cli_opt_parsers/clipboard.h"
#include "cli_opt_parsers/join_lines_limit.h"
#include "cli_opt_parsers/attach.h"
#include "cli_opt_parsers/dir-tree-sort.h"


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
                              << " [OPTIONS] FILE_PATTERN [FILE_PATTERN]\n"
                              << "\nOptions:\n\n"
                              << helpText;
                              //<< " [OPTIONS] input_file [output_file]\n\nOptions:\n\n"<<helpText;
                }

                if (pCol) // argsNeedHelp
                {
                    argsParser.printHelpPage( std::cout
                                            , "[OPTIONS] FILE_PATTERN [FILE_PATTERN]"
                                            , "" // "If output_file not taken, STDOUT used"
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
            LOG_MSG << "Processing options file: " << optFileName << "\n";
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
        throw std::runtime_error("Duplicated option key - '" + opt + "'");
    }

};
~~~

**\_src/almai/MdsAppConfig.h**
~~~C/C++ Header
#pragma once

//----------------------------------------------------------------------------
#include "umba/umba.h"
//
#include "AppConfigBase.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//
//#include "ListingInfo.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig : public AppConfigBase
{

    std::vector<std::string>                         inputFiles;
    std::string                                      descriptionFilename = "__description__.md";
    bool                                             keepFilenameComment = true;

}; // struct AppConfig
~~~

**\_src/almai/MdsAppVerConfig.h**
~~~C/C++ Header
std::string appFullName   = "al-martyn1 AI markdown split tool";

std::string appVersion    = "1.0";
std::string appCommitHash;  //UNDONE
std::string appBuildDate  = __DATE__;
std::string appBuildTime  = __TIME__;

const char *appHomeUrl    = "https://github.com/al-martyn1/almai";
const char *appistrPath   = "";

const char *appSubPath    = "bin/almai-md-split" UMBA_FILESYS_EXE_EXT;
~~~

**\_src/almai/MdsArgParser.h**
~~~C/C++ Header
#pragma once

#include <stack>

//#include "app_config.h"
#include "umba/cmd_line.h"
#include "umba/cli_tool_helpers.h"
#include "umba/shellapi.h"
#include "umba/string_plus.h"
//
#include "utils.h"
//
#include "marty_cpp/marty_cpp.h"
//
#include "MdsAppVerConfig.h"

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


#include "cli_opt_parsers/basic_options.h"
#include "cli_opt_parsers/overwrite.h"
#include "cli_opt_parsers/list.h"
#include "cli_opt_parsers/dict.h"
#include "cli_opt_parsers/output-dir.h"
#include "cli_opt_parsers/clipboard.h"
#include "cli_opt_parsers/description_filename.h"

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
                              << " [OPTIONS] input_file\n"
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
            LOG_MSG << "Processing options file: " << optFileName << "\n";
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

    appConfig.inputFiles.push_back(argsParser.makeAbsPath(a));

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
~~~

**\_src/almai/MdXml.h**
~~~C/C++ Header
/*! @file
    @brief
 */

#pragma once


#include "umba/umba.h"
//
#include "umba/html_base.h"
//
#include "utils.h"
#include "md_utils.h"

//
#include <string>
#include <vector>
#include <stdexcept>

//----------------------------------------------------------------------------
//
// umba::html::
//
// enum class TagType
// {
//     invalid = -1,
//     text    =  0,
//     tag         ,  // <tag> open tag
//     close       ,  // </tag> close tag
//     empty          // <empty-tag/> - не содержит в себе вложенные тэги
//
// };
//
//
//
// IteratorType parseSingleTag(HtmlTagType &parseTo, IteratorType b, IteratorType e)

//----------------------------------------------------------------------------
// almai::mdxml::
namespace almai {
namespace mdxml { // Markdown XML

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
using TagType = umba::html::TagType;

using XmlTag  = umba::html::HtmlTagT< false // UseCaseIndependentNames
                                    , 0     // AttrAppendSeparatorChar
                                    , std::vector<std::string> // PayloadType (text)
                                    >;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
void stripEmptyLeadingTrailingLines(XmlTag &xmlTag)
{
    for(auto &child : xmlTag.childs)
    {
        child.text = md::stripEmptyLeadingTrailingLines(child.text);
    }
}

//----------------------------------------------------------------------------
// Учитываем листинги
template<typename LineIterator>
LineIterator parseMarkdownXml(XmlTag &parseTo, LineIterator b, LineIterator e, bool throwErrors)
{

    //std::string listingMarker;
    //std::string listingMarker;

// //----------------------------------------------------------------------------
// template<bool UseCaseIndependentNames = true, char AttrAppendSeparatorChar = 0, typename PayloadType = std::string>
// struct HtmlTagT
// {
//     TagType                                       tagType   = TagType::invalid;
//     std::string                                   name      ;
//     PayloadType                                   text      ;
//     std::unordered_map<std::string, std::string>  attributes;
//     std::vector<HtmlTagT>                         childs    ;  // Content

    auto checkAddTextChild = [&]()
    {
        if (parseTo.childs.empty() || parseTo.childs.back().tagType!=TagType::text)
        {
            XmlTag tag = { TagType::text };
            parseTo.childs.push_back(tag);
        }
    };

    auto addTextLine = [&](const std::string &line)
    {
        checkAddTextChild();
        parseTo.childs.back().text.push_back(line);
    };


    MdLineType  codeType = MdLineType::emptyLine;
    std::size_t codeMarkerLen = 0;

    while(b!=e)
    {
        // std::string trimmedLine = umba::string::trim_copy(*b);

        if (codeMarkerLen!=0)
        {
            // Мы читаем листинг и ждём точно такой же маркер окончания
            addTextLine(*b);

            char mdChar = 0;
            std::size_t numChars = 0;

            MdLineType mdLineType = md::detectMarkdownLineType(*b, &mdChar, &numChars);
            if ((mdLineType== MdLineType::codeTilda || mdLineType== MdLineType::codeBacktick) && codeType==mdLineType && numChars==codeMarkerLen)
            {
                // Найден завершающий маркер листинга
                codeType = MdLineType::emptyLine;
                codeMarkerLen = 0;
            }

            ++b;
            continue;
        }
        else
        {
            // Мы читаем обычный текст

            // Надо проверить на маркер листинга
            char mdChar = 0;
            std::size_t numChars = 0;

            MdLineType mdLineType = md::detectMarkdownLineType(*b, &mdChar, &numChars);
            if ((mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick) && numChars>=3)
            {
                codeType = mdLineType;
                codeMarkerLen = numChars;
                addTextLine(*b);
                ++b;
                continue;
            }

            // Надо проверить строку на предмет тэга

            std::string trimmedLine = umba::string::trim_copy(*b);

            if (umba::string::starts_with(trimmedLine, std::string("#//"))) // Просто комментарий
            {
                ++b;
                continue;
            }

            if (umba::string::starts_with(trimmedLine, std::string("#/*"))) // Многострочный комментарий
            {
                ++b;
                while(b!=e)
                {
                    trimmedLine = umba::string::trim_copy(*b);
                    if (umba::string::ends_with(trimmedLine, std::string("#*/")))
                        break;
                    ++b;
                }

                if (b!=e)
                   ++b;
                continue;
            }

            if (trimmedLine.empty() || trimmedLine.front()!='<')
            {
                // Это просто строка
                addTextLine(*b);
                ++b;
                continue;
            }

            std::string tagLine = trimmedLine;
            ++b;
            while(b!=e && tagLine.back()!='>')
            {
                tagLine.append(1, ' ');
                tagLine.append(umba::string::trim_copy(*b));
                ++b;
            }

            if (umba::string::starts_with(tagLine, std::string("<!--"))) // тэг коментария
            {
                // ++b;
                continue;
            }

            XmlTag tmpParseTo;
            // IteratorType
            parseSingleTag(tmpParseTo, tagLine.begin(), tagLine.end());

            if (tmpParseTo.tagType==TagType::invalid)
            {
                if (throwErrors)
                {
                    if (tmpParseTo.name.empty())
                        throw std::runtime_error("failed to parse tag");
                    else
                        throw std::runtime_error("failed to parse tag '" + tmpParseTo.name + "'");
                }

                continue; // Игнорируем ошибочный тэг
            }

            if (tmpParseTo.tagType==TagType::text || tmpParseTo.tagType==TagType::empty)
            {
                // tmpParseTo.text = stripEmptyLeadingTrailingLines(tmpParseTo.text);
                parseTo.childs.push_back(tmpParseTo);
                continue;
            }

            if (tmpParseTo.tagType==TagType::tag)
            {
                b = parseMarkdownXml(tmpParseTo, b, e, throwErrors);
                //tmpParseTo.text = stripEmptyLeadingTrailingLines(tmpParseTo.text);
                stripEmptyLeadingTrailingLines(tmpParseTo);
                parseTo.childs.push_back(tmpParseTo);
                continue;
            }

            if (tmpParseTo.tagType==TagType::close)
            {
                if (!parseTo.isTag(tmpParseTo.name))
                {
                    if (throwErrors)
                    {
                        throw std::runtime_error("closing tag '" + tmpParseTo.name + "' doesn't match opened tag '" + parseTo.name + "'");
                    }
                }

                //tmpParseTo.text = stripEmptyLeadingTrailingLines(tmpParseTo.text);

                return b;
            }

            if (throwErrors)
            {
                throw std::runtime_error("unexpected condition while parsing MD-XML");
            }

        } // if (codeMarkerLen!=0)

    } // while(b!=e)

    //parseTo.text = stripEmptyLeadingTrailingLines(parseTo.text);
    stripEmptyLeadingTrailingLines(parseTo);

    return b;
}

//----------------------------------------------------------------------------
template<typename LineIterator>
XmlTag parseMarkdownXml(LineIterator b, LineIterator e, bool throwErrors)
{
    XmlTag parseTo;
    parseTo.tagType = TagType::document;

    while(b!=e)
    {
        b = parseMarkdownXml(parseTo, b, e, throwErrors);
    }

    stripEmptyLeadingTrailingLines(parseTo);

    return parseTo;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace mdxml
} // namespace almai
~~~

**\_src/almai/PluralDatabase.h**
~~~C/C++ Header
/*! \file
    \brief Утилитарные функции для преобразования ед. число <-> мн. число
 */
#pragma once

#include "enums.h"
//
#include "umba/umba.h"
//
#include "umba/string.h"
#include "umba/filename.h"
#include "umba/filesys.h"

//

#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct PluralDatabase
{
    std::unordered_map<std::string, std::string> s2p;
    std::unordered_map<std::string, std::string> p2s;

    // --add-plural-pair

    bool addWordForms(std::string singular, std::string plural)
    {
        umba::string::trim(singular);
        umba::string::trim(plural);

        if (singular.empty() || plural.empty())
            return false;

        umba::string::tolower(singular);
        umba::string::tolower(plural);
        s2p[singular] = plural;
        p2s[plural] = singular;

        return true;
    }

    bool addWordForms(const std::string &singularPluralPair)
    {
        std::string s;
        std::string p;

        if (!umba::string::split_to_pair(singularPluralPair, s, p, ':'))
            return false;

        return addWordForms(s, p);
    }

    std::string findPlural(std::string word) const
    {
        umba::string::trim(word);
        if (word.empty())
            return word;

        umba::string::tolower(word);

        std::unordered_map<std::string, std::string>::const_iterator it = s2p.find(word);
        if (it!=s2p.end())
            return it->second; // нашлось отображение слова singular -> plural, возвращаем plural

        it = p2s.find(word);
        if (it!=p2s.end())
            return it->first; // нашлось отображение слова plural -> singular, возвращаем plural

        // Теперь простое правило - если есть 's' на конце - это множественное число, возвращаем его
        // Если нет - добавляем 's' и возвращаем

        if (word.back()!='s')
            word.append(1, 's');

        return word;
    }

    std::string findSingular(std::string word) const
    {
        umba::string::trim(word);
        if (word.empty())
            return word;

        umba::string::tolower(word);

        std::unordered_map<std::string, std::string>::const_iterator it = p2s.find(word);
        if (it!=p2s.end())
            return it->second; // нашлось отображение слова plural -> singular, возвращаем singular

        it = s2p.find(word);
        if (it!=s2p.end())
            return it->first; // нашлось отображение слова singular -> plural, возвращаем singular

        // Теперь простое правило - если есть 's' на конце - это множественное число, обрезаем 's' и получаем единственное

        if (word.back()=='s')
            word.erase(word.size()-1, 1);

        return word;
    }

}; // struct PluralDatabase

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------
~~~

**\_src/almai/Preprompt.h**
~~~C/C++ Header
/*! @file
    @brief Types which hold descriptions
 */

#pragma once

#include "Descriptions.h"
#include "MdXml.h"
//

#include "marty_cpp/src_normalization.h"
#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/filesys_scanners.h"
#include "umba/nul_ostream.h"
#include "umba/string.h"

//
#include <vector>
#include <set>
#include <stdexcept>


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct PrepromptProps
{
    std::string    type; // skill (skills), instruction (instructions), knowledge (knowledges), format (formats), output (outputs)
    std::string    name;

    std::string    file; // full file name or path
    bool           bExtended = false;  // somple .md file, not a folder


    template<typename MessageHandler>
    static
    std::vector<PrepromptProps> scanPath(const std::string &path, const std::string &scanForType, MessageHandler msgHandler)
    {
        UMBA_USED(msgHandler);

        using namespace umba::filename;
        using namespace umba::filesys;
        using namespace umba::string;

        umba::NulOstream log;

        std::vector<std::string>    foundFiles;
        std::vector<std::string>    excludedFiles;   // Это игнорится, просто оно требуется для вызова scanFolders, надо переделать на опциональное
        std::set<std::string>       foundExtentions; // Это игнорится, просто оно требуется для вызова scanFolders, надо переделать на опциональное

        umba::filesys::scanners::scanFolders( std::vector<std::string>(1, path) // rootScanPaths
                                            , std::vector<std::string>() // includeMaskList
                                            , std::vector<std::string>() // excludeMaskList
                                            , log
                                            , foundFiles
                                            , excludedFiles
                                            , foundExtentions
                                            , (std::vector<std::string>*)0 // pFoundFilesRootFolders
                                            , std::vector<std::string>() // excludeFoldersExact
                                            , false // recursive
                                            , false // logFoundHeader
                                            , true  // !addFolders
                                            , true  // compareOnlyFilenames
                                            );

        std::vector<PrepromptProps> resVec;

        for(auto &&foundFile : foundFiles)
        {
            auto fileName = getFileName(foundFile); // имя файла + расширение
            auto name     = getName(foundFile); // имя файла без пути и расширения
            auto ext      = getExt(foundFile);

            if (name.empty()) // вообще нет имени или имя пустое (вида .deepseek)
                continue;

            if (tolower_copy(ext)=="md" && isPathFile(foundFile) && isFileReadable(foundFile)) // isPathDirectory(foundFile)
            {
                // Найден читаемый md файл - у нас простой препромпт
                PrepromptProps pp = { scanForType, name, foundFile, false /* !bExtended */ };
                resVec.emplace_back(pp);
                continue;
            }

            if (isPathDirectory(foundFile))  // Возможно, это каталог с препромптом - расширенный препромпт
            {
                auto
                // mayBePrepromptFile = appendPath(foundFile, std::string(".md"));
                mayBePrepromptFile = appendPath(foundFile, ".md");
                if (isPathFile(mayBePrepromptFile) && isFileReadable(mayBePrepromptFile))
                {
                    PrepromptProps pp = { scanForType, name, mayBePrepromptFile, true /* bExtended */ };
                    resVec.emplace_back(pp);
                    continue;
                }

                //mayBePrepromptFile = appendPath(foundFile, std::string("SKILL.md"));
                mayBePrepromptFile = appendPath(foundFile, "SKILL.md");
                if (isPathFile(mayBePrepromptFile) && isFileReadable(mayBePrepromptFile))
                {
                    PrepromptProps pp = { scanForType, name, mayBePrepromptFile, true /* bExtended */ };
                    resVec.emplace_back(pp);
                    continue;
                }

                // Ещё какие-то варианты

            }

        }

        return resVec;
    }

    static
    std::vector<PrepromptProps> scanPath(const std::string &path, const std::string &scanForType)
    {
        return scanPath(path, scanForType, [](const std::string &) {});
    }

}; // struct PrepromptProps

//--------------------------------------------------------------------------------------------------------------------
template<typename StreamType>
StreamType& operator<<(StreamType &oss, const PrepromptProps &pp)
{
    oss << (pp.bExtended ? "E" : "S") << " " << pp.type << "/" << pp.name << " - " << pp.file;
    return oss;
}

//--------------------------------------------------------------------------------------------------------------------
template<typename StreamType>
StreamType& operator<<(StreamType &oss, const std::vector<PrepromptProps> &ppv)
{
    for(const auto &pp : ppv)
        oss << pp << "\n";
    return oss;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct Preprompt
{
    //std::string            type;  // Optional type string

    PrepromptDescription   description;
    PrepromptProps         props;
    mdxml::XmlTag          doc; // pp document

    static
    Preprompt parse(const std::vector<std::string> &lines, bool throwErrors)
    {
        if (lines.empty())
        {
            if (throwErrors)
                throw std::runtime_error("preprompt text is empty");

            return Preprompt();
        }

        Preprompt pp;

        auto it=lines.begin();
        if (umba::string::trim_copy(*it)=="---")
        {
            // read frontmatter here;
            ++it;
            auto fmBegin = it;

            for(; it!=lines.end(); ++it)
            {
                if (umba::string::trim_copy(*it)=="---")
                    break;
            }

            auto fmEnd = it;

            if (it!=lines.end())
            {
                ++it;
            }
            else
            {
                if (throwErrors)
                    throw std::runtime_error("preprompt has frontmatter begin marker, but has no end marker");
            }

            try
            {
                pp.description = PrepromptDescription::parse(std::vector<std::string>(fmBegin, fmEnd));
            }
            catch(const std::exception & /* e */ )
            {
                if (throwErrors)
                    throw;

                // Иначе - просто игнорируем и пытаемся распарсить хоть что-то

                try
                {
                    PrepromptDescription::parse(pp.description, std::vector<std::string>(fmBegin, fmEnd));
                }
                catch(const std::exception & /* e */ )
                {
                }
            }

        } // if (umba::string::trim_copy(*it)=="---")


        // Тут парсим тело препромпта

        pp.doc = mdxml::parseMarkdownXml(it, lines.end(), throwErrors);

        std::vector<std::string> allTextLines = pp.doc.getTextAll();

        for(auto &v : allTextLines)
            umba::string::trim(v);

        auto e =
        std::remove_if( allTextLines.begin(), allTextLines.end()
                      , [](const auto &s)
                        {
                            return s.empty();
                        }
                      );

        if (allTextLines.empty() || allTextLines.begin()==e)
        {
            if (throwErrors)
                throw std::runtime_error("preprompt text is empty");
        }

        return pp;
    }

    static
    Preprompt parse(const std::string &text, bool throwErrors)
    {
        std::vector<std::string> lines = marty_cpp::splitToLinesSimple(text);
        return parse(lines, throwErrors);
    }


    std::size_t findSections(std::vector<std::string> &secTagNames) const
    {
        for(const auto child : doc.childs)
        {
            if (child.tagType==mdxml::TagType::tag && !child.name.empty())
            {
                secTagNames.emplace_back(child.name);
            }
        }

        return secTagNames.size();
    }

    std::size_t findSections(std::set<std::string> &secTagNames) const
    {
        for(const auto child : doc.childs)
        {
            if (child.tagType==mdxml::TagType::tag && !child.name.empty())
            {
                secTagNames.insert(child.name);
            }
        }

        return secTagNames.size();
    }

    std::set<std::string> findSections() const
    {
        std::set<std::string> secTagNames;
        findSections(secTagNames);
        return secTagNames;
    }






}; // struct Preprompt






//----------------------------------------------------------------------------



} // namespace almai
~~~

**\_src/almai/PrepromptDatabase.h**
~~~C/C++ Header
/*! @file
    @brief Types which hold descriptions
 */

#pragma once

#include "Preprompt.h"
#include "PluralDatabase.h"
#include "utils.h"
//

#include "umba/parse_utils.h"
#include "umba/rule_of_five.h"

//
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <utility>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
using PrepromptMapType         = std::unordered_map<std::string, Preprompt>;
using PrepromptPropsMapType    = std::unordered_map<std::string, PrepromptProps>;
using PrepromptCategorySetType = std::unordered_set<std::string>;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AiPreprompts
{
    std::unordered_map< std::string, PrepromptMapType >            preprompts;
    std::unordered_map< std::string, PrepromptPropsMapType >       prepromptProps;
    std::unordered_map< std::string, PrepromptCategorySetType >    prepromptCategories;

    //--------------------------------------------------------------------------------------------------------------------
    template<typename WarningHandler>
    bool expandPrepromptDependenciesImpl(const std::string &prepromptId, const std::vector<std::string> &expanded, std::set<std::string> &alreadyUsed, WarningHandler warningHandler)
    {
        std::string ppFullName;
        PrepromptCategorySetType ppCategories;

        const Preprompt* pPreprompt = findPreprompt(prepromptId, ppFullName, &ppCategories);
        if (!pPreprompt)
        {
            std::string msg = makeCompletePpIdErrorMsg( prepromptId, ppFullName, ppCategories, 0 /* pGood */ );
            warningHandler(msg);
            return false;
        }

        if (alreadyUsed.find(ppFullName)!=alreadyUsed.end())
            return true;

        alreadyUsed.insert(ppFullName);

        // pPreprompt->description.requiresList // std::vector< std::vector<std::string> >     requiresList;

        return true;

    }

    //--------------------------------------------------------------------------------------------------------------------
    const Preprompt* findPreprompt(const PluralDatabase* pPluralDb, const std::string &prepromptId, std::string *pPrepromptFullName = 0, PrepromptCategorySetType *ppCategories=0) const
    {
        auto fullPpId = makeCompletePpId(pPluralDb, prepromptId, ppCategories);
        if (pPrepromptFullName)
           *pPrepromptFullName = fullPpId;

        if (fullPpId.empty())
            return 0;

        std::string category, id;
        splitPrepromptId(fullPpId, category, id);
        UMBA_ASSERT(!category.empty());

        auto ppCatIt = preprompts.find(category);
        if (ppCatIt==preprompts.end())
            return 0;

        // найдена категория
        auto ppIt = ppCatIt->second.find(id);
        if (ppIt==ppCatIt->second.end())
            return 0;

        return &ppIt->second;
    }

    //--------------------------------------------------------------------------------------------------------------------
    /*! Makes full preprompt name. Checks preprompt exist or defined in multiple categories
        @returns empty string if preprompt not found or exist in multiple categories
        @param ppCategories filled with preprompt categories. 0, 1, or more values, 0 or more than one if returns empty string
    */
    std::string makeCompletePpId(const PluralDatabase *pPluralDb, std::string prepromptId, PrepromptCategorySetType *ppCategories=0) const
    {
        if (ppCategories)
            ppCategories->clear();

        // *pluralDb.get()
        prepromptId = utils::normalizePrepromptId(*pPluralDb, prepromptId);

        std::string category, name;
        if (splitPrepromptId(prepromptId, category, name))
        {
            // Есть разделитель
            auto ppCatIt = preprompts.find(category);
            if (ppCatIt==preprompts.end())
	                return std::string(); // не найдено в категориях

            // найдена категория
            auto ppIt = ppCatIt->second.find(name);
            if (ppIt==ppCatIt->second.end())
                return std::string(); // не найдено в препромптах найденой категории

            if (ppCategories)
                ppCategories->insert(category); // вставляем единственную категорию, где найдено полностью квалифицированное имя препромпта

            return prepromptId;
        }

        auto ppCatIt = prepromptCategories.find(name);

        // UMBA_ASSERT(ppCatIt!=prepromptCategories.end()); // такого в норме не может быть
        if (ppCatIt==prepromptCategories.end())
            return std::string();

        if (ppCategories)
            *ppCategories = ppCatIt->second; // присваиваем все найденные категории препромпта

        if (ppCatIt->second.size()==1)
        {
            category = *ppCatIt->second.begin();
            return mergePrepromptId(category, name);
        }

        return std::string();
    }

    //--------------------------------------------------------------------------------------------------------------------
    std::string makeCompletePpIdErrorMsg( const std::string &requestedPrepromptId
                                        , const std::string &foundPrepromptId
                                        , const PrepromptCategorySetType &ppCategories
                                        , bool *pGood=0
                                        ) const
    {
        if (!foundPrepromptId.empty())
        {
            if (pGood)
               *pGood = true;

            return "Complete name of '" + requestedPrepromptId + "': '" + foundPrepromptId + "'";
        }
        else
        {
            UMBA_ASSERT(ppCategories.size()!=1); // Не может быть равно 1

            if (pGood)
               *pGood = false;

            if (ppCategories.empty())
            {
                return "Preprompt '" + requestedPrepromptId + "' not found";
            }
            else
            {
                std::string resStr = "Preprompt '" + requestedPrepromptId + "' found in multiple categories: ";
                std::string cats;
                for(auto && cat: ppCategories)
                {
                    if (!cats.empty())
                        cats.append(", ");
                    cats.append("'" + cat + "'");
                }

                return resStr + cats;
            }
        }
    }

    //--------------------------------------------------------------------------------------------------------------------
    static
    bool splitPrepromptId(const std::string prepromptId, std::string &category, std::string &id)
    {
         category.clear();
         id.clear();

         if (umba::parse_utils::optionStringSplitToPair(prepromptId, category, id, "/\\"))
         {
             // Есть разделитель
             umba::string::trim(category);
             umba::string::trim(id);
             return true;
         }

         // Нет разделителя
         // Если разделителя нет, то id помещается в первый элемент (category), а по логике id - это второй элемент. Меняем их местами
         using std::swap; swap(category, id);

         umba::string::trim(id);

         return false;
    }

    //--------------------------------------------------------------------------------------------------------------------
    static
    std::string mergePrepromptId(const std::string &category, const std::string &id)
    {
        if (category.empty())
            return id;

        return category + "/" + id;
    }

    //--------------------------------------------------------------------------------------------------------------------

};

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct PrepromptDatabase
{

    //--------------------------------------------------------------------------------------------------------------------

    using PluralDatabaseSharedPtrType = std::shared_ptr<PluralDatabase>;



    //--------------------------------------------------------------------------------------------------------------------
    PluralDatabaseSharedPtrType                       pluralDb;
    std::vector<std::string>                          prepromptDirs;
    std::unordered_map< std::string, AiPreprompts>    preprompts;  // aiEngine -> aiPreprompts


    //--------------------------------------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------------------------------------
    UMBA_RULE_OF_FIVE_DEFAULT(PrepromptDatabase);

    PrepromptDatabase( PluralDatabaseSharedPtrType pluralDb_
                     , const std::vector<std::string> &prepromptDirs_
                     )
    : pluralDb(pluralDb_)
    , prepromptDirs(prepromptDirs_)
    {}

    //--------------------------------------------------------------------------------------------------------------------



    //--------------------------------------------------------------------------------------------------------------------
    #if 0
    template<typename WarningHandler>
    bool expandPrepromptDependenciesImpl(const std::string &prepromptId, const std::vector<std::string> &expanded, std::set<std::string> &alreadyUsed, WarningHandler warningHandler)
    {
        std::string ppFullName;
        PrepromptCategorySetType ppCategories;

        const Preprompt* pPreprompt = findPreprompt(prepromptId, ppFullName, &ppCategories);
        if (!pPreprompt)
        {
            std::string msg = makeCompletePpIdErrorMsg( prepromptId, ppFullName, ppCategories, 0 /* pGood */ );
            warningHandler(msg);
            return false;
        }

        if (alreadyUsed.find(ppFullName)!=alreadyUsed.end())
            return true;

        alreadyUsed.insert(ppFullName);

        // pPreprompt->description.requiresList // std::vector< std::vector<std::string> >     requiresList;

        return true;

    }
    #endif

    //--------------------------------------------------------------------------------------------------------------------
    const Preprompt* findPreprompt(const std::string &aiEngine, const std::string &prepromptId, std::string *pPrepromptFullName = 0, PrepromptCategorySetType *ppCategories=0) const
    {
        auto it = preprompts.find(aiEngine);
        if (it==preprompts.end())
            return 0;

        return it->second.findPreprompt(pluralDb.get(), prepromptId, pPrepromptFullName, ppCategories);
    }

    //--------------------------------------------------------------------------------------------------------------------
    /*! Makes full preprompt name. Checks preprompt exist or defined in multiple categories
        @returns empty string if preprompt not found or exist in multiple categories
        @param ppCategories filled with preprompt categories. 0, 1, or more values, 0 or more than one if returns empty string
    */
    std::string makeCompletePpId(const std::string &aiEngine, const std::string &prepromptId, PrepromptCategorySetType *ppCategories=0) const
    {
        auto it = preprompts.find(aiEngine);
        if (it==preprompts.end())
            return std::string();

        return it->second.makeCompletePpId(pluralDb.get(), prepromptId, ppCategories);
    }

    //--------------------------------------------------------------------------------------------------------------------
    std::string makeCompletePpIdErrorMsg( const std::string &aiEngine
                                        , const std::string &requestedPrepromptId
                                        , const std::string &foundPrepromptId
                                        , const PrepromptCategorySetType &ppCategories
                                        , bool *pGood=0
                                        ) const
    {
        auto it = preprompts.find(aiEngine);
        if (it==preprompts.end())
            return std::string();

        return it->second.makeCompletePpIdErrorMsg( requestedPrepromptId, foundPrepromptId, ppCategories, pGood);
    }

    //--------------------------------------------------------------------------------------------------------------------
    static
    bool splitPrepromptId(const std::string prepromptId, std::string &category, std::string &id)
    {
         return AiPreprompts::splitPrepromptId(prepromptId, category, id);
    }

    //--------------------------------------------------------------------------------------------------------------------
    static
    std::string mergePrepromptId(const std::string &category, const std::string &id)
    {
        return AiPreprompts::mergePrepromptId(category, id);
    }

    //--------------------------------------------------------------------------------------------------------------------



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

    /*! Scan folders for preprompts and fill basic props without reading preprompt files
        @param scannedPrepromptProps [category][id] -> PrepromptProps
        @param scannedPrepromptTypes [id] -> set of categories
    */
    static
    void scanForPrepromptsProps( std::vector<std::string> *pScannedFolders
                               , std::unordered_map< std::string, std::unordered_map<std::string, almai::PrepromptProps> > &scannedPrepromptProps
                               , std::unordered_map< std::string, std::unordered_set<std::string> > &scannedPrepromptCategories
                               , const almai::PluralDatabase    &pluralDb_
                               //, std::vector<std::string>       aiEngines
                               , const std::string              &aiEngine_
                               , const std::vector<std::string> &ppDirs
                               , std::vector<std::string>       prepromptCategoriesToScan
                               )
    {
        std::vector<std::string> aiEngines;
        aiEngines.push_back(std::string());
        if (!aiEngine_.empty())
            aiEngines.push_back(aiEngine_);

        {
            std::vector<std::string> pptsTmp; pptsTmp.reserve(prepromptCategoriesToScan.size());
            for(const auto &ppt: prepromptCategoriesToScan)
            {
                pptsTmp.push_back(pluralDb_.findPlural(ppt));
            }

            using std::swap;

            swap(prepromptCategoriesToScan, pptsTmp);
        }


        for(const auto &aiEngine : aiEngines)
        {
            for(auto ppDir: ppDirs)
            {
                if (!aiEngine.empty())
                    ppDir = umba::filename::appendPath(ppDir, "." + aiEngine);

                for(const auto &ppType: prepromptCategoriesToScan)
                {
                    auto scanDir = umba::filename::appendPath(ppDir, ppType);
                    if (pScannedFolders)
                        pScannedFolders->push_back(scanDir);

                    auto ppPropsVec = almai::PrepromptProps::scanPath(scanDir, ppType);

                    for(const auto &pppItem : ppPropsVec)
                    {
                        scannedPrepromptProps[pppItem.type][pppItem.name] = pppItem;
                        scannedPrepromptCategories[pppItem.name].insert(pppItem.type);
                    }

                } // for(const auto &ppType: prepromptCategoriesToScan)

            } // for(auto ppDir: ppDirs)

        } // for(const auto &aiEngine : aiEngines)

    }

    // struct AiPreprompts
    // {
    //     std::unordered_map< std::string, PrepromptMapType >            preprompts;
    //     std::unordered_map< std::string, PrepromptPropsMapType >       prepromptProps;
    //     std::unordered_map< std::string, PrepromptCategorySetType >    prepromptCategories;
    // };
    // std::unordered_map< std::string, AiPreprompts>    preprompts;  // aiEngine -> aiPreprompts

    //--------------------------------------------------------------------------------------------------------------------
    template<typename PrepromptReadingErrorHandler, typename PrepromptParsingErrorHandler>
    void scanForPreprompts( std::vector<std::string>       *pScannedFolders
                          , std::vector<std::string>       aiEngines
                          , std::vector<std::string>       prepromptCategoriesToScan
                          , PrepromptReadingErrorHandler   readingErrHandler
                          , PrepromptParsingErrorHandler   parsingErrorHandler
                          )
    {
        // prepromptDirs.clear();
        // preprompts.clear();
        // prepromptProps.clear();
        // prepromptCategories.clear();

        aiEngines.insert(aiEngines.begin(), std::string());

        for(const auto &aiEngine: aiEngines)
        {
            AiPreprompts aiPrepromptsItem;

            scanForPrepromptsProps( pScannedFolders
                                  , aiPrepromptsItem.prepromptProps
                                  , aiPrepromptsItem.prepromptCategories
                                  , *pluralDb.get()
                                  , aiEngine
                                  , prepromptDirs
                                  , prepromptCategoriesToScan
                                  );

            for(const auto &[ppTypeStr, ppNameMap] : aiPrepromptsItem.prepromptProps)
            {
                for(const auto &[ppName, ppProps] : ppNameMap)
                {
                    std::vector<std::string> inputFileLines;

                    if (!utils::readFile(ppProps.file, inputFileLines))
                    {
                        readingErrHandler(ppProps.file);
                        continue;
                    }

                    Preprompt preprompt;

                    try
                    {
                        preprompt = Preprompt::parse(inputFileLines, true /* throwErrors */ );
                    }
                    catch(const std::exception &e)
                    {
                        parsingErrorHandler(ppProps.file, e);

                        // Пробуем игнорировать ошибки
                        // Но исключение всё равно может вылететь. Но мы его уже не ловим, пусть летит
                        preprompt = Preprompt::parse(inputFileLines, false /* !throwErrors */ );
                    }

                    preprompt.props = ppProps;

                    // Если до сюда не вылетели, то можно добавлять в результаты

                    // !!! Нужно поправить зависимости, привести всё в plural
                    for(auto &require : preprompt.description.requiresList)
                    {
                        for(auto &r: require)
                        {
                            r = utils::normalizePrepromptId(*pluralDb.get(), r);
                        }
                    }

                    aiPrepromptsItem.preprompts[ppTypeStr][ppName] = preprompt;
                }
            }

            preprompts[aiEngine] = aiPrepromptsItem;

        }

    }


}; // struct PrepromptDatabase

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------



} // namespace almai
~~~

**\_src/almai/Project.h**
~~~C/C++ Header
/*! \file
    \brief Almai проект
 */
#pragma once

#include "PrepromptDatabase.h"
//
#include "enums.h"
#include "utils.h"
//
#include "umba/umba.h"
//
#include "umba/string.h"
#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/parse_utils.h"

//

#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct Project
{
    std::unordered_map<std::string, std::vector<std::string> >       roles;


    template<typename WarningHandler>
    void normalizeRoles( const PrepromptDatabase &ppDb
                       , WarningHandler warningHandler
                       )
    {
        for(auto &[role, roleSkills] : roles)
        {
            for(auto &skill : roleSkills)
            {
                PrepromptCategorySetType skillCategories;

                std::string completeSkillName = ppDb.makeCompletePpId("", skill, &skillCategories);
                bool bGood = false;
                auto msg = ppDb.makeCompletePpIdErrorMsg("", skill, completeSkillName, skillCategories, &bGood);

                if (bGood)
                {
                    skill = completeSkillName;
                }
                else
                {
                    warningHandler(msg);
                }
            }
        }
    }

    template<typename ErrorHandler, typename WarningHandler>
    bool checkNormalize( const PrepromptDatabase &ppDb
                       , ErrorHandler   errorHandler
                       , WarningHandler warningHandler
                       )
    {
        UMBA_USED(errorHandler);
        normalizeRoles(ppDb, warningHandler);
        return true;
    }


    template<typename SkillNamePrepareHandler>
    bool updateRoleFromRoleString(const std::string &role, const std::string &str, SkillNamePrepareHandler skillNamePrepareHandler)
    {
        if (role.empty())
            return false;

        auto &rolesSet = roles[role];

        return umba::parse_utils::optionStringUpdateSet(str, rolesSet, skillNamePrepareHandler, umba::CaseOption::toLower);
    }

    template<typename SkillNamePrepareHandler>
    bool updateRoleFromRoleStringList(const std::string &role, const std::string &strList, SkillNamePrepareHandler skillNamePrepareHandler)
    {
        if (role.empty())
            return false;

        auto &rolesSet = roles[role];

        return umba::parse_utils::optionStringListUpdateSet(strList, rolesSet, skillNamePrepareHandler, umba::CaseOption::toLower);
    }


    template<typename SkillNamePrepareHandler>
    static
    marty::json parse(Project &p, const std::string &text, SkillNamePrepareHandler skillNamePrepareHandler, bool throwErrors)
    {
        auto j = yaml_json::parseToJson(text);


        if (j.find("roles")!=j.end())
        {
            //d.name = j["name"].get<std::string>();
            auto r = j["roles"];

            // https://github.com/nlohmann/json?tab=readme-ov-file#stl-like-access

            if (r.is_object())
            {
                for (const auto &[key, value] : r.items()) // object iteraion
                {
                    std::string role = key;
                    umba::string::case_convert(role, umba::CaseOption::toLower);

                    if (value.is_array())
                    {
                        for (nlohmann::json::iterator it = value.begin(); it!=value.end(); ++it)
                        {
                            std::string skillName = it->get<std::string>();
                            if (!p.updateRoleFromRoleString(role, skillName, skillNamePrepareHandler))
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role '" + role + "' with skill value '" + skillName + "'");
                            }
                        }
                    }

                    else if (value.is_string())
                    {
                            std::string skillsList = value.get<std::string>();
                            if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
                            }
                    }

                    else
                    {
                        throw std::runtime_error("role '" + role + "' is in " + marty::json_utils::nodeTypeName(value) + " format. Only string/array formats allowed");
                    }

                } // for (const auto &[key, value] : r.items())
            }


            else if (r.is_array())
            {
                //------------- !!!
                // parse array here
                for (nlohmann::json::iterator it = r.begin(); it!=r.end(); ++it) // array iteraion
                {
                    auto arrItem = *it;

                    if (arrItem.is_string())
                    {
                        std::string roleDefinitionString = arrItem.get<std::string>();
                        std::string role, skillsList;
                        if (!umba::parse_utils::optionStringSplitToPair(roleDefinitionString, role, skillsList, ":"))
                        {
                            // Нужна пара: "role: role-definition"
                            if (throwErrors)
                               throw std::runtime_error("failed to parse role definition string: '" + roleDefinitionString + "'");
                        }

                        umba::string::case_convert(role, umba::CaseOption::toLower);

                        if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
                        {
                            if (throwErrors)
                                throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
                        }
                    }


                    // Тут у нас могут быть объекты с одним или несколькими ключами
                    // compact mapping in sequence
                    // https://chat.deepseek.com/share/1gbmc561xdxi5teq79

                    // https://chat.deepseek.com/share/teujr7bferpccnduwp

                    else if (arrItem.is_object())
                    {
                        for (const auto &[key, value] : arrItem.items()) // object iteraion
                        {
                            std::string role = key;
                            umba::string::case_convert(role, umba::CaseOption::toLower);

                            if (role.empty())
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role - role name is empty");

                                continue;

                            }

                            auto &rolesSet = p.roles[role];

                            yaml_json::parseJsonKeyValueArrayOrString( value // j
                                                          , rolesSet
                                                          , skillNamePrepareHandler
                                                          , throwErrors
                                                          , "value can be only a string type, role: '" + role + "'" // msgValueOnlyString
                                                          , "value can be only a string or array type, role: '" + role + "'" // msgValueOnlyStringOrArray
                                                          , "failed to update role '" + role + "'" // msgUpdateFailedWithVal
                                                          );

                        } // for (const auto &[key, value] : r.items()) // object iteraion

                    } // else if (arrItem.is_object())


                    else if (arrItem.is_array())
                    {
                        // std::string text = it->get<std::string>();
                        throw std::runtime_error("UNEXPECTED ARRAY ITEM");
                    }


                    else
                    {
                        if (throwErrors)
                            throw std::runtime_error( /* "'" */   /* + arrItemStr + */   /* "'" */ "!!!" " is in " + marty::json_utils::nodeTypeName(arrItem) + " format.");
                    }

                } // for (nlohmann::json::iterator it = r.begin(); it!=r.end(); ++it) // array iteraion

            } // if (r.is_object()) else if (r.is_array())


        } // if (j.find("roles")!=j.end())

        return j;
    }


}; // struct Project

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StreamType> inline
StreamType& operator<<(StreamType &oss, const Project &p)
{
    oss << "Project:\n\n";

    oss << "  Roles:\n";
    for(const auto &[role,skills] : p.roles)
    {
        oss << "    " << role;

        for(std::size_t i=0; i!=skills.size(); ++i)
        {
            oss << (i==0 ? ": " : ", ") << skills[i];
        }

        oss << "\n";
    }

    oss << "\n";

    return oss;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------
~~~

**\_src/almai/Project\_new.h**
~~~C/C++ Header
/*! \file
    \brief Almai проект
 */
#pragma once

#include "enums.h"
#include "utils.h"
//
#include "umba/umba.h"
//
#include "umba/string.h"
#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/parse_utils.h"

//

#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct Project
{
    std::unordered_map<std::string, std::vector<std::string> >       roles;



    template<typename SkillNamePrepareHandler>
    bool updateRoleFromRoleString(const std::string &role, const std::string &str, SkillNamePrepareHandler skillNamePrepareHandler)
    {
        if (role.empty())
            return false;

        auto &rolesSet = roles[role];

        return umba::parse_utils::optionStringUpdateSet(str, rolesSet, skillNamePrepareHandler, umba::CaseOption::toLower);
    }

    template<typename SkillNamePrepareHandler>
    bool updateRoleFromRoleStringList(const std::string &role, const std::string &strList, SkillNamePrepareHandler skillNamePrepareHandler)
    {
        if (role.empty())
            return false;

        auto &rolesSet = roles[role];

        return umba::parse_utils::optionStringListUpdateSet(strList, rolesSet, skillNamePrepareHandler, umba::CaseOption::toLower);
    }


    template<typename SkillNamePrepareHandler>
    static
    marty::json parse(Project &p, const std::string &text, SkillNamePrepareHandler skillNamePrepareHandler, bool throwErrors)
    {
        auto j = parseToJson(text);


        if (j.find("roles")!=j.end())
        {
            //d.name = j["name"].get<std::string>();
            auto r = j["roles"];

            // if (!r.is_object())
            // {
            //     throw std::runtime_error("'roles' is in " + marty::json_utils::nodeTypeName(r) + " format. Only object format allowed");
            // }

            // https://github.com/nlohmann/json?tab=readme-ov-file#stl-like-access

            if (r.is_object())
            {
                for (const auto &[key, value] : r.items()) // object iteraion
                {
                    const std::string &role = key;

                    if (value.is_array())
                    {
                        for (nlohmann::json::iterator it = value.begin(); it!=value.end(); ++it)
                        {
                            std::string skillName = it->get<std::string>();
                            if (!p.updateRoleFromRoleString(role, skillName, skillNamePrepareHandler))
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role '" + role + "' with skill value '" + skillName + "'");
                            }
                        }
                    }

                    else if (value.is_string())
                    {
                            std::string skillsList = value.get<std::string>();
                            if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
                            {
                                if (throwErrors)
                                    throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
                            }
                    }

                    else
                    {
                        throw std::runtime_error("role '" + role + "' is in " + marty::json_utils::nodeTypeName(value) + " format. Only string/array formats allowed");
                    }

                } // for (const auto &[key, value] : r.items())
            }

            else if (r.is_array())
            {

// roles:
//   # Можно записать и как object, но массив гарантирует порядок элементов, а object - не факт. Впрочем, другой нефакт в том, что при настройке ролей нам порядок не особо и важен
//   - c-cpp-dev: -, skills/cpp-dev; skills/c-dev, -skills/jni-master # тут в одну строку сбросили все предыдущие скилы для роли, и добавили skills/cpp-dev и skills/c-dev
//   - c-cpp-no-java-dev: skills/cpp-dev; skills/c-dev, -skills/jni-master # тут только добавили skills/cpp-dev и skills/c-dev
//   - uni-tester:
//     - - # Сбрасываем все предыдущие скилы для роли (а это будет валидно в yml?)
//     - skills/auto-tester
//     - skills/manual-tester
//   - super-architect:
//     - sw-architect # Архитектор ПО
//     - sys-architect # Архитектор всей системы


                // parse array here
                for (nlohmann::json::iterator it = r.begin(); it!=r.end(); ++it) // array iteraion
                {
                    auto arrItem = *it;

                    if (arrItem.is_string())
                    {
                        std::string roleDefinitionString = arrItem.get<std::string>();
                        std::string role, skillsList;
                        if (!umba::parse_utils::optionStringSplitToPair(roleDefinitionString, role, skillsList, ":"))
                        {
                            // Нужна пара: "role: role-definition"
                            if (throwErrors)
                               throw std::runtime_error("failed to parse role definition string: '" + roleDefinitionString + "'");
                        }

                        if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
                        {
                            if (throwErrors)
                                throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
                        }
                    }


                    // Тут у нас могут быть объекты с одним или несколькими ключами
                    // compact mapping in sequence
                    // https://chat.deepseek.com/share/1gbmc561xdxi5teq79
                    else if (arrItem.is_object())
                    {
                        for (const auto &[role, value] : arrItem.items())
                        {
                            // ==================== ИСПРАВЛЕНИЕ: поддержка массива ====================
                            if (value.is_array())
                            {
                                // Каждый элемент массива — отдельная строка-спецификация скилла
                                for (const auto &skillItem : value)
                                {
                                    if (!skillItem.is_string())
                                    {
                                        if (throwErrors)
                                            throw std::runtime_error("Skill item must be a string, got " +
                                                                     marty::json_utils::nodeTypeName(skillItem));
                                    }
                                    std::string skillSpec = skillItem.get<std::string>();
                                    if (!p.updateRoleFromRoleString(role, skillSpec, skillNamePrepareHandler))
                                    {
                                        if (throwErrors)
                                            throw std::runtime_error("Failed to update role '" + role +
                                                                     "' with skill spec '" + skillSpec + "'");
                                    }
                                }
                            }
                            else if (value.is_string())
                            {
                                std::string skillsList = value.get<std::string>();
                                if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
                                {
                                    if (throwErrors)
                                        throw std::runtime_error("failed to update role '" + role +
                                                                 "' with skills: '" + skillsList + "'");
                                }
                            }
                            else
                            {
                                throw std::runtime_error("Value for role '" + role + "' must be string or array, got " +
                                                         marty::json_utils::nodeTypeName(value));
                            }
                            // ========================================================================
                        } // for (const auto &[role, value] : arrItem.items())
                    } // else if (arrItem.is_object())


                    else if (arrItem.is_array())
                    {
                        // std::string text = it->get<std::string>();
                        throw std::runtime_error("UNEXPECTED ARRAY ITEM");
                    }


                    else
                    {
                        //std::string arrItemStr = arrItem.get<std::string>();

                        if (throwErrors)
                            throw std::runtime_error( /* "'" */   /* + arrItemStr + */   /* "'" */ "!!!" " is in " + marty::json_utils::nodeTypeName(arrItem) + " format.");

                        // std::string altersListStr = it->get<std::string>();
                        // std::vector<std::string> altersList = splitString(altersListStr, '|');
                        // d.requires.emplace_back(altersList);

                    }

                }



            }


        } // if (j.find("roles")!=j.end())

        return j;
    }


}; // struct Project

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------
~~~

**\_src/almai/ProjectDirs.h**
~~~C/C++ Header
/*! \file
    \brief Информация по проекту - каталоги, порты, кеши
 */
#pragma once

//----------------------------------------------------------------------------
#include "umba/filesys.h"
#include "marty_cdt/utils.h"
#include "marty_cdt/types.h"
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
    std::string              projectPath;
    std::string              almaiDir;
    std::string              projectFile;

    marty::cdt::BrowserDirs  browserDirs;


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

        // browserDirs.userDataDir  = marty::cdt::utils::generateChromeUserProfileFolderForProject(projectPath, browserName, projectSubfolder);
        // browserDirs.cacheDataDir = marty::cdt::utils::generateChromeUserCacheFolderForProject(projectPath, browserName, projectSubfolder);
        browserDirs = marty::cdt::utils::generateChromeDirsForProject(projectPath, browserName, projectSubfolder);

        umba::filesys::createDirectoryEx(browserDirs.userDataDir , true /* forceCreatePath */ );
        umba::filesys::createDirectoryEx(browserDirs.cacheDataDir, true /* forceCreatePath */ );
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
        return marty::cdt::utils::generateArgsForSpawnChromeExactDirs(browserDirs.userDataDir, browserDirs.cacheDataDir, port, restoreLastSession);
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
~~~

**\_src/almai/result.h**
~~~C/C++ Header
/*! \file
    \brief ResultCode testing macros
 */

#pragma once

#include "enums.h"

#define ALMAI_SUCCESS(code)    (((unsigned)(code)) < ((unsigned)almai::ResultCode::error))
#define ALMAI_FAILED(code)     (((unsigned)(code)) >= ((unsigned)almai::ResultCode::error))
~~~

**\_src/almai/Test08ArgParser.h**
~~~C/C++ Header
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
            // LOG_MSG << "Processing options file: " << optFileName << "\n";
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
~~~

**\_src/almai/utils.h**
~~~C/C++ Header
/*! \file
    \brief Утилитарные функции проекта
 */
#pragma once

#include "enums.h"
#include "almai_enums.h"
#include "PluralDatabase.h"
//
#include "umba/umba.h"
#include "umba/parse_utils.h"
#include "umba/filename.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
#include "umba/filename.h"
#include "umba/filesys.h"

#if defined(WIN32) || defined(_WIN32)
    #include "umba/clipboard_win32.h"
#endif

//
#include "marty_cpp/src_normalization.h"
//
#include "marty_yaml_toml_json/marty_yaml_toml_json.h"

//
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// almai::utils::
namespace almai {
namespace utils {

//----------------------------------------------------------------------------







//----------------------------------------------------------------------------
inline
std::string formatFileSize(std::size_t sz, bool useDecimal=true)
{
     std::stringstream oss;
     std::size_t divider = useDecimal ? 1000u : 1024u;

     if (sz < 100u*divider) // меньше 100 килобайт выводим в байтах
     {
         oss << sz /*  << " bytes" */ ;
         return oss.str();
     }

     sz /= divider; // Теперь у нас килобайты

     if (sz < 10u*divider) // меньше 10*1000 килобайт выводим в килобайтах
     {
         oss << sz << " K";
         return oss.str();
     }

     sz /= divider; // Теперь у нас мегабайты

     if (sz < 10u*divider)
     {
         oss << sz << " M";
         return oss.str();
     }

     sz /= divider;

     oss << sz << " G";
     return oss.str();
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::string autoEncodeToUtf(const std::string &text)
{
    size_t bomSize = 0;
    //const charDataPtr =
    encoding::EncodingsApi* pEncodingsApi = encoding::getEncodingsApi();
    std::string detectRes = pEncodingsApi->detect( text.data(), text.size(), bomSize );
    auto cpId = pEncodingsApi->getCodePageByName(detectRes);
    std::string utfText = pEncodingsApi->convert( text.data()+bomSize, text.size()-bomSize, cpId, encoding::EncodingsApi::cpid_UTF8 );
    return utfText;
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> splitTextToLines(const std::string &text)
{
    return marty_cpp::splitToLinesSimple(text, false /*!addEmptyLineAfterLastLf*/);
}

//----------------------------------------------------------------------------
inline
bool readFile(const std::string &inputFilename, std::string &inputFileText)
{
    std::string inputFileTextOrg;

    if (!umba::filesys::readFile(inputFilename, inputFileTextOrg))
    {
        return false;
    }

    inputFileText = autoEncodeToUtf(inputFileTextOrg);
    inputFileText = marty_cpp::normalizeCrLfToLf(inputFileText);

    return true;
}

//----------------------------------------------------------------------------
inline
bool readFile(const std::string &inputFilename, std::vector<std::string> &inputFileLines)
{
    std::string text;
    if (!readFile(inputFilename, text))
         return false;

    inputFileLines = splitTextToLines(text);

    // Убираем мусорные пробелы в концах строк
    for(auto &l : inputFileLines)
        umba::string::rtrim(l);

    return true;
}

//----------------------------------------------------------------------------
inline
std::string normalizePrepromptId(const PluralDatabase &pluralDb, std::string prepromptId)
{
    umba::string::trim(prepromptId);
    umba::string::tolower(prepromptId);

    std::string category, name;
    if (!umba::parse_utils::optionStringSplitToPair(prepromptId, category, name, "/\\"))
    {
        // Нет разделителя
        return prepromptId;
    }

    category = pluralDb.findPlural(category);

    return category + "/" + name;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::vector<std::string> simpleReplaceClipboardMarkerLine(const std::vector<std::string> &lines)
{
    std::vector<std::string> resLines; resLines.reserve(lines.size());

    bool clipboardInserted = false;
    for(const auto &line : lines)
    {
        auto lineLower = umba::string::tolower_copy(umba::string::ltrim_copy(line));
        if (lineLower=="<clipboard>" || lineLower=="<clipboard/>" || lineLower=="<clipbrd>" || lineLower=="<clipbrd/>")
        {
            if (!clipboardInserted)
            {
                clipboardInserted = true;

                #if defined(WIN32) || defined(_WIN32)

                std::wstring clpbText;
                if (umba::win32::clipboardTextGet(clpbText))
                {
                    auto clpbTextUtf = umba::toUtf8(clpbText);
                    auto clpbLines   = marty_cpp::splitToLinesSimple(clpbTextUtf);
                    resLines.insert(resLines.end(), clpbLines.begin(), clpbLines.end());
                }

                #endif

            }
        }

        else if (lineLower=="<clipboard-code>" || lineLower=="<clipboard-code/>" || lineLower=="<clipbrd-code>" || lineLower=="<clipbrd-code/>")
        {
            if (!clipboardInserted)
            {
                clipboardInserted = true;

                #if defined(WIN32) || defined(_WIN32)

                std::wstring clpbText;
                if (umba::win32::clipboardTextGet(clpbText))
                {
                    auto clpbTextUtf = umba::toUtf8(clpbText);
                    auto clpbLines   = marty_cpp::splitToLinesSimple(clpbTextUtf);
                    resLines.push_back("~~~Text");
                    resLines.insert(resLines.end(), clpbLines.begin(), clpbLines.end());
                    resLines.push_back("~~~");
                }

                #endif

            }
        }

        else
        {
            resLines.push_back(line);
        }
    }

    return resLines;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::vector<std::string> makeAlmaiFolderNames()
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
inline
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

//--------------------------------------------------------------------------------------------------------------------
inline
bool isPathExistOneOf(const std::string &basePath, const std::unordered_set<std::string> &relNames)
{
    return isPathExistOneOf(basePath, std::vector<std::string>(relNames.begin(), relNames.end()));
}

//--------------------------------------------------------------------------------------------------------------------
inline
const std::vector<std::string>& getAlmaiFolderNames()
{
    static std::vector<std::string> names = makeAlmaiFolderNames();
    return names;
}

//--------------------------------------------------------------------------------------------------------------------
inline
std::vector<std::string> getAlmaiFolderFullNames(const std::string &path, const std::vector<std::string> &almaiNames=getAlmaiFolderNames())
{
    //const auto &almaiNames = getAlmaiFolderNames();

    std::vector<std::string> resVec; resVec.reserve(almaiNames.size());

    for(const auto &name : almaiNames)
    {
        resVec.push_back(umba::filename::appendPath(path, name));
    }

    return resVec;
}

//--------------------------------------------------------------------------------------------------------------------
inline
bool isProjectRootPath( const std::string &path
                      , std::string *pAlmaiFolderName
                      , const std::unordered_set<std::string> &projectRootStopNames = { ".git", ".vscode", ".build", "build", ".out", "out" }
                      )
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
inline
bool findProjectRoot( std::string startPath
                    , std::string *pFoundProjectPath=0
                    , std::string *pFoundAlmaiDir=0
                    , std::string *pProjectFile=0
                    , const std::unordered_set<std::string> &projectRootStopNames = { ".git", ".vscode", ".build", "build", ".out", "out" }
                    )
{
    std::string projectFile;
    std::string almaiDir;

    auto upLevelPath = startPath;
    auto curPath     = startPath;
    do
    {
        curPath = upLevelPath;

        if (isProjectRootPath(curPath, &almaiDir, projectRootStopNames))
        {
            projectFile = umba::filename::appendPath(almaiDir, std::string("PROJECT.yaml"));
            if (!umba::filesys::isFileExist(projectFile))
                projectFile.clear();

            if (pProjectFile)
               *pProjectFile = projectFile;

            if (pFoundProjectPath)
               *pFoundProjectPath = curPath;

            if (pFoundAlmaiDir)
               *pFoundAlmaiDir = almaiDir;

            return true;
        }

        upLevelPath = umba::filename::getPath(curPath);
    }
    while(upLevelPath!=curPath);

    return false;
}



//----------------------------------------------------------------------------

} // namespace utils
} // namespace almai

// almai::utils::

//----------------------------------------------------------------------------
~~~

**\_src/almai/yaml\_json.h**
~~~C/C++ Header
/*! \file
    \brief Утилитарные функции для YAML/JSON
 */

#pragma once
//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
#include "marty_yaml_toml_json/marty_yaml_toml_json.h"


//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
namespace almai {
namespace yaml_json {

//--------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
marty::json parseToJson(const std::string &text)
{
    std::string errMsg;
    marty::json_utils::FileFormat detectedFormat = marty::json_utils::FileFormat::unknown;

    auto res = marty::json_utils::parseJsonOrYaml( text
                                                 , true // allowComments
                                                 , &errMsg
                                                 , 0 // pTmpJson
                                                 , &detectedFormat
                                                 );
    if (detectedFormat==marty::json_utils::FileFormat::unknown)
    {
        if (errMsg.empty())
            errMsg = "unknown error found while parsing data";
    }

    if (detectedFormat==marty::json_utils::FileFormat::unknown || !errMsg.empty())
    {
        throw std::runtime_error(errMsg);
    }

    return res;
}

//----------------------------------------------------------------------------
inline
marty::json parseToJson(const std::vector<std::string> &lines)
{
    return parseToJson(umba::string::merge<std::string>(lines.begin(), lines.end(), '\n'));
}

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
/*

roles:
  - uni-tester:
    - - # Сбрасываем все предыдущие скилы для роли (а это будет валидно в yml?)
    - Skill/auto-tester
    - skills/manual-tester
  - super-architect:
    - sw-architect # Архитектор ПО
    - sys-architect # Архитектор всей системы
  # Можно записать и как object, но массив гарантирует порядок элементов, а object - не факт. Впрочем, другой нефакт в том, что при настройке ролей нам порядок не особо и важен
  - c-cpp-dev: -, skills/cpp-dev; skills/c-dev, -skills/jni-master # тут в одну строку сбросили все предыдущие скилы для роли, и добавили skills/cpp-dev и skills/c-dev
  - c-cpp-no-java-dev: skills/cpp-dev; skills/c-dev, -skills/jni-master # тут только добавили skills/cpp-dev и skills/c-dev
  - py-master: py-fullstack-master

*/

//--------------------------------------------------------------------------------------------------------------------
#if 1
template<typename SetType, typename OptPrepareHandler> inline
bool parseJsonKeyValueArrayOrString( marty::json j
                                   , SetType &valuesSet
                                   , OptPrepareHandler optPrepareHandler
                                   , bool throwErrors
                                   , const std::string &msgValueOnlyString  // "value can be only a string type, role: '" + role + "', value type: " + marty::json_utils::nodeTypeName(arrItem)
                                   , const std::string &msgValueOnlyStringOrArray // "value can be only a string or array type, role: '" + role + "', value type: " + marty::json_utils::nodeTypeName(value)
                                   , const std::string &msgUpdateFailedWithVal // "failed to update role '" + role + "' with skill value '" + skillName + "'"
                                   )
{
    using umba::parse_utils::optionStringUpdateSet;
    using umba::parse_utils::optionStringListUpdateSet;

    // return umba::parse_utils::optionStringListUpdateSet(strList, rolesSet, skillNamePrepareHandler, umba::CaseOption::toLower);

    //key.clear();
    valuesSet.clear();

    // #160
    if (j.is_string())
    {
        std::string strList = j.get<std::string>();

        // if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
        // {
        //     if (throwErrors)
        //         throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
        // }

        if (!optionStringListUpdateSet(strList, valuesSet, optPrepareHandler /* skillNamePrepareHandler */ , umba::CaseOption::keepCase /* toLower */ ))
        {
            if (throwErrors)
                throw std::runtime_error(msgUpdateFailedWithVal + " with value '" + strList + "'");
        }
    }

    else if (j.is_array())
    {
        for (nlohmann::json::iterator it = j.begin(); it!=j.end(); ++it) // array iteraion
        {
            auto arrItem = *it;

            if (arrItem.is_array())
            {
                // Значение из минуса ("-") без кавычек преобразовывается в массив с одним null элементом, только не понятно кем.
                // Это либо yaml-cpp, либо мой движок конвертации в nlohmann json
                if (arrItem.size()==1 && arrItem[0].is_null())
                {
                    std::string strValue = "-"; // arrItem.get<std::string>();
                    //if (!p.updateRoleFromRoleString(role, skillName, skillNamePrepareHandler))
                    if (!optionStringUpdateSet(strValue, valuesSet, optPrepareHandler, umba::CaseOption::keepCase))
                    {
                        if (throwErrors)
                            throw std::runtime_error(msgUpdateFailedWithVal + " with value '" + strValue + "'");
                    }
                }
                else
                {
                    if (throwErrors)
                        throw std::runtime_error(msgValueOnlyString + ", value type: " + marty::json_utils::nodeTypeName(arrItem));
                }
            }

            else if (arrItem.is_string())
            {
                std::string strValue = arrItem.get<std::string>();
                //if (!p.updateRoleFromRoleString(role, skillName, skillNamePrepareHandler))
                if (!optionStringUpdateSet(strValue, valuesSet, optPrepareHandler, umba::CaseOption::keepCase))
                {
                    if (throwErrors)
                        throw std::runtime_error(msgUpdateFailedWithVal + " with value '" + strValue + "'");
                }
            }

            else
            {
                if (throwErrors)
                    throw std::runtime_error(msgValueOnlyString + ", value type: " + marty::json_utils::nodeTypeName(arrItem));
            }

        }
    }

    else
    {
        if (throwErrors)
            throw std::runtime_error(msgValueOnlyStringOrArray + ", value type: " + marty::json_utils::nodeTypeName(j));
    }

    return true;

}
#endif





//--------------------------------------------------------------------------------------------------------------------


} // namespace yaml_json
} // namespace almai

//--------------------------------------------------------------------------------------------------------------------
~~~

**\_src/almai/cli\_opt\_parsers/add\_header\_footer.h**
~~~C/C++ Header
        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("add-header") || opt.isOption("header") || opt.isOption('H')
            || opt.setDescription("Add file as header to generated markdown.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto fileName = argsParser.makeAbsPath(strVal);
            auto filePath = umba::filename::getPath(fileName);

            appConfig.setMacro("HeaderFileName", fileName, false, true);
            appConfig.setMacro("HeaderFilePath", filePath, false, true);

            std::vector<std::string> lines;
            if (!almai::utils::readFile(fileName, lines))
            {
                LOG_ERR << "failed to read header file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            lines = appConfig.parseLinesExtractValues(lines, filePath);
            lines = appConfig.stripEmptyHeadTailLines(lines);
            if (!lines.empty())
                appConfig.headerLines.push_back(std::string());
            appConfig.appendLines(appConfig.headerLines, lines);

            return 0;
        }



        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("add-footer") || opt.isOption("footer") || opt.isOption('F')
            || opt.setDescription("Add file as footer to generated markdown.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto fileName = argsParser.makeAbsPath(strVal);
            auto filePath = umba::filename::getPath(fileName);

            appConfig.setMacro("FooterFileName", fileName, false, true);
            appConfig.setMacro("FooterFilePath", filePath, false, true);

            std::vector<std::string> lines;
            if (!almai::utils::readFile(fileName, lines))
            {
                LOG_ERR << "failed to read footer file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            lines = appConfig.parseLinesExtractValues(lines, filePath);
            lines = appConfig.stripEmptyHeadTailLines(lines);
            if (!lines.empty())
                appConfig.footerLines.push_back(std::string());
            appConfig.appendLines(appConfig.footerLines, lines);

            return 0;
        }



        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("add-hefooter") || opt.isOption("add-fooheader") || opt.isOption("hefooter") || opt.isOption("fooheader") || opt.isOption("prompt")
            || opt.setDescription("Add file as header/footer to generated markdown.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto fileName = argsParser.makeAbsPath(strVal);
            auto filePath = umba::filename::getPath(fileName);

            appConfig.setMacro("HeaderFileName", fileName, false, true);
            appConfig.setMacro("HeaderFilePath", filePath, false, true);
            appConfig.setMacro("FooterFileName", fileName, false, true);
            appConfig.setMacro("FooterFilePath", filePath, false, true);

            std::vector<std::string> lines;
            if (!almai::utils::readFile(fileName, lines))
            {
                LOG_ERR << "failed to read footer file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            std::vector<std::string> header;
            std::vector<std::string> footer;

            appConfig.splitHeaderFooter(lines, header, footer);

            header = appConfig.parseLinesExtractValues(header, filePath);
            footer = appConfig.parseLinesExtractValues(footer, filePath);

// #if defined(WIN32) || defined(_WIN32)
//     #include "umba/clipboard_win32.h"
// #endif
//     #if defined(WIN32) || defined(_WIN32)
//     if (appConfig.useClipboard)
//     {
//         auto allText = appConfig.mergeLines(resLines);
//         if (!umba::win32::clipboardTextSet( allText, [](const std::string &t ) { return umba::fromUtf8(t); } /* fromUtfConverter */ , true /* utf */ , umba::win32::clipboardGetConsoleHwnd()))
//         {
//             LOG_WARN("clipbrd") << "failed to set clipboard text\n";
//         }
//     }
//     #else
//     //if (!clipboardTextSet(text, fromUtfConverter, utfSource))
//     #endif


            if (!header.empty())
                appConfig.headerLines.push_back(std::string());
            appConfig.appendLines(appConfig.headerLines, header);

            if (!footer.empty())
                appConfig.footerLines.push_back(std::string());
            appConfig.appendLines(appConfig.footerLines, footer);

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/add\_plural\_pair.h**
~~~C/C++ Header
        if (   opt.setParam("SINGULAR:PLURAL", umba::command_line::OptionType::optString)
            || opt.isOption("add-plural-pair") || opt.isOption('U')
            || opt.setDescription("Add singular to plural and back dictionary pair.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.pluralDb->addWordForms(strVal))
            {
                LOG_ERR<<"failed to add singular <-> plural mapping (--add-plural-pair).\n";
                return -1;
            }

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/add\_project\_root\_marker.h**
~~~C/C++ Header
        if (   opt.setParam("MARKER[,MARKER...]", umba::command_line::OptionType::optString)
            || opt.isOption("add-project-root-marker") || opt.isOption("project-root-marker")
            || opt.setDescription("Add project root markers.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.addProjectRootMarkers(strVal))
            {
                LOG_ERR<<"failed to add project root markers (--add-project-root-marker).\n";
                return -1;
            }

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/add\_translation.h**
~~~C/C++ Header
        if (   opt.setParam("LANG:KEY:TEXT", umba::command_line::OptionType::optString)
            || opt.isOption("add-translation") || opt.isOption('T')
            || opt.setDescription("Add translation for preprompt strings.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.addLocalization(strVal))
            {
                LOG_ERR<<"failed to add translation for preprompt string (--add-translation).\n";
                return -1;
            }

            // auto optNames = opt.getOptionNamesVector();
            // for(auto optName : optNames)
            //     std::cout << optName << "\n";

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/attach.h**
~~~C/C++ Header
        if (   opt.setParam("NAME", umba::command_line::OptionType::optString)
            || opt.isOption("attach-filename")
            || opt.setDescription("Set attach output file name. If attach output file name not taken, output file name used to generate attach output file name.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto path = argsParser.makeAbsPath(strVal);

            appConfig.attachFileName = path;

            return 0;
        }


        if (   opt.setParam("FORMAT", umba::command_line::OptionType::optString)
            || opt.isOption("attach-format") // || opt.isOption("attach")
            || opt.setDescription("Set attach format. FORMAT can be one of: no, none; md, markdown; zip; 7z.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.attachFormat = enum_deserialize( strVal, almai::AttachFormat::invalid );

            if (appConfig.attachFormat==almai::AttachFormat::invalid)
            {
                LOG_ERR<<"Invalid attach format value: " << strVal << " (--attach-format)\n";
                return -1;
            }

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/basic\_options.h**
~~~C/C++ Header
        if (opt.isOption("quet") || opt.isOption('q') || opt.setDescription("Operate quetly"))  // . Short alias for '--verbose=quet'
        {
            argsParser.quet = true;
            appConfig.quet  = true;
            return 0;
        }

        if (opt.isOption("home") || opt.setDescription("Open homepage"))
        {
            if (argsParser.hasHelpOption) return 0;
            umba::shellapi::openUrl(appHomeUrl);
            return 1;
        }

        if ( opt.isBuiltinsDisableOptionMain  ()
               // || opt.setDescription( dppof + "main distribution options file `" + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::appGlobal   ) + "`"))
               || opt.setDescription( dppof + "main distribution options file"))
        { } // simple skip - обработка уже сделана

        if ( opt.isBuiltinsDisableOptionCustom()
               //|| opt.setDescription( dppof + "custom global options file `"     + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::customGlobal) + "`"))
               || opt.setDescription( dppof + "custom global options file"))
        { } // simple skip - обработка уже сделана

        if ( opt.isBuiltinsDisableOptionUser  ()
               //|| opt.setDescription( dppof + "user local options file `"        + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::userLocal   ) + "`"))
               || opt.setDescription( dppof + "user local options file"))
        { } // simple skip - обработка уже сделана

        if (opt.isOption("version") || opt.isOption('v') || opt.setDescription("Show version number"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                umba::cli_tool_helpers::printOnlyVersion(std::cout);
                return 1;
            }
        }

        if (opt.isOption("version-info") || opt.setDescription("Show version info - app name, version, host platform, build date and time"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                if (argsParser.getPrintHelpStyle()!=umba::command_line::PrintHelpStyle::md)
                {
                    umba::cli_tool_helpers::printNameVersion(std::cout);
                }
                else
                {
                    umba::cli_tool_helpers::printNameVersion(std::cout);
                }

                return 1;
            }
        }

        if (opt.isOption("builtin-options-info") || opt.setDescription("Show builtin options files location"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                argsParser.printBuiltinFileNames(std::cout);
                return 1;
            }
        }

        if (opt.isOption("where") || opt.setDescription("Show where the executable file is"))
        {
            if (argsParser.hasHelpOption) return 0;

            LOG_MSG << argsParser.programLocationInfo.exeFullName << "\n";
            return 0;
        }

        if (opt.setParam("CLR", 0, "no/none/file|"
                                        "ansi/term|"
                                        #if defined(WIN32) || defined(_WIN32)
                                        "win32/win/windows/cmd/console"
                                        #endif
                             )
              || opt.setInitial(-1) || opt.isOption("color")
              || opt.setDescription("Force set console output coloring")
              /* ", can be:\nno, none, file - disable coloring\nansi, term - set ansi terminal coloring\nwin32, win, windows, cmd, console - windows console specific coloring method" */
              )
        {
            if (argsParser.hasHelpOption) return 0;

            umba::term::ConsoleType res;
            auto mapper = [](int i) -> umba::term::ConsoleType
                          {
                              switch(i)
                              {
                                  case 0 : return umba::term::ConsoleType::file;
                                  case 1 : return umba::term::ConsoleType::ansi_terminal;
                                  case 2 : return umba::term::ConsoleType::windows_console;
                                  default: return umba::term::ConsoleType::file;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            coutWriter.forceSetConsoleType(res);
            cerrWriter.forceSetConsoleType(res);

            return 0;
        }

        if ( opt.isOption("gcc")
               || opt.setDescription("GCC messages format instead of MSVC format")
                )
        {
            if (argsParser.hasHelpOption) return 0;

            umbaLogGccFormat = true;

            return 0;
        }

        if ( opt.isOption("autocomplete-install")
               || opt.setDescription("Install autocompletion to bash"
                                     #if defined(WIN32) || defined(_WIN32)
                                         "/clink(cmd)"
                                     #endif
                                    )
               )
        {
            if (argsParser.hasHelpOption) return 0;

            //return autocomplete(opt, true);
            return umba::command_line::autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), true, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR : LOG_MSG; } );
        }

        if ( opt.isOption("autocomplete-uninstall")
               || opt.setDescription("Remove autocompletion from bash"
                                     #if defined(WIN32) || defined(_WIN32)
                                         "/clink(cmd)"
                                     #endif
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            //return autocomplete(opt, false);
            return umba::command_line::autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), false, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR : LOG_MSG; } );
        }
~~~

**\_src/almai/cli\_opt\_parsers/cdt-eval-opts.h**
~~~C/C++ Header
        if ( opt.setParam("?RETURN_BY_VALUE",true)
          || opt.isOption("return-by-value")
          || opt.setDescription("Set 'returnByValue' mode (default), or 'objectId' mode otherwise.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalReturnType = boolVal ? marty::cdt::RuntimeEvaluateReturnType::returnByValue : marty::cdt::RuntimeEvaluateReturnType::objectId;
            return 0;
        }


        if ( opt.setParam("?AWAIT_PROMISE",true)
          || opt.isOption("await-promise")
          || opt.setDescription("Set 'awaitPromise' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalAwaitPromise = boolVal ? marty::cdt::RuntimeEvaluateAwaitPromise::awaitPromise : marty::cdt::RuntimeEvaluateAwaitPromise::dontAwaitPromise;
            return 0;
        }


        if ( opt.setParam("?USER_GESTURE",true)
          || opt.isOption("user-gesture")
          || opt.setDescription("Set 'userGesture' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalUserGesture = boolVal ? marty::cdt::RuntimeEvaluateUserGesture::userGesture : marty::cdt::RuntimeEvaluateUserGesture::noUserGesture;
            return 0;
        }


        if ( opt.setParam("?THROW_ON_SIDE_EFFECT",true)
          || opt.isOption("throw-on-side-effect")
          || opt.setDescription("Set 'throwOnSideEffect' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalThrowOnSideEffect = boolVal ? marty::cdt::RuntimeEvaluateThrowOnSideEffect::throwOnSideEffect : marty::cdt::RuntimeEvaluateThrowOnSideEffect::noThrowOnSideEffect;
            return 0;
        }


        if ( opt.setParam("?DISABLE_BREAKS",true)
          || opt.isOption("disable-breaks")
          || opt.setDescription("Set 'disableBreaks' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalBreaksControl = boolVal ? marty::cdt::RuntimeEvaluateBreaksControl::disableBreaks : marty::cdt::RuntimeEvaluateBreaksControl::enableBreaks;
            return 0;
        }


        if ( opt.setParam("?REPL_MODE",true)
          || opt.isOption("repl-mode")
          || opt.setDescription("Set 'replMode' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalReplMode = boolVal ? marty::cdt::RuntimeEvaluateReplMode::replMode : marty::cdt::RuntimeEvaluateReplMode::noReplMode;
            return 0;
        }


        if ( opt.setParam("?CSP_MODE",true)
          || opt.isOption("csp-mode")
          || opt.setDescription("Set 'allowUnsafeEvalBlockedByCSP' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalCspMode = boolVal ? marty::cdt::RuntimeEvaluateCspMode::allowUnsafeEvalBlockedByCSP : marty::cdt::RuntimeEvaluateCspMode::useCsp;
            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/cdt-opts.h**
~~~C/C++ Header
        if ( opt.setParam("?MODE",true)
          || opt.isOption("wait-full-completion")
          || opt.setDescription("Wait full loading completion on 'Page.domContentEventFired' and 'Page.loadEventFired' events")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.waitFullCompletion = boolVal;
            return 0;
        }


        if ( opt.setParam("?MODE",true)
          || opt.isOption("no-wait-full-completion")
          || opt.setDescription("Disable waiting full loading completion on 'Page.domContentEventFired' and 'Page.loadEventFired' events")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.waitFullCompletion = !boolVal;
            return 0;
        }


        if ( opt.setParam("?MODE",true)
          || opt.isOption("restore-last-session")
          || opt.setDescription("Restore last browser session")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.restoreLastSession = boolVal;
            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/clipboard.h**
~~~C/C++ Header
        if ( opt.setParam("?MODE",true)
          || opt.isOption("clipboard") || opt.isOption('B')
          // || opt.setParam("VAL",true)
          || opt.setDescription("Use clipboard.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.useClipboard = boolVal;
            return 0;
        }


        if (   opt.setParam("FILENAME", umba::command_line::OptionType::optString)
            || opt.isOption("save-clipboard")
            || opt.setDescription("Save processed clipboard data to FILENAME file.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            //appConfig.saveClipboard = strVal;
            appConfig.saveClipboard = argsParser.makeAbsPath(strVal);

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/context-id.h**
~~~C/C++ Header
        if (   opt.setParam("CONTEXT_ID", umba::command_line::OptionType::optString)
            || opt.isOption("context-id")
            || opt.setDescription("Set context ID.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.evalContextId = strVal;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/description\_filename.h**
~~~C/C++ Header
        if (   opt.setParam("NAME", umba::command_line::OptionType::optString)
            || opt.isOption("description-filename")
            || opt.setDescription("Set filename to save description, if taken.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.descriptionFilename = strVal;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/dict.h**
~~~C/C++ Header
        if (   opt.setParam("EXT:LANGUAGE", umba::command_line::OptionType::optString)
            || opt.isOption("dict") || opt.isOption('d')
            || opt.setDescription("Add language dictionary entry.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.addLangExtention(strVal))
            {
                LOG_ERR << "failed to add extention for language\n";
                return -1;
            }

            return 0;
        }


        if (   opt.setParam("DICT_FILE", umba::command_line::OptionType::optString)
            || opt.isOption("dict-file") || opt.isOption('D')
            || opt.setDescription("Read language dictionary file.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto dictFile = argsParser.makeAbsPath(strVal);

            std::string dictFileText;
            if (!almai::utils::readFile(dictFile, dictFileText))
            {
                LOG_ERR << "failed to read langiage dictionary file: " << dictFile << "\n";
                return -1;
            }

            dictFileText       = marty_cpp::normalizeCrLfToLf(dictFileText);
            auto dictFileLines = marty_cpp::splitToLinesSimple(dictFileText);

            std::size_t lineNum = 0;
            for(const auto &l : dictFileLines)
            {
                lineNum++;

                auto line = l;
                umba::string::trim(line);
                if (line.empty())
                    continue;

                if (line[0]=='#')
                    continue;

                if (!appConfig.addLangExtention(line))
                {
                    curFile = dictFile;
                    lineNo = (unsigned)lineNum;
                    LOG_ERR_INPUT << "failed to add extention for language\n";
                    return -1;
                }

            }


            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/dir-tree-sort.h**
~~~C/C++ Header
        if (   opt.setParam("?CRITERIA", umba::command_line::OptionType::optString)
            || opt.isOption("dir-tree-sort")
            || opt.setDescription("Directory tree sort criteria.\nCRITERIA can be combination of: no, no-sort; asc, ascending; desc, descending; type, by-type, sort-by-type; dirs, dirs-first; asc-dirs, ascending-dirs, asc-dirs-first, ascending-dirs-first; asc-dirs-tType, asc-dirs-by-type, ascending-dirs-type, ascending-dirs-by-type, asc-dirs-first, asc-dirs-first-type, asc-dirs-first-by-type, ascending-dirs-first, ascending-dirs-first-type, ascending-dirs-first-by-type")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.dirTreeSortOrder = almai::FileNameSortOrder::ascending; // default (0) value
            try
            {
                enum_deserialize_flags(appConfig.dirTreeSortOrder, strVal);
            }
            catch(const std::exception &e)
            {
                LOG_ERR << e.what() << "\n";
                return -1;
            }


            ///appConfig.dirTreeSortOrder = enum_deserialize_flags( strVal, almai::FileNameSortOrder::invalid );

            // auto opts = SnifferLogOptions::none;
            // try
            // {
            //     enum_deserialize_flags(opts, strVal);
            // marty_cpp::EnumGeneratorOptionFlagsSerializable tmp;
            // try
            // {
            //     marty_cpp::enum_deserialize_flags(tmp, strVal /*, seps = ",|+"*/);
            //     if (tmp==marty_cpp::EnumGeneratorOptionFlagsSerializable::invalid)
            //     {
            //         //LOG_ERR_OPT << "Invalid options value: '" << strVal << "'\n"; // , --options\n"
            //         throw std::runtime_error("Invalid options value");
            //     }
            // }
            // catch(const std::exception &e)
            // {
            //     LOG_ERR_OPT << e.what() << "\n";
            //     return -1;
            // }


            if (appConfig.dirTreeSortOrder==almai::FileNameSortOrder::invalid)
            {
                LOG_ERR<<"Invalid directory tree comparison flags (sort criteria) value: " << strVal << " (--dir-tree-sort)\n";
                return -1;
            }

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/empty\_option\_error.h**
~~~C/C++ Header
        if (opt.name.empty())
        {
            LOG_ERR<<"invalid (empty) option name\n";
            return -1;
        }
~~~

**\_src/almai/cli\_opt\_parsers/empty\_option\_seal.h**
~~~C/C++ Header
        if (opt.name.empty())
        {
            cmdController.seal();
            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/expression.h**
~~~C/C++ Header
        if (   opt.setParam("EXPRESSION", umba::command_line::OptionType::optString)
            || opt.isOption("expression") || opt.isOption('e')
            || opt.setDescription("Set expression from command line.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.expression = strVal;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/fence\_style.h**
~~~C/C++ Header
        if (opt.setParam("STYLE", 0, "auto|"
                                     "backtick/backticks|"
                                     "tilde/tildes"
                             )
              || opt.setInitial(-1)
              || opt.isOption("fence-style") || opt.isOption('f')
              || opt.setDescription("Code block fence style. Default: auto.")
              )
        {
            almai::FenceStyle res;
            auto mapper = [](int i) -> almai::FenceStyle
                          {
                              switch(i)
                              {
                                  case 0 : return almai::FenceStyle::auto_;
                                  case 1 : return almai::FenceStyle::backtick;
                                  case 2 : return almai::FenceStyle::tilde;
                                  default: return almai::FenceStyle::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.fenceStyle = res;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/file-list.h**
~~~C/C++ Header
        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("file-list")
            || opt.setDescription("Add attach file patterns from file.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto fileName = argsParser.makeAbsPath(strVal);

            std::vector<std::string> lines;
            if (!almai::utils::readFile(fileName, lines))
            {
                LOG_ERR << "failed to read attach file patterns file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            appConfig.addScanPathsFromFileLines(lines, fileName);

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/filename\_decoration.h**
~~~C/C++ Header
        if (opt.setParam("TYPE", 0, "no/none|"
                                    "attr|"
                                    "text|"
                                    "title"
                             )
              || opt.setInitial(-1)
              || opt.isOption("filename-decoration-type") || opt.isOption('R')
              || opt.setDescription("Set filename decoration type.")
              )
        {
            almai::FilenameDecorationType res;
            auto mapper = [](int i) -> almai::FilenameDecorationType
                          {
                              switch(i)
                              {
                                  case 0 : return almai::FilenameDecorationType::none;
                                  case 1 : return almai::FilenameDecorationType::attr;
                                  case 2 : return almai::FilenameDecorationType::text;
                                  case 3 : return almai::FilenameDecorationType::title;
                                  default: return almai::FilenameDecorationType::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.filenameDecorationType = res;

            return 0;
        }


        if (opt.setParam("STYLE", 0, "no/none|"
                                     "bold|"
                                     "underscore-bold/ubold|"
                                     "italic|"
                                     "underscore-italic/uitalic|"
                                     "strikeout|"
                                     "quot|"
                                     "apos|"
                             )
              || opt.setInitial(-1)
              || opt.isOption("filename-decoration-style") || opt.isOption('S')
              || opt.setDescription("Set filename decoration style for `--filename-decoration-type=text` option.")
              )
        {
            almai::FilenameDecorationStyle res;
            auto mapper = [](int i) -> almai::FilenameDecorationStyle
                          {
                              switch(i)
                              {
                                  case 0 : return almai::FilenameDecorationStyle::none;
                                  case 1 : return almai::FilenameDecorationStyle::bold;
                                  case 2 : return almai::FilenameDecorationStyle::underscoreBold;
                                  case 3 : return almai::FilenameDecorationStyle::italic;
                                  case 4 : return almai::FilenameDecorationStyle::underscoreItalic;
                                  case 5 : return almai::FilenameDecorationStyle::strikeout;
                                  case 6 : return almai::FilenameDecorationStyle::quot;
                                  case 7 : return almai::FilenameDecorationStyle::apos;
                                  default: return almai::FilenameDecorationStyle::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.filenameDecorationStyle = res;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/filename\_title\_level.h**
~~~C/C++ Header
        if ( opt.setParam("?LEVEL", 6 /* defVal */, 1 /* minVal */ , 6 /* maxVal */ )
          || opt.isOption("filename-title-level") || opt.isOption('T')
          || opt.setDescription("Set title level for `--filename-decoration-type=title` option.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(intVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.filenameTitleLevel = std::size_t(intVal);

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/join\_lines\_limit.h**
~~~C/C++ Header
        if ( opt.setParam("LIMIT", -1 /* defVal */, 30 /* minVal */ , 0x7FFFFFFF /* maxVal */ )
          || opt.isOption("join-lines-limit") || opt.isOption('J')
          || opt.setDescription("Set limit for lines from each file to join.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(intVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.joinLinesLimit = std::size_t(intVal);

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/keep\_filename\_comment.h**
~~~C/C++ Header
        if ( opt.setParam("?MODE",true)
          || opt.isOption("keep-filename-comment")
          // || opt.setParam("VAL",true)
          || opt.setDescription("Keep filename comment (add it to front of filename).")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.keepFilenameComment = boolVal;
            return 0;
        }

        if (  /* opt.setParam("?MODE",true)
          ||  */ opt.isOption("no-keep-filename-comment")
          // || opt.setParam("VAL",true)
          || opt.setDescription("Don't keep filename comment (remove it from filename at all).")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            // if (!opt.getParamValue(boolVal,errMsg))
            // {
            //     LOG_ERR << errMsg << "\n";
            //     return -1;
            // }

            appConfig.keepFilenameComment = false;
            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/lang\_marker.h**
~~~C/C++ Header
        if (opt.setParam("TYPE", 0, "no/none|"
                                    "lang/lang-name|"
                                    "ext/file-ext"

//     unknown    = (std::uint32_t)(-1) /*!<  */,
//     none       = 0x0000 /*!<  */,
//     name       = 0x0001 /*!<  */,
//     langName   = 0x0001 /*!<  */,
//     ext        = 0x0002 /*!<  */,
//     fileExt    = 0x0002 /*!<  */

                             )
              || opt.setInitial(-1)
              || opt.isOption("lang-marker") || opt.isOption('M')
              || opt.setDescription("Code block language marker type. Default: lang.")
              )
        {
            almai::CodeLanguageMarker res;
            auto mapper = [](int i) -> almai::CodeLanguageMarker
                          {
                              switch(i)
                              {
                                  case 0 : return almai::CodeLanguageMarker::none;
                                  case 1 : return almai::CodeLanguageMarker::langName;
                                  case 2 : return almai::CodeLanguageMarker::fileExt;
                                  default: return almai::CodeLanguageMarker::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.langMarker = res;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/list.h**
~~~C/C++ Header
        if ( opt.setParam("?NUM_LINES", 5 /* defVal */, 0 /* minVal */ , 32 /* maxVal */ )
          || opt.isOption("list") || opt.isOption('L')
          // || opt.setParam("VAL",true)
          || opt.setDescription("Only list embedded files. NUM_LINES specifies the number of lines of file listing to be pronted")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(intVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.listOnly = true;
            appConfig.listLines = std::size_t(intVal);

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/object-group.h**
~~~C/C++ Header
        if (   opt.setParam("OBJECT_GROUP", umba::command_line::OptionType::optString)
            || opt.isOption("object-group")
            || opt.setDescription("Set object group.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.evalObjectGroup = strVal;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/open-url.h**
~~~C/C++ Header
        if (   opt.setParam("URL", umba::command_line::OptionType::optString)
            || opt.isOption("open-url") || opt.isOption("url")
            || opt.setDescription("Set URL.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.openUrl = strVal;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/output.h**
~~~C/C++ Header
        if (   opt.setParam("NAME", umba::command_line::OptionType::optString)
            || opt.isOption("output") || opt.isOption('o')
            || opt.setDescription("Set output file name. If output file name not taken, STDOUT used.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto path = argsParser.makeAbsPath(strVal);

            appConfig.output = path;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/output-dir.h**
~~~C/C++ Header
        if (   opt.setParam("PATH", umba::command_line::OptionType::optString)
            || opt.isOption("output-dir") || opt.isOption('o')
            || opt.setDescription("Set output root path to save files.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto path = argsParser.makeAbsPath(strVal);

            appConfig.output = path;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/overwrite.h**
~~~C/C++ Header
        if ( opt.setParam("?MODE",true)
          || opt.isOption("overwrite") || opt.isOption('Y')
          // || opt.setParam("VAL",true)
          || opt.setDescription("Allow overwrite existing files.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.overwrite = boolVal;
            return 0;
        }

        if ( opt.setParam("?MODE",true)
          || opt.isOption("no-overwrite") || opt.isOption('N')
          // || opt.setParam("VAL",true)
          || opt.setDescription("Disable overwrite existing files.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.overwrite = !boolVal;
            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/role\_setup.h**
~~~C/C++ Header
        if (   opt.setParam("ROLE:skill[,skill2...]", umba::command_line::OptionType::optString)
            || opt.isOption("role-setup")
            || opt.setDescription("Setup role.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.roleSetupFromCli(strVal))
            {
                LOG_ERR<<"failed to set up role " << opt.getNamesParen() << ".\n";
                return -1;
            }

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/sort.h**
~~~C/C++ Header
        if (   opt.setParam("?CMP[:ORDER]", "name-ext:ascending" /* umba::command_line::OptionType::optString */ )
            || opt.isOption("sort") || opt.isOption('s')
            || opt.setDescription("Set sort criteria.\nCMP can be: none, name, name-ext, full-name/full, ext-name/ext.\nORDER can be: aasc or desc")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            std::string strCriteria;
            std::string strOrder;

            if (umba::string::split_to_pair(strVal, strCriteria, strOrder, ':'))
            {
                auto tmpOrder = enum_deserialize( strOrder, almai::SortOrder::invalid );
                if (tmpOrder==almai::SortOrder::invalid)
                {
                    LOG_ERR<<"Invalid sort order value: "<<strOrder<<" (--sort)\n";
                    return -1;
                }

                appConfig.sortOrder = tmpOrder;
            }

            auto tmpComparisonType = enum_deserialize( strCriteria, almai::ComparisonType::invalid );
            if (tmpComparisonType==almai::ComparisonType::invalid)
            {
                LOG_ERR<<"Invalid comparison type (sort criteria) value: "<<strCriteria<<" (--sort)\n";
                return -1;
            }

            appConfig.sortCriteria = tmpComparisonType;

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/strip\_prefix.h**
~~~C/C++ Header
        if (   opt.setParam("PREFIX", umba::command_line::OptionType::optString)
            || opt.isOption("strip-prefix") || opt.isOption('X')
            || opt.setDescription("Add prefix to strip path prefixes list.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto stripPrefix = argsParser.makeAbsPath(strVal);
            appConfig.stripPrefixes.emplace_back(umba::filename::makeCanonical(stripPrefix));

            return 0;
        }
~~~

**\_src/almai/cli\_opt\_parsers/timeout.h**
~~~C/C++ Header
        if ( opt.setParam("TIMEOUT_MS", 10000 /* defVal */, 0 /* minVal */ , 3600000 /* maxVal */ )
          || opt.isOption("timeout")
          || opt.setDescription("Set timeout")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(szVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalTimeout = (unsigned)szVal;

            return 0;
        }


        if ( opt.setParam("TIMEOUT_MS", 10000 /* defVal */, 0 /* minVal */ , 3600000 /* maxVal */ )
          || opt.isOption("page-load-pause")
          || opt.setDescription("Set pause after page loading (used when '--wait-full-completion=false')")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(szVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.pageLoadPause = (unsigned)szVal;

            return 0;
        }
~~~

**prompts/07\_what\_to\_do.bat**
~~~Batch
@call "%~dp0\setup.bat"

@echo TESTS_ROOT=%TESTS_ROOT%
@echo PROMPTS_DIR=%PROMPTS_DIR%
@echo LIBS=%LIBS%
@echo SRC=%SRC%

almai-md-join %CMN_OPTS% "-o=%~dp0\%~n0.md" "--hefooter=%~dp0\%~n0.txt"


@rem по поводу автоматического преобразования инсерта из буфера в аттач
@rem https://chat.deepseek.com/share/lqyax24jcwwpd6vbwi
~~~

**prompts/07\_what\_to\_do.txt**
~~~txt
!import-var: TESTS_ROOT
!import-var: PROMPTS_DIR
!import-var: LIBS
; same as import-var
!set-var-from-env: SRC
!set-var-from-env: ALMAI_SRC

!scan-path: "$(ALMAI_SRC)/**/*.c*,*.h*"
!scan-path: "$(PROMPTS_DIR)/07_what_to_do.txt"
!scan-path: "$(PROMPTS_DIR)/07_what_to_do.bat"
!scan-path: "$(PROMPTS_DIR)/07_what_to_do_reply.bat"


Я пишу что-то типа системы "агентского" программирования при помощи публичных ИИ-чатов.

## JOIN-утилита

У меня есть препромпт (пример: `07_what_to_do.txt`, запуск сборки промпта - `07_what_to_do.bat`, разбор ответа -
`07_what_to_do_reply.bat`), в нём есть некоторые команды (начинаются с '!'),
в нём находится текст препромпта. Есть также подсистема поиска преднастроенных препромптов, но пока не используется.

Препромпт разделяется строкой '---' на хидер и футер. Хидер идёт в начале промпта, футер, соответственно, в конце.

Можно сканировать каталоги и собирать файлы. Раньше файлы вставлялись в виде "маркдаун-архива" между хидером и футером
прямо в подготовленный промпт, но сейчас я стал делать этот архив отдельным файлом, чтобы отдельно его прикреплять к
запросу (такой архив может быть довольно большим, и не всегда влезал в поле ввода).

Сгенерированный промпт помещается в буфер обмена, чтобы можно было сразу его вставить в поле ввода в ИИ-чате.

"Маркдаун-архив" - это просто .MD файл, где файлы вставлены как блоки кода, а их имя располагается непосредственно перед
блоком кода, оформленное как жирный (`**`) или жырный курсив (`***`).

Предполагается ещё добавить возможность создавать архивы в формате ZIP и 7Z, но пока не хочу тянуть внешние звисимости,
и эта фича не реализована.

Подготовкой данных занимается JOIN-утилита


## SPLIT-утилита

SPLIT-утилита производит разбор ответ ИИ в чате (мы просто копируем текст ответа). Она умеет извлекать текст из буфера обмена
или читать его из файла, а также умеет находить файлы "маркдаун-архива" в ответе ИИ. Она умеет извлекать файлы из ответа ИИ и
сохранять их на диск. Также, оставшийся текст (без файлов) сораняется в .MD документ.


## Что дальше?

Я хочу развить эту систему, чтоб с ней можно было реально программировать. Но мне не очень понятно, что мне ещё сделать
и куда двигаться дальше.
~~~

**prompts/07\_what\_to\_do\_reply.bat**
~~~Batch
@rem set PATCH=01
@call "%~dp0\setup.bat"

almai-md-split %SPLIT_OPTS% --overwrite --clipboard "-o=%~dp0\%~n0%PATCH%"
@rem start meld "%~dp0\%~n0" "%~dp0\%~n0%PATCH%"
~~~

