@set TEST_NAME=%~n0
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

%TEST_EXE% --list       -o=almai-md-split-test01    almai-md-split.md
%TEST_EXE%              -o=almai-md-split-test01    almai-md-split.md
%TEST_EXE% --overwrite  -o=almai-md-split-test01    almai-md-split.md

%TEST_EXE% --list       -o=almai-md-split-ai-result almai-md-split-ai-result.md
%TEST_EXE%              -o=almai-md-split-ai-result almai-md-split-ai-result.md
%TEST_EXE% --overwrite  -o=almai-md-split-ai-result almai-md-split-ai-result.md




exit /B 0
:NOT_FOUND
@echo Test executable not found
