        if (   opt.setParam("PREFIX", umba::command_line::OptionType::optString)
            || opt.isOption("strip-prefix") || opt.isOption('X')
            || opt.setDescription("Add prefix to strip path prefixes list.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto stripPrefix = argsParser.makeAbsPath(strVal);
            appConfig.stripPrefixes.emlace_back(umba::filename::makeCanonical(stripPrefix))

            return 0;
        }

