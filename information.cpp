#include "information.h"

Information::Information(int index, int begin,int end){
   this->index = (unsigned int) index;
    this->end = (unsigned int) end;
    this->begin = (unsigned int) begin;
}

unsigned int Information::getBegin() const {
    return this->begin;
}

unsigned int Information::getEnd() const {
    return this->begin;
}

unsigned int Information::getIndex() const{
    return this->index;
}

unsigned int Information::getSize() const{
    return this->end - this->begin;
}

void Information::setBegin(unsigned int begin){
    this->begin = begin;
}

void Information::setEnd(unsigned int end){
    this->end = end;
}

void Information::setIndex(unsigned int index){
    this->index = index;
}

bool Information::operator ==(const Information &b) const{
    if (this->index == b.index){ //verifico se o index é igual
        return true;
    }

    return false;
}
