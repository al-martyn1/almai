        if (   opt.setParam("OBJECT_GROUP", umba::command_line::OptionType::optString)
            || opt.isOption("object-group")
            || opt.setDescription("Set object group.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.evalObjectGroup = strVal;

            return 0;
        }
