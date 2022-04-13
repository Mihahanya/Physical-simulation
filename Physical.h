#pragma once

#include "Spring.h"

class SBody
{
public:
    vector<PPoint> points;
    vector<Spring> springs;
    vector<vector<float>> arms;
    float mass, elasticity, jumpling, friction, resistance_f;
    vec2 center, velocity;

    SBody(float mass, float jumpling, float elasticity, float resistance_f, float friction) {
        this->mass = mass;
        this->elasticity = elasticity;
        this->jumpling = jumpling;
        this->friction = friction;
        this->resistance_f = resistance_f;
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

    void frame(float delta_time);
    
    void move(vec2 d);

private:
    RenderWindow *window;
    vector<vec2> fluct;

    inline void take_arms();
};


// Creating
void SBody::create_regular_polygon(vec2 cntr, int cnt_of_sds, float size) {
    for (float a=0; a<PI*2 and points.size()<cnt_of_sds; a+=PI/cnt_of_sds*2) {
        PPoint p(mass, jumpling, friction, cntr + vec2(cos(a), sin(a))*size);
        points.push_back(p);
        fluct.push_back(zero);
    }
    elasticity /= (float)cnt_of_sds;
    take_arms();
}

void SBody::create_custom_polygon(vector<vec2> crnrs) {
    for (auto t : crnrs) {
        PPoint p(mass, jumpling, friction, t);
        points.push_back(p);
        fluct.push_back(zero);
    }
    elasticity /= (float)points.size();
    take_arms();
}

// Simulation
void SBody::frame(float dt) {
    float delta_time = min(dt, 0.1f);
    center = velocity = zero;
    
    for (Spring &s : springs) s.calculate_force();
    for (Spring &s : springs) s.add_force();
    
    for (PPoint &p : points) {
        p.frame(delta_time);

        center += p.pos;
        velocity += p.vel;
    }

    center /= (float)points.size();
    velocity /= (float)points.size();
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
    for (int i = 0; i < points.size(); i++)
        points[i].show_av();
    
    (*window).draw(easy_line(center, center+norm(velocity)*30.f, Color::Blue), 2, LinesStrip);
}

void SBody::show_dots(float r) {
    for (int i=0; i<points.size(); i++) points[i].draw(r);

    CircleShape c(r); c.setFillColor(Color::Black); c.setPosition(center - vec2(r, r));
    (*window).draw(c);
}

// Taking arm connections
inline void SBody::take_arms() {
    for (int i = 0; i < points.size(); i++) {
        for (int j = (i == 0 ? 0 : i+1); j<points.size(); j++) {
            Spring s(points[i], points[j], elasticity, resistance_f);
            springs.push_back(s);
        }
    }
}

// Moving
void SBody::move(vec2 d) {
    for (PPoint &p : points)
        p.move(d);
}

