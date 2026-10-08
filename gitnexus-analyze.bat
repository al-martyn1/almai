set GITNEXUS_LBUG_EXTENSION_INSTALL=auto
echo GITNEXUS_LBUG_EXTENSION_INSTALL=%GITNEXUS_LBUG_EXTENSION_INSTALL%
npx gitnexus analyze --repair-fts --index-only
@rem npx gitnexus analyze --index-only > gitnexus_analyze.log 2>&1