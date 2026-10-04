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


        if (   opt.setParam("NAME[:BASE]", umba::command_line::OptionType::optString)
            || opt.isOption("add-known-engine") || opt.isOption("known-engine") || opt.isOption("add-known-ai-engine") || opt.isOption("known-ai-engine")
            || opt.setDescription("Add known AI engine.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.addKnownEngine(strVal))
            {
                LOG_ERR<<errMsg << "failed to add known AI engine, may be unknown base engine (--add-known-engine)" << "\n";
                return -1;
            }

            return 0;
        }


        if (   opt.setParam("TYPE", umba::command_line::OptionType::optString)
            || opt.isOption("add-preprompt-type") || opt.isOption("preprompt-type")
            || opt.setDescription("Add known preprompt type.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.addPrepromtType(strVal))
            {
                LOG_ERR<<errMsg << "failed to add known preprompt type (--add-preprompt-type)" << "\n";
                return -1;
            }

            return 0;
        }


