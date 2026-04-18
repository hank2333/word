#ifndef WORDSTORE_H
#define WORDSTORE_H

#include <vector>
#include <string>
#include "WordEntry.h"

class WordStore {
    private:
    std::vector<WordEntry> words;
    std::string filename;
    public:
    WordStore(const std::string& outputfile);
    bool load();
    bool save() const;//const function means u promise not revising the members in the object
    bool addWord(const std::string& w);
    bool addWord(const WordEntry& entry);
    void listWords() const;
    bool clear();
    WordEntry* findword(const std::string& w);
    const WordEntry* findword(const std::string& w) const;
};

#endif