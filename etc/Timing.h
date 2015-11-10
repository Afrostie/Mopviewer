#pragma once
#include <ctime>
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
	clock_t timer_starting_var;
	clock_t timer_finishing_var;
	int time;

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
		timer_starting_var = clock();
	}
	/**
	* stop the clock and send time taken to std::out
	*/
	void stop() {
		timer_finishing_var = clock();
		time = timer_finishing_var-timer_starting_var;
		time /=1000;
		div_t divresult= div(time,1000);
		if (divresult.quot<60) {
			std::cout << "> Time Taken: "<< divresult.quot << "." << divresult.rem << " Seconds"<<std::endl;
		}else if ((divresult.quot>=60)&&(divresult.quot<3600)) {
			div_t divresult2 = div(divresult.quot,60);
			std::cout << "> Time Taken: "<<divresult2.quot <<" Minutes "<< divresult2.rem  <<  "." << divresult.rem << " Seconds"<<std::endl;
		}
		else if (divresult.quot>=3600) {
			div_t divresult3 = div(divresult.quot,3600);
			div_t divresult4 = div(divresult.rem,60);
			std::cout << "> Time Taken: "<<divresult3.quot<<" Hours, " <<divresult4.quot <<" Minutes, "<< divresult4.rem  <<  " Seconds, " << divresult4.rem << " MilliSeconds."<<std::endl;
		}
	}
};