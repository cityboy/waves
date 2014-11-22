//----
// ArcBall.cpp
//----

#include "ArcBall.hpp"

ArcBall::ArcBall (double w, double h) {
	SetScreenSize(w,h);
}

ArcBall::~ArcBall() {}

void ArcBall::SetScreenSize (double w, double h) {
	screen_width = w;
	screen_height = h;
	half_diameter = sqrt(screen_width*screen_width + screen_height*screen_height) * 0.5f;
}

glm::vec3 ArcBall::GetVector (double x, double y) {
	x = (x-screen_width/2.0f) / half_diameter;
	y = (screen_height/2.0f-y) / half_diameter;
	double z = sqrt(1-x*x-y*y);
	return glm::vec3(x,y,z);
}


