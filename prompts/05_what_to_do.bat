@call "%~dp0\setup.bat"

@set TESTS_SRCS="%TESTS_ROOT%\_test15.cpp" "%TESTS_ROOT%\_test16.cpp" "%TESTS_ROOT%\_test17.cpp"
@set TESTS_OUTPUT="%PROMPTS_DIR%\_test16_03.txt" "%PROMPTS_DIR%\_test16_03_DOM.txt" "%PROMPTS_DIR%\_test17.txt"

almai-md-join %CMN_OPTS% "-o=%~dp0\%~n0.md" "--hefooter=%~dp0\%~n0.txt" %CDT_SRC% %TESTS_SRCS% %TESTS_OUTPUT% 
