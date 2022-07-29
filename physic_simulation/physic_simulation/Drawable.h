#pragma once

#include "include.h"

class Drawable {
public:
    void set_window(RenderWindow* window) {
        this->window = window;
    }

protected:
    RenderWindow* window = nullptr;
};
