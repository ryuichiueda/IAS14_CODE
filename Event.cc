#include "Event.h"
#include <iostream>
#include <string>
using namespace std;

Event::Event(int trial_no)
{
	trial_number = trial_no;
	reward = 0;
	action = "notyet";
}

