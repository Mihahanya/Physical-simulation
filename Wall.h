#pragma once

#include "Scene.h"
#include "Direct.h"

enum WallOrient {
	Horizontal,
	Vertical
};

class Wall : public Direct, public Drawable
{
public:
	float drop_zone = 20;
	WallOrient s_orient;

	Wall(vec2 ibeg, vec2 iend, bool is_dynamic);

	void draw(Color color);

	void show_normals();
private:
	bool is_dynamic;

	void init();
};


Wall::Wall(vec2 ibeg, vec2 iend, bool is_dynamic=false) : Direct(ibeg, iend), is_dynamic(is_dynamic) {
	init();
}

void Wall::init() {
	init_param();

	s_orient = (direction.x > direction.y) ? Horizontal : Vertical;
}

void Wall::draw(Color color=Color::Black) {
	if (is_dynamic) init();
	ff::easy_line(beg, end, *window);
}

void Wall::show_normals() {
	vec2 mid = (beg + end) / 2.f;
	ff::easy_line(mid + normal * 10.f, mid, *window, Color(255, 0, 255));
	ff::easy_line(mid + normal * 10.f, mid, *window, Color(255, 0, 255));
}
