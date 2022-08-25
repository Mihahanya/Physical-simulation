#pragma once

#include "SoftBodyObject.h"
#include "fs.h"

class SoftBody : public SoftBodyObject {
public:
    SoftBody(float mass, float bounciness, float friction, float elasticity, float resistance);

    void create_figure(const vec2& pos, int w, int h, float step);

    void draw() override;
    void set_window(RenderWindow*) override;

private:
    vector<ff::FixedLine> lines;
    size_t h=-1, w=-1;

    void take_arms() override;
};

// Constructors 

SoftBody::SoftBody(float mass, float bounciness, float friction, float elasticity, float resistance) :
    SoftBodyObject{mass, bounciness, friction, elasticity, resistance} {}

// Drawing

void SoftBody::draw() {
    for (auto& l : lines) l.draw();
}

// Add window

void SoftBody::set_window(RenderWindow* window) {
    SoftBodyObject::set_window(window);
    for (auto& l : lines) l.set_window(window);
}

// Taking arm connections

inline void SoftBody::take_arms() {
    //const static int fasteners[4][2] = { {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
    const static int fasteners[8][2] = { {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1} };
    
    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            for (const auto& fst : fasteners) {
                int ii=i+fst[1], jj=j+fst[0];
                if (ii >= 0 and ii < h and jj >= 0 and jj < w) {
                    size_t rel_i = i*w + j, rel_ii = ii*w + jj;

                    Spring s(&points[rel_i], &points[rel_ii], elasticity, resistance, 100);
                    springs.push_back(s);

                    ff::FixedLine l(points[rel_i].get_pos_ptr(), points[rel_ii].get_pos_ptr(), window, color);
                    lines.push_back(l);
                }
            }
        }
    }
}

// Creating

void SoftBody::create_figure(const vec2& pos, int w, int h, float step) {
    this->pos = pos;
    this->w = w; this->h = h;
    //points.clear();

    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            PPoint p(mass, bounciness, friction);

            float x = (j-(float)(w-1)/2.)*step;
            float y = (i-(float)(h-1)/2.)*step;
            p.set_pos(pos + vec2{x, y});
            
            points.push_back(p);
        }
    }
    take_arms();
}
