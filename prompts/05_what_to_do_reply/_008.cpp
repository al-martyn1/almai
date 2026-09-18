struct JournalBundle {
    std::shared_ptr<NetworkJournal> network;
    std::shared_ptr<ConsoleJournal> console;
    std::shared_ptr<DomJournal>     dom;
    std::shared_ptr<ScriptJournal>  script;
    std::shared_ptr<TargetJournal>  target;
};

void Connection::attachJournals(const JournalBundle&);
