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
std::string stripLeadingPathSeps(const std::string &name)
{
    auto pos = name.find_first_not_of("/");
    if (pos==name.npos) // Ничего нет
        return std::string();

    return std::string(name, pos, name.size()-pos);
}

//----------------------------------------------------------------------------
inline
std::string makeNormalizedRelativePath(std::string name)
{
    name = normalizePathSeps(name);

    // \\?\
    // \\?\UNC\

}




//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------


