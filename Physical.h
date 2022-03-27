#pragma once
#include "h.h"
#include "Wall.h"
#include "PPoint.h"

class Physical
{
public:
    vector<PPoint> points;
    vector<vector<float>> arms;
    vector<Wall> walls;
    float mass, elasticity, jumpling;

    Physical(float mass, float elasticity, float jumpling=1) {
        this->mass = mass;
        this->elasticity = elasticity;
        this->jumpling = jumpling;
    }

    void create_regular_polygon(vec2 cntr, int cnt_of_sds, float size);
    void create_custom_polygon(vector<vec2> crnrs);

    void add_wall(Wall wall) { walls.push_back(wall); }

    void draw(RenderWindow &window, Color color);
    void show_av(RenderWindow &window);

    void step(float delta_time);

private:
    inline void take_arms();
    inline vector<vec2> stbl_vel();
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
void Physical::step(float delta_time) {
    vector<vec2> add_acc = stbl_vel();
    for (int i = 0; i < points.size(); i++) {
        points[i].accel += add_acc[i];
        
        do_walls_collision(points[i]);

        points[i].step(delta_time);
        points[i].accel -= add_acc[i];
    }
}

inline void Physical::do_walls_collision(PPoint &point) {
    for (auto w : walls) {
        float h = w.y_by_x(point.pos.x);
        if (h != INT_MAX) {
            if (point.pos.y > h and !w.upper) {
                point.vel = reflect(point.vel, norm(vec2(1, -1/w.k))) * jumpling;
                point.pos.y = h;
                break;
            }
            else if (point.pos.y < h and w.upper) {
                point.vel = reflect(point.vel, norm(vec2(1, -1/w.k))) * jumpling;
                point.pos.y = h;
                break;
            }
        }
    }
}

inline vector<vec2> Physical::stbl_vel() {
    vector<vec2> add_acc;
    for (int i = 0; i < points.size(); i++) {
        vec2 add_ac = zero;
        int m = 0;
        for (int j=0; j<points.size(); j++) {
            if (j == i) { m = 1; continue; }

            float k = 1-arms[i][j-m] / dist(points[i].pos, points[j].pos);
            add_ac += (points[j].pos-points[i].pos) * (float)pow(k, 1);

            /*vec2 n = points[j].pos-points[i].pos;
            float a = dist(zero, n) - arms[i][j-m];
            vec2 b = (points[j].vel-points[i].vel), norm(n);
            add_ac += a + b;*/
        }
        add_acc.push_back(add_ac*elasticity/(float)points.size());
    }
    return add_acc;
}

// Drawing
void Physical::draw(RenderWindow &window, Color color=Color::Black) {
    vector<Vertex> vtx; 
    for (int i = 0; i <= points.size(); i++) {
        vtx.push_back(points[(i == points.size()) ? 0 : i].pos);
        vtx[i].color = color;
    }
    Vertex *arrvtx = &vtx[0];
    window.draw(arrvtx, points.size()+1, LinesStrip);
}

void Physical::show_av(RenderWindow &window) {
    for (int i = 0; i < points.size(); i++) {
        window.draw(easy_line(points[i].pos, points[i].pos+points[i].vel/G*30.f, Color(0, 0, 255, 120)), 2, LinesStrip);
        window.draw(easy_line(points[i].pos, points[i].pos+points[i].force/G*10.f, Color::Red), 2, LinesStrip);

        for (int i = 0; i < points.size(); i++) {
            for (int j = 0; j < points.size(); j++) {
                if (abs(j-i) <= 1 or abs(i-j) == points.size()-1) continue;
                window.draw(easy_line(points[i].pos, points[j].pos, Color(0, 100, 0, 70)), 2, LinesStrip);
            }
        }
    }
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