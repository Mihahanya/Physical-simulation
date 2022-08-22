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
	virtual void set_max_force_dist(float mfd) { max_force_dist = mfd; }
	
	float get_elasticity() const { return elasticity; }
	float get_resistance() const { return resistance; }
	float get_distance()   const { return distance; }
	float get_max_force_dist() const { return max_force_dist; }
	
	void set_p1(PPoint* p) { p1 = p; }
	void set_p2(PPoint* p) { p2 = p; }

protected:
	Spring(float elasticity, float resistance) : elasticity{elasticity}, resistance{resistance} {}

	float elasticity, resistance, distance, max_force_dist;

private:
	PPoint *p1{nullptr}, *p2{nullptr};
	vec2 fluctuation, vel1, vel2;
};


Spring::Spring(PPoint* p1, PPoint* p2, float elasticity, float resistance, float max_force_dist=300) :
	p1{p1}, p2{p2}, elasticity{elasticity}, resistance{resistance}, max_force_dist{max_force_dist}
{
	distance = vs::dist(p1->get_pos(), p2->get_pos());
}

void Spring::calculate_force() {
	const vec2 p2_pos = p2->get_pos(), p1_pos = p1->get_pos();
	const float current_dist = min(vs::dist(p1_pos, p2_pos), max_force_dist);

	const vec2 f_e = (p2_pos - p1_pos) * (current_dist - distance) * elasticity;
	const vec2 fl = fluctuation * resistance;

	vel1 = f_e - fl;
	vel2 = -f_e + fl;

	fluctuation = f_e - fl;
}

void Spring::add_force() {
	p1->add_force(vel1);
	p2->add_force(vel2);
}
