#pragma once

#include "Wall.h"

class VolumetricWall
{
public:
	vector<Wall> walls;
	float angle;

	VolumetricWall(vec2 pos, vec2 sizes, float angle);

	void move_to(vec2);

private:
	vec2 pos, sizes;
};


VolumetricWall::VolumetricWall(vec2 pos, vec2 sizes, float angle) : pos{pos}, sizes{sizes}, angle{angle}
{
	vector<vec2> ps {
		pos + vs::rotate(vec2(sizes.x, sizes.y), angle) / 2.f,
		pos + vs::rotate(vec2(-sizes.x, sizes.y), angle) / 2.f,
		pos + vs::rotate(vec2(-sizes.x, -sizes.y), angle) / 2.f,
		pos + vs::rotate(vec2(sizes.x, -sizes.y), angle) / 2.f,
	};
	std::reverse(ps.begin(), ps.end());

	float wdz = sizes.x / 3.f,
		hdz = sizes.y / 3.f;

	for (int i = 0; i < ps.size(); i++) {
		Wall w(ps[i], ps[(i == ps.size() - 1) ? 0 : i + 1], true);
		w.drop_zone = (i % 2 == 0) ? hdz : wdz;
		walls.push_back(w);
	}
}

void VolumetricWall::move_to(vec2 new_pos) {
	for (Wall& w : walls) {
		w.beg += -pos + new_pos;
		w.end += -pos + new_pos;
	}
	pos = new_pos;
}
