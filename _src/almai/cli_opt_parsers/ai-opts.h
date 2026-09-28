        if (   opt.setParam("NAME", umba::command_line::OptionType::optString)
            || opt.isOption("engine") || opt.isOption("ai-engine")
            || opt.setDescription("Set AI engine.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.aiName = strVal;

            return 0;
        }


