#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "windows.h"

#include "Physical.h"

#define W 800
#define H 600

int main()
{
    RenderWindow window(VideoMode(W, H), "Physical Simulation");
    Clock delta_clock;

    //

    vec2 cntr = vec2(W/2, H/2);
    const int cnt_of_sds = 6;
    const float size = 30;

    Physical figr;
    figr.create_regular_polygon(cntr, cnt_of_sds, size);

    while (window.isOpen())
    {
        Time delta_time = delta_clock.restart();
        cout << delta_time.asSeconds() << '\n';

        //

        Event e;
        while (window.pollEvent(e))
            if (e.type == Event::Closed) window.close();

        window.clear(Color(250, 250, 250));
        figr.draw(window);
        window.display();

        //Sleep(200);
    }

    return 0;
}