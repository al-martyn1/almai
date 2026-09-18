struct ConsoleMessage {
    std::string level;
    std::vector<std::string> argTexts;
    double timestamp;
    std::string url;
    int line = -1;
};
struct ExceptionRecord {
    std::string text, className, description, url;
    int line = -1, column = -1;
    double timestamp;
    std::vector<std::string> stackTop;
};
