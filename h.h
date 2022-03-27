#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#define PI 3.14159265

#define W 800
#define H 600

#define VISION false

using namespace std; 
using namespace sf;

typedef Vector2f vec2;

const float G = 10;
vec2 zero = vec2(0, 0);

float dist(vec2 v1, vec2 v2) {
	return sqrt(pow(v1.x-v2.x, 2) + pow(v1.y-v2.y, 2));
}

vec2 norm(vec2 v) {
	return v / dist(zero, v);
}
