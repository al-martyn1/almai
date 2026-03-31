/*! @file
    @brief Types which hold descriptions
 */

#pragma once

#include "Descriptions.h"
#include "MdXml.h"
//

#include "marty_cpp/src_normalization.h"
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
    std::string    file; // full file name
    bool           bExtended = false;


}; // struct PrepromptProps

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct Preprompt
{
    std::string            type;  // Optional type string

    PrepromptDescription   description;
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


