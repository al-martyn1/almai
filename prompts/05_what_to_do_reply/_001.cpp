struct RequestRecord {
    std::string requestId;
    std::string url;
    std::string method;
    std::string type;         // Document/Script/XHR/...
    std::string frameId;
    double timestamp = 0.0;
    bool finished = false;
    bool failed = false;
};
