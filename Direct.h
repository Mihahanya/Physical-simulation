#pragma once

#include "h.h"

class Direct
{
public:
	vec2 beg, end, normal, direction, coordinates;
	float angle;

	Direct(vec2 ibeg, vec2 iend) {
		this->beg = ibeg; this->end = iend;
		init();
	}

	float y_by_x(float x) {
		float y = -(a*x + c) / b;
		return y;
	}

	float x_by_y(float y) {
		float x = -(b*y + c) / a;
		return x;
	}

	tuple<vec2, bool> collised(Direct dir) {
		if (direction == dir.direction or dir.beg == dir.end) return { vs::zero, false };

		float den = a*dir.b - dir.a*b,
			  x = - (c*dir.b - dir.c*b) / den,
			  y = - (a*dir.c - dir.a*c) / den;
		
		vec2 cross = vec2(x, y);

		return { cross, !(out_of_p(cross) or dir.out_of_p(cross)) };
	}
	
	inline bool out_of_x(float x) {	return x < lx or x > rx; }
	inline bool out_of_y(float y) {	return y < uy or y > dy; }
	inline bool out_of_p(vec2 v) { return out_of_x(v.x) or out_of_y(v.y); }

protected:
	float a, b, c, lx, rx, uy, dy;

	void init() {
		a = beg.y-end.y;
		b = end.x-beg.x;
		c = -a * beg.x - b * beg.y;

		normal = vs::norm(vec2(a, b));

		coordinates = end - beg;

		direction = (end == beg) ? vs::zero : vs::norm(coordinates);

		angle = atan(coordinates.y/coordinates.x); //

		lx = min(beg.x, end.x); rx = max(beg.x, end.x);
		uy = min(beg.y, end.y); dy = max(beg.y, end.y);		
	}
};
