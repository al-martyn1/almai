        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("add-header") || opt.isOption('H')
            || opt.setDescription("Add file as header to generated markdown.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto fileName = argsParser.makeAbsPath(strVal);

            std::vector<std::string> lines;
            if (!appConfig.readFile(fileName, lines))
            {
                LOG_ERR << "failed to read header file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            lines = appConfig.stripEmptyTailLines(appConfig.stripEmptyHeadLines(lines));
            appConfig.headerLines.push_back(std::string());
            appConfig.appendLines(appConfig.headerLines, lines);

            return 0;
        }

        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("add-footer") || opt.isOption('F')
            || opt.setDescription("Add file as footer to generated markdown.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto fileName = argsParser.makeAbsPath(strVal);

            std::vector<std::string> lines;
            if (!appConfig.readFile(fileName, lines))
            {
                LOG_ERR << "failed to read footer file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            lines = appConfig.stripEmptyTailLines(appConfig.stripEmptyHeadLines(lines));
            appConfig.footerLines.push_back(std::string());
            appConfig.appendLines(appConfig.footerLines, lines);

            return 0;
        }

