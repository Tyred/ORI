#include "information.h"

Information::Information(unsigned int begin, unsigned int end, unsigned int index){
    this->end = end;
    this->begin = begin;
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
    if (this->index == b.getIndex()){ //verifico se o index é igual
        return true;
    }

    if (this->begin == b.getBegin() && this->end == b.getEnd()){ //verifico se o intervalo é igual
        return true;
    }

    return false;
}
