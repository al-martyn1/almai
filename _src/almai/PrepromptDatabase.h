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
struct PrepromptDatabase
{

    //--------------------------------------------------------------------------------------------------------------------
    using PrepromptMapType         = std::unordered_map<std::string, Preprompt>;
    using PrepromptPropsMapType    = std::unordered_map<std::string, PrepromptProps>;
    using PrepromptCategorySetType = std::unordered_set<std::string>;

    using PluralDatabaseSharedPtrType = std::shared_ptr<almai::PluralDatabase>;


    //--------------------------------------------------------------------------------------------------------------------
    PluralDatabaseSharedPtrType                                    pluralDb;
    std::vector<std::string>                                       prepromptDirs;
    std::unordered_map< std::string, PrepromptMapType >            preprompts;
    std::unordered_map< std::string, PrepromptPropsMapType >       prepromptProps;
    std::unordered_map< std::string, PrepromptCategorySetType >    prepromptCategories;

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


    // umba::parse_utils::
    //bool optionStringUpdateSet(std::string opt, SetType &s, OptPrepareHandler optPrepareHandler=OptionPrepareDefault(), CaseOption caseOption=CaseOption::toLower, bool defaultAdd=true)
    // bool optionStringInsertToSet(const std::string &opt, SetType &s)


        // bool bGood = false;

    // std::string makeCompletePpIdErrorMsg( const std::string &requestedPrepromptId
    //                                     , const std::string &foundPrepromptId
    //                                     , const PrepromptCategorySetType &ppCategories
    //                                     , bool *pGood=0
    //                                     ) const

    //--------------------------------------------------------------------------------------------------------------------
    const Preprompt* findPreprompt(const std::string &prepromptId, std::string *pPrepromptFullName = 0, PrepromptCategorySetType *ppCategories=0) const
    {
        auto fullPpId = makeCompletePpId(prepromptId, ppCategories);
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
    std::string makeCompletePpId(std::string prepromptId, PrepromptCategorySetType *ppCategories=0) const
    {
        if (ppCategories)
            ppCategories->clear();

        prepromptId = utils::normalizePrepromptId(*pluralDb.get(), prepromptId);

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
                               , std::vector<std::string>       aiEngines
                               , const std::vector<std::string> &ppDirs
                               , std::vector<std::string>       prepromptCategoriesToScan
                               )
    {
        aiEngines.insert(aiEngines.begin(), std::string());

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
        preprompts.clear();
        prepromptProps.clear();
        prepromptCategories.clear();

        scanForPrepromptsProps( pScannedFolders
                              , prepromptProps
                              , prepromptCategories
                              , *pluralDb.get()
                              , aiEngines
                              , prepromptDirs
                              , prepromptCategoriesToScan
                              );

        for(const auto &[ppTypeStr, ppNameMap] : prepromptProps)
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

                preprompts[ppTypeStr][ppName] = preprompt;

            }
        }

    }


}; // struct PrepromptDatabase

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------



} // namespace almai


