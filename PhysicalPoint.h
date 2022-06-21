#pragma once

#include "h.h"
#include "Wall.h"

class PhysicalPoint;

typedef PhysicalPoint PPoint;

class PhysicalPoint
{
public:
    vec2 vel, pos, sliding_f;
    float mass, jumpling, friction;
    vector<Wall*> walls;
    Color color;
    bool is_static;
    
    PhysicalPoint() {
        this->mass = 1;
        this->jumpling = 0.9;
        this->friction = 0.5;
        this->color = Color::Black;
        
        is_static = false;
        pos = vs::zero; prev_pos = pos;
        vel = vs::zero;
    }

    PhysicalPoint(float mass, float jumpling, float friction, Color color=Color::Black) {
        this->mass = mass;
        this->jumpling = jumpling;
        this->friction = friction;
        this->color = color;
        
        is_static = false;
        pos = vs::zero; prev_pos = pos;
        vel = vs::zero;
    }
    
    void update(float delta_time);
    void move(vec2 d) { mov += d; }
    void add_wall(Wall &wall) { walls.push_back(&wall); }
    
    void add_window(RenderWindow &window) { this->window = &window; }
    void draw(float r);
    void show_av();

private:
    vec2 mov, prev_pos, force;
    RenderWindow *window = nullptr;
    
    inline void do_walls_collision();
};

// Simulation

void PhysicalPoint::update(float delta_time) {
    if (is_static) return;

    vec2 gravity_force = GRAVITY * mass;
    force = gravity_force + mov;

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
        bool is_reflect = false;

        if (abs(wall_direct.x) > abs(wall_direct.y)) {
            float match_y = (*w).y_by_x(pos.x);
        
            if (!(*w).out_of_y(match_y) and !(*w).out_of_x(pos.x)) {
                if ((pos.y > match_y and pos.y < match_y+(*w).drop_zone and wall_direct.x < 0) or 
                    (pos.y < match_y and pos.y > match_y-(*w).drop_zone and wall_direct.x > 0)) 
                {
                    is_reflect = true;
                    pos.y = match_y;
                }
            }
        }
        else {
            float match_x = (*w).x_by_y(pos.y);
        
            if (!(*w).out_of_x(match_x) and !(*w).out_of_y(pos.y)) {
                if ((pos.x > match_x and pos.x < match_x+(*w).drop_zone and wall_direct.y > 0) or 
                    (pos.x < match_x and pos.x > match_x-(*w).drop_zone and wall_direct.y < 0)) 
                {
                    is_reflect = true;
                    pos.x = match_x;
                }
            }
        }

        if (is_reflect) {
            vec2 reflexed = vs::reflect(vel * jumpling, (*w).normal);
            vel = reflexed;
            break;
        }
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
