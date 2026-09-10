@set PRJ_ROOT=%~dp0\..
@set LIBS=%PRJ_ROOT%\_libs
@set SRC=%PRJ_ROOT%\_src
@set PROMPTS_DIR=%PRJ_ROOT%\prompts
@set CDT_SRC="%LIBS%/marty_cdt/**/*.h,*.c"
@set TESTS_ROOT=%SRC%\almai
@set CMN_OPTS=--overwrite --clipboard

@set SPLIT_OPTS=--description-filename=__description__.md_

@rem if not "%PATCH%"=="" @set PATCH=_patch%PATCH%
@if defined PATCH @set PATCH=_patch%PATCH%

