#include "Agent.h"
#include "Event.h"
#include "ParticleFilter.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

Agent::Agent(ifstream *ifs)
{
	srand(time(NULL));
	m_pf = new ParticleFilter(ifs);
}

Agent::~Agent()
{
	delete m_pf;
}

string Agent::decision(char state, bool food)
{
	return "left";
}

string Agent::randomDecision(char state)
{
	if(rand()%2)
		return "left";

	return "right";
}

void Agent::print(ofstream *ofs)
{
	int n = -1;
	for(auto e : m_episode){
		n++;
		*ofs << e.trial_number << " " << n << " ";
		for(int i=0;i<4;i++)
			*ofs << e.sensor_values[i] << " ";
		*ofs << e.reward << " " << e.action << endl;
	}
}

void Agent::printLastTrial(ofstream *ofs)
{
	int id = m_episode.back().trial_number;

	int n = -1;
	for(auto e : m_episode){
		n++;
		if(e.trial_number != id)
			continue;

		*ofs << e.trial_number << " " << n << " ";
		for(int i=0;i<4;i++)
			*ofs << e.sensor_values[i] << " ";
		*ofs << e.reward << " " << e.action << endl;
	}
}
