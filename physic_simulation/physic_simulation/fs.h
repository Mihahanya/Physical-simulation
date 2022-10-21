#pragma once

#include "Drawable.h"

namespace vs
{
    const vec2 zero {0, 0};

    inline float dist(const vec2& v1, const vec2& v2) {
        return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
    }

    inline float length(const vec2& v) {
        return dist(zero, v);
    }

    inline float dot(const vec2& a, const vec2& b) {
        return a.x * b.x + a.y * b.y;
    }

    inline vec2 norm(const vec2& v) {
        return v / dist(vs::zero, v);
    }

    inline float angle(const vec2& v1, const vec2& v2) {
        return acos(dot(norm(v1), norm(v2)));
    }

    inline vec2 reflect(const vec2& rd, const vec2& n) {
        return rd - n * 2.f * dot(n, rd);
    }

    inline vec2 rotate(const vec2& v, float a) {
        return { v.x * cos(a) - v.y * sin(a), v.x * sin(a) + v.y * cos(a) };
    }

    float polygon_area(const vector<vec2>& points) {
        float area = 0;
        for (int i=0; i<points.size(); ++i) {
            if (i != points.size()-1)
                area += (points[i].x*points[i+1].y - points[i].y*points[i+1].x) / 2.;
            else 
                area += (points[i].x*points[1].y - points[i].y*points[1].x) / 2.;
        }
        return area;
    }

    inline vec2 vec_mul(const vec2& v1, const vec2& v2) {
        return vec2{v1.x*v2.x, v1.y*v2.y};
    }
}

namespace ff
{
    inline void easy_line(const vec2& v1, const vec2& v2, RenderWindow& window, Color color = Color::Black) {
        Vertex vtx[] = { Vertex(v1), Vertex(v2) };
        vtx[0].color = vtx[1].color = color;
        window.draw(vtx, 2, Lines);
    }

    inline void easy_circle(const vec2& pos, float r, RenderWindow& window, Color color = Color::Black) {
        sf::CircleShape c(r);
        c.setFillColor(color);
        c.setPosition(vec2(pos - vec2(r, r)));

        window.draw(c);
    }

    class FixedLine : public sfDrawable {
    public:
        vec2 *p1, *p2;

        FixedLine(vec2 *p1, vec2 *p2, RenderWindow* window, Color col=Color::Black) : sfDrawable{col}, p1{p1}, p2{p2} {
            set_window(window);
        }

        void draw() override {
            easy_line(*p1, *p2, *window, color);
        }
    };
}

Color HSVtoRGB(int hue, float sat, float val)
{
    hue %= 360;
    while (hue < 0) hue += 360;

    if (sat < 0.f) sat = 0.f;
    if (sat > 1.f) sat = 1.f;

    if (val < 0.f) val = 0.f;
    if (val > 1.f) val = 1.f;

    int h = hue / 60;
    float f = float(hue) / 60 - h;
    float p = val * (1.f - sat);
    float q = val * (1.f - sat * f);
    float t = val * (1.f - sat * (1 - f));

    switch (h)
    {
    default:
    case 0:
    case 6: return Color(val * 255, t * 255, p * 255);
    case 1: return Color(q * 255, val * 255, p * 255);
    case 2: return Color(p * 255, val * 255, t * 255);
    case 3: return Color(p * 255, q * 255, val * 255);
    case 4: return Color(t * 255, p * 255, val * 255);
    case 5: return Color(val * 255, p * 255, q * 255);
    }
}
