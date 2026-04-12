        if (   opt.setParam("ROLE:skill[,skill2...]", umba::command_line::OptionType::optString)
            || opt.isOption("role-setup")
            || opt.setDescription("Setup role.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.roleSetupFromCli(strVal))
            {
                LOG_ERR<<"failed to set up role " << opt.getNamesParen() << ".\n";
                return -1;
            }

            return 0;
        }

