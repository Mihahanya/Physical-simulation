#pragma once

#include "Spring.h"

class SoftBody
{
public:
    vector<PPoint> points;
    vector<Spring> springs;
    float mass, jumpling, elasticity, resistance, connection_by_dist, friction;
    vec2 obj_center, obj_velocity;

    SoftBody(float mass, float jumpling, float elasticity, float resistance, float connection_by_dist, float friction) 
    {
        this->mass = mass;
        this->elasticity = elasticity;
        this->jumpling = jumpling;
        this->friction = friction;
        this->resistance = resistance;
        this->connection_by_dist = connection_by_dist;
        window = nullptr;

        central_point = new PPoint(1, 0, 0);
        central_point->is_static = true; 
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
void SoftBody::create_regular_polygon(vec2 cntr, int cnt_of_sds, float size) 
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

void SoftBody::create_custom_polygon(vector<vec2> crnrs) {
    for (auto t : crnrs) 
    {
        PPoint p(mass, jumpling, friction); p.pos = t;
        points.push_back(p);

        central_point->pos += p.pos / (float)crnrs.size();
    }
    take_arms();
}

// Simulation
void SoftBody::update(float dt) {
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
void SoftBody::draw(Color color=Color::Black) {
    vector<Vertex> vtx; 
    for (int i = 0; i <= points.size(); i++) {
        vtx.push_back(points[(i == points.size()) ? 0 : i].pos);
        vtx[i].color = color;
    }
    Vertex *arrvtx = &vtx[0];
    (*window).draw(arrvtx, points.size()+1, LinesStrip);
}

void SoftBody::show_av() {
    for (PPoint p : points) p.show_av();
    
    ff::easy_line(obj_center, obj_center + vs::norm(obj_velocity)*30.f, *window, Color::Blue);
}

void SoftBody::show_dots(float r) {
    for (PPoint p : points) p.draw(r);

    ff::easy_circle(obj_center, r, *window);
}

// Taking arm connections
inline void SoftBody::take_arms() 
{
    fluct = vector<vec2>(points.size(), vs::zero);
    float elas_norm = elasticity / (float)points.size();

    for (int i = 0; i < points.size(); i++) {
        for (int j = i+1; j<points.size(); j++) {
            Spring s(points[i], points[j], elas_norm/(vs::dist(points[i].pos, points[j].pos)*connection_by_dist), resistance);
            springs.push_back(s);
        }
    }
}

// Moving
void SoftBody::move(vec2 d) {
    for (PPoint &p : points)
        p.move(d);
}

