#include <iostream>

#include "Scene.h"


vector<PPoint> points_circle(int sides, float size, vec2 center) {
    vector<PPoint> points;
    for (float a=0; a<PI*2 and points.size()<sides; a+=PI/sides*2) {
        PPoint p(100, 0.95, 0, HSVtoRGB(360*a/PI/2, 1, 1)); p.pos = center + vec2(cos(a), sin(a))*size;
        points.push_back(p);
    }
    return points;
}


int main()
{
    RenderWindow window(sf::VideoMode(W, H), "Physical Simulation", Close | Titlebar);
    window.setFramerateLimit(60);

    vec2 center = vec2(W/2, H/2);

    Scene scene(window);

    bool is_focuse = true;

    /// Game objects

    float mass=1, jumpling=0.1, elastic=20, resistance=0.5, connection_by_dist=1./100, friction=1;
    SoftContour fig(mass, jumpling, elastic, resistance, connection_by_dist, friction);   
    
    int cnt_of_sds = 100; float size=50;
    fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size);
    //fig.create_custom_polygon({ vec2(300, 200), vec2(280, 500), vec2(700, 500), vec2(400, 380) });
    
    scene.add(&fig);
    
    auto pnts = points_circle(2000, 150, center+vec2(15, -70));
    for (PPoint &p : pnts) scene.add(&p);

    /// add walls

    VolumetricWall vw(center+vec2(-60, 250), vec2(100, 100), 0);
    scene.add(&vw);

    vector<vec2> ps = { vec2(0, H-200), vec2(W/2, 0), vec2(W, H-400), vec2(W/2, H-100), vec2(0, H-200) };

    vector<Wall> walls;
    for (int i=0; i<ps.size()-1; i++) {
        Wall w(ps[i], ps[i+1]);
        w.drop_zone = 100;
        walls.push_back(w);
    }
    for (Wall &w : walls) scene.add(&w);

    /// Main cycle ///

    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::P)) scene.pause = true;
        else scene.pause = false; 

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            
            if (e.type == Event::LostFocus) is_focuse = false;
            if (e.type == Event::GainedFocus) is_focuse = true;
        }
        if (!is_focuse) scene.pause = true;

        window.clear(Color::White);

        //
        
        cout << "FPS: " << round(1/scene.delta_time * 10)/10 << "    \r";
        

        if (Keyboard::isKeyPressed(Keyboard::Q)) {
            for (SoftContour *p : scene.bodys) (*p).show_av();
            for (PPoint *p : scene.points) (*p).show_av();
            for (Wall *w : scene.walls) (*w).show_normals();
        }
        
        if (Mouse::isButtonPressed(Mouse::Left) and !scene.pause) {
            vec2 m = (vec2)Mouse::getPosition(window);
            vec2 p = m-fig.points[0].pos;

            fig.points[0].move(p*300.f - fig.points[0].vel*20.f);

            ff::easy_line(m, fig.points[0].pos, window, Color::Green);
        }

        fig.show_dots(1);

        vw.move_to((vec2)Mouse::getPosition(window));
        
        //

        scene.draw();
        scene.update();

        window.display();
    }

    return 0;
}