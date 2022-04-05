#pragma once
#include "h.h"

class Wall
{
public:
	vec2 beg, end, normal, direction;
	const int OUT = INT32_MAX;
	bool upper;
	float k, b, alpha;

	Wall(vec2 begining, vec2 end, bool upper=false) {
		beg = begining; this->end = end+vec2(1, 1);
		this->upper = upper;
			
		lx = min(begining.x, end.x); rx = max(begining.x, end.x);
		uy = min(begining.y, end.y); dy = max(begining.y, end.y);

		k = (end.y-beg.y) / (end.x-beg.x);
		b = beg.y - k*beg.x;

		alpha = atan(k);

		normal = norm(vec2(1, -1/k));
		direction = norm(vec2(1, k));
	}

	void draw(Color color=Color::Black) {
		Vertex vtx[2] ={Vertex(beg), Vertex(end)};
		vtx[0].color = vtx[1].color = color;
		(*window).draw(vtx, 2, Lines);
	}

	void show_normals() {
		vec2 mid = (beg+end)/2.f;
		(*window).draw(easy_line(mid, mid+normal*10.f, Color(255, 0, 255)), 2, LinesStrip);
	}

    void add_window(RenderWindow &window) { this->window = &window; }

	float y_by_x(float x) {
		if (x < lx or x > rx) return OUT; 
		float y = k*x + b;
		return (y > dy or y < uy) ? OUT : y;
	}

	float x_by_y(float y) {
		if (y > dy or y < uy) return OUT; 
		float x = (y - b) / k;
		return (x < lx or x > rx) ? OUT : x;
	}

private:
	float lx, rx, uy, dy;
    RenderWindow *window;
};
