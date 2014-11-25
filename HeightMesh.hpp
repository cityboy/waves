//----
// HeightMesh.hpp
//----

#ifndef HEIGHTMESH_HPP
#define HEIGHTMESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

class HeightMesh {
	public:
		HeightMesh(unsigned int xc, unsigned int zc, float xl, float zl);
		~HeightMesh();
		GLuint VAO();
		void Display();
		virtual void AddWave();
		virtual void Simulate(float dt);
		virtual void Randomize(unsigned int n);
	protected:
		unsigned int x_sz, z_sz;
		float x_inc;
		float z_inc;
		GLuint id[3];
		unsigned int* index;
		glm::vec3* vertex;
		unsigned int array_sz, index_sz;
};

#endif

