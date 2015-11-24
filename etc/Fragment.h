#pragma once
#include <iostream>
#include <cstdlib>
#include <math.h>

class Fragment {
private:

    std::string string;

public:
    Fragment() {
    }

    ~Fragment() {
    }

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
 * fill this Fragment with the content of a standard string up to count chars
 */
 std::string fill(std::string s,int count) {
    int i;
    std::string tmp;
    for (i=0;i<count;i++) {
        tmp.push_back(s.at(i));
    }
    deleteTill(count);
    deleteTill(1);

   std::cout<< tmp <<std::endl;
   // std::cout<< test <<std::endl;
     return tmp;
}


    std::size_t coumtUpTo (std::string s) {
        return this->string.find(s.at(0));
    }

    void deleteTill(std::size_t loc) {
        this->string.erase(0,loc);
    }

    /**
    * fill this Fragment with the content of a standard string
    */
    void fill(std::string input) {
        this->string=input;

    }
    /**
    * fill this Fragment with the content of another Fragment.
    */
    void fill (Fragment &input) {
        if(this!=&input) {
            this->string = input.string;
        }
    }


    int toInt() {
      std::cout << this->string << std::endl;
        return atoi(this->string.c_str());

    }

    float toFloat() {
        return  atof(this->string.c_str());
    }


};
