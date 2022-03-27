#include <iostream>

#include "Physical.h"
#include "Wall.h"

int main()
{
    RenderWindow window(VideoMode(W, H), "Physical Simulation");
    window.setFramerateLimit(60);

    Clock delta_clock;

    // Game objects

    vec2 cntr = vec2(W/2-50, H-200);
    const int cnt_of_sds = 5;
    const float size = 30, mass = 1, elastic = 10;
    Physical fig(mass, elastic);
    fig.create_regular_polygon(cntr, cnt_of_sds, size);

    Wall wall(vec2(0, H-150), vec2(W, H));    fig.add_wall(wall);
    Wall wall2(vec2(0, H), vec2(W, H-150));   fig.add_wall(wall2);

    CircleShape c(2);
    c.setFillColor(Color::Black);
    
    // Main cycle

    while (window.isOpen())
    {
        float delta_time = delta_clock.restart().asSeconds();
        cout << "FPS: " << 1/delta_time << '\n';

        fig.step(delta_time);

        c.setPosition(fig.corners[0].x-2, wall.y_by_x(fig.corners[0].x)-2);

        //

        Event e;
        while (window.pollEvent(e))
            if (e.type == Event::Closed) window.close();

        window.clear(Color::White);
        window.draw(c);
        fig.draw(window);
#if VISION
        fig.show_av(window);
#endif
        wall.draw(window);
        wall2.draw(window);
        
        window.display();
    }

    return 0;
}