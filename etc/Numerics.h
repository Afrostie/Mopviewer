#pragma once
/**
* This class contains numeric methods and defines
*/
#include <cmath>
#include <string>
#include <sstream>
class Numerics
{
public:

#define PI 3.141592653589793

#define AU 1.49598E+11
    /**
    * rounding method
    */
    int round(double x) {
        return int(x > 0.0 ? x + 0.5 : x - 0.5);
    }

    /**
    * sqr method
    */
    double sqr(double x) {
        return ((x)*(x));
    }

    /**
    * Measure the distance between two bodies in 3d space.
    */
    double calculateDistance(const double & m1x, const double & m1y, const double & m1z, const double & m2x, const double & m2y, const double & m2z) {
        return sqrt(this->sqr((m1x - m2x)+this->sqr(m1y - m2y)+this->sqr(m1z - m2z)));
    }
    /**
    * generate a random number within the given range (double)
    */
    double randomRange(const double low, const double high) {
        double ret_val;
        double range = high-low;
        // generate a number between low..high
        ret_val = 0;
        while (ret_val==0) {
            ret_val =  (range*((rand()/(double) RAND_MAX)));
        }

        return low+ret_val;
    }

    /**
    * generate a random number within the given range (int)
    */
    int randomRangeInt( const int low,  const int high) {
        int ret_val;
        // generate a number between low..high
        ret_val = 0;
        while (ret_val==0) {
            ret_val =  rand() % high;
        }

        return low + ret_val;
    }

    /**
    *  generate a gaussian random number
    */
    double generateGaussian (void) {
        int r1, r2;
        double nr1, nr2, gr, k;
        k = 0.1; //a fairly decent value to have as the variance, but lower may work well too
        //get a couple of ordinary (uniform) random numbers
        r1 = (rand () % 1000) + 1;
        r2 = (rand () % 1000) + 1;
        //scale them between 0 and 1
        nr1 = (r1) / 1000.0;
        nr2 = (r2) / 1000.0;
        gr = k * sqrt (-2.0 * log (nr1)) * cos (2.0 * PI * nr2);
        return gr;
    }

    /**
    *  unipolar sigmoid function
    */
    double sigmoid(float x) {
        double ev;
        ev = exp((double) -x);
        return 1 / (1 + ev);
    }

    /**
    * check to see whether a provided value is within a specified range - integer version
    */
    bool isValueWithinRange( const int value, const int lower, const int upper) {
        if ((value>=lower)&&(value<=upper)) {
            return true;
        }
        return false;
    }

    /**
    * check to see whether a provided value is within a specified range - doubles version
    */
    bool isValueWithinRange( const double value, const double lower, const double upper) {
        if ((value>=lower)&&(value<=upper)) {
            return true;
        }
        return false;
    }

    /**
    * Converts a double value to a string
    */
    std::string doubleToString( const double val) {
        std::stringstream out;
        out<<val;
        return out.str();
    }

    /**
    * Converts an integer value to a string
    */
    std::string intToString(int val) {
        std::stringstream out;
        out<<val;
        return out.str();
    }
};