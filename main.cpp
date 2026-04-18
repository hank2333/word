#include <iostream>
#include "WordStore.h"
#include <string>
#include <vector>
#include "LLMClient.h"
void printhelp();

int main(int argc,char* argv[]){
    WordStore list("storedlist.txt");
    list.load();
    if(argc > 1){
        std::string command = argv[1];
        if(command == "list"){
            list.listWords();
        }
        else if(command == "add"){
            if(argc <3){
                std::cout<<"Not enough argument!"<<std::endl;
                printhelp();
                return 1;
            }
            else{
                std::string word = argv[2];
                if(list.findword(word) != nullptr){
                    std::cout<<"Word already exists!"<<std::endl;
                    return 2;
                }
                LLMClient Client("qwen3.5:4b");
                LLMResult result = Client.enrichWord(word);
                WordEntry entry(word);
                if(result.success){
                    entry.example = result.example;
                    std::cout <<result.meaning<<std::endl;
                    entry.meaning = result.meaning;
                }
                list.addWord(entry);
                if(!list.save()){
                    std::cout<<"Save failed!"<<std::endl;
                    return 1;
                }
                std::cout<<"Word added!"<<std::endl;
                return 0;
            }
        }
        else if(command == "clear"){//for developers
            list.clear();
            list.save();
        }   
        else if(command == "help"){
            printhelp();
        }
        else if(command == "show"){
            if(argc < 3){
                std::cout <<"Not enough argument!"<<std::endl;
                printhelp();
                return 1;
            }
            else{
                if(list.findword(argv[2]) != nullptr){
                    (list.findword(argv[2]))->list();
                }
                else{
                    std::cout<<"Word not found!"<<std::endl;
                    return 2;
                }
            }
        }
        else{
            std::cout<<"error!"<<std::endl;
        }
    }
    else{
        std::cout<<"Not enough argument!"<<std::endl;
        printhelp();
        return 1;
    }
    return 0;
}

void printhelp(){
    std::cout<<"Usage:"<<std::endl;
    std::cout<<"word add <word>\t\tAdd single word"<<std::endl;
    std::cout<<"word list\t\tlist down all the words"<<std::endl;
    std::cout<<"word show <word>\t\tshow exact word info"<<std::endl;
}