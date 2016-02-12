#pragma once
#include<sys/time.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
/**
* Simple timing class
*/
class Timing
{
private:
     timeval  startTime,endTime;

     double totalTime;

public:

    Timing(void)
    {
    }

    ~Timing(void)
    {
    }
    /**
    * start the clock
    */
    void start() {
      gettimeofday(&endTime, NULL);
    }
    /**
    * stop the clock and send time taken to std::out
    */
    void stop() {
      gettimeofday(&endTime, NULL);
      totalTime =  (endTime.tv_sec - startTime.tv_sec) * 1000000L;
      totalTime += (endTime.tv_usec - startTime.tv_usec);

       std::cout << "> Time Taken: "<< totalTime<<std::endl;
    }
};
