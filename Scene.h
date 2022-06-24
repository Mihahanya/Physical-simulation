#pragma once

#include "SoftBody.h"
#include "PhysicalPoint.h"

class Scene
{
public:
	vector<SoftContour*> bodys;
	vector<PPoint*> points;
	vector<Wall*> walls;
	bool pause;
	float delta_time;

	Scene(RenderWindow &window) {
		this->window = &window;
		pause = false;
	}

	void add(PPoint *p) { 
		(*p).add_window(window);
		for (Wall *w : walls) (*p).add_wall(w);
		
		points.push_back(p); 
	}
	void add(Wall *w) { 
		(*w).add_window(*window);
		for (PPoint *p : points) (*p).add_wall(w);
		for (SoftContour *p : bodys) (*p).add_wall(w);
		
		walls.push_back(w); 
	}
	void add(VolumetricWall *obj) { 
		for (Wall &w : (*obj).walls) add(&w);
	}
	void add(SoftContour *sc) { 
		(*sc).add_window(window);
		for (Wall *w : walls) (*sc).add_wall(w);
		
		bodys.push_back(sc); 
	}

	void update();
	void draw();

private:
	RenderWindow *window = nullptr;
    sf::Clock delta_clock;
};

void Scene::update() {
	//delta_time = delta_clock.restart().asSeconds();
	delta_time = 1./60;

	if (pause) return;
	
	for (PPoint *p : points) (*p).update(delta_time);
	for (SoftContour *p : bodys) (*p).update(delta_time);
}

void Scene::draw() {
	for (Wall *w : walls) (*w).draw();
	for (PPoint *p : points) (*p).draw();
	for (SoftContour *b : bodys) (*b).draw();
}
