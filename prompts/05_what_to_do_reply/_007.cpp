struct TargetInfo { std::string targetId, type, title, url, browserContextId; bool attached; };
void onAttached(const json&);
void onDetached(const json&);
std::vector<std::pair<std::string, TargetInfo>> all() const;
std::string getSessionForTarget(const std::string&) const;
