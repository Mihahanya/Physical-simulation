#pragma once
#include "h.h"
#include "Wall.h"

class Physical
{
public:
    vector<vec2> corners, vel, accel;
    vector<vector<float>> arms;
    vector<Wall> walls;
    float mass, el, b;

    Physical(float mass, float el, float b=0.97) {
        this->mass = mass;
        this->el = el;
        this->b = b;
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
};


// Creating
void Physical::create_regular_polygon(vec2 cntr, int cnt_of_sds, float size) {
    for (float a=0; a<PI*2 and corners.size()<cnt_of_sds; a+=PI/cnt_of_sds*2) {
        corners.push_back(cntr + vec2(cos(a), sin(a))*size);
        vel.push_back(vec2(0, 0));
        accel.push_back(vec2(0, G));
    }
    take_arms();
}

void Physical::create_custom_polygon(vector<vec2> crnrs) {
    for (auto t : crnrs) {
        corners.push_back(t);
        vel.push_back(vec2(0, 0));
        accel.push_back(vec2(0, G));
    }
    take_arms();
}

// Simulation
void Physical::step(float delta_time) {
    vector<vec2> add_acc = stbl_vel();
    for (int i = 0; i < corners.size(); i++) {
        vel[i] += accel[i] + add_acc[i];
        
        for (auto w : walls) {
            float h = w.y_by_x(corners[i].x);
            if (h != INT_MAX) {
                if (corners[i].y > h and !w.upper) {
                    vel[i].y = abs(vel[i].y) * -b;
                    corners[i].y = h;
                    break;
                }
                else if (corners[i].y < h and w.upper) {
                    vel[i].y = abs(vel[i].y) * b;
                    corners[i].y = h;
                    break;
                }
            }
        }

        corners[i] += vel[i] * delta_time;
    }
}

inline vector<vec2> Physical::stbl_vel() {
    vector<vec2> add_acc;
    for (int i = 0; i < corners.size(); i++) {
        vec2 add_ac = zero;
        int m = 0;
        for (int j=0; j<corners.size(); j++) {
            if (j == i) { 
                m = 1;
                continue;
            }
            float k = 1-arms[i][j-m] / dist(corners[i], corners[j]);
            add_ac += (corners[j]-corners[i]) * (float)pow(k, 1);
        }
        add_acc.push_back(add_ac*el/(float)corners.size());
    }
    return add_acc;
}

// Drawing
void Physical::draw(RenderWindow &window, Color color=Color::Black) {
    vector<Vertex> vtx; 
    for (int i = 0; i <= corners.size(); i++) {
        vtx.push_back(corners[(i == corners.size()) ? 0 : i]);
        vtx[i].color = color;
    }
    Vertex *arrvtx = &vtx[0];
    window.draw(arrvtx, corners.size()+1, LinesStrip);
}

void Physical::show_av(RenderWindow &window) {
    for (int i = 0; i < corners.size(); i++) {
        Vertex vtxv[] = { Vertex(corners[i]), Vertex(corners[i]+vel[i]) };
        vtxv[0].color = vtxv[1].color = Color(0, 0, 255, 70);
        window.draw(vtxv, 2, LinesStrip);

        Vertex vtxa[] = { Vertex(corners[i]), Vertex(corners[i]+accel[i]) };
        vtxa[0].color = vtxa[1].color = Color(255, 0, 0, 70);
        window.draw(vtxa, 2, LinesStrip);

        for (int i = 0; i < corners.size(); i++) {
            for (int j=0; j<corners.size(); j++) {
                if (abs(j-i) <= 1 or abs(i-j) == corners.size()-1) continue;
                Vertex vtx[] = { Vertex(corners[i]), Vertex(corners[j]) };
                vtx[0].color = vtx[1].color = Color(0, 100, 0, 70);
                window.draw(vtx, 2, LinesStrip);
            }
        }
    }
}

// Taking arm connections
inline void Physical::take_arms() {
    for (int i=0; i<corners.size(); i++) {
        vector<float> a;
        for (int j=0; j<corners.size(); j++) {
            if (j != i) a.push_back(dist(corners[j], corners[i]));
        }
        arms.push_back(a);
    }
}
