#pragma once

#include "include.h"

class Drawable {
public:
    Color color = Color::Black;

    Drawable() {}
    Drawable(Color color) : color{color} {}

    virtual void draw() = 0;

    virtual void set_window(RenderWindow* window) {
        this->window = window;
    }

protected:
    RenderWindow* window {nullptr};
};
