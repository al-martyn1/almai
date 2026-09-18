struct ContextInfo { int id; std::string origin, name, type, frameId; bool isDefault; };
std::optional<ContextInfo> getContextForFrame(const std::string& frameId) const;
