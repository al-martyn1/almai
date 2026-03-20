        if (   opt.setParam("EXT:LANGUAGE", umba::command_line::OptionType::optString)
            || opt.isOption("dict") || opt.isOption('d')
            || opt.setDescription("Add language dictionary entry.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.addLangExtention(strVal))
            {
                LOG_ERR << "failed to add extention for language\n";
                return -1;
            }

            return 0;
        }


        if (   opt.setParam("DICT_FILE", umba::command_line::OptionType::optString)
            || opt.isOption("dict-file") || opt.isOption('D')
            || opt.setDescription("Read language dictionary file.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto dictFile = argsParser.makeAbsPath(strVal);

            std::string dictFileText;
            if (!appConfig.readFile(dictFile, dictFileText))
            {
                LOG_ERR << "failed to read langiage dictionary file: " << dictFile << "\n";
                return -1;
            }

            dictFileText       = marty_cpp::normalizeCrLfToLf(dictFileText);
            auto dictFileLines = marty_cpp::splitToLinesSimple(dictFileText);

            std::size_t lineNum = 0;
            for(const auto &l : dictFileLines)
            {
                lineNum++;

                auto line = l;
                umba::string::trim(line);
                if (line.empty())
                    continue;

                if (line[0]=='#')
                    continue;

                if (!appConfig.addLangExtention(line))
                {
                    curFile = dictFile;
                    lineNo = (unsigned)lineNum;
                    LOG_ERR_INPUT << "failed to add extention for language\n";
                    return -1;
                }

            }


            return 0;
        }
