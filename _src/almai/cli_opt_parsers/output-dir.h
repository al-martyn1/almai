        if (   opt.setParam("PATH", umba::command_line::OptionType::optString)
            || opt.isOption("output-dir") || opt.isOption('o')
            || opt.setDescription("Set output root path to save files.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto path = argsParser.makeAbsPath(strVal);

            appConfig.output = path;

            return 0;
        }
