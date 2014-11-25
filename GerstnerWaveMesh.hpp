//----
// SineWaveMesh.hpp
//----

#ifndef GERSTNERWAVEMESH_HPP
#define GERSTNERWAVEMESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "HeightMesh.hpp"

struct GerstnerWave {
	glm::vec2 dir;
	float amp;
	float wlength;
	float speed;
	float Q;
	float phase;
	float freq;
};

class GerstnerWaveMesh: public HeightMesh {
	public:
		GerstnerWaveMesh(unsigned int xc, unsigned int zc, float xl, float zl);
		~GerstnerWaveMesh();
		void AddWave(glm::vec2 D, float A, float L, float s, float Q);
		void Simulate(float dt);
		void Randomize(unsigned int n, float max_A, float min_L, float max_L, float max_s);
	private:
		float cum_time;
		std::vector<GerstnerWave> waves;
};

#endif

