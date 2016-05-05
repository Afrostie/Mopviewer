#pragma once

#include <iostream>


class ImageLoader{
public:
   unsigned char* loadImage(std::string path);
   void freeData(unsigned char* data);
   int returnWidth();
   int returnHeight();
private:
  int width;
  int height;
};

