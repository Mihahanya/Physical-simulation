#pragma once

#include "h.h"
#include "Scene.h"
#include "Direct.h"

class Wall : public Direct
{
public:
	float drop_zone = 20;

	Wall(vec2 ibeg, vec2 iend, bool is_dynamic=false) : Direct(ibeg, iend), is_dynamic(is_dynamic) {}

	void draw(Color color=Color::Black) {
		if (is_dynamic) init();
		ff::easy_line(beg, end, *window);
	}

	void show_normals() {
		vec2 mid = (beg+end)/2.f;
		ff::easy_line(mid-normal*drop_zone, mid, *window, Color(255, 0, 255));
	}
    
	void add_window(RenderWindow &window) { this->window = &window; }

private:
	bool is_dynamic;
    RenderWindow *window = nullptr;
};


class VolumetricWall
{
public:
	vector<Wall> walls;

	VolumetricWall(vec2 pos, vec2 sizes, float angle) : pos(pos), sizes(sizes), angle(angle)
	{
		vector<vec2> ps = {
			pos + vs::rotate(vec2(sizes.x, sizes.y), angle) / 2.f,
			pos + vs::rotate(vec2(-sizes.x, sizes.y), angle) / 2.f,
			pos + vs::rotate(vec2(-sizes.x, -sizes.y), angle) / 2.f,
			pos + vs::rotate(vec2(sizes.x, -sizes.y), angle) / 2.f,
		};
		std::reverse(ps.begin(), ps.end());

		float wdz = sizes.x / 3.f,
			  hdz = sizes.y / 3.f;

		for (int i=0; i<ps.size(); i++) {
			Wall w(ps[i], ps[(i == ps.size()-1) ? 0 : i+1], true);
			w.drop_zone = (i % 2 == 0) ? hdz : wdz;
			walls.push_back(w);
		}
	}

	void move_to(vec2 new_pos) {
		for (Wall &w : walls) {
			w.beg += -pos + new_pos;
			w.end += -pos + new_pos;
		}
		pos = new_pos;
	}

private:
	vec2 pos, sizes;
	float angle;
};
