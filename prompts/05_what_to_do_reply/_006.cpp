struct ScriptInfo {
    std::string scriptId, url, sourceMapURL, hash;
    int executionContextId = 0;
    bool isModule = false;
    int64_t length = 0;
};
void onScriptParsed(const json&, const std::string& sessionId);
std::vector<ScriptInfo> all() const;
