        if (   opt.setParam("?CMP[:ORDER]", "name-ext:ascending" /* umba::command_line::OptionType::optString */ )
            || opt.isOption("sort") || opt.isOption('s')
            || opt.setDescription("Set sort criteria.\nCMP can be: none, name, name-ext, full-name/full, ext-name/ext.\nORDER can be: aasc or desc")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR<<errMsg<<"\n";
                return -1;
            }

            std::string strCriteria;
            std::string strOrder;

            if (umba::string::split_to_pair(strVal, strCriteria, strOrder, ':'))
            {
                auto tmpOrder = enum_deserialize( strOrder, almai::SortOrder::invalid );
                if (tmpOrder==almai::SortOrder::invalid)
                {
                    LOG_ERR<<"Invalid sort order value: "<<strOrder<<" (--sort)\n";
                    return -1;
                }
    
                appConfig.sortOrder = tmpOrder;
            }

            auto tmpComparisonType = enum_deserialize( strCriteria, almai::ComparisonType::invalid );
            if (tmpComparisonType==almai::ComparisonType::invalid)
            {
                LOG_ERR<<"Invalid comparison type (sort criteria) value: "<<strCriteria<<" (--sort)\n";
                return -1;
            }

            appConfig.sortCriteria = tmpComparisonType;

            return 0;
        }


