#pragma once

#include "Scene.h"
#include "Direct.h"

enum WallOrient {
	Horizontal,
	Vertical,
    NoneOrient
};

class Wall : public Direct, public Drawable
{
public:
	float drop_zone = 20;
	WallOrient orient;
	bool clockwise=false;

	Wall(vec2 ibeg, vec2 iend, bool is_dynamic);

	void draw_update();
	void draw() override;
	void show_normals() const;

	bool vertical_ingress(vec2) const;
	bool horizontal_ingress(vec2) const;

private:
	bool is_dynamic;
	vec2 old_beg, old_end;

	void init();
};


Wall::Wall(vec2 ibeg, vec2 iend, bool is_dynamic=false) : Direct{ibeg, iend}, Drawable{}, is_dynamic{is_dynamic} {
	init();
}

void Wall::init() {
	old_beg, old_end = beg, end;

	init_param();

	orient = (abs(direction.x) > abs(direction.y)) ? Vertical : Horizontal;
    if (beg == end) orient = NoneOrient;

    if (orient == Vertical) clockwise = direction.x < 0;
    else if (orient == Horizontal) clockwise = direction.y < 0;
}

bool Wall::vertical_ingress(vec2 p) const {
	const float match_y = y_by_x(p.x);

	return !out_of_p(vec2(p.x, match_y)) and
		   ((clockwise and p.y > match_y and p.y < match_y + drop_zone) or
		   (!clockwise and p.y < match_y and p.y > match_y - drop_zone));
}

bool Wall::horizontal_ingress(vec2 p) const {
	const float match_x = x_by_y(p.y);

	return !out_of_p(vec2(match_x, p.y)) and
		   ((clockwise and p.x < match_x and p.x > match_x - drop_zone) or
		   (!clockwise and p.x > match_x and p.x < match_x + drop_zone));
}

void Wall::draw_update() {
	if (is_dynamic and (beg != old_beg or end != old_end)) init();
	draw();
}

void Wall::draw() {
	ff::easy_line(beg, end, *window, color);
}

void Wall::show_normals() const {
	const vec2 mid = (beg + end) / 2.f;
	ff::easy_line(mid + normal * 10.f, mid, *window, Color(255, 0, 255));

	vec2 drop{};
	if (orient == Vertical) {
		if (clockwise) drop = vec2{0, 1};
		else drop = vec2{0, -1};
	}
	else if (orient == Horizontal) {
		if (clockwise) drop = vec2{-1, 0};
		else drop = vec2{1, 0};
	}
	ff::easy_line(mid + drop*drop_zone, mid, *window, Color(0, 255, 0));
}
