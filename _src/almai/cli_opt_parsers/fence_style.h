        if (opt.setParam("STYLE", 0, "auto|"
                                     "backtick/backticks|"
                                     "tilde/tildes"
                             )
              || opt.setInitial(-1) 
              || opt.isOption("fence-style") || opt.isOption('f')
              || opt.setDescription("Code block fence style. Default: auto.")
              )
        {
            almai::FenceStyle res;
            auto mapper = [](int i) -> almai::FenceStyle
                          {
                              switch(i)
                              {
                                  case 0 : return almai::FenceStyle::auto_;
                                  case 1 : return almai::FenceStyle::backtick;
                                  case 2 : return almai::FenceStyle::tilde;
                                  default: return almai::FenceStyle::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.fenceStyle = res;

            return 0;
        }

