#pragma once

//----------------------------------------------------------------------------
#include "umba/umba.h"
#include "encoding/encoding.h"
//
#include "umba/string.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//

//
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig
{

    std::vector<std::string>                         inputFiles;
    std::string                                      outputDir;
    std::unordered_map<std::string, std::string>     langExtentionDict;

    bool overwrite = false;
    bool listOnly  = false; // list files, but don't save them


    static
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

    static
    bool readInputFile(const std::string &inputFilename, std::string &inputFileText)
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

        umba::string::tolower(langName);

        langExtentionDict[langName] = langExtention;

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

    std::string findLangExtention(std::string &langName)
    {
        umba::string::trim(langName);
        umba::string::tolower(langName);

        std::unordered_map<std::string, std::string> it = langExtentionDict.find(langName);
        if (it==langExtentionDict.end())
            return std::string();

        return it->second;
    }



}; // struct AppConfig

