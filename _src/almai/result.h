/*! \file
    \brief ResultCode testing macros
 */

#pragma once

#include "enums.h"

#define ALMAI_SUCCESS(code)    (((unsigned)(code)) < ((unsigned)almai::ResultCode::error))
#define ALMAI_FAILED(code)     (((unsigned)(code)) >= ((unsigned)almai::ResultCode::error))

