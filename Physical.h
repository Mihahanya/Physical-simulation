#pragma once

#include "Spring.h"

class SBody
{
public:
    vector<PPoint> points;
    vector<Spring> springs;
    float mass, elasticity, jumpling, friction, resistance;
    vec2 obj_center, obj_velocity;

    SBody(float mass, float jumpling, float elasticity, float resistance, float friction) 
    {
        this->mass = mass;
        this->elasticity = elasticity;
        this->jumpling = jumpling;
        this->friction = friction;
        this->resistance = resistance;
        window = nullptr;

        central_point = new PPoint(1, 0, 0);
        central_point->is_static = true; 
    }
    ~SBody() {
        delete central_point;
    }

    void create_regular_polygon(vec2 cntr, int cnt_of_sds, float size);
    void create_custom_polygon(vector<vec2> crnrs);

    void add_wall(Wall &wall) { 
        for (int i=0; i<points.size(); i++) points[i].add_wall(wall);
    }

    void draw(Color color);
    void show_av();
    void show_dots(float r);
    void add_window(RenderWindow &window) { 
        this->window = &window; 
        for (int i=0; i<points.size(); i++) points[i].add_window(window);
    }

    void update(float delta_time);
    
    void move(vec2 d);

private:
    PPoint *central_point;

    RenderWindow *window;
    vector<vec2> fluct;

    inline void take_arms();
};


// Creating
void SBody::create_regular_polygon(vec2 cntr, int cnt_of_sds, float size) 
{
    for (float a=0; (a < PI*2) and (points.size() < cnt_of_sds); a+=PI*2.f/cnt_of_sds) 
    {
        PPoint p(mass, jumpling, friction); 
        p.pos = cntr + vec2(cos(a), sin(a))*size;
        points.push_back(p);

        central_point->pos += p.pos / (float)cnt_of_sds;
    }
    take_arms();
}

void SBody::create_custom_polygon(vector<vec2> crnrs) {
    for (auto t : crnrs) 
    {
        PPoint p(mass, jumpling, friction); p.pos = t;
        points.push_back(p);

        central_point->pos += p.pos / (float)crnrs.size();
    }
    take_arms();
}

// Simulation
void SBody::update(float dt) {
    float delta_time = min(dt, 0.1f);
    obj_center = obj_velocity = vs::zero;
    
    for (Spring &s : springs) s.calculate_force();
    for (Spring &s : springs) s.add_force();
    
    for (PPoint &p : points) {
        p.update(delta_time);

        obj_center += p.pos / (float)points.size();
        obj_velocity += p.vel / (float)points.size();
    }

    central_point->pos = obj_center;
}

// Drawing
void SBody::draw(Color color=Color::Black) {
    vector<Vertex> vtx; 
    for (int i = 0; i <= points.size(); i++) {
        vtx.push_back(points[(i == points.size()) ? 0 : i].pos);
        vtx[i].color = color;
    }
    Vertex *arrvtx = &vtx[0];
    (*window).draw(arrvtx, points.size()+1, LinesStrip);
}

void SBody::show_av() {
    for (PPoint p : points) p.show_av();
    
    ff::easy_line(obj_center, obj_center + vs::norm(obj_velocity)*30.f, *window, Color::Blue);
}

void SBody::show_dots(float r) {
    for (PPoint p : points) p.draw(r);

    ff::easy_circle(obj_center, r, *window);
}

// Taking arm connections
inline void SBody::take_arms() 
{
    fluct = vector<vec2>(points.size(), vs::zero);
    elasticity /= (float)points.size();

    for (int i = 0; i < points.size(); i++) {
        for (int j = (i == 0 ? 0 : i+1); j<points.size(); j++) {
            Spring s(points[i], points[j], elasticity, resistance);
            springs.push_back(s);
        }
    }
}

// Moving
void SBody::move(vec2 d) {
    for (PPoint &p : points)
        p.move(d);
}

