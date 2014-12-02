//----
// ArcBall.hpp
//----

#ifndef ARCBALL_HPP
#define ARCBALL_HPP

#include <glm/glm.hpp>

class ArcBall {
	public:
		ArcBall(double w, double h);
		~ArcBall();
		void SetScreenSize(double w, double h);
		glm::vec3 GetVector(double x, double y);
	private:
		double half_width, half_height;
		double radius;
};

#endif
