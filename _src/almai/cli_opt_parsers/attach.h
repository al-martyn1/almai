        if (   opt.setParam("NAME", umba::command_line::OptionType::optString)
            || opt.isOption("attach-filename")
            || opt.setDescription("Set attach output file name. If attach output file name not taken, output file name used to generate attach output file name.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            auto path = argsParser.makeAbsPath(strVal);

            appConfig.attachFileName = path;

            return 0;
        }


        if (   opt.setParam("FORMAT", umba::command_line::OptionType::optString)
            || opt.isOption("attach-format") // || opt.isOption("attach")
            || opt.setDescription("Set attach format. FORMAT can be one of: no, none; md, markdown; zip; 7z.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.attachFormat = enum_deserialize( strVal, almai::AttachFormat::invalid );

            if (appConfig.attachFormat==almai::AttachFormat::invalid)
            {
                LOG_ERR<<"Invalid attach format value: " << strVal << " (--attach-format)\n";
                return -1;
            }

            return 0;
        }


