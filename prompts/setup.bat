@set PRJ_ROOT=%~dp0\..
@set LIBS=%PRJ_ROOT%\_libs
@set SRC=%PRJ_ROOT%\_src
@set CMN_OPTS=--overwrite --clipboard

@rem if not "%PATCH%"=="" @set PATCH=_patch%PATCH%
@if defined PATCH @set PATCH=_patch%PATCH%

