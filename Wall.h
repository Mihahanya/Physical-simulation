#pragma once
#include "h.h"

class Wall
{
public:
	vec2 beg, end;
	int ERR = INT32_MAX;
	bool upper;

	Wall(vec2 begining, vec2 end, bool upper=false) {
		beg = begining; this->end = end;
		this->upper = upper;
			
		lx = min(begining.x, end.x); rx = max(begining.x, end.x);
		uy = min(begining.y, end.y); dy = max(begining.y, end.y);
	}

	void draw(RenderWindow &window, Color color=Color::Black) {
		Vertex vtx[2] = { Vertex(beg), Vertex(end) };
		vtx[0].color = vtx[1].color = color;
		window.draw(vtx, 2, Lines);
	}

	float y_by_x(float x) {
		if (x < lx or x > rx) return ERR; 
		float k = (end.y-beg.y) / (end.x-beg.x),
			  b = beg.y - k*beg.x,
			  y = k*x + b; // y=xv v=y/x
		return (y > dy or y < uy) ? ERR : y;
	}

	float x_by_y(float y) {
		if (y > dy or y < uy) return ERR;
		float k = (end.x-beg.x) / (end.y-beg.y),
			  b = beg.x - k*beg.y,
			  x = k*y + b;
		return (x < lx or x > rx) ? ERR : x;
	}
private:
	float lx, rx, uy, dy;
};
