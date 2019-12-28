
#include "Swarm.h"

namespace caveofprogramming {

Swarm::Swarm(): lastTime(0) {
//this is going to allocate memory & also fill that memory with bunch of particle objects
	m_pParticles = new Particle[NPARTICLES];
}

Swarm::~Swarm() {
	delete[] m_pParticles;
}
void Swarm::update(int elapsed){
	int interval = elapsed - lastTime;
	for(int i=0; i<Swarm::NPARTICLES; i++){
		m_pParticles[i].update(interval);
	}
		lastTime = elapsed;
}

} /* namespace caveofprogramming */
