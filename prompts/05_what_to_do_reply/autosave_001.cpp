for (auto& rid : networkJournal->takePendingBodies()) {
    auto br = connection->cdtGetResponseBody(rid);
    networkJournal->setBody(rid, br);
}
