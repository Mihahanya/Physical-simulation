#pragma once

#include "PhysicalPoint.h"

class Spring
{
public:
	PPoint *p1, *p2;
	float elasticity, resistance, distance;

	Spring(PPoint *p1, PPoint *p2, float elasticity, float resistance);

	void calculate_force();
	void add_force();

private:
	vec2 fluctuation, vel1, vel2;
};


Spring::Spring(PPoint* p1, PPoint* p2, float elasticity, float resistance) :
	p1{p1}, p2{p2}, elasticity{elasticity}, resistance{resistance}
{
	distance = vs::dist(p1->pos, p2->pos);
}

void Spring::calculate_force() {
	const vec2 v = (p2->pos - p1->pos) * (vs::dist(p1->pos, p2->pos) - distance);
	const vec2 f_e = v * elasticity;

	const vec2 fl = fluctuation * resistance;

	vel1 = f_e - fl;
	vel2 = -f_e + fl;

	fluctuation = f_e - fl;
}

void Spring::add_force() {
	p1->add_force(vel1);
	p2->add_force(vel2);
}
