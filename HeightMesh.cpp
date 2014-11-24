//----
// HeightMesh.cpp
//----

#include "HeightMesh.hpp"

HeightMesh::HeightMesh (unsigned int xc, unsigned int zc, float xl, float zl) {
	x_sz = xc; 
	z_sz = zc;
	x_inc = xl / (float)(xc-1);
	z_inc = zl / (float)(zc-1);

	array_sz = x_sz*z_sz;  //9
	index_sz = x_sz*(z_sz-1)*2 + (x_sz-1)*z_sz*2;  //24

	height = new float[array_sz]; 
	//vertex = new glm::vec3[array_sz];
	vertex = new float[array_sz*3];
	index = new unsigned int[index_sz];

	//-- Construct vertex data
	float x_start = xl / 2.0f;
	float z_start = zl / 2.0f;
	float *vptr = vertex;
	for (unsigned int i=0; i<x_sz; i++) {
		for (unsigned int k=0; k<z_sz; k++) {
			height[i*z_sz+k] = 0.0f;
			*vptr++ = -x_start + (float)i * x_inc;
			*vptr++ = height[i*z_sz+k];
			*vptr++ = -z_start + (float)k * z_inc;
		}
	}
/*
	x_inc = xl / 2.0f;
	z_inc = zl / 2.0f;
	float *vptr = vertex;
	*vptr++ = -x_inc;  *vptr++ = 0.0f;  *vptr++ = -z_inc;
	*vptr++ = -x_inc;  *vptr++ = 0.0f;  *vptr++ = 0.0f;
	*vptr++ = -x_inc;  *vptr++ = 0.0f;  *vptr++ = z_inc;
	*vptr++ = 0.0f;    *vptr++ = 0.0f;  *vptr++ = -z_inc;
	*vptr++ = 0.0f;    *vptr++ = 0.0f;  *vptr++ = 0.0f;
	*vptr++ = 0.0f;    *vptr++ = 0.0f;  *vptr++ = z_inc;
	*vptr++ = x_inc;   *vptr++ = 0.0f;  *vptr++ = -z_inc;
	*vptr++ = x_inc;   *vptr++ = 0.0f;  *vptr++ = 0.0f;
	*vptr++ = x_inc;   *vptr++ = 0.0f;  *vptr++ = z_inc;
*/
	//-- Construct index data
	unsigned int *ptr = index;
	for (unsigned int i=0; i<x_sz; i++) {
		for (unsigned int k=0; k<z_sz-1; k++) {
			*ptr++ = i*z_sz + k;
			*ptr++ = i*z_sz + k+1;
		}
	}
	for (unsigned int k=0; k<z_sz; k++) {
		for (unsigned int i=0; i<x_sz-1; i++) {
			*ptr++ = i*z_sz + k;
			*ptr++ = (i+1)*z_sz + k;
		}
	}
/*
	unsigned int *iptr = index;
	*iptr++ = 0; 	*iptr++ = 1;
	*iptr++ = 1; 	*iptr++ = 2;
	*iptr++ = 3; 	*iptr++ = 4;
	*iptr++ = 4; 	*iptr++ = 5;
	*iptr++ = 6; 	*iptr++ = 7;
	*iptr++ = 7; 	*iptr++ = 8;
	*iptr++ = 0; 	*iptr++ = 3;
	*iptr++ = 3; 	*iptr++ = 6;
	*iptr++ = 1; 	*iptr++ = 4;
	*iptr++ = 4; 	*iptr++ = 7;
	*iptr++ = 2; 	*iptr++ = 5;
	*iptr++ = 5; 	*iptr++ = 8;
*/
	//-- VAO
	glGenVertexArrays(1, id);
	glBindVertexArray(id[0]);
	//-- VBO
	glGenBuffers(2, id+1);
	glBindBuffer(GL_ARRAY_BUFFER, id[1]);
	glBufferData(GL_ARRAY_BUFFER, array_sz * 3 * sizeof(float), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_sz * sizeof(unsigned int), index, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

}

HeightMesh::~HeightMesh () {
	delete[] height;
}

GLuint HeightMesh::VAO () { return id[0]; }

void HeightMesh::Display () {
	glBindVertexArray(id[0]);
	glDrawElements(GL_LINES, index_sz, GL_UNSIGNED_INT, (void*)0);
/*
	glBindVertexArray(id[0]);
	glDrawElements(GL_LINES, index_sz, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
*/
}


