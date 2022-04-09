#include <iostream>

#include "Scene.h"


vector<PPoint> points_circle(int sides, float size, vec2 center) {
    vector<PPoint> points;
    for (float a=0; a<PI*2 and points.size()<sides; a+=PI/sides*2) {
        PPoint p(1, 0.2, 0, center + vec2(cos(a), sin(a))*size, HSVtoRGB(360*a/PI/2, 1, 1));
        points.push_back(p);
    }
    return points;
}


int main()
{
    RenderWindow window(VideoMode(W, H), "SBody Simulation", Style::Close | Style::Titlebar);
    window.setFramerateLimit(60);

    Clock delta_clock;
    vec2 center = vec2(W/2, H/2);

    Scene scene(window);

    /// Game objects

    int cnt_of_sds = 30;
    float size=50, mass=1, elastic=50, resistance=0.5, jumpling=0.4, friction=1;
    SBody fig(mass, jumpling, elastic, resistance, friction);
    fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size);
    scene.add(fig);

    /*auto pnts = points_circle(4, 150, center+vec2(15, -70));
    for (PPoint &p : pnts) scene.add(p);*/

    /// add walls

    vector<vec2> ps = { vec2(0, H-200), vec2(W/2, 0), vec2(W, H-200), vec2(W/2, H), vec2(0, H-200) };

    vector<Wall> walls;
    for (int i=0; i<ps.size()-1; i++) {
        Wall w(ps[i], ps[i+1], i==0 or i==1);
        walls.push_back(w);
    }
    for (Wall &w : walls) scene.add(w);
    
    /// Main cycle

    while (window.isOpen())
    {
        float delta_time = delta_clock.restart().asSeconds();
        cout << "FPS: " << round(1/delta_time * 10)/10 << "    \r";
        
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

        //fig.show_dots(5);
        if (Keyboard::isKeyPressed(Keyboard::Q)) {
            for (SBody *p : scene.bodys) (*p).show_av();
            for (PPoint *p : scene.points) (*p).show_av();
            for (Wall *w : scene.walls) (*w).show_normals();
        }
        
        if (Keyboard::isKeyPressed(Keyboard::P)) scene.pause = true;
        else scene.pause = false; 
        
        fig.show_dots(2);
        
        scene.draw();
        scene.frame(delta_time);

        window.display();
    }

    return 0;
}