#pragma once
#include <iostream>
#include <cstdlib>
#include <math.h>

class Fragment {

    friend std::ostream& operator<<(std::ostream &os, const Fragment &fr) {
        os << fr.string;
        return os;
    }

private:
    char * string;
    int len;

public:
    Fragment() {
        this->len=0;
    }

    ~Fragment() {
        if (this->len!=0) {
            delete [] this->string;
        }
    }

    char *  c_str() {
        return this->string;
    }

    char at(int x) {
        return this->string[x];
    }

    int size() {
        return this->len;
    }

    /**
    * fill this Fragment with the content of a c string
    */
    void fill(char * input, int len) {
        int s = len;
        if (this->len !=0) {
            delete this->string;
            this->len=0;
        }
        this->string = new char[s];
        for (int j(0); j<len; j++) {
            this->string[j] = input[j];
        }
        this->len = len;
    }

    /**
    * fill this Fragment with the content of a standard string
    */
    void fill(std::string input, int len) {
        int s = len;
        if (this->len !=0) {
            delete this->string;
            this->len=0;
        }
        this->string = new char[s];
        for (int j(0); j<len; j++) {
            this->string[j] = input.at(j);
        }
        this->len = len;
    }
    /**
    * fill this Fragment with the content of another Fragment.
    */
    void fill (Fragment &input) {
        if(this!=&input) {
            if (this->len !=0) {
                delete this->string;
                this->len = 0;
            }
            this->string = new char[input.len];
            for (int j(0); j<input.len; j++) {
                this->string[j] = input.string[j];
            }
            this->len = input.len;
        }
    }

    /**
    * return the string found between the specified start and end indexes.
    */
    void fill (Fragment &input, int start, int stop) {
        if(this!=&input) {
            if (this->len !=0) {
                delete this->string;
                this->len = 0;
            }
            this->string = new char[stop-start];
            if (start>0&&stop<input.len) {
                for (int j(start); j<stop; j++) {
                    this->string[j] = input.string[j];
                }
                this->len = stop-start;
            }
        }
    }

    /**
    * return the string found from the specified starting index to the given character.
    * records the index in the string reached.
    */
    void fill(Fragment &source,  char endChar, int &pos) {
        // first step, advance from the given starting position in the string until the specified end character is found or the end of the string is reached.
        int stringSize(0);
        for (int x(pos); x<source.size(); x++) {
            if (source.at(x)==endChar) {
                break;
            }
            stringSize++;
        }
        // now copy the substring into this Fragment, deleting the old content first if required
        if (this->len!=0) {
            delete [] this->string;
            this->len =0;
        }
        this->string = new char[stringSize+1];
        int loc(0);
        for (int y(pos); y<pos+stringSize; y++) {
            this->string[loc] = source.at(y);
            loc++;
        }
        pos+=stringSize+1;
        this->len =  loc;
    }


    /**
    * return the string found from the specified starting index of a char array to the given character.
    * records the index in the char array reached.
    */
    void fill(char *source, int srclen, char endChar, int &pos) {
        // first step, advance from the given starting position in the string until the specified end character is found or the end of the string is reached.
        int stringSize(0);
        for (int x(pos); x<srclen; x++) {
            if (source[x]==endChar) {
                break;
            }
            stringSize++;
        }
        // now copy the substring into this Fragment, deleting the old content first if required
        if (this->len!=0) {
            delete [] this->string;
            this->len =0;
        }
        this->string = new char[stringSize+1];
        int loc(0);
        for (int y(pos); y<pos+stringSize; y++) {
            this->string[loc] = source[y];
            loc++;
        }
        pos+=stringSize+1;
        this->len =  loc;
    }




