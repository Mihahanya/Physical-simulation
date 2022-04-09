#pragma once

#include "h.h"
#include "Wall.h"

class PPoint
{
public:
    vec2 vel, force, pos, sliding_f, gravity_f;
    float mass, jumpling, friction, speed;
    vector<Wall> walls;
    Color color;
    
    PPoint(float mass, float jumpling, float friction, vec2 position, Color color=Color::Black) {
        /// PPoint fig(mass, jumpling, friction, pos, col); 
        this->mass = mass;
        this->jumpling = jumpling;
        this->friction = friction;
        this->color = color;
        pos = position;
        vel = zero;
        sliding_f = zero;
        gravity_f = GRAVITY * mass;
    }
    
    void frame(float delta_time);
    void move(vec2 d) { mov += d; }
    void add_wall(Wall wall) { walls.push_back(wall); }
    
    void add_window(RenderWindow &window) { this->window = &window; }
    void draw(float r);
    void show_av();

private:
    vec2 mov;
    RenderWindow *window;
    
    inline void do_walls_collision();
};

// Simulation

void PPoint::frame(float delta_time) {
    force = sliding_f + gravity_f + mov;
    vel += force * delta_time;

    vec2 sqr = vec2(vel.x*abs(vel.x), vel.y*abs(vel.y));
    vel -= air_resist * (sqr/2.f);
    
    pos += vel * delta_time;

    speed = dist(zero, force);

    do_walls_collision();
    
    mov = zero;
}

inline void PPoint::do_walls_collision() {
    for (auto w : walls) {
        float h = w.y_by_x(pos.x);
        if (h != INT_MAX) {
            if ((pos.y > h and !w.upper) or (pos.y < h and w.upper)) {
                vec2 r = reflect(vel * jumpling, w.normal);
                if (dist(zero, r) > 20) {
                    vel = r;
                    sliding_f = zero;
                }
                else {
                    vel = zero;
                    //sliding_f += w.direction * dot(w.direction, norm(gravity_f)) * mass * (1.f-friction) * speed;
                }

                pos = vec2(w.x_by_y(h), h);
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
    (*window).draw(easy_line(pos, pos+norm(vel)*20.f, Color(0, 0, 255, 120)), 2, LinesStrip);
    (*window).draw(easy_line(pos, pos+norm(force)*10.f, Color::Red), 2, LinesStrip);
}
