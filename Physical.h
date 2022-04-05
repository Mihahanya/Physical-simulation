#pragma once

#include "PPoint.h"

class Physical
{
public:
    vector<PPoint> points;
    vector<vector<float>> arms;
    vector<Wall> walls;
    float mass, elasticity, jumpling, friction;
    vec2 center;

    Physical(float mass, float elasticity, float jumpling=1, float friction=1) {
        this->mass = mass;
        this->elasticity = elasticity;
        this->jumpling = jumpling;
        this->friction = friction;
    }

    void create_regular_polygon(vec2 cntr, int cnt_of_sds, float size);
    void create_custom_polygon(vector<vec2> crnrs);

    void add_wall(Wall wall) { walls.push_back(wall); }

    void draw(Color color);
    void show_av();
    void show_dots(float r);
    void add_window(RenderWindow &window) { this->window = &window; }

    void frame(float delta_time);
    
    void move(vec2 d);

private:
    RenderWindow *window;

    inline void take_arms();
    inline vector<vec2> form_shape();
    inline void do_walls_collision(PPoint &point);
};


// Creating
void Physical::create_regular_polygon(vec2 cntr, int cnt_of_sds, float size) {
    for (float a=0; a<PI*2 and points.size()<cnt_of_sds; a+=PI/cnt_of_sds*2) {
        PPoint p(mass, cntr + vec2(cos(a), sin(a))*size);
        points.push_back(p);
    }
    take_arms();
}

void Physical::create_custom_polygon(vector<vec2> crnrs) {
    for (auto t : crnrs) {
        PPoint p(mass, t);
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

        points[i].frame(delta_time); // Make frame of current point
        center += points[i].pos;
        
        do_walls_collision(points[i]); // Check and make collisions
    }
    center /= (float)points.size();
}

inline void Physical::do_walls_collision(PPoint &point) {
    for (auto w : walls) {
        float h = w.y_by_x(point.pos.x);
        if (h != INT_MAX) {
            if ((point.pos.y > h and !w.upper) or (point.pos.y < h and w.upper)) {
                vec2 r = reflect(point.vel * jumpling, norm(vec2(1, -1/w.k)));
                if (dist(zero, r) > 1) {
                    point.vel = r;
                    point.pos = vec2(w.x_by_y(h), h);
                }
                break;
            }
        }
    }
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
        (*window).draw(easy_line(points[i].pos, points[i].pos+norm(points[i].vel)*20.f, Color(0, 0, 255, 120)), 2, LinesStrip);
        (*window).draw(easy_line(points[i].pos, points[i].pos+norm(points[i].force)*10.f, Color::Red), 2, LinesStrip);

        for (int i = 0; i < points.size(); i++) {
            for (int j = (i == 0 ? 0 : i+1); j < points.size(); j++) {
                if (abs(j-i) <= 1 or abs(i-j) == points.size()-1) continue;
                (*window).draw(easy_line(points[i].pos, points[j].pos, Color(0, 100, 0, 70)), 2, LinesStrip);
            }
        }
    }
}

void Physical::show_dots(float r) {
    for (PPoint p : points) {
        CircleShape c(r); c.setFillColor(Color::Black); c.setPosition(p.pos - vec2(r, r));
        (*window).draw(c);
    }
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

