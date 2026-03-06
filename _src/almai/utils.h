/*! \file
    \brief Утилитарные функции проекта
 */


#include "umba/umba.h"
//
#include "umba/string_plus.h"
//

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai {

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
bool isValidPathName(const std::string &pathName)
{
    auto pos = pathName.find_first_of(getInvalidPathNameChars());
    return pos==pathName.npos; // Если недопустимых символов не найдено, то это хорош
}

//----------------------------------------------------------------------------
inline
bool isValidPathPart(const std::string &pathName)
{
    auto pos = pathName.find_first_of(getInvalidFileNameChars());
    return pos==pathName.npos; // Если недопустимых символов не найдено, то это хорош
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

    return name;
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
     umba::string::merge<std::string>(pathParts.begin(), pathParts.end(), std::string("/"), [](const auto &s){ return s; });
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
const std::vector<std::string>& getAllowedFilenameEdgings()
{
    static std::vector<std::string> edgings = 
    { "**" // жирный
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
bool stripFilenameEdging(std::string &name) // Возвращает true, если было обрезно обрамление
{
    umba::string::trim(name);

    const auto& edgings = getAllowedFilenameEdgings();

    bool unedged = false;

    for(const auto &e : edgings)
    {
        if (umba::string::starts_with(name, e) && umba::string::ends_with(name, e))
        {
            umba::string::starts_with_and_strip(name, e);
            umba::string::ends_with_and_strip(name, e);

            unedged = true;

            break;
        }
    }

    umba::string::trim(name);

    return unedged;
}

//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------


