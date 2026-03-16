        if (   opt.setParam("NAME", umba::command_line::OptionType::optString)
            || opt.isOption("output") || opt.isOption('o')
            || opt.setDescription("Set output file name. If output file name not taken, STDOUT used.")
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
