@set ALMAI_SRC_ALL=%~dp0\_src\almai\**\*.c*,*.h*
almai-md-join --clipboard "--hefooter=%~dp0\prompt.txt" "%ALMAI_SRC_ALL%"