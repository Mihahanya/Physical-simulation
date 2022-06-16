#pragma once

#include "h.h"
#include "Wall.h"

class PPoint
{
public:
    vec2 vel, force, pos, sliding_f, gravity_f;
    float mass, jumpling, friction, speed;
    vector<Wall*> walls;
    Color color;
    bool is_static;
    
    PPoint(float mass, float jumpling, float friction, Color color=Color::Black) {
        /// PPoint fig(mass, jumpling, friction, pos, col); 
        this->mass = mass;
        this->jumpling = jumpling;
        this->friction = friction;
        this->color = color;
        
        is_static = false;
        pos = vs::zero; prev_pos = pos;
        vel = vs::zero;
        gravity_f = GRAVITY * mass;
    }
    
    void update(float delta_time);
    void move(vec2 d) { mov += d; }
    void add_wall(Wall &wall) { walls.push_back(&wall); }
    
    void add_window(RenderWindow &window) { this->window = &window; }
    void draw(float r);
    void show_av();

private:
    vec2 mov, prev_pos;
    RenderWindow *window;
    
    inline void do_walls_collision();
};

// Simulation

void PPoint::update(float delta_time) {
    if (is_static) return;

    force = gravity_f + mov;
    vel += force * delta_time;
    
    prev_pos = pos;
    pos += vel * delta_time;

    do_walls_collision();
    
    mov = vs::zero;
}

inline void PPoint::do_walls_collision() {
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
            auto ybx = (*w).y_by_x(pos.x);
            float match_y = get<0>(ybx);
        
            if (get<1>(ybx)) {
                if ((pos.y > match_y and wall_direct.x < 0) or (pos.y < match_y and wall_direct.x > 0)) {
                    is_reflect = true;
                    pos.y = match_y;
                }
            }
        }
        else {
            auto xby = (*w).x_by_y(pos.y);
            float match_x = get<0>(xby);
        
            if (get<1>(xby)) {
                if ((pos.x > match_x and wall_direct.y > 0) or (pos.x < match_x and wall_direct.y < 0)) {
                    is_reflect = true;
                    pos.x = match_x;
                }
            }
        }

        if (is_reflect) {
            vec2 r = vs::reflect(vel * jumpling, (*w).normal);
            
            if (vs::dist(vs::zero, r) > 50) vel = r;
            else vel = vs::zero;
            
            break;
        }
    }
}

// Drawing

void PPoint::draw(float r=3) {
    ff::easy_circle(pos, r, *window, color);
}

void PPoint::show_av() {
    ff::easy_line(pos, prev_pos, *window, Color::Cyan);
    ff::easy_line(pos, pos+vs::norm(vel)*20.f, *window, Color(0, 0, 255, 120));
    ff::easy_line(pos, pos+vs::norm(force)*10.f, *window, Color::Red);
}
