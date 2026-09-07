        if (   opt.setParam("EXPRESSION", umba::command_line::OptionType::optString)
            || opt.isOption("expression") || opt.isOption('e')
            || opt.setDescription("Set expression from command line.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.expression = strVal;

            return 0;
        }
