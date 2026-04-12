        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("add-header") || opt.isOption("header") || opt.isOption('H')
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
            if (!almai::utils::readFile(fileName, lines))
            {
                LOG_ERR << "failed to read header file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            lines = appConfig.stripEmptyHeadTailLines(lines);
            if (!lines.empty())
                appConfig.headerLines.push_back(std::string());
            appConfig.appendLines(appConfig.headerLines, lines);

            return 0;
        }



        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("add-footer") || opt.isOption("footer") || opt.isOption('F')
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
            if (!almai::utils::readFile(fileName, lines))
            {
                LOG_ERR << "failed to read footer file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            lines = appConfig.stripEmptyHeadTailLines(lines);
            if (!lines.empty())
                appConfig.footerLines.push_back(std::string());
            appConfig.appendLines(appConfig.footerLines, lines);

            return 0;
        }



        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("add-hefooter") || opt.isOption("add-fooheader") || opt.isOption("hefooter") || opt.isOption("fooheader")
            || opt.setDescription("Add file as header/footer to generated markdown.")
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
            if (!almai::utils::readFile(fileName, lines))
            {
                LOG_ERR << "failed to read footer file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            std::vector<std::string> header;
            std::vector<std::string> footer;

            appConfig.splitHeaderFooter(lines, header, footer);

            if (!header.empty())
                appConfig.headerLines.push_back(std::string());
            appConfig.appendLines(appConfig.headerLines, header);

            if (!footer.empty())
                appConfig.footerLines.push_back(std::string());
            appConfig.appendLines(appConfig.footerLines, footer);

            return 0;
        }

