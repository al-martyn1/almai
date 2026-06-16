        if (   opt.setParam("NAME", umba::command_line::OptionType::optString)
            || opt.isOption("description-filename")
            || opt.setDescription("Set filename to save description, if taken.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.descriptionFilename = strVal;

            return 0;
        }

