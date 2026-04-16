@set TEST_NAME=%~n0
@call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

%TEST_EXE% --help >"%~dp0\%~n0-help.txt"
%TEST_EXE% --message --help >"%~dp0\%~n0-help-on-message.txt"
%TEST_EXE% --md --help >"%~dp0\%~n0-help-md.txt"
%TEST_EXE% --md --message --help >"%~dp0\%~n0-help-on-message-md.txt"
%TEST_EXE% --wiki --help >"%~dp0\%~n0-help-wiki.txt"
%TEST_EXE% --wiki --message --help >"%~dp0\%~n0-help-on-message-wiki.txt"
%TEST_EXE% --ule-ule --help >"%~dp0\%~n0-help-on-ule-ule.txt"

@rem %TEST_EXE% submodule foreach git pull submodule --init --recursive update
@rem %TEST_EXE% branch
@rem %TEST_EXE% branch -d
@rem %TEST_EXE% branch -D
@rem %TEST_EXE% -D branch
@rem %TEST_EXE% stash --message push
@rem %TEST_EXE% submodule --init --recursive update
@rem %TEST_EXE% submodule update --init --recursive
@rem %TEST_EXE% submodule update
@rem %TEST_EXE% worktree add ../backup-branch backup
@rem %TEST_EXE% worktree add ../backup-branch backup workout
@rem %TEST_EXE% 


exit /B 0
:NOT_FOUND
@echo Test executable not found
