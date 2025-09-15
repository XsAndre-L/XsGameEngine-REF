
// #include "page/page.h"

// class StorageManager {
// public:
//     StorageManager(const std::string& filePath);

//     bool appendRow(const Row& row);
//     Row readRow(uint32_t pageId, uint32_t offset);

//     void flush(); // write all dirty pages to disk
//     void close(); // flush + close file

// private:
//     std::fstream file;
//     std::vector<Page> pages;

//     Page& getOrCreateWritablePage();
//     void writePageToDisk(Page& page);
//     void loadAllPages();
// };