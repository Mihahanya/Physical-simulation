#pragma once

#include "config.h"
#include "fs.h"
#include "Wall.h"

class PhysicalPoint;

using PPoint = PhysicalPoint;

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

    
    PhysicalPoint();
    PhysicalPoint(float mass, float jumpling, float friction, Color color);
    ~PhysicalPoint();

    void update(float delta_time);

    void add_force(vec2);
    
    void add_wall(Wall*);
    
    void draw() const override;
    void drawr(float rad) const;
    void show_av() const;

private:
    vector<Wall*> walls{};

    vec2 addf, force, gravity_force,
         friction_force = vs::zero,
         normal_force = vs::zero,
         prev_pos = vs::zero;

    void do_walls_collision();
};

// Constructors

PhysicalPoint::PhysicalPoint() {
    gravity_force = GRAVITY * mass;
}

PhysicalPoint::PhysicalPoint(float mass, float jumpling, float friction, Color color=Color::Black) :
    mass{mass}, jumpling{jumpling}, friction{friction}, Drawable{color} 
{
    gravity_force = GRAVITY * mass;
}

PhysicalPoint::~PhysicalPoint() {
    /*for (auto p : walls) delete p;
    walls.clear();*/
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
    for (auto i=0; i < walls.size(); i++) 
    {
        const WallOrient wall_orient = walls[i]->orient;
        const vec2 wall_normal = walls[i]->normal;

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

        if (wall_orient == Vertical) {
            if (walls[i]->vertical_ingress(pos)) {
                is_collised = true;
                pos.y = walls[i]->y_by_x(pos.x);
            }
            else is_collised = false;
        }
        else if (wall_orient == Horizontal) {
            if (walls[i]->horizontal_ingress(pos)) {
                is_collised = true;
                pos.x = walls[i]->x_by_y(pos.y);
            }
            else is_collised = false;
        }

        if (is_collised) {
            const vec2 reflexed = vs::reflect(vel * jumpling, wall_normal);

            if (vs::length(reflexed) > 50) vel = reflexed;
            else vel = vs::zero;

            normal_force = wall_normal * vs::length(force-normal_force) * (1-friction);

            if (i == 0) std::swap(walls[0], walls[1]);
            else if (i != 1) std::swap(walls[1], walls[i]);

            return;
        }
        friction_force = normal_force = vs::zero;
    }
}

void PhysicalPoint::add_force(vec2 f) { addf += f; }

void PhysicalPoint::add_wall(Wall* wall) { walls.push_back(wall); }

// Drawing

void PhysicalPoint::draw() const {
    drawr(3);
}

void PhysicalPoint::drawr(float rad) const {
    ff::easy_circle(pos, rad, *window, color);
}

void PhysicalPoint::show_av() const {
    ff::easy_line(pos, prev_pos, *window, Color::Cyan);
    ff::easy_line(pos, pos+vs::norm(vel)*20.f, *window, Color(0, 0, 255, 120));
    ff::easy_line(pos, pos+vs::norm(force)*10.f, *window, Color::Red);
}
