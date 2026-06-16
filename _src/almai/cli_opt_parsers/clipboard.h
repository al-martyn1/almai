        if ( opt.setParam("?MODE",true)
          || opt.isOption("clipboard") || opt.isOption('B')
          // || opt.setParam("VAL",true)
          || opt.setDescription("Use clipboard.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.useClipboard = boolVal;
            return 0;
        }


        if (   opt.setParam("FILENAME", umba::command_line::OptionType::optString)
            || opt.isOption("save-clipboard")
            || opt.setDescription("Save processed clipboard data to FILENAME file.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            //appConfig.saveClipboard = strVal;
            appConfig.saveClipboard = argsParser.makeAbsPath(strVal);

            return 0;
        }

