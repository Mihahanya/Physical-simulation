#include <iostream>

#include "Physical.h"
#include "Wall.h"

int main()
{
    RenderWindow window(VideoMode(W, H), "Physical Simulation");
    window.setFramerateLimit(60);

    Clock delta_clock;
    vec2 center = vec2(W/2, H/2);

    // Game objects

    vec2 cntr = vec2(W/2, 200);
    int cnt_of_sds = 6;
    float size=30, mass=1, elastic=100, jumpling=0.3, friction=1;
    Physical fig(mass, elastic, jumpling, friction);

    fig.create_regular_polygon(cntr, cnt_of_sds, size);

    // add walls

    vector<vec2> ps = { vec2(10, H-200), vec2(W/2, 10), vec2(W-10, H-200), vec2(W/2, H-10), vec2(10, H-200) };

    vector<Wall> walls;
    for (int i=0; i<ps.size()-1; i++) {
        Wall w(ps[i], ps[i+1], i==0 or i==1);
        fig.add_wall(w);
        walls.push_back(w);
    }
    
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
        fig.show_dots(1, window);
        if (Keyboard::isKeyPressed(Keyboard::Q)) fig.show_av(window);

        for (auto w : walls) w.draw(window);
        
        window.display();
    }

    return 0;
}