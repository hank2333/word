#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "WordStore.h"
#include <algorithm>

WordStore::WordStore(const std::string& outputfile){
    words = {};
    filename = outputfile;
}
bool WordStore::addWord(const std::string& w){
    WordEntry entry(w);
    words.push_back(entry);
    return true;
}
bool WordStore::addWord(const WordEntry& entry){
    words.push_back(entry);
    return true;
}
void WordStore::listWords() const{
    if(words.empty()){
        std::cout<<"Word list empty!"<<std::endl;
        return;
    }
    int index = 1;
    for(const auto& ele: words){
        std::cout<<index<<"."<<ele.word<<"\nmeaning:"<<ele.meaning<<"\nexample:"<<ele.example<<"\nnote:"<<ele.note<<std::endl;
        index++;
    }
}
bool WordStore::save() const{
    std::ofstream output(filename);
    if (!output){
        std::cout<<"Fail to open file"<<std::endl;
        return false;
    }
    for(const auto& ele:words){
        output << "---WORD---" << std::endl;
        output << "word=" << ele.word << std::endl;
        output << "meaning=" << ele.meaning << std::endl;
        output << "example=" << ele.example << std::endl;
        output << "note=" << ele.note << std::endl;
    }
    return true;
}
bool WordStore::load(){
    words.clear();
    std::ifstream input(filename);
    if(!input){
        return true;
    }
    WordEntry current;
    std::string line;
    while(std::getline(input,line)){
        if(line == "---WORD---"){
            current = WordEntry();
        }
        else if(line.find("word=")==0){
            current.word = line.substr(5);
        }
        else if(line.find("meaning=")==0){
            current.meaning = line.substr(8);
        }
        else if(line.find("example=")==0){
            current.example = line.substr(8);
        }
        else if(line.find("note=")==0){
            current.note = line.substr(5);
            //store!
            words.push_back(current);
        }
    }
    return 0;
}
bool WordStore::clear(){
    words.clear();
    std::cout<<"list clear!"<<std::endl;
    return true;
}
WordEntry* WordStore::findword(const std::string& w){
    for(auto& key: words){
        if(key.word == w){
            return &key;
        }
    }
    return nullptr;
}
const WordEntry* WordStore::findword(const std::string& w) const{
    for(const auto& key:words){
        if(key.word == w){
            return &key;
        }
    }
    return nullptr;
}
