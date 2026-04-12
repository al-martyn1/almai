
#pragma once

#include "enums.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace almai{

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
enum class MdLineType
{
    emptyLine   = 0,
    regularLine    ,
    headerAtx      ,           // # - Atx - word processor on Amiga, min 1 char
    headerSetext   ,           // ---- / ==== Setext (Structure Enhanced Text), min 1 char
    codeTilda      ,
    codeBacktick   ,
    codeIndentTab  ,
    codeIndentSpace,
    unorderedList  ,
    orderedList    ,
    quotation                 // >

}; // enum class MdLineType

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace almai

//----------------------------------------------------------------------------
