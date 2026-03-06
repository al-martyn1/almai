@set CONFIG=Debug
@if exist "%~dp0\.set_config.bat" @call "%~dp0\.set_config.bat"
@call %~dp0\..\.umba.bat\find-exe-in-out.bat %1
