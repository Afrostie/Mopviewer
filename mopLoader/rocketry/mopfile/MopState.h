#pragma once
/*
* This class is used to store a single state, containing many MopItems, for writing to, or when read from, a MOP file.
*/
#include "MopItem.h"
class MopState {
private:
    MopItem * content;
    int itemCount;
public:
    MopState() {
        this->itemCount = 0;
    }
    
    ~MopState() {
        delete [] this->content;
    }

    MopState(const MopState& bs) {
        this->itemCount = bs.itemCount;
        if (this->itemCount!=0) {
            MopItem *tmp = new MopItem[this->itemCount];
            for (int x(0); x<this->itemCount; x++) {
                tmp[x] = bs.content[x];
            }

            delete [] this->content;
            this->content = tmp;
            tmp = 0;
        }
    }

    MopState & operator = (const MopState & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->itemCount = other.itemCount;
            if (this->itemCount!=0) {
                MopItem *tmp = other.content;
                delete [] this->content;
                this->content = tmp;
                tmp = 0;
            }
        }
        return *this;
    }

    MopItem *getContent() const
    {
        return content;
    }

    /**
    * returns the specified MopItem
    */
    MopItem getMopItem(int x) const
    {
        return content[x];
    }

    int getItemCount() const
    {
        return itemCount;
    }

    void addMopItem(MopItem current)
    {   int x;
        MopItem * temp;
        if (this->itemCount==0) {
            this->content = new MopItem[1];
            this->content[0].fillFromExistingMopItem(current);
            this->itemCount = 1;
        }
        else {
            this->itemCount++;
            temp = new MopItem[this->itemCount];
            for(x = 0; x <this->itemCount-1; x++) {
                temp[x].fillFromExistingMopItem(this->content[x]);
            }
            temp[this->itemCount-1].fillFromExistingMopItem(current);
            delete [] this->content;
            this->content = temp;
            temp = NULL;
        }
    }
    
    void addMopItemGravMap(MopItem current)
    {   int x;
        MopItem * temp;
        if (this->itemCount==0) {
            this->content = new MopItem[1];
            this->content[0].fillFromExistingMopItem(current);
            this->itemCount = 1;
        }
        else {
            this->itemCount++;
            temp = new MopItem[this->itemCount];
            for(x = 0; x <this->itemCount-1; x++) {
                temp[x].fillFromExistingMopItem(this->content[x]);
            }
            temp[this->itemCount-1].fillFromExistingMopItem(current);
            delete [] this->content;
            this->content = temp;
            temp = NULL;
        }
    }
};
