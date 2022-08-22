#pragma once

#include "fs.h"

class PhysicalObject {
public:
    bool is_static = false;
    
    PhysicalObject() {}
    PhysicalObject(float mass, float bounciness, float friction) : mass{mass}, bounciness{bounciness}, friction{friction} {}

    virtual void update(float deltatime) = 0;

    vec2 get_vel() const { return vel; }
    vec2 get_pos() const { return pos; }
    vec2* get_pos_ptr() { return &pos; }

    virtual void set_pos(vec2 position) { pos = position; }
    virtual void set_vel(vec2 velocity) { vel = velocity; }

    float get_mass()     const { return mass; }
    float get_bounc()    const { return bounciness; }
    float get_friction() const { return friction; }
    
    virtual void set_mass(float mass) { this->mass = mass; }
    virtual void set_bounc(float bounciness) { this->bounciness = bounciness; }
    virtual void set_friction(float friction) { this->friction = friction; }

protected:
    vec2 vel = vs::zero,
         pos = vs::zero;

    float mass = 1,
          bounciness = 0.9,
          friction = 0.5;
};
