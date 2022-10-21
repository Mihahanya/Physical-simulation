#include <iostream>

#include "../physic_simulation/Scene.h"

// WARNING: not ready!

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 5;

    RenderWindow window(sf::VideoMode(W, H), "Soft Iceberg Simulation", Close | Titlebar, settings);

#if FPS > 0
    window.setFramerateLimit(FPS);
#endif

    vec2 center = vec2(W / 2, H / 2);

    Scene scene(&window);

    /// Game objects

    float mass = 0.85, bounciness = 0.1, friction = 1, elastic = 1500, resistance = 0.3;
    SoftBody fig(mass, bounciness, friction, elastic, resistance);
    fig.color = Color{0, 0, 0, 100};

    fig.create_figure(center, 10, 7, 25);

    scene.add(fig);

    //float mass=1., bounciness=0.1, friction=1, elastic=5e5, resistance=0.6;
    //SoftContour fig(mass, bounciness, friction, elastic, resistance);

    //int cnt_of_sds = 50; float size=50;
    ////fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size);
    ////fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size, [](float a) { return vec2{sin(a*1.5f), cos(a-4.6f)}; });
    //fig.create_regular_polygon(vec2(W/2+50, 200), cnt_of_sds, size, [](float t) { return vec2{cos(t), sin(t)}*(4+cos(t*5.f))/5.f; });
    ////fig.create_custom_polygon({ vec2(300, 200), vec2(280, 500), vec2(700, 500), vec2(400, 380) });
    //
    //scene.add(fig);

    //

    float water_level = 500, actual_wtr_lvl = water_level,
          water_density = 3e-2,
          water_resist = 6.5e-3;

    sf::RectangleShape water_shape(Vector2f(W, H));
    water_shape.setFillColor(Color(100, 150, 220));

    /// add walls

    vector<vec2> ps = { vec2(0, 0), vec2(W, 0), vec2(W, H), vec2(0, H) };
    ps.push_back(*ps.begin());

    vector<Wall> walls;
    for (int i = 0; i < ps.size() - 1; i++) {
        Wall w(ps[i], ps[i + 1]);
        w.drop_zone = 100;
        walls.push_back(w);
    }
    for (Wall& w : walls) scene.add(w);

    /// Main cycle ///

    main_loop([&]
        {
            water_shape.setPosition(Vector2f{0, actual_wtr_lvl});
            window.draw(water_shape);
         
            if (Keyboard::isKeyPressed(Keyboard::Q)) {
                for (auto& p : scene.contours) p->show_av();
                for (auto& p : scene.sbodys)   p->show_av();
                for (auto& p : scene.points)   p->show_av();
                for (auto& w : scene.walls)    w->show_normals();
                fig.show_dots(1);
            }

            if (Mouse::isButtonPressed(Mouse::Left) and !scene.pause) {
                PPoint& p = fig.points[0];

                vec2 m = (vec2)Mouse::getPosition(window);

                p.add_force((m - p.get_pos()) * 300.f - p.get_vel() * 20.f);

                ff::easy_line(m, p.get_pos(), window, Color::Green);
            }

            int under_n = 0;
            for (auto& p : fig.points) {
                if (p.get_pos().y > actual_wtr_lvl) {
                    //p.add_force(-vs::norm(fig.get_pos()-p.get_pos())*GRAV.y*water_density);
                    p.add_force(-vec2{0, min(50.f, p.get_pos().y-actual_wtr_lvl)}*GRAV.y*water_density);
                    vec2 vel = p.get_vel();
                    p.add_force(-vec2{vel.x*abs(vel.x), vel.y*abs(vel.y)}*water_resist);

                    under_n++;
                }
            }

            // Smooth changing water level
            actual_wtr_lvl += (water_level - (float)under_n/fig.points.size()*(7*10*25*25)/W - actual_wtr_lvl) * 0.1f;

            cout << "FPS: " << round(1 / scene.delta_time * 10) / 10 << " \r";
        }, scene);

    return 0;
}