@rem Add next option to UMBA_MDPP_EXTRA_OPTIONS variable to regenerate all images on next generation
@rem --clear-generation-cache=true
@call "%~dp0.umba.bat\make-cli-options-docs.bat"

@rem call "%~dp0\.umba.bat\fix_tabs.bat"

@set CLOCMD=_md
@call "%~dp0\.umba.bat\run_clock.bat"

@if exist "%~dp0.umba.bat\gen-doc.bat" @(
    @set UMBA_MDPP_EXTRA_OPTIONS="--batch-generate-page-index" "--batch-page-index-file=doc/Index.md"
    @call "%~dp0.umba.bat\gen-doc.bat"
) else @(
    @echo.
)
