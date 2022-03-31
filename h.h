#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#define PI 3.14159265

#define W 800
#define H 800

using namespace std; 
using namespace sf;

typedef Vector2f vec2;

const float G = 300;
vec2 zero = vec2(0, 0);

float dist(vec2 v1, vec2 v2) {
	return sqrt(pow(v1.x-v2.x, 2) + pow(v1.y-v2.y, 2));
}

inline float dot(vec2 a, vec2 b) {
    return a.x*b.x + a.y*b.y;
}

vec2 norm(vec2 v) {
	return v / dist(zero, v);
}

inline vec2 reflect(vec2 rd, vec2 n) {
    return rd - n*2.f*dot(n, rd);
}

Vertex* easy_line(vec2 v1, vec2 v2, Color color=Color::Black) {
    Vertex vtx[] = { Vertex(v1), Vertex(v2) };
    vtx[0].color = vtx[1].color = color;
    return vtx;
}
