#pragma once

#include "SoftBodyObject.h"
#include "fs.h"

class SoftBody : public SoftBodyObject {
public:
    vector<vector<PPoint>> points;

    SoftBody(float mass, float bounciness, float friction, float elasticity, float resistance);

    void create_figure(const vec2& pos, int w, int h, float step);

    void draw() override;
    
private:
    vector<ff::FixedLine> lines;

    void take_arms() override;
};

// Constructors 

SoftBody::SoftBody(float mass, float bounciness, float friction, float elasticity, float resistance) :
    SoftBodyObject{mass, bounciness, friction, elasticity, resistance} {}

// Drawing
void SoftBody::draw() {
    for (auto& l : lines) l.draw();
}


// Taking arm connections

inline void SoftBody::take_arms() {
    const static int fasteners[4][2] = { {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
    size_t h=points.size(), w=points[0].size();
    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            for (const auto& fst : fasteners) {
                int ii=i+fst[1], jj=j+fst[0];
                if (ii >= 0 and ii < h and jj >= 0 and jj < w) {
                    Spring s(&points[i][j], &points[ii][jj], elasticity, resistance);
                    springs.push_back(s);

                    ff::FixedLine l(points[i][j].get_pos_ptr(), points[ii][jj].get_pos_ptr(), window, color);
                    lines.push_back(l);
                }
            }
        }
    }
}

// Creating

void SoftBody::create_figure(const vec2& pos, int w, int h, float step) {
    this->pos = pos;
    for (int i=0; i<h; i++) {
        vector<PPoint> l{};
        for (int j=0; j<w; j++) {
            PPoint p(mass, bounciness, friction);

            float x = (j-(float)(w-1)/2.)*step;
            float y = (i-(float)(h-1)/2.)*step;
            p.set_pos(pos + vec2{x, y});

            l.push_back(p);
        }
        points.push_back(l);
    }
    take_arms();
}

