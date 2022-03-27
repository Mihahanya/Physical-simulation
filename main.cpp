#include <iostream>

#include "Physical.h"
#include "Wall.h"

int main()
{
    RenderWindow window(VideoMode(W, H), "Physical Simulation");
    window.setFramerateLimit(60);

    Clock delta_clock;

    // Game objects

    vec2 cntr = vec2(W/2-50, 200);
    const int cnt_of_sds = 3;
    const float size=50, mass=1, elastic=1000, jumpling=0.6;
    Physical fig(mass, elastic, jumpling);

    fig.create_regular_polygon(cntr, cnt_of_sds, size);

    Wall wall(vec2(0, H-150), vec2(W, H));    fig.add_wall(wall);
    Wall wall2(vec2(0, H), vec2(W, H-150));   fig.add_wall(wall2);
    
    Wall walll(vec2(0, 500), vec2(W/2, 0), true);   fig.add_wall(walll);
    Wall wallr(vec2(W/2, 0), vec2(W, 500), true);   fig.add_wall(wallr);

    CircleShape c(2);
    c.setFillColor(Color::Black);
    
    // Main cycle

    while (window.isOpen())
    {
        float delta_time = delta_clock.restart().asSeconds();
        cout << "FPS: " << round(1/delta_time * 10)/10 << "    \r";

        fig.step(delta_time);
        if (Keyboard::isKeyPressed(Keyboard::A)) fig.points[0].vel += vec2(-100, 0);
        if (Keyboard::isKeyPressed(Keyboard::D)) fig.points[0].vel += vec2(100, 0);

        c.setPosition(fig.points[0].pos.x-2, wall.y_by_x(fig.points[0].pos.x)-2);

        //

        Event e;
        while (window.pollEvent(e))
            if (e.type == Event::Closed) window.close();

        window.clear(Color::White);
        window.draw(c);
        fig.draw(window);

        if (Keyboard::isKeyPressed(Keyboard::Q)) fig.show_av(window);

        wall.draw(window);
        wall2.draw(window);

        walll.draw(window);
        wallr.draw(window);
        
        window.display();
    }

    return 0;
}