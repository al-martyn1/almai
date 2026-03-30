/*! @file
    @brief 
 */

#pragma once


#include "umba/umba.h"
//
#include "umba/html_base.h"

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
        child.text = almai::stripEmptyLeadingTrailingLines(child.text);
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

            MdLineType mdLineType = detectMarkdownLineType(*b, &mdChar, &numChars);
            if ((mdLineType==MdLineType::codeTilda || mdLineType==MdLineType::codeBacktick) && codeType==mdLineType && numChars==codeMarkerLen)
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

            MdLineType mdLineType = detectMarkdownLineType(*b, &mdChar, &numChars);
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

