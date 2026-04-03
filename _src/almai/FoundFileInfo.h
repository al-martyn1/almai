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


