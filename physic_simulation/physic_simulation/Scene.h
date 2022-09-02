#pragma once

#include "SoftBody.h"
#include "SoftContour.h"
#include "PhysicalPoint.h"
#include "VolumetricWall.h"
#include <functional>

class Scene;

void main_loop(auto, Scene&);

class Scene : public Drawable {
public:
	vector<SoftBody*> sbodys{};
	vector<SoftContour*> contours{};
	vector<PPoint*> points{};
	vector<Wall*> walls{};
	float delta_time = 0.01; // ~ 100 fps
	bool pause = false;

	Scene(RenderWindow* window);
	~Scene();

	void add(PPoint&);
	void add(Wall&);
	void add(VolumetricWall&);
	void add(SoftContour&);
	void add(SoftBody&);

	void update();
	void draw() override;

	friend void main_loop(auto, Scene&);

private:
	sf::Clock delta_clock{};

	float stabilize_dt(float new_dt);
	static float median(float, float, float);
};


Scene::Scene(RenderWindow* window) {
	set_window(window);
	color = Color::White;
}

Scene::~Scene() {}

void Scene::add(PPoint& p) {
	p.set_window(window);
	for (Wall* w : walls) p.add_wall(w);

	points.push_back(&p);
}

void Scene::add(SoftContour& sc) {
	sc.set_window(window);
	for (Wall* w : walls) sc.add_wall(w);

	contours.push_back(&sc);
}

void Scene::add(SoftBody& sb) {
	sb.set_window(window);
	for (Wall* w : walls) sb.add_wall(w);

	sbodys.push_back(&sb);
}

void Scene::add(Wall& w) {
	w.set_window(window);
	for (auto p : points) p->add_wall(&w);
	for (auto p : contours) p->add_wall(&w);
	for (auto p : sbodys) p->add_wall(&w);

	walls.push_back(&w);
}

void Scene::add(VolumetricWall& obj) {
	for (Wall& w : obj.walls) add(w);
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

#if FPS > 0
	float dt = 1./FPS;
#else
	float dt = stabilize_dt(delta_time);
#endif

	if (pause) return;

	for (auto p : points) p->update(dt);
	for (auto p : contours) p->update(dt);
	for (auto p : sbodys) p->update(dt);
}

void Scene::draw() {
	for (auto w : walls) w->draw_update();
	for (auto p : points) p->draw();
	for (auto b : contours) b->draw();
	for (auto b : sbodys) b->draw();
}

void main_loop(auto main, Scene& scene) {
	bool is_focuse = true;
	while (scene.window->isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::P)) scene.pause = true;
		else scene.pause = false;

		Event e;
		while (scene.window->pollEvent(e)) {
			if (e.type == Event::Closed) scene.window->close();

			if (e.type == Event::LostFocus) is_focuse = false;
			if (e.type == Event::GainedFocus) is_focuse = true;
		}
		if (!is_focuse) continue;

		scene.window->clear(scene.color);

		main(); 

		scene.draw();
		scene.update();

		scene.window->display();
	}
}
