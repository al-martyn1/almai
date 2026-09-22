/*! \file
    \brief Базовый конфиг
 */

//----------------------------------------------------------------------------
#pragma once

//----------------------------------------------------------------------------
#include "enums.h"
#include "utils.h"
#include "md_utils.h"
#include "FileSystemScanInfo.h"
#include "FoundFileInfo.h"
#include "Preprompt.h"
#include "PluralDatabase.h"
#include "Localization.h"
#include "Project.h"
#include "PrepromptDatabase.h"

//
#include "umba/umba.h"
#include "encoding/encoding.h"
//
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_cpp.h"
//
#include "umba/string.h"
//
#include "utils.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "umba/macro_helpers.h"
#include "umba/macros.h"


//
#include <algorithm>
#include <iterator>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfigBase
{

    bool                         overwrite         = false;
    bool                         quet              = false;
    bool                         useClipboard      = false;
    std::string                  saveClipboard;

    bool                         listOnly          = false; // list files, but don't save them
    std::size_t                  listLines         = 5;

    std::size_t                  joinLinesLimit    = (std::size_t)-1;

    almai::FileNameSortOrder     dirTreeSortOrder  = almai::FileNameSortOrder::ascDirsType; // asc | dirsFirst | sortByType
    almai::AttachFormat          attachFormat      = almai::AttachFormat::none;
    std::string                  attachFileName;


    //------------------------------
    using ELinefeedType = marty_cpp::ELinefeedType;

    std::string                  output;
    ELinefeedType                outputLinefeedType = ELinefeedType::systemDefault;


    bool isSourcesInline() const
    {
        return attachFormat == almai::AttachFormat::none;
    }


    std::string getAttachName() const
    {
        if (!attachFileName.empty())
            return attachFileName;

        auto name = umba::filename::getName(output);
        auto path = umba::filename::getPath(output);

        if (!name.empty())
            name += "_sources";
        else
            name = "sources_attach";

        if (path.empty())
            path = umba::filesys::getCurrentDirectory();

        switch(attachFormat)
        {
            case almai::AttachFormat::invalid : throw std::runtime_error("saveAttach: invalid attachFormat"); // break;

            case almai::AttachFormat::no : throw std::runtime_error("getAttachName: no attacment"); // break;
            case almai::AttachFormat::md : name = umba::filename::appendExtention(name, std::string("md") ); break;
            case almai::AttachFormat::zip: name = umba::filename::appendExtention(name, std::string("zip")); break;
            case almai::AttachFormat::_7z: name = umba::filename::appendExtention(name, std::string("7z") ); break;
        }

        return umba::filename::appendPath(path, name);
    }

    bool saveAttach(std::string & /* errMsg */ ) const
    {
        std::vector<std::uint8_t> attachData;

        switch(attachFormat)
        {
            case almai::AttachFormat::invalid : throw std::runtime_error("saveAttach: invalid attachFormat"); // break;

            case almai::AttachFormat::no : throw std::runtime_error("saveAttach: no attacment"); // break;

            case almai::AttachFormat::md : 
            {
                std::stringstream oss;
                
                for(const auto &ffi: foundFileInfos)
                {
                    generateMarkdownListing(oss, ffi.displayName, ffi.fileLines);
                }

                auto mdSourcesLines = marty_cpp::splitToLinesSimple(oss.str());
                std::string mdSources = mergeLines(mdSourcesLines);

                const std::uint8_t *pBegin = (const std::uint8_t*)mdSources.data();
                const std::uint8_t *pEnd   = std::next(pBegin, std::ptrdiff_t(mdSources.size()));
                attachData = std::vector<std::uint8_t>(pBegin, pEnd);
                
                break;
            }

            case almai::AttachFormat::zip: throw std::runtime_error("saveAttach: zip attachments not implemented"); break;
            case almai::AttachFormat::_7z: throw std::runtime_error("saveAttach: zip attachments not implemented"); break;
        }

        return umba::filesys::writeFile(getAttachName(), attachData, overwrite);
    }

    //  
    // no         = 0x0000 /*!<  */,
    // md         = 0x0001 /*!<  */,
    // markdown   = 0x0001 /*!<  */,
    // zip        = 0x0002 /*!<  */,
    // _7z        = 0x0003 /*!<  */
    //  

        //std::string mergeLines(const std::vector<std::string> &lines) const

    // if (appConfig.isSourcesInline())
    // {
    //     std::stringstream oss;
    //  
    //     for(auto &ffi: appConfig.foundFileInfos)
    //     {
    //         appConfig.generateMarkdownListing(oss, ffi.displayName, ffi.fileLines);
    //     }
    //  
    //     auto mdLines = marty_cpp::splitToLinesSimple(oss.str());
    //  
    //     resLines.insert(resLines.end(), mdLines.begin(), mdLines.end());
    // }
    // else
    // {
    //     std::string errStr;
    //     if (!appConfig.saveAttach(errStr))
    //     {
    //         LOG_ERR << "failed to write attach file: '" << appConfig.getAttachName() << "': " << errStr << "\n";
    //         return 1;
    //     }
    // }


    void checkUpdateOutputDir()
    {
        if (output.empty())
        {
            output = umba::filesys::getCurrentDirectory();
        }
    
        output = umba::filename::makeAbsPath(output);
    }

    void checkUpdateOutput()
    {
        if (!output.empty())
        {
            // output = umba::filesys::getCurrentDirectory();
            output = umba::filename::makeAbsPath(output);
        }
    }


    static std::vector<std::string> stripEmptyHeadLines(const std::vector<std::string> &lines) { return md::stripEmptyLeadingLines(lines); }
    static std::vector<std::string> stripEmptyTailLines(std::vector<std::string> lines)        { return md::stripEmptyTrailingLines(lines); }
    static std::vector<std::string> stripEmptyHeadTailLines(std::vector<std::string> lines)    { return md::stripEmptyLeadingTrailingLines(lines); }

    static
    void appendLines(std::vector<std::string> &linesAppendTo, const std::vector<std::string> &lines)
    {
        linesAppendTo.insert(linesAppendTo.end(), lines.begin(), lines.end());
    }

    std::string mergeLines(const std::vector<std::string> &lines) const
    {
        bool addTrailingNewLine = true;
        if (lines.empty())
        {
            addTrailingNewLine = false;
        }
        else
        {
            if (lines.back().empty())
                addTrailingNewLine = false;
        }
     
        //std::string text = 
        return marty_cpp::mergeLines(lines, outputLinefeedType,  /* true */ addTrailingNewLine);
    }

    static 
    void splitHeaderFooter(const std::vector<std::string> &hefooter, std::vector<std::string> &header, std::vector<std::string> &footer)
    {
        // header.clear();
        // footer.clear();

        std::vector<std::string> *pResVec = &header;

        for(auto line : hefooter)
        {
            umba::string::rtrim(line);

            bool foundSep = false;

            if (!line.empty())
            {
                std::size_t nChars = md::getNumberOfFirstSameChars(line);
                if (line[0]=='-' && nChars>=3)
                {
                    foundSep = true;
                }
                else if (line[0]==';')
                {
                    continue; // строка закоментированна
                }
            }

            if (foundSep)
            {
                pResVec = &footer;
                continue;
            }

            pResVec->push_back(line);
        }

        header = stripEmptyHeadTailLines(header);
        footer = stripEmptyHeadTailLines(footer);
    }

    // Если передавать абсолютное имя файла, то можно писать куда угодно
    // pFullName - опционально возвращаемое значение, возвращает полное имя файла, куда производилась запись (или пытались)
    bool writeFile(const std::string &filename, const std::string &filedata, std::string *pFullName=0) const
    {
        auto fullName = umba::filename::makeAbsPath(filename, output);
        if (pFullName)
           *pFullName = fullName;

        auto dir = umba::filename::getPath(fullName);
        if (!dir.empty())
            umba::filesys::createDirectoryEx(dir, true /* forceCreatePath */ );

        return umba::filesys::writeFile(fullName, filedata, overwrite);
    }

    bool writeFile(const std::string &filename, const std::vector<std::string> &lines, std::string *pFullName=0, std::size_t *pSizeTotal=0) const
    {
        // bool addTrailingNewLine = true;
        // if (lines.empty())
        // {
        //     addTrailingNewLine = false;
        // }
        // else
        // {
        //     if (lines.back().empty())
        //         addTrailingNewLine = false;
        // }
        //  
        // std::string text = marty_cpp::mergeLines(lines, outputLinefeedType,  /* true */ addTrailingNewLine);

        std::string text = mergeLines(lines);

        if (pSizeTotal)
           *pSizeTotal = text.size();
        
        return writeFile(filename, text, pFullName);
    }


    //------------------------------

    std::unordered_map<std::string, std::string>     langExtDict;
    std::unordered_map<std::string, std::string>     extLangDict;


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


        umba::string::tolower(langExtention);

        extLangDict[langExtention] = langName;


        umba::string::tolower(langName);

        langExtDict[langName] = langExtention;

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

    std::string findLangExtention(std::string langName) const
    {
        umba::string::trim(langName);
        umba::string::tolower(langName);

        std::unordered_map<std::string, std::string>::const_iterator it = langExtDict.find(langName);
        if (it==langExtDict.end())
        {
            return md::replaceInvalidFileNameChars(langName, true /* replaceSpaceAlso */ );
        }

        return it->second;
    }

    std::string findExtentionLang(std::string ext) const
    {
        umba::string::trim(ext);
        umba::string::tolower(ext);

        std::unordered_map<std::string, std::string>::const_iterator it = extLangDict.find(ext);
        if (it==extLangDict.end())
        {
            return ext;
        }

        return it->second;
    }

    //------------------------------



    //------------------------------
    using StringStringMap = umba::macros::StringStringMap<std::string>;
    StringStringMap         macros;

    std::map<std::string, std::string> getSortedMacros() const
    {
        std::map<std::string, std::string> resMap;
        for(const auto& [k, v] : macros)
            resMap[k] = v;
        return resMap;
    }

    std::string substMacros(const std::string &str) const
    {
        using namespace umba::macros;
        return umba::macros::substMacros( str, MacroTextFromMapRef(macros)
                                        , smf_uppercaseNames | smf_KeepUnknownVars | smf_uppercaseNames
                                        );
    }

    // return true if added
    bool setMacro(std::string name, std::string value, bool bSubst, bool allowOverwrite)
    {
        umba::string::trim(name);
        umba::string::trim(value);

        umba::string::toupper(name);

        auto it = macros.find(name);

        if (it!=macros.end())
        {
            if (!allowOverwrite)
                return false;
        }

        if (bSubst)
            value = substMacros(value);

        macros[name] = value;

        return true;
    }

    bool setMacro(std::string nameValue, bool bSubst, bool allowOverwrite)
    {
        std::string name, value;
        if (!umba::string::split_to_pair(nameValue, name, value, '='))
            return false;

        return setMacro(name, value, bSubst, allowOverwrite);
    }

    bool setMacroFromEnv(std::string name, bool bSubst, bool allowOverwrite)
    {
        umba::string::trim(name);

        std::string value;
        umba::env::getVar(name, value);

        return setMacro(name, value, bSubst, allowOverwrite);
    }

    //------------------------------



    //------------------------------
    std::vector<almai::FileSystemScanInfo>    scanInfos;
    std::vector<almai::FoundFileInfo>         foundFileInfos;
    std::vector<std::string>                  stripPrefixes;


    almai::FilenameDecorationType             filenameDecorationType  = almai::FilenameDecorationType::text;
    almai::FilenameDecorationStyle            filenameDecorationStyle = almai::FilenameDecorationStyle::bold;
    std::size_t                               filenameTitleLevel = 6;
    almai::ComparisonType                     sortCriteria = almai::ComparisonType::nameExt;
    almai::SortOrder                          sortOrder    = almai::SortOrder::ascending;
    almai::FenceStyle                         fenceStyle   = almai::FenceStyle::auto_;
    almai::CodeLanguageMarker                 langMarker   = almai::CodeLanguageMarker::langName;

    //------------------------------

    // Extract "name: value" pairs from lines
    template<typename NameValueHandlerType>
    std::vector<std::string> parseLinesExtractValues(const std::vector<std::string> &lines, const std::string &basePath, NameValueHandlerType handler)
    {
        // auto basePath = umba::filename::getPath(linesFromFile);

        std::vector<std::string> resLines;

        for(const auto &l : lines)
        {
            auto lCopy = umba::string::trim_copy(l);

            if (lCopy.empty())
            {
                resLines.push_back(l);
                continue;
            }

            if (lCopy.front()==';')
            {
                continue;
            }

            if (lCopy.front()!='!')
            {
                resLines.push_back(l);
                continue;
            }

            lCopy.erase(0, 1);

            std::string name, value;
            if (umba::string::split_to_pair(lCopy, name, value, ':'))
            {
                umba::string::trim(name);
                umba::string::trim(value);
                value = marty_cpp::unquoteSimpleQuoted(value);
                if (!name.empty() && handler(basePath, name, value))
                    continue;
            }

            resLines.push_back(l);
        }

        return resLines;
    }

    std::vector<std::string> parseLinesExtractValues(const std::vector<std::string> &lines, const std::string &basePath)
    {
        auto handler = [&](const auto &basePath, const auto &name, auto value) -> bool
        {
            auto nameEnum = enum_deserialize( name, almai::PrepromptTextCommands::invalid );

            if (nameEnum==almai::PrepromptTextCommands::invalid)
                return false;

            if (nameEnum==almai::PrepromptTextCommands::setVar)
            {
                return setMacro(value, false /* !bSubst */, true /* allowOverwrite */);
            }

            if (nameEnum==almai::PrepromptTextCommands::importVar)
            {
                return setMacroFromEnv(value, false /* !bSubst */, true /* allowOverwrite */);
            }

            if (nameEnum==almai::PrepromptTextCommands::expandSetVar)
            {
                return setMacro(value, true /* bSubst */, true /* allowOverwrite */);
            }

            if (nameEnum==almai::PrepromptTextCommands::expandImportVar)
            {
                return setMacroFromEnv(value, true /* bSubst */, true /* allowOverwrite */);
            }


            value = substMacros(value);


            if (nameEnum==almai::PrepromptTextCommands::scanPath)
            {
                value = umba::filename::makeAbsPath(value, basePath);
                // return setMacro(name, value, true /* allowOverwrite */);
                scanInfos.emplace_back(almai::FileSystemScanInfo::parse(value));
                return true;
            }

            return false; // Неизвестная команда
        };

        return parseLinesExtractValues(lines, basePath, handler);
    }

    //template<typename ReadFileErrorHandler>
    bool addScanPathsFromFileLines(std::vector<std::string> &fileLines, const std::string linesFromFile /* , ReadFileErrorHandler errHandler */ )
    {
        auto basePath = umba::filename::getPath(linesFromFile);

        for(auto line: fileLines)
        {
            umba::string::ltrim(line);

            if (line.empty())
                continue;

            if (line.front()=='#' || line.front()==';')
                continue;

            auto fullFileName = umba::filename::makeAbsPath(line, basePath);

            scanInfos.emplace_back(almai::FileSystemScanInfo::parse(fullFileName));

        }
        
        return true;
        // appConfig.scanInfos.emplace_back(almai::FileSystemScanInfo::parse(argsParser.makeAbsPath(a)));

        // if (!almai::utils::readFile(ffi.fullName, ffi.fileLines))
        // {
        //     LOG_WARN("read-failed") << "failed to read file: '" << ffi.fullName << "'";
        // }
        // else
        // {
        //     readedFiles.push_back(ffi.fullName);
        //     //std::cerr << "  " << ffi.fullName << "\n";
        // }
    
    }

    std::string makeFilenameTitlePrefix() const
    {
        std::size_t lvl = filenameTitleLevel;
        if (lvl<1) lvl = 1;
        if (lvl>6) lvl = 6;

        return std::string(lvl, '#');
    }

    std::string makeFilenameTitle(const std::string &fileName) const
    {
        return makeFilenameTitlePrefix() + " " + fileName;
    }

    std::string makeLangMarker(const std::string &fileName) const
    {
        auto ext = umba::filename::getExt(fileName);

        if (langMarker==almai::CodeLanguageMarker::langName)
        {
            return findExtentionLang(ext);
        }
        else if (langMarker==almai::CodeLanguageMarker::fileExt)
        {
            return ext;
        }

        return std::string();
    }

    std::string getFilenameTextDecoration() const
    {
        switch(filenameDecorationStyle)
        {
            case almai::FilenameDecorationStyle::quot:             return "\"";
            case almai::FilenameDecorationStyle::apos:             return "\'";
            case almai::FilenameDecorationStyle::bold:             return "**";
            case almai::FilenameDecorationStyle::underscoreBold:   return "__";
            case almai::FilenameDecorationStyle::italic:           return "*" ;
            case almai::FilenameDecorationStyle::underscoreItalic: return "_" ;
            case almai::FilenameDecorationStyle::strikeout:        return "~~";
            case almai::FilenameDecorationStyle::none:             return ""  ;
            case almai::FilenameDecorationStyle::invalid:          return ""  ;
            default: return "";
        }
    }

    std::string makeFilenameTextDecorated(const std::string &fileName_) const
    {
        // Все символы декорирования надо предварить обратным слэшем
        // Все обратные слэши надо заменить на прямой
        std::string fileName;
        for(auto ch: fileName_)
        {
            if ( ch=='\"'
              || ch=='\''
              || ch=='*' 
              || ch=='_' 
              || ch=='*' 
              || ch=='_' 
              || ch=='~' 
               )
            {
                fileName.append(1, '\\');
                fileName.append(1, ch);
                continue;
            }

            if ( ch=='\\')
            {
                fileName.append(1, '/');
                continue;
            }

            fileName.append(1, ch);
        }

        auto decoration = getFilenameTextDecoration();
        return decoration + fileName + decoration;
    }


    void generateMarkdownListing(std::ostream &oss, const std::string &displayFileName, std::vector<std::string> fileLines) const
    {
        //displayFileName = umba::filename::makeCanonical(displayFileName, '/');

        if (filenameDecorationType==almai::FilenameDecorationType::title)
        {
            oss << makeFilenameTitle(displayFileName) << "\n\n";
        }
        else if (filenameDecorationType==almai::FilenameDecorationType::text)
        {
            oss << makeFilenameTextDecorated(displayFileName) << "\n";
        }

        std::string fence = md::generateFence(fenceStyle, fileLines);

        oss << fence << makeLangMarker(displayFileName);

        if (filenameDecorationType==almai::FilenameDecorationType::attr)
        {
            oss << " {filename=\"" << displayFileName << "\"}";
        }

        oss << "\n";

        fileLines = stripEmptyHeadTailLines(fileLines);

        std::size_t fileLinesSize = fileLines.size();
        if (isSetJoinLinesLimit())
        {
            fileLinesSize = joinLinesLimit;
        }

        if (fileLinesSize>fileLines.size())
        {
            fileLinesSize = fileLines.size();
        }

        for(std::size_t i=0; i!=fileLinesSize; ++i)
        {
            auto line = fileLines[i];
            umba::string::rtrim(line);
            oss << line << "\n";
        }

        oss << fence << "\n\n";

    }

    std::string generateMarkdownListing(const std::string &displayFileName, const std::vector<std::string> fileLines) const
    {
        std::stringstream oss;
        generateMarkdownListing(oss, displayFileName, fileLines);
        return oss.str();
    }

    bool isSetJoinLinesLimit() const
    {
        return joinLinesLimit!=0 && joinLinesLimit!=(std::size_t)-1;
    }


    //--------------------------------------------------------------------------------------------------------------------

    using PluralDatabaseSharedPtrType = std::shared_ptr<almai::PluralDatabase>;
    using PrepromptDatabaseMap        = std::unordered_map<std::string, almai::PrepromptDatabase>;


    std::string                       appRoot;
    std::string                       appConfPath;
    std::string                       projectRoot; // устанавливается только через setProjectRoot
    std::string                       almaiDir; // 
    std::string                       projectFile; // Полное имя '.almai/project.yaml', с путём
    std::string                       aiName; // Например, deepeek, qwen - используется для поиска кастомизированных препромптов

    std::unordered_set<std::string>   projectRootStopNames; //  = { ".git", ".out", ".vscode", ".build", "build" }; // пока явно инициализируем
    // almai.yaml

    std::unordered_map<almai::PrepromptPathType, std::vector<std::string> > prepromptDirs;
    almai::PrepromptPathType          curPrepromptPathType = almai::PrepromptPathType::builtinOptions;

    PluralDatabaseSharedPtrType       pluralDb = std::make_shared<almai::PluralDatabase>();
    almai::Localization               localizations;
    std::string                       curLang;

    //std::string                       curAiEngine; // deepseek, qwen
    PrepromptDatabaseMap              ppDBases;

    almai::Project                    almaiProject;



    // UMBA_RULE_OF_FIVE_COPY_MOVE(FoundFileInfo, default, default, default, default);
    // UMBA_RULE_OF_FIVE(FoundFileInfo, default, default, default, default, default);

    //------------------------------
    std::string normalizePrepromptId(const std::string &prepromptId) const { return almai::utils::normalizePrepromptId(*pluralDb.get(), prepromptId); }

    //------------------------------
    bool addProjectRootMarker(std::string marker); // Добавляет маркер остановки поиска корневого каталога проекта
    bool addProjectRootMarkers(const std::vector<std::string> &markersList);
    bool addProjectRootMarkers(const std::string &markersListStr);

    //------------------------------
    bool addLocalization(const std::string &langKeyTextTriplet);
    std::string getLocalizedText(std::string lang, const std::string &key) const;
    std::string getLocalizedText(const std::string &key) const;

    //------------------------------
    static
    std::string getPrepromptPathTypeAnnotation(almai::PrepromptPathType ppt) ;

    std::vector<std::string> getPrepromptDirs() const;
    std::vector<std::pair<almai::PrepromptPathType, std::string> > getPrepromptDirsAnnotated() const;

    void addPrepromptPath(almai::PrepromptPathType ppt, std::string path);
    void addPrepromptPath(const std::string &path);

    //------------------------------
    bool roleSetupFromCli(const std::string &roleSetupStr);

    //------------------------------
    void setAppRoot(const std::string &appRoot_, const std::string &appConfPath_);
    void setProjectRoot(const std::string &projectRoot_);

    //------------------------------
    void addEnvironmentPrepromptPaths();

    //------------------------------
    //! Проверка существования каталога или файла, одного из многих, по заданному пути
    static bool isPathExistOneOf(const std::string &basePath, const std::vector<std::string> &relNames);
    static bool isPathExistOneOf(const std::string &basePath, const std::unordered_set<std::string> &relNames);
    static bool isPathExistOneOf(const std::string &basePath, const std::string &relName);

    //------------------------------
    static std::vector<std::string> makeAlmaiFolderNames();
    static const std::vector<std::string>& getAlmaiFolderNames();
    static std::vector<std::string> getAlmaiFolderFullNames(const std::string &path);

    //------------------------------
    bool isProjectRootPath(const std::string &path, std::string *pAlmaiYamlName) const;
    bool findProjectRoot(std::string startPath=umba::filesys::getCurrentDirectory());

    //------------------------------
    auto makeSkillPrepareHandler() const
    {
        auto skillPrepareHandler = [&](std::string str)
        {
            umba::string::case_convert(str, umba::CaseOption::toLower);
            return normalizePrepromptId(str);
        };

        return skillPrepareHandler;
    }

    //------------------------------
    template<typename PrepromptReadingErrorHandler, typename PrepromptParsingErrorHandler>
    void readProjectFile( PrepromptReadingErrorHandler   readingErrHandler
                        , PrepromptParsingErrorHandler   parsingErrorHandler
                        )
    {
        if (projectFile.empty())
            return;

        std::string projectText;

        if (!almai::utils::readFile(projectFile, projectText))
        {
            readingErrHandler(projectFile);
            return;
        }

        try
        {
            almai::Project::parse( almaiProject, projectText
                                 , makeSkillPrepareHandler()
                                 , true /* throwErrors */
                                 );
        }
        catch(const std::exception &e)
        {
            parsingErrorHandler(projectFile, e);

            // Пробуем игнорировать ошибки
            // Но исключение всё равно может вылететь. Но мы его уже не ловим, пусть летит
            almai::Project::parse( almaiProject, projectText
                                 , makeSkillPrepareHandler()
                                 , false /* !throwErrors */
                                 );
        }

    }

    template<typename ErrorHandler, typename WarningHandler>
    bool projectCheckNormalize( const almai::PrepromptDatabase &ppDb
                              , ErrorHandler   errorHandler
                              , WarningHandler warningHandler
                              )
    {
        return almaiProject.checkNormalize(ppDb, errorHandler, warningHandler);
    }

    //------------------------------



}; // struct AppConfigBase



