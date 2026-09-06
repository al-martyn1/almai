@set PATCH=01

@call "%~dp0\setup.bat"
almai-md-join %CMN_OPTS% "-o=%~dp0\01_review.md" "--hefooter=%~dp0\01_review.txt" "%LIBS%/marty_cbp/*.h,*.c" "%SRC%\almai/_test16.cpp" "%PRJ_ROOT%\prompts\_test16_01.txt"
