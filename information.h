#ifndef INFORMATION_H
#define INFORMATION_H

class Information{
    private:
        unsigned int index;
        unsigned int begin;
        unsigned int end;
    public:
        Information(unsigned int begin = 0,unsigned int end = 0, unsigned int index = 0);

        unsigned int getBegin() const;
        unsigned int getEnd() const;
        unsigned int getIndex() const;

        unsigned int getSize() const;

        void setBegin(unsigned int);
        void setEnd(unsigned int);
        void setIndex(unsigned int);

        bool operator ==(const Information &b) const;
};

#endif // INFORMATION_H
