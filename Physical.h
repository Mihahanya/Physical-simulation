#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#define PI 3.14159265

using namespace std; 
using namespace sf;

typedef Vector2f vec2;

class Physical
{
public:
    vector<vec2> corners;
    char cnt_of_sds;

    void create_regular_polygon(vec2 cntr, int cnt_of_sds, float size) {
        this->cnt_of_sds = cnt_of_sds;

        for (float a=0; a<PI*2; a+=PI/cnt_of_sds*2) {
            corners.push_back(cntr + vec2(cos(a), sin(a))*size);
        }
    }

    void draw(RenderWindow &window) {
        vector<Vertex> vtx; 
        for (int i = 0; i <= cnt_of_sds; i++) {
            vtx.push_back(corners[(i == cnt_of_sds) ? 0 : i]);
            vtx[i].color = Color(0, 0, 0);
        }
        Vertex *arrvtx = &vtx[0];
        window.draw(arrvtx, cnt_of_sds+1, LinesStrip);
    }

private:

};

