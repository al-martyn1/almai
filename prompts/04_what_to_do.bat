@call "%~dp0\setup.bat"
almai-md-join %CMN_OPTS% "-o=%~dp0\%~n0.md" "--hefooter=%~dp0\%~n0.txt" "%LIBS%/marty_cbp/**/*.h,*.c" "%SRC%\almai/_test17.cpp" "%PRJ_ROOT%\prompts\_test17.txt"
