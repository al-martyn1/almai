        if ( opt.setParam("TIMEOUT_MS", 10000 /* defVal */, 0 /* minVal */ , 3600000 /* maxVal */ )
          || opt.isOption("timeout")
          || opt.setDescription("Set timeout")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(szVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalTimeout = (unsigned)szVal;

            return 0;
        }


