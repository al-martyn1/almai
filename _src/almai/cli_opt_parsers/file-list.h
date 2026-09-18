        if (   opt.setParam("FILE", umba::command_line::OptionType::optString)
            || opt.isOption("file-list")
            || opt.setDescription("Add attach file patterns from file.")
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
                LOG_ERR << "failed to read attach file patterns file: '" << fileName << "'"; // ("read-failed")
                return -1;
            }

            appConfig.addScanPathsFromFileLines(lines, fileName);

            return 0;
        }


