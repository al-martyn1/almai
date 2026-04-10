@set TEST_NAME=%~n0
@call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

%TEST_EXE% --help >"%~dp0\%~n0-help.txt"
%TEST_EXE% branch
%TEST_EXE% branch -d
%TEST_EXE% branch -D
%TEST_EXE% -D branch
%TEST_EXE% stash --message push
%TEST_EXE% submodule --init --recursive update
%TEST_EXE% submodule update --init --recursive
%TEST_EXE% submodule update
@rem %TEST_EXE% 


exit /B 0
:NOT_FOUND
@echo Test executable not found
