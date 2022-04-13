#pragma once

#include "h.h"

class Direct
{
public:
	vec2 beg, end, normal, direction;
	float k, b, angle, lx, rx, uy, dy;

	Direct(vec2 ibeg, vec2 iend) {
		this->beg = ibeg; this->end = iend;
		init();
	}

	optional<float> y_by_x(float x) {
		if (x < lx or x > rx) return nullopt; 
		float y = k*x + b;
		if (y > dy or y < uy) return nullopt;
		return y;
	}

	optional<float> x_by_y(float y) {
		if (y > dy or y < uy) return nullopt; 
		float x = (y - b) / k;
		if (x < lx or x > rx) return nullopt; 
		return x;
	}

	tuple<vec2, bool> collised(vec2 from, vec2 to) {
		Direct path(from, to);
		bool contact = true;
		if (k == path.k) contact = false;

		float collised_x = (path.b - b) / (k - path.k);
		if (out_of_x(collised_x) or path.out_of_x(collised_x)) contact = false;

		float collised_y = k * collised_x + b;
		if (out_of_y(collised_y) or path.out_of_y(collised_y)) contact = false;

		return { vec2(collised_x, collised_y), contact };
	}
	
	inline bool out_of_x(float x) {	return x < lx or x > rx; }
	inline bool out_of_y(float y) {	return y < uy or y > dy; }

protected:
	void init() {
		if (abs(end.x - beg.x) < 0.5) beg.x += 1;

		k = (end.y-beg.y) / (end.x - beg.x);
		b = beg.y - k*beg.x;

		if (abs(end.x - beg.x) < 1.5) normal = vec2(1, 0);
		else if (beg.y == end.y) normal = vec2(0, 1);
		else normal = norm(vec2(1, -1/k));

		direction = norm(vec2(1, k));

		lx = min(beg.x, end.x); rx = max(beg.x, end.x);
		uy = min(beg.y, end.y); dy = max(beg.y, end.y);
		
		angle = atan(k);
	}
};


class Wall : public Direct
{
public:
	int orient;
	float drop_zone;

	Wall(vec2 ibeg, vec2 iend, int orient=0) : Direct(ibeg, iend) {
		this->orient = orient;
		this->drop_zone = drop_zone;
	}

	void show_normals() {
		vec2 mid = (beg+end)/2.f;
		(*window).draw(easy_line(mid+normal*-10.f, mid+normal*10.f, Color(255, 0, 255)), 2, LinesStrip);
	}

	void draw(Color color=Color::Black) {
		Vertex vtx[2] ={Vertex(beg), Vertex(end)};
		vtx[0].color = vtx[1].color = color;
		(*window).draw(vtx, 2, Lines);
	}
    
	void add_window(RenderWindow &window) { this->window = &window; }

private:
    RenderWindow *window;
};
