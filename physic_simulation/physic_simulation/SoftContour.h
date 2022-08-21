#pragma once

#include "SoftBodyObject.h"

class SoftContour : public SoftBodyObject {
public:
    SoftContour(float mass, float bounciness, float friction, float elasticity, float resistance);

    void create_regular_polygon(vec2 cntr, int sides_n, float radius);
    void create_regular_polygon(vec2 cntr, int sides_n, float radius, vec2(*)(float));
    void create_custom_polygon(vector<vec2> crnrs);

    void draw() override;
    
private:
    void take_arms() override;
};

// Constructors 

SoftContour::SoftContour(float mass, float bounciness, float friction, float elasticity, float resistance) :
    SoftBodyObject{mass, bounciness, friction, elasticity, resistance} {}

// Drawing
void SoftContour::draw() {
    vector<Vertex> vtx{}; 
    for (int i = 0; i <= points.size(); i++) {
        vtx.push_back(points[(i == points.size()) ? 0 : i].get_pos());
        vtx[i].color = color;
    }
    Vertex* arrvtx = &vtx[0];
    (*window).draw(arrvtx, points.size()+1, LinesStrip);
}


// Taking arm connections

inline void SoftContour::take_arms() 
{
    float elas_norm = elasticity / (float)points.size();

    for (int i = 0; i < points.size(); i++) {
        for (int j = i+1; j<points.size(); j++) {
            Spring s(&points[i], &points[j], elas_norm/vs::dist(points[i].get_pos(), points[j].get_pos()), resistance);
            springs.push_back(s);
        }
    }
}

// Creating

void SoftContour::create_regular_polygon(vec2 cntr, int sides_n, float radius) {
    pos = cntr;
    for (float a=0; (a < PI*2) and (points.size() < sides_n); a+=PI*2.f/ sides_n)
    {
        PPoint p(mass, bounciness, friction); 
        p.set_pos(cntr + vec2(cos(a), sin(a)) * radius);
        points.push_back(p);
    }
    take_arms();
}

void SoftContour::create_regular_polygon(vec2 cntr, int sides_n, float radius, vec2(*key)(float))
{
    for (float a = 0; (a < PI * 2) and (points.size() < sides_n); a += PI * 2.f / sides_n)
    {
        PPoint p(mass, bounciness, friction);
        p.set_pos(cntr + key(a) * radius);
        points.push_back(p);

        pos += p.get_pos() / (float)sides_n;
    }
    take_arms();
}

void SoftContour::create_custom_polygon(vector<vec2> crnrs) {
    for (auto t : crnrs) {
        PPoint p(mass, bounciness, friction); p.set_pos(t);
        points.push_back(p);

        pos += p.get_pos() / (float)crnrs.size();
    }
    take_arms();
}
