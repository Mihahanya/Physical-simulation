#pragma once

#include "PhysicalPoint.h"

class Spring {
public:
	Spring(PPoint *p1, PPoint *p2, float elasticity, float resistance, float max_force_dist);

	virtual void calculate_force();
	virtual void add_force();

	virtual void set_elasticity(float el) { elasticity = el; }
	virtual void set_resistance(float re) { resistance = re; }
	virtual void set_distance(float d)    { distance = d; }
	
	float get_elasticity() const { return elasticity; }
	float get_resistance() const { return resistance; }
	float get_distance()   const { return distance; }
	
	void set_p1(PPoint* p) { p1 = p; }
	void set_p2(PPoint* p) { p2 = p; }

protected:
	Spring(float elasticity, float resistance) : elasticity{elasticity}, resistance{resistance} {}

	float elasticity, resistance, distance;

private:
	PPoint *p1{nullptr}, *p2{nullptr};
	vec2 fluctuation, f_1, f_2;
};


Spring::Spring(PPoint* p1, PPoint* p2, float elasticity, float resistance, float max_force_dist=300) :
	p1{p1}, p2{p2}, elasticity{elasticity}, resistance{resistance}
{
	distance = vs::dist(p1->get_pos(), p2->get_pos());
}

void Spring::calculate_force() {
	const vec2 p2_pos = p2->get_pos(), p1_pos = p1->get_pos();
	const float current_dist = vs::dist(p1_pos, p2_pos);

	const vec2 f_y = -vs::norm(p2_pos - p1_pos) * (distance-current_dist) * elasticity, // Fy = -xk
	//		   f_c = (p2->get_vel()-p1->get_vel()) * resistance; // Fc = -vc
			   f_c = -fluctuation * resistance; // Fc = -vc

	f_1 = f_y + f_c;
	f_2 = -f_y - f_c;

	fluctuation = f_y;
}

void Spring::add_force() {
	p1->add_force(f_1);
	p2->add_force(f_2);
}
