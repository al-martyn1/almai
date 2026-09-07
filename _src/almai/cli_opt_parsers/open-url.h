        if (   opt.setParam("URL", umba::command_line::OptionType::optString)
            || opt.isOption("open-url") || opt.isOption("url")
            || opt.setDescription("Set URL.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.openUrl = strVal;

            return 0;
        }
