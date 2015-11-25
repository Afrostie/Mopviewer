#pragma once
#include <iostream>
#include <cstdlib>
#include <math.h>

class Fragment {
private:


public:
    Fragment() {
    }

    ~Fragment() {
    }
    std::string string;
    std::string  str() {
        return this->string;
    }

    char at(int x) {
        return this->string.at(x);
    }

    int size() {
        return this->string.size();
    }

    /**
     * fill this Fragment with the content of a standard string
     */
    void fill(std::string s) {
        // this->string.clear();
        this->string = s;
        //std::cout<< tmp <<std::endl;
        // std::cout<< test <<std::endl;
    }
    /**
 * fill this Fragment with the content of a standard string up to count chars
 */
    int fill(Fragment s,int start, char c) {
    int i= start;
        this->string.clear();
        char ch = s.string.at(i);

    while(ch!=c) {
        this->string.push_back(ch);
        //std::cout<< ch <<std::endl;
        i++;
        ch = s.string.at(i);
        //std::cout<< ch <<std::endl;
    }
     return i;
}





    int toInt() {
      //std::cout << this->string << std::endl;
        return atoi(this->string.c_str());

    }

    float toFloat() {
        return  atof(this->string.c_str());
    }


};
