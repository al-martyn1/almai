@call "%~dp0\setup.bat"

@echo TESTS_ROOT=%TESTS_ROOT%
@echo PROMPTS_DIR=%PROMPTS_DIR%
@echo LIBS=%LIBS%
@echo SRC=%SRC%

almai-md-join %CMN_OPTS% "-o=%~dp0\%~n0.md" "--hefooter=%~dp0\%~n0.txt"


@rem по поводу автоматического преобразования инсерта из буфера в аттач
@rem https://chat.deepseek.com/share/lqyax24jcwwpd6vbwi