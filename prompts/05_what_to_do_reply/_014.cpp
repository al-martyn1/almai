std::optional<RequestRecord> cdtWaitForNetworkResponse(
    std::function<bool(const RequestRecord&)> pred, int timeoutMs = 30000);
