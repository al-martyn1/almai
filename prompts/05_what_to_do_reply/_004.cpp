struct FrameInfo {
    std::string frameId, parentFrameId, url, name, securityOrigin, mimeType;
    bool isMain = false;
};
void onFrameAttached(const json&, const std::string& sessionId);
void onFrameNavigated(const json&, const std::string& sessionId);
void onFrameDetached(const json&);
std::vector<FrameInfo> allFrames() const;
std::string getMainFrameId() const;
