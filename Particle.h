#ifndef __PARTICLE_H_
#define __PARTICLE_H_

class Particle
{
public:
	Particle(double w);
	virtual ~Particle(){}
	double m_w;

	int m_experience_index;
};

#endif
