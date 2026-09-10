@set PATCH=04
@call "%~dp0\setup.bat"

almai-md-split --overwrite --clipboard "-o=%~dp0\%~n0%PATCH%"
@rem start meld "%~dp0\%~n0" "%~dp0\%~n0%PATCH%"