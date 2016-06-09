#include "Mouse.h"
#include "Particle.h"
#include "ParticleFilter.h"
#include "Event.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <numeric>
using namespace std;

Mouse::Mouse(ifstream *ifs) : Agent(ifs)
{
	m_trial_id = 0;
}

Mouse::~Mouse()
{
}

int Mouse::reload(string dirname)
{
	int trial_no = 0;
	for(int n=1;n<10000;n++){
		stringstream ss;
		ss << dirname << "/trial." << setw(4) << setfill('0') << n;
		ifstream ifs(ss.str());
		if(!ifs.good())
			break;

		while(1){
			int step_no, s[4], rw;
			string action;
			ifs >> trial_no >> step_no 
				>> s[0] >> s[1] >> s[2] >> s[3] >> rw >> action;

			if(ifs.eof())
				break;
	
			Event e(trial_no);
			for(int i=0;i<4;i++)
				e.sensor_values.push_back(s[i]);
	
			e.reward = rw;
			e.action = action;
	
			m_episode.push_back(e);
		}

		ifs.close();
	}

	/* reload particles */
	int final_step = m_episode.size() - 1;
	stringstream ss;
	ss << dirname << "/particles." << final_step;
	ifstream ifs(ss.str());
	if(!ifs.good())
		exit(1);

	m_pf->m_particles.clear();

	while(1){
		int exp_num;
		double w;
		ifs >> exp_num >> w;

		if(ifs.eof())
			break;

		Particle p(0.0);
		p.m_experience_index = exp_num;
		p.m_w = w;

		m_pf->m_particles.push_back(p);
	}

	return trial_no+1;
}

void Mouse::debug(void)
{
	m_pf->reset(&m_episode);
}

void Mouse::initTrial(void)
{
	m_trial_id++;
	m_pf->motionUpdate(&m_episode);
}

void Mouse::sensorUpdate(void)
{
	m_pf->sensorUpdate(&m_episode);
	stringstream ss;
	ss << "/var/tmp/particles." << (int)m_episode.size() - 1;
	ofstream ofs(ss.str().c_str());
	m_pf->print(&ofs);
}

void Mouse::observe(void)
{
	Event e(m_trial_id);
	readSensor(e.sensor_values);
	m_episode.push_back(e);
}

void Mouse::giveReward(int r)
{
	m_episode.back().action = "stay";
	m_episode.back().reward = r;
}

void Mouse::actionForward(void)
{
	m_episode.back().action = "fw";
	forwardToWall(5000);
	m_pf->motionUpdate(&m_episode);
}

void Mouse::decision(ofstream *ofs)
{
	string ans;


	double r_weight = m_pf->getFuture(&m_episode,"right");
	double l_weight = m_pf->getFuture(&m_episode,"left");
	*ofs << "L " << l_weight << " R " << r_weight << endl;

	if(r_weight == l_weight)
		ans = rand()%2==0 ? "right" : "left";
	else if(r_weight > l_weight)
		ans = "right";
	else
		ans = "left";

	if(ans == "right"){
		turnAvoidWall('R');
	}else{
		turnAvoidWall('L');
	}

	m_episode.back().action = ans;
	m_pf->motionUpdate(&m_episode);
}

void Mouse::readSensor(vector<int> &sv)
{
	if(sv.size() == 0){
		for(int i=0;i<4;i++)
			sv.push_back(0);
	}

	while(1){
		ifstream ifs("/dev/rtlightsensor0");
		if(ifs.bad()){
			ifs.close();
			continue;
		}
	
		ifs >> sv[0] >> sv[1] >> sv[2] >> sv[3];
		ifs.close();
		return;
	}
}

int Mouse::readButton(char number)
{
	stringstream ss;
	ss << "/dev/rtswitch" << number;
	while(1){
		ifstream ifs(ss.str().c_str());
		if(ifs.bad()){
			ifs.close();
			continue;
		}

		int val;	
		ifs >> val;
		ifs.close();
		return val;
	}
	return 1;
}

void Mouse::motorOn(void)
{
	ofstream motor("/dev/rtmotoren0");
	motor << 1;
	motor.close();
}

void Mouse::motorOff(void)
{
	ofstream motor("/dev/rtmotoren0");
	motor << 0;
	motor.close();
}

void Mouse::putMotorHz(int lvalue,int rvalue)
{
	ofstream r_motor("/dev/rtmotor_raw_r0");
	ofstream l_motor("/dev/rtmotor_raw_l0");
	r_motor << rvalue;
	l_motor << lvalue;
	r_motor.close();
	l_motor.close();
}

void Mouse::allLedOn(void)
{
	putLed('0','1');
	putLed('1','1');
	putLed('2','1');
	putLed('3','1');
}

void Mouse::allLedOff(void)
{
	putLed('0','0');
	putLed('1','0');
	putLed('2','0');
	putLed('3','0');
}

void Mouse::putLed(char no, char value)
{
	stringstream ss;
	ss << "/dev/rtled" << no;
	ofstream led(ss.str().c_str());
	led << value;
	led.close();
}

void Mouse::forwardToWall(int th)
{
	putMotorHz(400,400);

	//int ls1,ls2,ls3,ls4;
	vector<int> sv;
	while(1){
		usleep(200000);
		//readSensor(&ls1,&ls2,&ls3,&ls4);
		readSensor(sv);

	//	if(ls1 + ls2 + ls3 + ls4 > th){
		if(accumulate(sv.begin(), sv.end(), 0) > th){
			putMotorHz(0,0);
			return;
		}
	}
}

void Mouse::turnAvoidWall(char dir)
{
	if(dir == 'R')
		putMotorHz(300,-300);
	else
		putMotorHz(-300,300);

	//int ls1,ls2,ls3,ls4;
	vector<int> sv;
	for(int i=0;i<15;i++){
		usleep(200000);
		readSensor(sv);

		if(sv[0] < 300 && sv[3] < 300){
			putMotorHz(0,0);
			return;
		}
	}
	putMotorHz(0,0);
}
