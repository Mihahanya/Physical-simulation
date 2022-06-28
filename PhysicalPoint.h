#pragma once

#include "h.h"
#include "Wall.h"

class PhysicalPoint;

typedef PhysicalPoint PPoint;

class PhysicalPoint : public Drawable
{
public:
    vec2 vel = vs::zero, 
         pos = vs::zero;

    float mass = 1, 
          jumpling = 0.9, 
          friction = 0.5;
    
    bool is_static = false, 
         is_collised = false;
    
    Color color = Color::Black;

    
    PhysicalPoint();
    PhysicalPoint(float mass, float jumpling, float friction, Color color);
    
    void update(float delta_time);

    void add_force(vec2);
    
    void add_wall(Wall*);
    
    void draw(float radius);
    void show_av();

private:
    vector<Wall*> walls{};

    vec2 addf, force, gravity_force,
         friction_force = vs::zero,
         normal_force = vs::zero,
         prev_pos = vs::zero;

    inline void do_walls_collision();
};

// Constructors

PhysicalPoint::PhysicalPoint() {
    gravity_force = GRAVITY * mass;
}

PhysicalPoint::PhysicalPoint(float mass, float jumpling, float friction, Color color=Color::Black) :
    mass(mass), jumpling(jumpling), friction(friction), color(color)
{
    gravity_force = GRAVITY * mass;
}

// Simulation

void PhysicalPoint::update(float delta_time) {
    if (is_static) return;

    force = gravity_force + normal_force + friction_force + addf;

    vel += force / mass * delta_time;
    
    prev_pos = pos;
    pos += vel * delta_time;

    do_walls_collision();
    
    addf = vs::zero;
}

inline void PhysicalPoint::do_walls_collision() {
    for (auto i=0; i < walls.size(); i++) {
        Wall wll = *walls[i];

        /*Direct temp(prev_pos, pos);
        vec2 dir = temp.direction == vs::zero ? -vs::norm(GRAVITY) : temp.direction;
        dir *= 2.f;

        vec2 cross; bool contact;
        tie(cross, contact) = wll.collised(Direct(prev_pos-dir, pos));

        if (contact or vel == vs::zero) {
            pos = cross;
            vec2 reflected_vel = vs::reflect(vel * jumpling, wll.normal);
            
            if (vs::dist(vs::zero, reflected_vel) > 50) vel = reflected_vel;
            else vel = vs::zero;

            break;
        }*/

        if (wll.orient == Vertical) {        
            if (wll.vertical_ingress(pos)) {
                is_collised = true;
                pos.y = wll.y_by_x(pos.x);
            }
            else is_collised = false;
        }
        else if (wll.orient == Horizontal) {
            if (wll.horizontal_ingress(pos)) {
                is_collised = true;
                pos.x = wll.x_by_y(pos.y);
            }
            else is_collised = false;
        }

        if (is_collised) {
            const vec2 reflexed = vs::reflect(vel * jumpling, wll.normal);

            if (vs::length(reflexed) > 50) vel = reflexed;
            else vel = vs::zero;

            normal_force = wll.normal * vs::length(force-normal_force) * (1-friction);

            if (i == 0) walls[0], walls[1] = walls[1], walls[0];
            else if (i != 1) walls[1], walls[i] = walls[i], walls[1];

            return;
        }
        friction_force = normal_force = vs::zero;
    }
}

void PhysicalPoint::add_force(vec2 f) { addf += f; }

void PhysicalPoint::add_wall(Wall* wall) { walls.push_back(wall); }

// Drawing

void PhysicalPoint::draw(float radius=3) {
    ff::easy_circle(pos, radius, *window, color);
}

void PhysicalPoint::show_av() {
    ff::easy_line(pos, prev_pos, *window, Color::Cyan);
    ff::easy_line(pos, pos+vs::norm(vel)*20.f, *window, Color(0, 0, 255, 120));
    ff::easy_line(pos, pos+vs::norm(force)*10.f, *window, Color::Red);
}
