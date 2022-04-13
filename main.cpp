#include <iostream>

#include "Scene.h"


vector<PPoint> points_circle(int sides, float size, vec2 center) {
    vector<PPoint> points;
    for (float a=0; a<PI*2 and points.size()<sides; a+=PI/sides*2) {
        PPoint p(1, 0.9, 0, center + vec2(cos(a), sin(a))*size, false, HSVtoRGB(360*a/PI/2, 1, 1));
        points.push_back(p);
    }
    return points;
}


int main()
{
    RenderWindow window(VideoMode(W, H), "Physical Simulation", Style::Close | Style::Titlebar);
    window.setFramerateLimit(60);

    vec2 center = vec2(W/2, H/2);

    Scene scene(window);

    /// Game objects

    float mass=2, elastic=15, resistance=0.4, jumpling=0.1, friction=1;
    SBody fig(mass, jumpling, elastic, resistance, friction);   
    
    int cnt_of_sds = 30; float size=50;
    fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size);
    //fig.create_custom_polygon({ vec2(300, 200), vec2(280, 500), vec2(700, 500), vec2(400, 380) });
    
    scene.add(fig);
    
    auto pnts = points_circle(100, 150, center+vec2(15, -70));
    for (PPoint &p : pnts) scene.add(p);

    /// add walls

    vector<vec2> ps = { vec2(0, H-200), vec2(W/2, 0), vec2(W, H-200), vec2(W/2, H), vec2(0, H-200) };

    vector<Wall> walls;
    for (int i=0; i<ps.size()-1; i++) {
        Wall w(ps[i], ps[i+1], i<2 ? 1 : -1);
        walls.push_back(w);
    }
    for (Wall &w : walls) scene.add(w);
    
    //

    Wall test_wall(vec2(W-130, 0), vec2(W-130, H));
    scene.add(test_wall);

    /// Main cycle ///

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
            if (e.type == Event::Closed) window.close();

        window.clear(Color::White);

        //
        
        cout << "FPS: " << round(1/scene.delta_time * 10)/10 << "    \r";
        
        if (Mouse::isButtonPressed(Mouse::Left)) {
            vec2 m = (vec2)Mouse::getPosition(window);
            vec2 p = m-fig.center;

            fig.move(p*20.f - fig.velocity*2.f);

            window.draw(easy_line(m, fig.center, Color::Green), 2, LinesStrip);
        }

        if (Keyboard::isKeyPressed(Keyboard::Q)) {
            for (SBody *p : scene.bodys) (*p).show_av();
            for (PPoint *p : scene.points) (*p).show_av();
            for (Wall *w : scene.walls) (*w).show_normals();
            test_wall.show_normals();
        }
        
        if (Keyboard::isKeyPressed(Keyboard::P)) scene.pause = true;
        else scene.pause = false; 
        
        fig.show_dots(1);
        
        //

        scene.draw();
        scene.frame();

        window.display();
    }

    return 0;
}