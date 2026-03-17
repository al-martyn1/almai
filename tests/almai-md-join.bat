@set TEST_NAME=%~n0
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

%TEST_EXE% -Y -o="%~dp0\almai-md-join_01.md" ../_src/almai/**/*.bat
@rem *.cpp,


exit /B 0
:NOT_FOUND
@echo Test executable not found
