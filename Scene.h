#pragma once

#include "Physical.h"
#include "PPoint.h"

class Scene
{
public:
	vector<PPoint*> points;
	vector<Wall*> walls;
	vector<SBody*> bodys;
	bool pause;
	float delta_time;

	Scene(RenderWindow &window) {
		this->window = &window;
		pause = false;
	}

	void add(PPoint &obj) { 
		PPoint *p = &obj;
		(*p).add_window(*window);
		for (Wall *w : walls) (*p).add_wall(*w);
		
		points.push_back(p); 
	}
	void add(Wall &obj) { 
		Wall *w = &obj;
		(*w).add_window(*window);
		for (PPoint *p : points) (*p).add_wall(*w);
		for (SBody *p : bodys) (*p).add_wall(*w);
		
		walls.push_back(w); 
	}
	void add(SBody &obj) { 
		SBody *p = &obj;
		(*p).add_window(*window);
		for (Wall *w : walls) (*p).add_wall(*w);
		
		bodys.push_back(p); 
	}

	void update();
	void draw();

private:
	RenderWindow *window;
    sf::Clock delta_clock;
};

void Scene::update() {
	//delta_time = delta_clock.restart().asSeconds();
	delta_time = 1./1000;

	if (pause) return;
	
	for (SBody *p : bodys) (*p).update(delta_time);
	for (PPoint *p : points) (*p).update(delta_time);
}

void Scene::draw() {
	for (Wall *w : walls) (*w).draw();
	for (SBody *b : bodys) (*b).draw();
	for (PPoint *p : points) (*p).draw();
}
