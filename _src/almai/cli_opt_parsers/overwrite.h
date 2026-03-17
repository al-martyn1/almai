        if ( opt.setParam("?MODE",true)
          || opt.isOption("overwrite") || opt.isOption('Y')
          // || opt.setParam("VAL",true)
          || opt.setDescription("Allow overwrite existing files.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.overwrite = boolVal;
            return 0;
        }

