        if ( opt.setParam("?NUM_LINES", 5 /* defVal */, 0 /* minVal */ , 32 /* maxVal */ )
          || opt.isOption("list") || opt.isOption('L')
          // || opt.setParam("VAL",true)
          || opt.setDescription("Only list embedded files. NUM_LINES specifies the number of lines of file listing to be pronted")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(intVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.listOnly = true;
            appConfig.listLines = std::size_t(intVal);

            return 0;
        }
