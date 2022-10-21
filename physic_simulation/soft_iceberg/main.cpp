#include <iostream>

#include "config.h"
#include "Scene.h"

vector<PPoint> points_circle(int sides, float size, vec2 center) {
    vector<PPoint> points;
    for (float a=0; a<PI*2 and points.size()<sides; a+=PI/sides*2) {
        PPoint p(100, 0.95, 0, HSVtoRGB(360*a/PI/2, 1, 1)); 
        p.set_pos(center + vec2(cos(a), sin(a))*size);
        points.push_back(p);
    }
    return points;
}


int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 5;

    RenderWindow window(sf::VideoMode(W, H), "Physical Simulation", Close | Titlebar, settings);

#if FPS > 0
    window.setFramerateLimit(FPS);
#endif

    vec2 center = vec2(W/2, H/2);

    Scene scene(&window);

    /// Game objects

    float mass = 0.85, bounciness = 0.1, friction = 1, elastic = 1500, resistance = 0.3;
    SoftBody fig(mass, bounciness, friction, elastic, resistance);
    fig.color = Color{0, 0, 0, 100};

    fig.create_figure(center, 10, 7, 25);

    scene.add(fig);

    //float mass=0.8, bounciness=0.1, friction=1, elastic=5e4, resistance=0.4;
    //SoftContour fig(mass, bounciness, friction, elastic, resistance);

    //int cnt_of_sds = 50; float size=50;
    //fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size);
    ////fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size, [](float a) { return vec2{sin(a*1.5f), cos(a-4.6f)}; });
    ////fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size, [](float t) { return vec2{cos(t), sin(t)}*(4+cos(t*5.f))/5.f; });
    ////fig.create_custom_polygon({ vec2(300, 200), vec2(280, 500), vec2(700, 500), vec2(400, 380) });
    //
    //scene.add(fig);
    
    /*auto pnts = points_circle(2000, 150, center+vec2(15, -70));
    for (PPoint &p : pnts) scene.add(p);*/

    /// add walls

    /*VolumetricWall vw(center+vec2(250, 170), vec2(100, 100), 0);
    scene.add(vw);*/

    //vector<vec2> ps = { vec2(0, H-200), vec2(W/2, 0), vec2(W, H-400), vec2(W/2, H-100) };
    vector<vec2> ps = { vec2(50, 50), vec2(W-50, 50), vec2(W-50, H*0.6), vec2(W/3, H-50), vec2(0, H-50) };
    ps.push_back(*ps.begin());

    vector<Wall> walls;
    for (int i=0; i<ps.size()-1; i++) {
        Wall w(ps[i], ps[i+1]);
        w.drop_zone = 100;
        walls.push_back(w);
    }
    for (Wall& w : walls) scene.add(w);

    /// Main cycle ///

    main_loop([&]
    {
        if (Keyboard::isKeyPressed(Keyboard::Q)) {
            for (auto& p : scene.contours) p->show_av();
            for (auto& p : scene.sbodys)   p->show_av();
            for (auto& p : scene.points)   p->show_av();
            for (auto& w : scene.walls)    w->show_normals();
        }
        
        if (Mouse::isButtonPressed(Mouse::Left) and !scene.pause) {
            PPoint& p = fig.points[0];

            vec2 m = (vec2)Mouse::getPosition(window);

            p.add_force((m-p.get_pos())*300.f - p.get_vel()*20.f);

            ff::easy_line(m, p.get_pos(), window, Color::Green);
        }

        fig.show_dots(1);

        /*if (Mouse::isButtonPressed(Mouse::Right)) {
            vw.angle += 0.01;
            vw.move_to((vec2)Mouse::getPosition(window));
        }*/
        
        cout << "FPS: " << round(1/scene.delta_time * 10)/10 << " \r";
    }, scene);

    return 0;
}