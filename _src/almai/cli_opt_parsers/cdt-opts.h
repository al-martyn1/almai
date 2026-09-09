        if ( opt.setParam("?MODE",true)
          || opt.isOption("wait-full-completion")
          || opt.setDescription("Wait full loading completion on 'Page.domContentEventFired' and 'Page.loadEventFired' events")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.waitFullCompletion = boolVal;
            return 0;
        }


        if ( opt.setParam("?MODE",true)
          || opt.isOption("no-wait-full-completion")
          || opt.setDescription("Disable waiting full loading completion on 'Page.domContentEventFired' and 'Page.loadEventFired' events")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.waitFullCompletion = !boolVal;
            return 0;
        }


        if ( opt.setParam("?MODE",true)
          || opt.isOption("restore-last-session")
          || opt.setDescription("Restore last browser session")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.restoreLastSession = boolVal;
            return 0;
        }


