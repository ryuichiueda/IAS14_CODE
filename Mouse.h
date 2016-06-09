#ifndef _MOUSE__H__
#define _MOUSE__H__

#include <vector>
#include <string>
#include <fstream>
#include "Agent.h"
using namespace std;

class ParticleFilter;

class Mouse : public Agent {
public:
	Mouse(ifstream *ifs);
	virtual ~Mouse();

	int readButton(char number);

	void motorOn(void);
	void motorOff(void);
	void putMotorHz(int lvalue,int rvalue);

	void putLed(char no, char value);
	void forwardToWall(int th);
	void turnAvoidWall(char dir);

	void allLedOff(void);
	void allLedOn(void);


	void observe(void);
	void sensorUpdate(void);
	void giveReward(int r);

	void decision(ofstream *ofs = NULL);

	void debug(void);
/*
	void print(void);
*/
	void actionForward(void);

	void initTrial(void);

	int reload(string dirname);

private:
	//void readSensor(int *ls1, int *ls2, int *ls3, int *ls4);
	void readSensor(vector<int> &sv);
	int m_trial_id;
/*
	Episode m_episode;
	ParticleFilter* m_pf;

	string randomDecision(char state);
*/
};

#endif
