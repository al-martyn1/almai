template<typename Pred>
json cdtWaitForEvent(const std::string& method, Pred pred, int timeoutMs = 30000, const std::string& sessionId = "");
