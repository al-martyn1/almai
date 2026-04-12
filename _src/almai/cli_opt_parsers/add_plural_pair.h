        if (   opt.setParam("SINGULAR:PLURAL", umba::command_line::OptionType::optString)
            || opt.isOption("add-plural-pair") || opt.isOption('U')
            || opt.setDescription("Add singular to plural and back dictionary pair.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.pluralDb->addWordForms(strVal))
            {
                LOG_ERR<<"failed to add singular <-> plural mapping (--add-plural-pair).\n";
                return -1;
            }

            return 0;
        }

