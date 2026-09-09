        if ( opt.setParam("?RETURN_BY_VALUE",true)
          || opt.isOption("return-by-value")
          || opt.setDescription("Set 'returnByValue' mode (default), or 'objectId' mode otherwise.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalReturnType = boolVal ? marty::cdt::RuntimeEvaluateReturnType::returnByValue : marty::cdt::RuntimeEvaluateReturnType::objectId;
            return 0;
        }


        if ( opt.setParam("?AWAIT_PROMISE",true)
          || opt.isOption("await-promise")
          || opt.setDescription("Set 'awaitPromise' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalAwaitPromise = boolVal ? marty::cdt::RuntimeEvaluateAwaitPromise::awaitPromise : marty::cdt::RuntimeEvaluateAwaitPromise::dontAwaitPromise;
            return 0;
        }


        if ( opt.setParam("?USER_GESTURE",true)
          || opt.isOption("user-gesture")
          || opt.setDescription("Set 'userGesture' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalUserGesture = boolVal ? marty::cdt::RuntimeEvaluateUserGesture::userGesture : marty::cdt::RuntimeEvaluateUserGesture::noUserGesture;
            return 0;
        }


        if ( opt.setParam("?THROW_ON_SIDE_EFFECT",true)
          || opt.isOption("throw-on-side-effect")
          || opt.setDescription("Set 'throwOnSideEffect' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalThrowOnSideEffect = boolVal ? marty::cdt::RuntimeEvaluateThrowOnSideEffect::throwOnSideEffect : marty::cdt::RuntimeEvaluateThrowOnSideEffect::noThrowOnSideEffect;
            return 0;
        }


        if ( opt.setParam("?DISABLE_BREAKS",true)
          || opt.isOption("disable-breaks")
          || opt.setDescription("Set 'disableBreaks' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalBreaksControl = boolVal ? marty::cdt::RuntimeEvaluateBreaksControl::disableBreaks : marty::cdt::RuntimeEvaluateBreaksControl::enableBreaks;
            return 0;
        }


        if ( opt.setParam("?REPL_MODE",true)
          || opt.isOption("repl-mode")
          || opt.setDescription("Set 'replMode' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalReplMode = boolVal ? marty::cdt::RuntimeEvaluateReplMode::replMode : marty::cdt::RuntimeEvaluateReplMode::noReplMode;
            return 0;
        }


        if ( opt.setParam("?CSP_MODE",true)
          || opt.isOption("csp-mode")
          || opt.setDescription("Set 'allowUnsafeEvalBlockedByCSP' option on/off, default - unspecified.")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.evalCspMode = boolVal ? marty::cdt::RuntimeEvaluateCspMode::allowUnsafeEvalBlockedByCSP : marty::cdt::RuntimeEvaluateCspMode::useCsp;
            return 0;
        }


        if ( opt.setParam("?MODE",true)
          || opt.isOption("wait-full-completion")
          || opt.setDescription("Wait full loading completion on 'Page.domContentEventFired' and 'Page.loadEventFired' events")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.waitFullCompletion = boolVal;
            return 0;
        }


        if ( opt.setParam("?MODE",true)
          || opt.isOption("no-wait-full-completion")
          || opt.setDescription("Disable waiting full loading completion on 'Page.domContentEventFired' and 'Page.loadEventFired' events")
           )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR << errMsg << "\n";
                return -1;
            }

            appConfig.waitFullCompletion = !boolVal;
            return 0;
        }


