struct Snapshot {
    std::string url;
    double takenAt = 0.0;
    std::string domHtml;
    std::vector<std::string> requestIds;
    std::unordered_map<std::string, std::string> localStorage;
    std::unordered_map<std::string, std::string> sessionStorage;
    std::vector<json> interactive;
};
Snapshot cdtSnapshot();
