        if (   opt.setParam("CONTEXT_ID", umba::command_line::OptionType::optString)
            || opt.isOption("context-id")
            || opt.setDescription("Set context ID.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.evalContextId = strVal;

            return 0;
        }
