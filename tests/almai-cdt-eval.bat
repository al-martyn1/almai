@set TEST_NAME=%~n0
call "%~dp0\.find_test_exe.bat" %TEST_NAME%
@echo FOUND_EXE: %FOUND_EXE%
@set FOUND_EXE=%FOUND_EXE:"=%
@if "%FOUND_EXE%"=="" @goto NOT_FOUND
@set TEST_EXE="%FOUND_EXE%"

@set RESTORE_OPTS=--restore-last-session

@set COMMON_OPTS=%RESTORE_OPTS% --timeout=30000 --no-wait-full-completion
@rem set COMMON_OPTS=%RESTORE_OPTS% --timeout=300000

@rem %TEST_EXE% %COMMON_OPTS% --url=https://yandex.ru --expression=document.documentElement.outerHTML >cdt-eval-01-html.txt
%TEST_EXE% %COMMON_OPTS% "--url=https://яндекс.рф" --expression=document.documentElement.outerHTML >cdt-eval-01-html.txt
@rem такого свойства нет
%TEST_EXE% %COMMON_OPTS% "--url=https://яндекс.рф" --expression=document.documentElement.formattedOuterHTML >cdt-eval-02-html.txt
@rem exit /B 0



exit /B 0
:NOT_FOUND
@echo Test executable not found
