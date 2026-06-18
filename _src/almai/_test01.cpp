/*! \file
    \brief Тесты утилитарных функций
 */

#include "utils.h"
//

#include <iostream>

using std::cout;
using std::cerr;
using namespace almai;

//----------------------------------------------------------------------------
inline 
void performMakeNormalizedRelativePathTest(const std::string &name)
{
    cout << "  [" << name << "] - [" << almai::utils::makeNormalizedRelativePath(name) << "]\n";
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
    bool hasEdging = almai::utils::findListingFilenames(lastSignificantLines, filenames, true /*bCommentPrependName*/);

    for(auto &name : filenames)
    {
        name = almai::utils::replaceInvalidPathNameChars(name, !hasEdging);
        name = almai::utils::makeNormalizedRelativePath(name);
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
// almai::utils::
// inline
// std::vector<std::string> splitTextToLines(const std::string &text)
// {
//     return marty_cpp::splitToLinesSimple(text);
// }




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

    return 0;
}
