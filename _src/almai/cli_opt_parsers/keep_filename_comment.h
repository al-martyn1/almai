        if ( opt.setParam("?MODE",true)
          || opt.isOption("keep-filename-comment")
          // || opt.setParam("VAL",true)
          || opt.setDescription("Keep filename comment (add it to front of filename).")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.keepFilenameComment = boolVal;
            return 0;
        }

        if (  /* opt.setParam("?MODE",true)
          ||  */ opt.isOption("no-keep-filename-comment")
          // || opt.setParam("VAL",true)
          || opt.setDescription("Don't keep filename comment (remove it from filename at all).")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            // if (!opt.getParamValue(boolVal,errMsg))
            // {
            //     LOG_ERR << errMsg << "\n";
            //     return -1;
            // }

            appConfig.keepFilenameComment = false;
            return 0;
        }

