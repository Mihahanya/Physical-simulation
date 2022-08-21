#pragma once

#include "Spring.h"

class SoftBodyObject : public PhysicalObject, public Drawable
{
public:
    vector<PPoint> points{};
    float elasticity, resistance;

    SoftBodyObject(float mass, float bounciness, float friction, float elasticity, float resistance);

    void update(float delta_time) override;
    
    void add_force(vec2);
    void set_pos(vec2) override;

    virtual void draw() = 0;
    void show_av() const;
    void show_dots(float radius) const;

    void add_wall(Wall*);

    void set_window(RenderWindow*) override;

protected:
    vector<Spring> springs{};

    virtual void take_arms() = 0;
};

// Construction
SoftBodyObject::SoftBodyObject(float mass, float bounciness, float friction, float elasticity, float resistance) :
    PhysicalObject{ mass, bounciness, friction }, elasticity(elasticity), resistance(resistance) {}

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

    for (Spring& s : springs) s.calculate_force();
    for (Spring& s : springs) s.add_force();

    for (PPoint& p : points) {
        p.update(delta_time);

        pos += p.get_pos() / (float)points.size();
        vel += p.get_vel() / (float)points.size();
    }
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

// Moving
void SoftBodyObject::add_force(vec2 f) {
    for (PPoint& p : points)
        p.add_force(f);
}

void SoftBodyObject::set_pos(vec2 position) {
    for (auto& p : points) {
        p.set_pos(position + (p.get_pos() - pos));
        p.set_vel(vs::zero);
    }
    pos = position;
}

