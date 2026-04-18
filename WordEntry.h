#ifndef WORDENTRY_H
#define WORDENTRY_H
#include <string>
class WordEntry{
    public:
    std::string word;
    std::string meaning;
    std::string example;
    std::string note;
    
    WordEntry();
    WordEntry(const std::string& w);
    void list();
};

#endif