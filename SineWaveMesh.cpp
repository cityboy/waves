//----
// SineWaveMesh.cpp
//----

#include "SineWaveMesh.hpp"
#include "math.h"

SineWaveMesh::SineWaveMesh (unsigned int xc, unsigned int zc, float xl, float zl) :
		HeightMesh(xc,zc,xl,zl) {
	cum_time = 0.0f;
}

SineWaveMesh::~SineWaveMesh () {}

void SineWaveMesh::AddWave(glm::vec2 D, float A, float L, float s) {
	SineWave wave;
	wave.dir = D;
	wave.amp = A;
	wave.wlength = L;
	wave.speed = s;
	wave.freq = 2.0f * M_PI / L;
	wave.phase = s * wave.freq;
	waves.push_back(wave);
	Simulate(0.0f);	
}

void SineWaveMesh::Simulate(float dt) {
	cum_time += dt;
	glm::vec3 *vptr;
	for (unsigned int i=0; i<x_sz; i++) {
		for (unsigned int k=0; k<z_sz; k++) {
			vptr = vertex + i*z_sz + k;
			vptr->y = 0.0f;
			for (unsigned int c=0; c<waves.size(); c++) {
				vptr->y += waves[c].amp * pow((sin((waves[c].dir.x*vptr->x+waves[c].dir.y*vptr->z)*waves[c].freq + cum_time*waves[c].phase)+1.0f)/2.0f,4.0f);
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, id[1]);
	glBufferData(GL_ARRAY_BUFFER, array_sz * sizeof(glm::vec3), vertex, GL_STATIC_DRAW);
}

