#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#define PI 3.14159265

#define W 800
#define H 800

using namespace std; 
using namespace sf;

typedef Vector2f vec2;

const vec2 GRAVITY = vec2(0, 300);
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

Color HSVtoRGB(int hue, float sat, float val)
{
    hue %= 360;
    while (hue<0) hue += 360;

    if (sat<0.f) sat = 0.f;
    if (sat>1.f) sat = 1.f;

    if (val<0.f) val = 0.f;
    if (val>1.f) val = 1.f;

    int h = hue/60;
    float f = float(hue)/60-h;
    float p = val*(1.f-sat);
    float q = val*(1.f-sat*f);
    float t = val*(1.f-sat*(1-f));

    switch(h)
    {
    default:
        case 0:
        case 6: return sf::Color(val*255, t*255, p*255);
        case 1: return sf::Color(q*255, val*255, p*255);
        case 2: return sf::Color(p*255, val*255, t*255);
        case 3: return sf::Color(p*255, q*255, val*255);
        case 4: return sf::Color(t*255, p*255, val*255);
        case 5: return sf::Color(val*255, p*255, q*255);
    }
}
