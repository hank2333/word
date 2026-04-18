#include "WordEntry.h"
#include <iostream>
WordEntry::WordEntry():word(""),meaning(""),example(""),note(""){}
WordEntry::WordEntry(const std::string& w):word(w),meaning(""),example(""),note(""){}
void WordEntry::list(){
    std::cout<<word<<"\nmeaning="<<meaning<<"\nexample="<<example<<"\nnote="<<note<<std::endl;
    return;
}