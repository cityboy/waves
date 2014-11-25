//----
// SineWaveMesh.cpp
//----

#include "GerstnerWaveMesh.hpp"
#include <math.h>
#include <stdlib.h>
#include <time.h>

GerstnerWaveMesh::GerstnerWaveMesh (unsigned int xc, unsigned int zc, float xl, float zl) :
		HeightMesh(xc,zc,xl,zl) {
	cum_time = 0.0f;
}

GerstnerWaveMesh::~GerstnerWaveMesh () {}

void GerstnerWaveMesh::AddWave(glm::vec2 D, float A, float L, float s, float Q) {
	GerstnerWave wave;
	wave.dir = D;
	wave.amp = A;
	wave.wlength = L;
	wave.speed = s;
	wave.freq = 2.0f * M_PI / L;
	wave.phase = s * wave.freq;
	wave.Q = Q;
	waves.push_back(wave);
	Simulate(0.0f);	
}

void GerstnerWaveMesh::Randomize (unsigned int n, float max_A, float min_L, float max_L, float max_s) {
	glm::vec2 D;
	float A, L, s, Q;
	float max_rand = (float)RAND_MAX;
	srand(time(NULL));
	for (unsigned int i=0; i<n; i++) {
		D = glm::vec2((float)rand()/max_rand,(float)rand()/max_rand);
		A = (float)rand()/max_rand * max_A;
		L = (float)rand()/max_rand * (max_L-min_L) + min_L;
		s = (float)rand()/max_rand * max_s;
		Q = (float)rand()/max_rand * (L/(A*2.0f*M_PI));
		AddWave(D,A,L,s,Q);
	}
}

void GerstnerWaveMesh::Simulate(float dt) {
	cum_time += dt;
	glm::vec3 *vptr = vertex;
	float x,z;
	float x_start = x_inc * (float)(x_sz-1) / 2.0f;
	float z_start = z_inc * (float)(z_sz-1) / 2.0f;
	for (unsigned int i=0; i<x_sz; i++) {
		for (unsigned int k=0; k<z_sz; k++) {
			vptr->x = x = -x_start + (float)i * x_inc;
			vptr->y = 0.0f;
			vptr->z = z = -z_start + (float)k * z_inc;
			for (unsigned int c=0; c<waves.size(); c++) {
				vptr->x += waves[c].Q * waves[c].amp * waves[c].dir.x * cos((waves[c].dir.x*x+waves[c].dir.y*z)*waves[c].freq + cum_time*waves[c].phase);
				vptr->y +=              waves[c].amp *                    sin((waves[c].dir.x*x+waves[c].dir.y*z)*waves[c].freq + cum_time*waves[c].phase);
				vptr->z += waves[c].Q * waves[c].amp * waves[c].dir.y * cos((waves[c].dir.x*x+waves[c].dir.y*z)*waves[c].freq + cum_time*waves[c].phase);
			}
			vptr++;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, id[1]);
	glBufferData(GL_ARRAY_BUFFER, array_sz * sizeof(glm::vec3), vertex, GL_STATIC_DRAW);
}

