        if (   opt.setParam("MARKER[,MARKER...]", umba::command_line::OptionType::optString)
            || opt.isOption("add-project-root-marker") || opt.isOption("project-root-marker")
            || opt.setDescription("Add project root markers.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            if (!appConfig.addProjectRootMarkers(strVal))
            {
                LOG_ERR<<"failed to add project root markers (--add-project-root-marker).\n";
                return -1;
            }

            return 0;
        }

