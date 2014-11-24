//----
// SineWaveMesh.hpp
//----

#ifndef SINEWAVEMESH_HPP
#define SINEWAVEMESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "HeightMesh.hpp"

struct SineWave {
	glm::vec2 dir;
	float amp;
	float wlength;
	float speed;
	float phase;
	float freq;
};

class SineWaveMesh: public HeightMesh {
	public:
		SineWaveMesh(unsigned int xc, unsigned int zc, float xl, float zl);
		~SineWaveMesh();
		void AddWave(glm::vec2 D, float A, float L, float s);
		void Simulate(float dt);
	private:
		float cum_time;
		std::vector<SineWave> waves;
};

#endif

