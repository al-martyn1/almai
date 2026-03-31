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

        umba::NulOstream log;

        // std::string                  path;
        // std::vector<std::string>     includeMaskList;
        // std::vector<std::string>     excludeMaskList;
    
        // bool                         recursive = false;

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
            PrepromptProps pp = { scanForType, "dummy", foundFile, false };
            resVec.emplace_back(pp);
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
    PrepromptProps         props; // Заполняется при считывании
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


