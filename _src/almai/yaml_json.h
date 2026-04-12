/*! \file
    \brief Утилитарные функции для YAML/JSON
 */

#pragma once
//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
#include "marty_yaml_toml_json/marty_yaml_toml_json.h"


//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
namespace almai {
namespace yaml_json {

//--------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
marty::json parseToJson(const std::string &text)
{
    std::string errMsg;
    marty::json_utils::FileFormat detectedFormat = marty::json_utils::FileFormat::unknown;

    auto res = marty::json_utils::parseJsonOrYaml( text
                                                 , true // allowComments
                                                 , &errMsg
                                                 , 0 // pTmpJson
                                                 , &detectedFormat
                                                 );
    if (detectedFormat==marty::json_utils::FileFormat::unknown)
    {
        if (errMsg.empty())
            errMsg = "unknown error found while parsing data";
    }

    if (detectedFormat==marty::json_utils::FileFormat::unknown || !errMsg.empty())
    {
        throw std::runtime_error(errMsg);
    }

    return res;
}

//----------------------------------------------------------------------------
inline
marty::json parseToJson(const std::vector<std::string> &lines)
{
    return parseToJson(umba::string::merge<std::string>(lines.begin(), lines.end(), '\n'));
}

//--------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------
/*

roles:
  - uni-tester:
    - - # Сбрасываем все предыдущие скилы для роли (а это будет валидно в yml?)
    - Skill/auto-tester
    - skills/manual-tester
  - super-architect:
    - sw-architect # Архитектор ПО
    - sys-architect # Архитектор всей системы
  # Можно записать и как object, но массив гарантирует порядок элементов, а object - не факт. Впрочем, другой нефакт в том, что при настройке ролей нам порядок не особо и важен
  - c-cpp-dev: -, skills/cpp-dev; skills/c-dev, -skills/jni-master # тут в одну строку сбросили все предыдущие скилы для роли, и добавили skills/cpp-dev и skills/c-dev
  - c-cpp-no-java-dev: skills/cpp-dev; skills/c-dev, -skills/jni-master # тут только добавили skills/cpp-dev и skills/c-dev
  - py-master: py-fullstack-master

*/

//--------------------------------------------------------------------------------------------------------------------
#if 1
template<typename SetType, typename OptPrepareHandler> inline
bool parseJsonKeyValueArrayOrString( marty::json j
                                   , SetType &valuesSet
                                   , OptPrepareHandler optPrepareHandler
                                   , bool throwErrors
                                   , const std::string &msgValueOnlyString  // "value can be only a string type, role: '" + role + "', value type: " + marty::json_utils::nodeTypeName(arrItem)
                                   , const std::string &msgValueOnlyStringOrArray // "value can be only a string or array type, role: '" + role + "', value type: " + marty::json_utils::nodeTypeName(value)
                                   , const std::string &msgUpdateFailedWithVal // "failed to update role '" + role + "' with skill value '" + skillName + "'"
                                   )
{
    using umba::parse_utils::optionStringUpdateSet;
    using umba::parse_utils::optionStringListUpdateSet;

    // return umba::parse_utils::optionStringListUpdateSet(strList, rolesSet, skillNamePrepareHandler, umba::CaseOption::toLower);

    //key.clear();
    valuesSet.clear();

    // #160
    if (j.is_string())
    {
        std::string strList = j.get<std::string>();
    
        // if (!p.updateRoleFromRoleStringList(role, skillsList, skillNamePrepareHandler))
        // {
        //     if (throwErrors)
        //         throw std::runtime_error("failed to update role '" + role + "' with skills: '" + skillsList + "'");
        // }

        if (!optionStringListUpdateSet(strList, valuesSet, optPrepareHandler /* skillNamePrepareHandler */ , umba::CaseOption::keepCase /* toLower */ ))
        {
            if (throwErrors)
                throw std::runtime_error(msgUpdateFailedWithVal + " with value '" + strList + "'");
        }
    }
    
    else if (j.is_array())
    {
        for (nlohmann::json::iterator it = j.begin(); it!=j.end(); ++it) // array iteraion
        {
            auto arrItem = *it;
    
            if (arrItem.is_array())
            {
                // Значение из минуса ("-") без кавычек преобразовывается в массив с одним null элементом, только не понятно кем.
                // Это либо yaml-cpp, либо мой движок конвертации в nlohmann json
                if (arrItem.size()==1 && arrItem[0].is_null()) 
                {
                    std::string strValue = "-"; // arrItem.get<std::string>();
                    //if (!p.updateRoleFromRoleString(role, skillName, skillNamePrepareHandler))
                    if (!optionStringUpdateSet(strValue, valuesSet, optPrepareHandler, umba::CaseOption::keepCase))
                    {
                        if (throwErrors)
                            throw std::runtime_error(msgUpdateFailedWithVal + " with value '" + strValue + "'");
                    }
                }
                else
                {
                    if (throwErrors)
                        throw std::runtime_error(msgValueOnlyString + ", value type: " + marty::json_utils::nodeTypeName(arrItem));
                }
            }
    
            else if (arrItem.is_string())
            {
                std::string strValue = arrItem.get<std::string>();
                //if (!p.updateRoleFromRoleString(role, skillName, skillNamePrepareHandler))
                if (!optionStringUpdateSet(strValue, valuesSet, optPrepareHandler, umba::CaseOption::keepCase))
                {
                    if (throwErrors)
                        throw std::runtime_error(msgUpdateFailedWithVal + " with value '" + strValue + "'");
                }
            }
    
            else
            {
                if (throwErrors)
                    throw std::runtime_error(msgValueOnlyString + ", value type: " + marty::json_utils::nodeTypeName(arrItem));
            }
    
        }
    }
    
    else 
    {
        if (throwErrors)
            throw std::runtime_error(msgValueOnlyStringOrArray + ", value type: " + marty::json_utils::nodeTypeName(j));
    }
    
    return true;

}
#endif





//--------------------------------------------------------------------------------------------------------------------


} // namespace yaml_json
} // namespace almai

//--------------------------------------------------------------------------------------------------------------------

