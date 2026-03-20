        if (opt.setParam("TYPE", 0, "no/none|"
                                    "attr|"
                                    "text|"
                                    "title"
                             )
              || opt.setInitial(-1) 
              || opt.isOption("filename-decoration-type") || opt.isOption('R')
              || opt.setDescription("Set filename decoration type.")
              )
        {
            almai::FilenameDecorationType res;
            auto mapper = [](int i) -> almai::FilenameDecorationType
                          {
                              switch(i)
                              {
                                  case 0 : return almai::FilenameDecorationType::none;
                                  case 1 : return almai::FilenameDecorationType::attr;
                                  case 2 : return almai::FilenameDecorationType::text;
                                  case 3 : return almai::FilenameDecorationType::title;
                                  default: return almai::FilenameDecorationType::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.filenameDecorationType = res;

            return 0;
        }


        if (opt.setParam("STYLE", 0, "no/none|"
                                     "bold|"
                                     "underscore-bold/ubold|"
                                     "italic|"
                                     "underscore-italic/uitalic|"
                                     "strikeout|"
                                     "quot|"
                                     "apos|"
                             )
              || opt.setInitial(-1) 
              || opt.isOption("filename-decoration-style") || opt.isOption('S')
              || opt.setDescription("Set filename decoration style for `--filename-decoration-type=text` option.")
              )
        {
            almai::FilenameDecorationStyle res;
            auto mapper = [](int i) -> almai::FilenameDecorationStyle
                          {
                              switch(i)
                              {
                                  case 0 : return almai::FilenameDecorationStyle::none;
                                  case 1 : return almai::FilenameDecorationStyle::bold;
                                  case 2 : return almai::FilenameDecorationStyle::underscoreBold;
                                  case 3 : return almai::FilenameDecorationStyle::italic;
                                  case 4 : return almai::FilenameDecorationStyle::underscoreItalic;
                                  case 5 : return almai::FilenameDecorationStyle::strikeout;
                                  case 6 : return almai::FilenameDecorationStyle::quot;
                                  case 7 : return almai::FilenameDecorationStyle::apos;
                                  default: return almai::FilenameDecorationStyle::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.filenameDecorationStyle = res;

            return 0;
        }

