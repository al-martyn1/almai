void onRequestWillBeSent(const json& params);
void onLoadingFinished(const json& params);
void onLoadingFailed(const json& params);

std::vector<RequestRecord> allOrdered() const;
std::optional<RequestRecord> get(const std::string& requestId) const;
int inflightCount() const;
