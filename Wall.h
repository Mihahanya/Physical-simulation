#pragma once

#include "h.h"
#include "Direct.h"

class Wall : public Direct
{
public:
	int orient;
	float drop_zone;

	Wall(vec2 ibeg, vec2 iend) : Direct(ibeg, iend) {
		this->orient = orient;
		drop_zone = 20;
	}

	void draw(Color color=Color::Black) {
		ff::easy_line(beg, end, *window);
	}

	void show_normals() {
		vec2 mid = (beg+end)/2.f;
		ff::easy_line(mid+normal*0.f, mid+normal*10.f, *window, Color(255, 0, 255));
	}
    
	void add_window(RenderWindow &window) { this->window = &window; }

private:
    RenderWindow *window = nullptr;
};
