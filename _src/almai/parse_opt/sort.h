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
                    LOG_ERR<<"Invalid sort order value: "<<strOrder<<"\n";
                    return -1;
                }
    
                appConfig.sortOrder = tmpOrder;
            }

            auto tmpComparisonType = enum_deserialize( strCriteria, almai::ComparisonType::invalid );
            if (tmpComparisonType==almai::ComparisonType::invalid)
            {
                LOG_ERR<<"Invalid comparison type (sort criteria) value: "<<strCriteria<<"\n";
                return -1;
            }

            appConfig.sortCriteria = tmpComparisonType;

            return 0;
        }



// bool split_to_pair( const StringType str, StringType &first, StringType &second, const typename StringType::value_type &sep, typename StringType::size_type pos = 0 )

// enum class SortOrder : std::uint32_t
// {
//     invalid      = (std::uint32_t)(-1) /*!<  */,
//     unknown      = (std::uint32_t)(-1) /*!<  */,
//     asc          = 0x0000 /*!<  */,
//     ascending    = 0x0000 /*!<  */,
//     desc         = 0x0001 /*!<  */,
//     descending   = 0x0001 /*!<  */

// enum class ComparisonType : std::uint32_t
// {
//     invalid    = (std::uint32_t)(-1) /*!<  */,
//     unknown    = (std::uint32_t)(-1) /*!<  */,
//     none       = 0x0000 /*!<  */,
//     name       = 0x0001 /*!< Compare by name part, ignore extention */,
//     nameExt    = 0x0002 /*!< Compare by name part first, than ext part */,
//     fullName   = 0x0003 /*!< Compare by name and extention as single string */,
//     full       = 0x0003 /*!< Compare by name and extention as single string */,
//     ext        = 0x0004 /*!< Compare extention part first, than name part */,
//     extName    = 0x0004 /*!< Compare extention part first, than name part */
//  
// }; // enum 
// //#!