    /**
    * check to see whether a substring, as stored in one Fragment, is present in another
    */
    bool isPresent (Fragment &fr) {
        if (this->len!=0) {
            int count(0);
            if (this->len>fr.size()) {
                for (int i=0; i<this->len-fr.size(); i++) {
                    count=0;
                    int pos(i);
                    for (int j=0; j<fr.size(); j++) {
                        if (this->string[pos]==fr.at(j)) {
                            count++;
                        }
                        pos++;
                    }
                    if (count==fr.size()) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /**
    * Turn the contents of this Fragment into a string array around the specified delimeter.
    * The size of the resultant array is stored in &len.
    * Will not work for Fragments of length <=1.
    */
    Fragment * split( char delim, int &len) {
        if (this->len!=0&&this->len>1) {
            // first off, count the amount of times the delimeter appears in this Fragment
            int count(0);
            for (int x(0); x<this->len; x++) {
                if(this->string[x]==delim) {
                    count++;
                }
            }
            if(count>0) {
                // little check, is the delimiter the last character too? if not we need to +1 the count
                if(this->string[this->len-1] != delim) {
                    count++;
                }
                int pos(0);
                Fragment *output = new Fragment[count];
                for (int i(0); i<count; i++) {
                    output[i].fill(this->string, this->len,delim,pos);
                }

                len = count;
                return output;
            }
        }
        return NULL;
    }

    /**
    * Turn an array of Fragments into a single Fragment, delilineated by the specified char
    * takes as parameters the array of Fragments and the size of the array
    */
    void join( Fragment *src, char delim, int size) {
        if (this->len!=0) {
            delete [] this->string;
            this->len =0;
        }
        // count the total number of characters involved first
        int total(0);
        for (int x(0); x<size; x++) {
            total+=src[x].size();
        }
        this->string = new char[total+size];
        int count(0);
        for (int i(0); i<size; i++) {
            for(int j(0); j<src[i].size(); j++) {
                this->string[count] = src[i].at(j);
                count++;
            }
            if (i<size-1) {
                this->string[count] = delim;
                count++;
            }
        }
        this->len=count;
    }

    /**
    * Replace a specified char with another
    */
    void replace(char target, char replacement) {
        if (this->len!=0) {
            for (int x(0); x<this->len; x++) {
                if (this->string[x] == target) {
                    this->string[x] = replacement;
                }
            }
        }
    }

    /**
    * extract a string that contiguously holds  all of the characters in the supplied list
    */
    Fragment * regex1 (Fragment mask, int &pos) {
        if (this->len!=0) {
            Fragment * tmp = new Fragment();
            for (int x(pos); x<this->len; x++) {
                for (int i(0); i<mask.len; i++) {
                    if (this->string[x]== mask.string[i]) {
                        int count(x);
                        while(count<this->len) {
                            bool found(false);
                            for (int j(0); j<mask.len; j++) {
                                if (this->string[count]== mask.string[j]) {
                                    found = true;
                                    char t[1] = {this->string[count]};
                                    tmp->fill(t,1);
                                }
                                if (found==false) {
                                    pos = x;
                                    return tmp;
                                }
                            }
                            count++;
                        }
                    }
                }
            }
            return tmp;
        }
    }

    /**
    * does the string have at least one of the characters required?
    */
    bool regex2 (Fragment mask, int &pos) {
        if (this->len!=0) {
            int count(0);
            for (int x(pos); x<this->len; x++) {
                for (int i(0); i<mask.len; i++) {
                    if (this->string[x]== mask.string[i]) {
                        count=x;
                        while(count<this->len) {
                            for (int j(0); j<mask.len; j++) {
                                if (this->string[count]== mask.string[j]) {
                                    pos = x;
                                    return true;
                                }
                            }
                            count++;
                        }

                    }
                }
            }
            return false;
        }
        return false;
    }

    /**
    * special character escaping, stores a single escaped character in this Fragment
    */
    bool regex3 (Fragment &dest, int &pos) {
        if (this->len!=0) {
            if (dest.len!=0) {
                delete [] dest.string;
                dest.len =0;
            }
            for (int x(pos); x<this->len; x++) {
                if (x<this->len) {
                    if (this->string[x]=='\\') {
                        switch(this->string[x+1]) {
                        case '.':
                        case '*':
                        case '+':
                        case '-':
                        case '|':
                        case '\\':
                            char t[1] = {this->string[x+1]};
                            dest.fill(t,1);
                            pos=x+2;
                            return true;
                        }
                    }
                }
            }

        }
        return false;
    }

    /**
    * store all the characters between [ and ]
    */
    bool regex4 (Fragment &dest, int &pos) {
        if (this->len!=0) {
            if (dest.len!=0) {
                delete [] dest.string;
                dest.len =0;
            }
            for (int x(pos); x<this->len; x++) {

            }

        }
        return false;
    }


    int toInt() {
        return atoi(this->string);
    }

    float toFloat() {
        return atof(this->string);
    }

    /**
    * equality check
    */
    bool operator == ( const Fragment & other) {
        if (this->len != other.len) {
            return false;
        }

        for (int x(0); x<this->len; x++) {
            if (this->string[x] != other.string[x]) {
                return false;
            }
        }
        return true;
    }

    /**
    * inequality check
    */
    bool operator  != ( const Fragment & other) {
        return !(*this==other);
    }

    /**
    * concatenation
    */
    Fragment  & operator  + ( const Fragment &other) {
        Fragment *tmp = new Fragment;
        char *str = new char[this->len+other.len];
        int p(0);
        for (int x(0); x<this->len; x++) {
            str[x] = this->string[x];
            p++;
        }
        for (int x(0); x<other.len; x++) {
            str[p] = other.string[x];
            p++;
        }
        tmp->fill(str,this->len+other.len);
        delete [] str;
        return *tmp;
    }

    /**
    * assignment
    */
    Fragment & operator  = ( const Fragment & other) {
        if (this->len !=0) {
            delete this->string;
            this->len=0;
        }
        this->string =  new char[other.len];
        for (int x(0); x<other.len; x++) {
            this->string[x] = other.string[x];
        }
        this->len = other.len;
        return *this;
    }




};
