#pragma once

//----------------------------------------------------------------------------
#include "umba/umba.h"
//
// #include "AppConfigBase.h"
//
#include "encoding/encoding.h"
//
#include "umba/string.h"
//
#include "umba/filename.h"
#include "umba/filesys.h"
//
#include "marty_cdt/types.h"
//#include "ListingInfo.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct AppConfig
{
    using RuntimeEvaluateReturnType        = marty::cdt::RuntimeEvaluateReturnType       ;
    using RuntimeEvaluateAwaitPromise      = marty::cdt::RuntimeEvaluateAwaitPromise     ;
    using RuntimeEvaluateUserGesture       = marty::cdt::RuntimeEvaluateUserGesture      ;
    using RuntimeEvaluateThrowOnSideEffect = marty::cdt::RuntimeEvaluateThrowOnSideEffect;
    using RuntimeEvaluateBreaksControl     = marty::cdt::RuntimeEvaluateBreaksControl    ;
    using RuntimeEvaluateReplMode          = marty::cdt::RuntimeEvaluateReplMode         ;
    using RuntimeEvaluateCspMode           = marty::cdt::RuntimeEvaluateCspMode          ;


    bool                              overwrite         = false;
    bool                              quet              = false;

    unsigned                          evalTimeout       = 10000; // ms
    unsigned                          pageLoadPause     = 5000; // ms
    bool                              waitFullCompletion = true;

    std::string                       inputFile;
    std::string                       output;

    std::string                       openUrl;

    std::string                       expression;       // либо inputFile, либо expression

    std::string                       evalContextId     = std::string();  // integer as string or empty string
    std::string                       evalObjectGroup   = std::string();  // object group name string         


    RuntimeEvaluateReturnType         evalReturnType        = RuntimeEvaluateReturnType::returnByValue     ;
    RuntimeEvaluateAwaitPromise       evalAwaitPromise      = RuntimeEvaluateAwaitPromise::unspecified     ;
    RuntimeEvaluateUserGesture        evalUserGesture       = RuntimeEvaluateUserGesture::unspecified      ;
    RuntimeEvaluateThrowOnSideEffect  evalThrowOnSideEffect = RuntimeEvaluateThrowOnSideEffect::unspecified;
    RuntimeEvaluateBreaksControl      evalBreaksControl     = RuntimeEvaluateBreaksControl::unspecified    ;
    RuntimeEvaluateReplMode           evalReplMode          = RuntimeEvaluateReplMode::unspecified         ;
    RuntimeEvaluateCspMode            evalCspMode           = RuntimeEvaluateCspMode::unspecified          ;

}; // struct AppConfig

