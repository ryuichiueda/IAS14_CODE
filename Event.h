#ifndef __EVENT__H_ 
#define __EVENT__H_ 

#include <string>
#include <vector>
using namespace std;

class Event{
public:
	Event(int trial_no);
	vector<int> sensor_values;
	int reward;
	string action;
	int trial_number;
};

#endif
