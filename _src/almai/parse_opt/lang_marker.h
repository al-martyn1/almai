        if (opt.setParam("TYPE", 0, "no/none|"
                                    "lang/lang-name|"
                                    "ext/file-ext"

//     unknown    = (std::uint32_t)(-1) /*!<  */,
//     none       = 0x0000 /*!<  */,
//     name       = 0x0001 /*!<  */,
//     langName   = 0x0001 /*!<  */,
//     ext        = 0x0002 /*!<  */,
//     fileExt    = 0x0002 /*!<  */

                             )
              || opt.setInitial(-1) 
              || opt.isOption("lang-marker") || opt.isOption('M')
              || opt.setDescription("Code block language marker type. Default: lang.")
              )
        {
            almai::CodeLanguageMarker res;
            auto mapper = [](int i) -> almai::CodeLanguageMarker
                          {
                              switch(i)
                              {
                                  case 0 : return almai::CodeLanguageMarker::none;
                                  case 1 : return almai::CodeLanguageMarker::langName;
                                  case 2 : return almai::CodeLanguageMarker::fileExt;
                                  default: return almai::FenceStyle::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.langMarker = res;

            return 0;
        }

