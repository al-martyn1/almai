        if ( opt.setParam("LIMIT", -1 /* defVal */, 30 /* minVal */ , 0x7FFFFFFF /* maxVal */ )
          || opt.isOption("join-lines-limit") || opt.isOption('J')
          || opt.setDescription("Set limit for lines from each file to join.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(intVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.joinLinesLimit = std::size_t(intVal);

            return 0;
        }
