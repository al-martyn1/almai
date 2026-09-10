@rem set PATCH=01
@call "%~dp0\setup.bat"

almai-md-split %SPLIT_OPTS% --overwrite --clipboard "-o=%~dp0\%~n0%PATCH%"
@rem start meld "%~dp0\%~n0" "%~dp0\%~n0%PATCH%"