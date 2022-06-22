#pragma once

#include "h.h"
#include "Wall.h"

class PhysicalPoint;

typedef PhysicalPoint PPoint;

class PhysicalPoint
{
public:
    vec2 vel, pos;
    float mass, jumpling, friction;
    vector<Wall*> walls;
    Color color;
    bool is_static, is_collised;
    
    PhysicalPoint() {
        this->mass = 1;
        this->jumpling = 0.9;
        this->friction = 0.5;
        this->color = Color::Black;
        
        is_collised = is_static = false;
        pos = vs::zero; prev_pos = pos;
        vel = vs::zero;
        
        gravity_force = GRAVITY * mass;
        normal_force = friction_force = vs::zero;
    }

    PhysicalPoint(float mass, float jumpling, float friction, Color color=Color::Black) {
        this->mass = mass;
        this->jumpling = jumpling;
        this->friction = friction;
        this->color = color;
        
        is_collised = is_static = false;
        pos = vs::zero; prev_pos = pos;
        vel = vs::zero;

        gravity_force = GRAVITY * mass;
        normal_force = friction_force = vs::zero;
    }
    
    void update(float delta_time);
    void move(vec2 d) { mov += d; }
    void add_wall(Wall &wall) { walls.push_back(&wall); }
    
    void add_window(RenderWindow &window) { this->window = &window; }
    void draw(float r);
    void show_av();

private:
    vec2 mov, prev_pos, force, friction_force, gravity_force, normal_force;
    RenderWindow *window = nullptr;
    
    inline void do_walls_collision();
};

// Simulation

void PhysicalPoint::update(float delta_time) {
    if (is_static) return;

    force = gravity_force + normal_force + friction_force + mov;

    vel += force / mass * delta_time;
    
    prev_pos = pos;
    pos += vel * delta_time;

    do_walls_collision();
    
    mov = vs::zero;
}

inline void PhysicalPoint::do_walls_collision() {
    for (Wall *w : walls) {
        /*Direct temp(prev_pos, pos);
        vec2 dir = temp.direction == vs::zero ? -vs::norm(GRAVITY) : temp.direction;
        dir *= 2.f;

        vec2 cross; bool contact;
        tie(cross, contact) = (*w).collised(Direct(prev_pos-dir, pos));

        if (contact or vel == vs::zero) {
            pos = cross;
            vec2 reflected_vel = vs::reflect(vel * jumpling, (*w).normal);
            
            if (vs::dist(vs::zero, reflected_vel) > 50) vel = reflected_vel;
            else vel = vs::zero;

            break;
        }*/

        vec2 wall_direct = (*w).direction;

        if (abs(wall_direct.x) > abs(wall_direct.y)) {
            float match_y = (*w).y_by_x(pos.x);
        
            if (!(*w).out_of_p(vec2(pos.x, match_y)) and
                (pos.y > match_y and pos.y < match_y+(*w).drop_zone and wall_direct.x < 0) or 
                (pos.y < match_y and pos.y > match_y-(*w).drop_zone and wall_direct.x > 0)) 
            {
                is_collised = true;
                pos.y = match_y;
            }
            else is_collised = false;
        }
        else {
            float match_x = (*w).x_by_y(pos.y);
        
            if (!(*w).out_of_p(vec2(match_x, pos.y)) and
                (pos.x > match_x and pos.x < match_x+(*w).drop_zone and wall_direct.y > 0) or 
                (pos.x < match_x and pos.x > match_x-(*w).drop_zone and wall_direct.y < 0)) 
            {
                is_collised = true;
                pos.x = match_x;
            }
            else is_collised = false;
        }

        if (is_collised) {
            vec2 reflexed = vs::reflect(vel * jumpling, (*w).normal);

            if (vs::length(reflexed) > 50) vel = reflexed;
            else vel = vs::zero;

            normal_force = (*w).normal * vs::length(force-normal_force) * (1-friction);
            //friction_force = (*w).direction * vs::dot(vs::norm(gravity_force), (*w).direction);

            return;
        }

        friction_force = normal_force = vs::zero;
    }
}

// Drawing

void PhysicalPoint::draw(float r=3) {
    ff::easy_circle(pos, r, *window, color);
}

void PhysicalPoint::show_av() {
    ff::easy_line(pos, prev_pos, *window, Color::Cyan);
    ff::easy_line(pos, pos+vs::norm(vel)*20.f, *window, Color(0, 0, 255, 120));
    ff::easy_line(pos, pos+vs::norm(force)*10.f, *window, Color::Red);
}
