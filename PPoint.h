#pragma once
#include "h.h"

class PPoint
{
public:
    vec2 vel, force, pos, accel;
    float mass;
    
    PPoint(float mass, vec2 position, vec2 a=vec2(0, G)) {
        this->mass = mass;
        pos = position;
        vel = zero;
        accel = a;
    }
    
    void frame(float delta_time) {
        force = mass * (accel+addf) + mov;
        vel += force * delta_time;
        pos += vel * delta_time;

        mov = zero;
    }

    void move(vec2 d) { mov += d; }
    void add_force(vec2 d) { addf += d; }

private:
    vec2 mov, addf;
};
