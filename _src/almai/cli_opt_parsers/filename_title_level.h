        if ( opt.setParam("?LEVEL", 6 /* defVal */, 1 /* minVal */ , 6 /* maxVal */ )
          || opt.isOption("filename-title-level") || opt.isOption('T')
          || opt.setDescription("Set title level for `--filename-decoration-type=title` option.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(intVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.filenameTitleLevel = std::size_t(intVal);

            return 0;
        }
