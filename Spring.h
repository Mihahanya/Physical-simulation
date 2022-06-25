#pragma once

#include "PhysicalPoint.h"

class Spring
{
public:
	PPoint *p1, *p2;
	float elasticity, resistance, distance;

	Spring(PPoint *p1, PPoint *p2, float elasticity, float resistance) : 
		p1(p1), p2(p2), elasticity(elasticity), resistance(resistance) 
	{
		distance = vs::dist(p1->pos, p2->pos);
	}

	void calculate_force() {
		vec2 v = (p2->pos - p1->pos) * (vs::dist(p1->pos, p2->pos)-distance);
        vec2 f_e = v * elasticity;

		vec2 fl = fluctuation * resistance;

        vel1 = f_e - fl;
        vel2 = -f_e + fl;

		fluctuation = f_e - fl;
	}

	void add_force() {
		p1->move(vel1);
        p2->move(vel2);
	}

private:
	vec2 fluctuation, vel1, vel2;
};
