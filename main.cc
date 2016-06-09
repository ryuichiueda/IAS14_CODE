#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "Mouse.h"
using namespace std;

void loop(Mouse &m,int trial);

int main(int argc, char const* argv[]){
	ifstream ifs("/dev/urandom");

	Mouse m(&ifs);	

	int trial = 1;

	if(argc == 2){
		trial = m.reload(string(argv[1]));
		m.debug();
		exit(0);
	}

	while(1){
		loop(m,trial);
		trial++;
	}
}

void loop(Mouse &m, int trial)
{
	while(m.readButton('0') == 1){
		usleep(200000);
	}

	m.putLed('0','0');
	m.putLed('1','1');
	m.putLed('2','1');
	m.putLed('3','0');
	m.motorOn();
	m.initTrial();


	// first state
	m.observe();
	m.sensorUpdate();
	m.actionForward();

	// second state
	m.observe();
	m.sensorUpdate();
	stringstream ssd;
	ssd << "/var/tmp/decision." << setw(4) << setfill('0') << trial;
	ofstream dofs(ssd.str().c_str());
	m.decision(&dofs);

	// third state
	m.observe();
	m.sensorUpdate();
	m.actionForward();

	// 4th state
	m.motorOff();
	int reward = 0;


	while(1){
		usleep(200000);

		if(m.readButton('1') == 0){
			m.allLedOn();
			reward = 1;
			cout << setw(3) << setfill('0') << trial << " OK" << endl;
			break;
		}else if(m.readButton('2') == 0){
			m.allLedOff();
			reward = -1;
			cout << setw(3) << setfill('0') << trial << " NG" << endl;
			break;
		}
	}

	m.giveReward(reward);
	m.observe();
	m.sensorUpdate();

	stringstream ss;
	ss << "/var/tmp/trial." << setw(4) << setfill('0') << trial;
	ofstream ofs(ss.str().c_str());
	m.printLastTrial(&ofs);
}

