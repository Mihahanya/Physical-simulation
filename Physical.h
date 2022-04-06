#pragma once

#include "PPoint.h"

class Physical
{
public:
    vector<PPoint> points;
    vector<vector<float>> arms;
    float mass, elasticity, jumpling, friction;
    vec2 center;

    Physical(float mass, float jumpling, float elasticity, float friction) {
        this->mass = mass;
        this->elasticity = elasticity;
        this->jumpling = jumpling;
        this->friction = friction;
    }

    void create_regular_polygon(vec2 cntr, int cnt_of_sds, float size);
    void create_custom_polygon(vector<vec2> crnrs);

    void add_wall(Wall wall) { 
        for (int i=0; i<points.size(); i++) points[i].add_wall(wall);
    }

    void draw(Color color);
    void show_av();
    void show_dots(float r);
    void add_window(RenderWindow &window) { 
        this->window = &window; 
        for (int i=0; i<points.size(); i++) points[i].add_window(window);
    }

    void frame(float delta_time);
    
    void move(vec2 d);

private:
    RenderWindow *window;

    inline void take_arms();
    inline vector<vec2> form_shape();
};


// Creating
void Physical::create_regular_polygon(vec2 cntr, int cnt_of_sds, float size) {
    for (float a=0; a<PI*2 and points.size()<cnt_of_sds; a+=PI/cnt_of_sds*2) {
        PPoint p(mass, jumpling, friction, cntr + vec2(cos(a), sin(a))*size);
        points.push_back(p);
    }
    take_arms();
}

void Physical::create_custom_polygon(vector<vec2> crnrs) {
    for (auto t : crnrs) {
        PPoint p(mass, jumpling, friction, t);
        points.push_back(p);
    }
    take_arms();
}

// Simulation
void Physical::frame(float dt) {
    float delta_time = min(dt, 0.02f);
    vector<vec2> frm = form_shape();
    center = zero;
    for (int i = 0; i < points.size(); i++) {
        points[i].move(frm[i]); // Regulate the shape
        points[i].frame(delta_time);

        center += points[i].pos;
    }
    center /= (float)points.size();
}

inline vector<vec2> Physical::form_shape() {
    vector<vec2> frm(points.size());
    for (int i = 0; i < points.size(); i++) {
        vec2 acc = zero;
        int m = 0;
        for (int j = (i == 0 ? 0 : i+1); j<points.size(); j++) {
            vec2 v = (points[j].pos-points[i].pos) * (dist(points[i].pos, points[j].pos)-arms[i][j-1]);
            acc += v;
            frm[j] -= v;
        }
        frm[i] += acc;
        frm[i] *= elasticity/(float)points.size();
    }
    return frm;
}

// Drawing
void Physical::draw(Color color=Color::Black) {
    vector<Vertex> vtx; 
    for (int i = 0; i <= points.size(); i++) {
        vtx.push_back(points[(i == points.size()) ? 0 : i].pos);
        vtx[i].color = color;
    }
    Vertex *arrvtx = &vtx[0];
    (*window).draw(arrvtx, points.size()+1, LinesStrip);
}

void Physical::show_av() {
    for (int i = 0; i < points.size(); i++) {
        points[i].show_av();

        /*for (int i = 0; i < points.size(); i++) {
            for (int j = (i == 0 ? 0 : i+1); j < points.size(); j++) {
                if (abs(j-i) <= 1 or abs(i-j) == points.size()-1) continue;
                (*window).draw(easy_line(points[i].pos, points[j].pos, Color(0, 100, 0, 70)), 2, LinesStrip);
            }
        }*/
    }
}

void Physical::show_dots(float r) {
    for (int i=0; i<points.size(); i++) points[i].draw(r);

    CircleShape c(r); c.setFillColor(Color::Black); c.setPosition(center - vec2(r, r));
    (*window).draw(c);
}

// Taking arm connections
inline void Physical::take_arms() {
    for (int i=0; i<points.size(); i++) {
        vector<float> a;
        for (int j=0; j<points.size(); j++) {
            if (j != i) a.push_back(dist(points[j].pos, points[i].pos));
        }
        arms.push_back(a);
    }
}

// Moving
void Physical::move(vec2 d) {
    for (PPoint &p : points)
        p.move(d);
}

