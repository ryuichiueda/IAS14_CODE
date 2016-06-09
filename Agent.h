#ifndef _AGENT__H__
#define _AGENT__H__

#include <vector>
#include <string>
using namespace std;

class Event;

typedef vector<Event> Episode;

class ParticleFilter;

class Agent {
public:
	Agent(ifstream *ifs);
	~Agent();

	string decision(char state,bool food);

	void print(ofstream *ofs);
	void printLastTrial(ofstream *ofs);

protected:
	Episode m_episode;
	ParticleFilter* m_pf;

	string randomDecision(char state);

};

/*

map 

---------
3   2   4
---   ---
  | 1 |

*/
#endif
