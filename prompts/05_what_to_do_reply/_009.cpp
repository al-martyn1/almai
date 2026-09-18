struct PendingWait {
    std::string method;
    std::function<bool(const json&)> predicate;
    std::shared_ptr<std::promise<json>> promise;
    std::chrono::steady_clock::time_point deadline;
    std::string sessionId;
    uint64_t id = 0;
};

class WaitManager {
    uint64_t add(PendingWait);
    void cancel(uint64_t);
    void dispatch(const std::string& method, const json& params, const std::string& sessionId);
    void checkTimeouts();
};
