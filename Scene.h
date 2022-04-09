#pragma once

#include "Physical.h"
#include "PPoint.h"

class Scene
{
public:
	//vector<SBody> bodys;
	vector<PPoint*> points;
	vector<Wall*> walls;
	vector<SBody*> bodys;
	bool pause;

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

	void frame(float delta_time);
	void draw();

private:
	RenderWindow *window;
};

void Scene::frame(float delta_time) {
	if (pause) return;
	
	for (PPoint *p : points) (*p).frame(delta_time);
	for (SBody *p : bodys) (*p).frame(delta_time);
}

void Scene::draw() {
	for (Wall *w : walls) (*w).draw();
	for (SBody *b : bodys) (*b).draw();
	for (PPoint *p : points) (*p).draw();
}
