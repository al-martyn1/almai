        if (   opt.setParam("?CRITERIA", umba::command_line::OptionType::optString)
            || opt.isOption("dir-tree-sort")
            || opt.setDescription("Directory tree sort criteria.\nCRITERIA can be combination of: no, no-sort; asc, ascending; desc, descending; type, by-type, sort-by-type; dirs, dirs-first; asc-dirs, ascending-dirs, asc-dirs-first, ascending-dirs-first; asc-dirs-tType, asc-dirs-by-type, ascending-dirs-type, ascending-dirs-by-type, asc-dirs-first, asc-dirs-first-type, asc-dirs-first-by-type, ascending-dirs-first, ascending-dirs-first-type, ascending-dirs-first-by-type")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            appConfig.dirTreeSortOrder = almai::FileNameSortOrder::ascending; // default (0) value
            try
            {
                enum_deserialize_flags(appConfig.dirTreeSortOrder, strVal);
            }
            catch(const std::exception &e)
            {
                LOG_ERR << e.what() << "\n";
                return -1;
            }


            ///appConfig.dirTreeSortOrder = enum_deserialize_flags( strVal, almai::FileNameSortOrder::invalid );

            // auto opts = SnifferLogOptions::none;
            // try
            // {
            //     enum_deserialize_flags(opts, strVal);
            // marty_cpp::EnumGeneratorOptionFlagsSerializable tmp;
            // try
            // {
            //     marty_cpp::enum_deserialize_flags(tmp, strVal /*, seps = ",|+"*/);
            //     if (tmp==marty_cpp::EnumGeneratorOptionFlagsSerializable::invalid)
            //     {
            //         //LOG_ERR_OPT << "Invalid options value: '" << strVal << "'\n"; // , --options\n"
            //         throw std::runtime_error("Invalid options value");
            //     }
            // }
            // catch(const std::exception &e)
            // {
            //     LOG_ERR_OPT << e.what() << "\n";
            //     return -1;
            // }


            if (appConfig.dirTreeSortOrder==almai::FileNameSortOrder::invalid)
            {
                LOG_ERR<<"Invalid directory tree comparison flags (sort criteria) value: " << strVal << " (--dir-tree-sort)\n";
                return -1;
            }

            return 0;
        }

