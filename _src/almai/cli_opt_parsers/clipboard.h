        if ( opt.setParam("?MODE",true)
          || opt.isOption("clipboard") || opt.isOption('B')
          // || opt.setParam("VAL",true)
          || opt.setDescription("Use clipboard.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.useClipboard = boolVal;
            return 0;
        }

