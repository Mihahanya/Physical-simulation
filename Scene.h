#pragma once

#include "SoftBody.h"
#include "PhysicalPoint.h"
#include "VolumetricWall.h"

class Scene
{
public:
	vector<SoftContour*> bodys{};
	vector<PPoint*> points{};
	vector<Wall*> walls{};
	float delta_time;
	bool pause = false;

	Scene(RenderWindow* window);

	void add(PPoint&);
	void add(Wall&);
	void add(VolumetricWall&);
	void add(SoftContour&);

	void update();
	void draw();

private:
	RenderWindow *window = nullptr;
    sf::Clock delta_clock;

	float stabilize_dt(float new_dt);
	static float median(float, float, float);
};


Scene::Scene(RenderWindow* window) : window(window) {}

void Scene::add(PPoint& p) {
	p.set_window(window);
	for (Wall* w : walls) p.add_wall(w);

	points.push_back(&p);
}

void Scene::add(Wall& w) {
	w.set_window(window);
	for (PPoint* p : points) (*p).add_wall(&w);
	for (SoftContour* p : bodys) (*p).add_wall(&w);

	walls.push_back(&w);
}

void Scene::add(VolumetricWall& obj) {
	for (Wall& w : obj.walls) add(w);
}

void Scene::add(SoftContour& sc) {
	sc.set_window(window);
	for (Wall* w : walls) sc.add_wall(w);

	bodys.push_back(&sc);
}

float Scene::stabilize_dt(float new_dt) {
	static float dt = new_dt;

	const float d = new_dt - dt,
				k = (abs(d) < 15) ? 0.7 : 0.3;
	dt += d * k;
	return dt;
}

inline float Scene::median(float a, float b, float c) {
	return (max(a, b) == max(b, c)) ? max(a, c) : max(b, min(a, c));
}

void Scene::update() {
	delta_time = delta_clock.restart().asSeconds();
	float dt = stabilize_dt(delta_time);
	delta_time = dt;
	//float dt = 1./100;

	if (pause) return;

	for (PPoint* p : points) (*p).update(dt);
	for (SoftContour* p : bodys) (*p).update(dt);
}

void Scene::draw() {
	for (Wall* w : walls) (*w).draw_update();
	for (PPoint* p : points) (*p).draw();
	for (SoftContour* b : bodys) (*b).draw();
}
