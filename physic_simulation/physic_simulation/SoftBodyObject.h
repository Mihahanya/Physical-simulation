#pragma once

#include "Spring.h"

class SoftBodyObject : public PhysicalObject, public Spring, public Drawable
{
public:
    vector<PPoint> points{};

    SoftBodyObject(float mass, float bounciness, float friction, float elasticity, float resistance);

    virtual void update(float delta_time) override;
    
    virtual void draw() = 0;
    void show_av() const;
    void show_dots(float radius) const;

    void add_wall(Wall*);

    virtual void set_window(RenderWindow*) override;

    void add_force(vec2);
    void set_pos(vec2) override;
    void set_vel(vec2) override;

    void set_mass    (float) override;
    void set_bounc   (float) override;
    void set_friction(float) override;

    void set_elasticity    (float) override;
    void set_resistance    (float) override;
    void set_distance      (float) override;

protected:
    vector<Spring> springs{};

    virtual void take_arms() = 0;
    void calculate_force() override;
};

// Construction
SoftBodyObject::SoftBodyObject(float mass, float bounciness, float friction, float elasticity, float resistance) :
    PhysicalObject{mass, bounciness, friction}, Spring{elasticity, resistance} {}

void SoftBodyObject::add_wall(Wall* wall) {
    for (PPoint& p : points) p.add_wall(wall);
}

void SoftBodyObject::set_window(RenderWindow* window) {
    this->window = window;
    for (PPoint& p : points) p.set_window(window);
}

// Simulation
void SoftBodyObject::update(float delta_time) {
    pos = vel = vs::zero;

    calculate_force();
    for (Spring& s : springs) s.add_force();

    for (PPoint& p : points) {
        p.update(delta_time);

        pos += p.get_pos() / (float)points.size();
        vel += p.get_vel() / (float)points.size();
    }
}

void SoftBodyObject::calculate_force() {
    for (Spring& s : springs) s.calculate_force();
}

// Drawing
void SoftBodyObject::show_av() const {
    for (PPoint p : points) p.show_av();
    ff::easy_line(pos, pos + vs::norm(vel) * 30.f, *window, Color::Blue);
}

void SoftBodyObject::show_dots(float radius) const {
    for (PPoint p : points) p.drawr(radius);
    ff::easy_circle(pos, radius, *window);
}

// setter
void SoftBodyObject::add_force(vec2 f)              { for (auto& p : points) p.add_force(f); }
void SoftBodyObject::set_vel(vec2 vel)              { for (auto& p : points) p.set_vel(vel); }
void SoftBodyObject::set_mass(float mass)           { for (auto& p : points) p.set_mass(mass); }
void SoftBodyObject::set_bounc(float bounciness)    { for (auto& p : points) p.set_bounc(bounciness); }
void SoftBodyObject::set_friction(float friction)   { for (auto& p : points) p.set_friction(friction); }

void SoftBodyObject::set_elasticity(float el)       { for (auto& p : springs) p.set_elasticity(el);}
void SoftBodyObject::set_resistance(float re)       { for (auto& p : springs) p.set_resistance(re);}
void SoftBodyObject::set_distance(float d)          { for (auto& p : springs) p.set_distance(d);}

void SoftBodyObject::set_pos(vec2 position) {
    for (auto& p : points) {
        p.set_pos(position + (p.get_pos() - pos));
        p.set_vel(vs::zero);
    }
    pos = position;
}
