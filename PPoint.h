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
    
    PPoint(float mass, float jumpling, float friction, vec2 position, bool is_static=false, Color color=Color::Black) {
        /// PPoint fig(mass, jumpling, friction, pos, col); 
        this->mass = mass;
        this->jumpling = jumpling;
        this->friction = friction;
        this->color = color;
        this->is_static = is_static;
        
        pos = position; prev_pos = pos;
        vel = zero+vec2(-30, 10)*0.f;
        gravity_f = GRAVITY * mass;
    }
    
    void frame(float delta_time);
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

void PPoint::frame(float delta_time) {
    if (is_static) return;

    force = gravity_f + mov;
    vel += force * delta_time;

    vec2 sqr = vec2(vel.x*abs(vel.x), vel.y*abs(vel.y));
    vel -= air_resist * (sqr/2.f);
    
    prev_pos = pos;
    pos += vel * delta_time;

    do_walls_collision();
    
    mov = zero;
}

inline void PPoint::do_walls_collision() {
    for (Wall *w : walls) {
        /*vec2 collision; bool contact;
        tie(collision, contact) = (*w).collised(prev_pos, pos);

        if (contact) {
            pos = collision + norm(prev_pos-pos)*1.f;
            
            vec2 r = reflect(vel * jumpling, (*w).normal);
            if (dist(zero, r) > 20) vel = r;
            else vel = zero;

            break;
        }*/

        auto h = (*w).y_by_x(pos.x);
        if (h != nullopt) {
            if ((pos.y > h and (*w).orient == -1) or (pos.y < h and (*w).orient == 1)) {
                vec2 r = reflect(vel * jumpling, (*w).normal);
                if (dist(zero, r) > 50) vel = r;
                else vel = zero;

                pos = vec2((*w).x_by_y(h.value()).value(), h.value());
                break;
            }
        }
    }
}

// Drawing

void PPoint::draw(float r=3) {
    CircleShape c(r); c.setFillColor(color); c.setPosition(pos - vec2(r, r));
    (*window).draw(c);
}

void PPoint::show_av() {
    (*window).draw(easy_line(pos, prev_pos, Color::Cyan), 2, LinesStrip);
    (*window).draw(easy_line(pos, pos+norm(vel)*20.f, Color(0, 0, 255, 120)), 2, LinesStrip);
    (*window).draw(easy_line(pos, pos+norm(force)*10.f, Color::Red), 2, LinesStrip);
}
