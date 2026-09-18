struct AiBundleOptions {
    bool includeScripts = false;
    bool includeBodies = false;
    bool includeScreenshots = true;
    size_t maxScriptSize = 5 * 1024 * 1024;
    std::vector<std::string> scriptUrlExcludePatterns;
};

void cdtBuildAiBundle(const std::string& outDir, const AiBundleOptions& opts = {});
