#pragma once

#include "config.h"

template<class Rendering=RenderWindow, class Color=Color>
class Drawable {
public:
    Color color = Color::Black;

    Drawable() {}
    Drawable(Color color) : color{color} {}

    virtual void draw() = 0;

    virtual void set_window(Rendering* window) {
        this->window = window;
    }

protected:
    Rendering* window {nullptr};
};

using sfDrawable = Drawable<RenderWindow, Color>;
