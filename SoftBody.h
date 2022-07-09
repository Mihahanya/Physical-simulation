#pragma once

#include "Spring.h"

class SoftContour : public Drawable
{
public:
    vector<PPoint> points{};
    float mass, jumpling, friction, elasticity, resistance, connection_by_dist;
    vec2 obj_center, obj_velocity;

    SoftContour(float mass, float jumpling, float friction, float elasticity, float resistance, float connection_by_dist);

    void create_regular_polygon(vec2 cntr, int sides_n, float radius);
    void create_custom_polygon(vector<vec2> crnrs);

    void update(float delta_time);
    void add_force(vec2);

    void add_wall(Wall*);

    void draw(Color);
    void show_av();
    void show_dots(float radius);

    void set_window(RenderWindow*);

private:
    PPoint central_point {1, 0, 0};
    vector<Spring> springs{};

    void take_arms();
};

// Constructors 

SoftContour::SoftContour(float mass, float jumpling, float friction, float elasticity, float resistance, float connection_by_dist) :
    mass(mass), jumpling(jumpling), friction(friction), elasticity(elasticity), resistance(resistance), connection_by_dist(connection_by_dist)
{
    central_point.is_static = true;
}

// Creating
void SoftContour::create_regular_polygon(vec2 cntr, int sides_n, float radius)
{
    for (float a=0; (a < PI*2) and (points.size() < sides_n); a+=PI*2.f/ sides_n)
    {
        PPoint p(mass, jumpling, friction); 
        p.pos = cntr + vec2(cos(a), sin(a)) * radius;
        points.push_back(p);

        central_point.pos += p.pos / (float)sides_n;
    }
    take_arms();
}

void SoftContour::create_custom_polygon(vector<vec2> crnrs) {
    for (auto t : crnrs) 
    {
        PPoint p(mass, jumpling, friction); p.pos = t;
        points.push_back(p);

        central_point.pos += p.pos / (float)crnrs.size();
    }
    take_arms();
}

void SoftContour::add_wall(Wall* wall) {
    for (PPoint& p : points) p.add_wall(wall);
}

void SoftContour::set_window(RenderWindow* window) {
    this->window = window;
    for (PPoint& p : points) p.set_window(window);
}

// Simulation
void SoftContour::update(float delta_time) {
    obj_center = obj_velocity = vs::zero;
    
    for (Spring &s : springs) s.calculate_force();
    for (Spring &s : springs) s.add_force();
    
    for (PPoint &p : points) {
        p.update(delta_time);

        obj_center += p.pos / (float)points.size();
        obj_velocity += p.vel / (float)points.size();
    }

    central_point.pos = obj_center;
}

// Drawing
void SoftContour::draw(Color color=Color::Black) {
    vector<Vertex> vtx; 
    for (int i = 0; i <= points.size(); i++) {
        vtx.push_back(points[(i == points.size()) ? 0 : i].pos);
        vtx[i].color = color;
    }
    Vertex *arrvtx = &vtx[0];
    (*window).draw(arrvtx, points.size()+1, LinesStrip);
}

void SoftContour::show_av() {
    for (PPoint p : points) p.show_av();
    ff::easy_line(obj_center, obj_center + vs::norm(obj_velocity)*30.f, *window, Color::Blue);
}

void SoftContour::show_dots(float radius) {
    for (PPoint p : points) p.draw(radius);
    ff::easy_circle(obj_center, radius, *window);
}

// Taking arm connections
inline void SoftContour::take_arms() 
{
    float elas_norm = elasticity / (float)points.size();

    for (int i = 0; i < points.size(); i++) {
        for (int j = i+1; j<points.size(); j++) {
            Spring s(&points[i], &points[j], elas_norm/(vs::dist(points[i].pos, points[j].pos)*connection_by_dist), resistance);
            springs.push_back(s);
        }
    }
}

// Moving
void SoftContour::add_force(vec2 f) {
    for (PPoint &p : points)
        p.add_force(f);
}
