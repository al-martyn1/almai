/*!
    \file
    \brief Тесты утилитарных функций
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

    return 0;
}


