@call "%~dp0\setup.bat"

@set TESTS_SRCS="%TESTS_ROOT%\_test15.cpp" "%TESTS_ROOT%\_test16.cpp" "%TESTS_ROOT%\_test17.cpp"
@set TESTS_OUTPUT="%PROMPTS_DIR%\_test16_03.txt" "%PROMPTS_DIR%\_test16_03_DOM.txt" "%PROMPTS_DIR%\_test17.txt"

@set PROMPTS_MD_01="%PROMPTS_DIR%\04_what_to_do_reply_patch01\__description__.md_"
@set PROMPTS_MD_04="%PROMPTS_DIR%\04_what_to_do_reply_patch04\__description__.md_"
@set PROMPTS_MD_05="%PROMPTS_DIR%\04_what_to_do_reply_patch05\__description__.md_"
@set PROMPTS_MD_06="%PROMPTS_DIR%\04_what_to_do_reply_patch06\__description__.md_"

@set PROMPTS_MDS=%PROMPTS_MD_01% %PROMPTS_MD_04% %PROMPTS_MD_05% %PROMPTS_MD_06%

almai-md-join %CMN_OPTS% "-o=%~dp0\%~n0.md" "--hefooter=%~dp0\%~n0.txt" %CDT_SRC% %TESTS_SRCS% %TESTS_OUTPUT% %PROMPTS_MDS%


@rem по поводу автоматического преобразования инсерта из буфера в аттач
@rem https://chat.deepseek.com/share/lqyax24jcwwpd6vbwi