#pragma once

#include "PhysicalObject.h"
#include "Wall.h"

class PhysicalPoint;

using PPoint = PhysicalPoint;

class PhysicalPoint : public PhysicalObject, public sfDrawable
{
public:
    bool is_collised = false;
    
    PhysicalPoint();
    PhysicalPoint(float mass, float bounciness, float friction, Color color);
    ~PhysicalPoint();

    void update(float delta_time) override;
    
    void add_force(vec2);
    void set_force(vec2);
    vec2 get_force() const;

    void add_wall(Wall*);
    
    void draw() override;
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

PhysicalPoint::PhysicalPoint(float mass, float bounciness, float friction, Color color=Color::Black) :
    PhysicalObject{mass, bounciness, friction}, Drawable{color} 
{
    gravity_force = GRAVITY * mass;
}

PhysicalPoint::~PhysicalPoint() {}

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
            vec2 reflected_vel = vs::reflect(vel * bounciness, wll.normal);
            
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
            const vec2 reflexed = vs::reflect(vel * bounciness, wall_normal);

            //vel = reflexed;
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
void PhysicalPoint::set_force(vec2 f) { force = f; }
vec2 PhysicalPoint::get_force() const { return force; }

void PhysicalPoint::add_wall(Wall* wall) { walls.push_back(wall); }

// Drawing

void PhysicalPoint::draw() {
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
