struct SnapshotDiff {
    std::vector<std::string> newRequestIds;
    std::vector<std::pair<std::string,std::string>> domChanges;
    std::unordered_map<std::string,std::string> storageAdded, storageChanged, storageRemoved;
    std::vector<json> interactiveAdded, interactiveRemoved;
};
SnapshotDiff cdtDiff(const Snapshot& a, const Snapshot& b);
