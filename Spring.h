#pragma once

#include "PPoint.h"

class Spring
{
public:
	PPoint *p1, *p2;
	float elasticity, resistance_f, distance;

	Spring(PPoint &p1, PPoint &p2, float elasticity, float resistance_f) {
		this->p1 = &p1; this->p2 = &p2;
		this->elasticity = elasticity;
		this->resistance_f = resistance_f;
		fluctuation = zero;
		distance = dist((*this->p1).pos, (*this->p2).pos);
	}

	void calculate_force() {
		vec2 v = ((*p2).pos-(*p1).pos) * (dist((*p1).pos, (*p2).pos)-distance);
        vec2 f_e = v * elasticity;

		vec2 fl = fluctuation*resistance_f;

        vel1 = f_e - fl;
        vel2 = -f_e + fl;

		fluctuation = f_e - fl;
	}

	void add_force() {
		(*p1).move(vel1);
        (*p2).move(vel2);
	}

private:
	vec2 fluctuation, vel1, vel2;
};
