#ifndef __PF_H_
#define __PF_H_

#include <vector>
#include <fstream>
using namespace std;

class Particle;
class Event;
typedef vector<Event> Episode;

class ParticleFilter
{
public:
	ParticleFilter(ifstream *ifs);
	virtual ~ParticleFilter();

	void sensorUpdate(Episode *e);
	void motionUpdate(Episode *e);

	void normalizeWeights(void);
/*
	virtual void sensorUpdate(SensorData *data) = 0;
	virtual void normalizeWeights(void);
	void solveResamplingList(int *list);

	vector<Particle *> *getParticles(void){return &m_particles;}
*/
	vector<Particle> m_particles;

	void print(ofstream *ofs);

	//double getWeight(Episode *e,string action);
	double getFuture(Episode *e,string action);

	void reset(Episode *e);
private:
	unsigned int getIntRand();
	double getDoubleRand();
	ifstream *m_rand_ifs;

	void randomReset(Episode *e);
	void retrospectiveReset(Episode *e);

	void resampling(void);
	double likelihood(vector<int> &cur, vector<int> &past);
};

#endif
