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
    const int cnt_of_sds = 5;
    const float size=30, mass=1, elastic=50, jumpling=0.3, friction=1;
    Physical fig(mass, elastic, jumpling, friction);

    fig.create_regular_polygon(cntr, cnt_of_sds, size);

    Wall wall(vec2(0, H-150), vec2(W/2, H-10));    fig.add_wall(wall);
    Wall wall2(vec2(W/2, H-10), vec2(W, H-150));   fig.add_wall(wall2);
    
    Wall walll(vec2(0, 500), vec2(W/2, 0), true);   fig.add_wall(walll);
    Wall wallr(vec2(W/2, 0), vec2(W, 500), true);   fig.add_wall(wallr);
    
    // Main cycle

    while (window.isOpen())
    {
        float delta_time = delta_clock.restart().asSeconds();
        cout << "FPS: " << round(1/delta_time * 10)/10 << "    \r";

        fig.frame(delta_time);
        /*if (Keyboard::isKeyPressed(Keyboard::A)) fig.move(vec2(-500, 0));
        if (Keyboard::isKeyPressed(Keyboard::D)) fig.move(vec2( 500, 0));*/
        
        if (Mouse::isButtonPressed(Mouse::Left)) {
            vec2 m = (vec2)Mouse::getPosition(window);
            float d = dist(m, fig.center);
            fig.move(norm(m-fig.center) * d*10.f);
        }

        //

        Event e;
        while (window.pollEvent(e))
            if (e.type == Event::Closed) window.close();

        window.clear(Color::White);

        fig.draw(window);
        fig.show_dots(2, window);
        if (Keyboard::isKeyPressed(Keyboard::Q)) fig.show_av(window);

        wall.draw(window);
        wall2.draw(window);

        walll.draw(window);
        wallr.draw(window);
        
        window.display();
    }

    return 0;
}