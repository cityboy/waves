//----
// ArcBall.cpp
//----

#include "ArcBall.hpp"

ArcBall::ArcBall (double w, double h) {
	SetScreenSize(w,h);
}

ArcBall::~ArcBall() {}

void ArcBall::SetScreenSize (double w, double h) {
	half_width = w / 2.0f;
	half_height = h / 2.0f;
	//radius = sqrt(screen_width*screen_width + screen_height*screen_height) * 0.5f;
	radius = (half_width<half_height) ? half_width : half_height;
}

glm::vec3 ArcBall::GetVector (double x, double y) {
//	x = (x - half_width) / radius;
//	y = (half_height - y) / radius;
	x = x / half_width - 1.0f;
	y = 1 - y / half_height;
	double r = x*x + y*y;
	double z;
	//-- If the mouse is outside the unit circle scale it back
	if (r>1.0f) {
		r = sqrt (r);
		x = x / r;
		y = y / r;
		z = 0.0f;
	} else {		
		z = sqrt (1 - r);
	}
	return glm::vec3(x,y,z);
}


