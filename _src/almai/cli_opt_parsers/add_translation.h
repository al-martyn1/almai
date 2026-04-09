        if (   opt.setParam("LANG:KEY:TEXT", umba::command_line::OptionType::optString)
            || opt.isOption("add-translation") || opt.isOption('T')
            || opt.setDescription("Add translation for preprompt strings.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.addLocalization(strVal))
            {
                LOG_ERR<<"failed to add translation for preprompt string (--add-translation).\n";
                return -1;
            }

            // auto optNames = opt.getOptionNamesVector();
            // for(auto optName : optNames)
            //     std::cout << optName << "\n";

            return 0;
        }

