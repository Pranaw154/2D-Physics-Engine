#include "precompiled.h"

using namespace std;

S2D_Window *window;
Scene scene(1.0f / 60.0f, 10);
bool frameStepping = false;
bool canStep = false;
Clock g_Clock;

void on_mouse(S2D_Event e)
{
    switch (e.type)
    {
    case S2D_MOUSE_UP: // button is released

        if (e.button == S2D_MOUSE_LEFT)
        {
            // Draw a circle at that point
            cout << "Left click at (" << window->mouse.x << ", " << window->mouse.y << ")\n";
            Circle c(Random(10.0, 80.0));
            Body *b = scene.Add(&c, window->mouse.x, window->mouse.y);
        }
        else if (e.button == S2D_MOUSE_RIGHT)
        {
            // Draw a polygon
            int numVertex = rand() % 2 + 3;
            cout << numVertex << "\n";
            cout << "Right click at (" << window->mouse.x << ", " << window->mouse.y << ")\n";
            PolygonShape poly;
            Vec *vertices = new Vec[numVertex];
            double e = 10;
            for (int i = 0; i < numVertex; ++i)
            {
                int x = Random(-e, e);
                int factor = 40;
                if (x < 0)
                    x -= factor;
                else
                    x += factor;
                int y = Random(-e, e);
                if (y < 0)
                    y -= factor;
                else
                    y += factor;
                cout << x << " " << y << endl;
                vertices[i].Set(x, y);
            }
            poly.Set(vertices, numVertex);

            Body *b = scene.Add(&poly, window->mouse.x, window->mouse.y);

            b->restitution = 1.0;
            b->dynamicFriction = 0.0;
            b->staticFriction = 0.0;
            double radians = Random(0, PI / 3);
            cout << radians << endl;
            b->SetOrient(radians);
            delete[] vertices;
        }
        else if (e.button == S2D_MOUSE_X1)
        {
            // static circle
            Circle c(100.0);
            Body *b = scene.Add(&c, window->mouse.x, window->mouse.y);
            b->SetStatic();
        }
        else if (e.button == S2D_MOUSE_X2)
        {
            // Draw a polygon
            int numVertex = rand() % 2 + 3;
            cout << numVertex << "\n";
            cout << "Right click at (" << window->mouse.x << ", " << window->mouse.y << ")\n";
            PolygonShape poly;
            Vec *vertices = new Vec[numVertex];
            double e = 10;
            for (int i = 0; i < numVertex; ++i)
            {
                int x = Random(-e, e);
                int factor = 0;
                if (x < 0)
                    x -= factor;
                else
                    x += factor;
                int y = Random(-e, e);
                if (y < 0)
                    y -= factor;
                else
                    y += factor;
                vertices[i].Set(x, y);
            }
            vertices[0].Set(-e, e);
            vertices[0].Set(e, e);
            vertices[0].Set(-e, -e);
            vertices[0].Set(e, -e);
            poly.Set(vertices, numVertex);

            Body *b = scene.Add(&poly, window->mouse.x, window->mouse.y);
            double radians = Random(-PI, PI);
            cout << radians << endl;
            b->SetOrient(radians);
            b->SetStatic();

            delete[] vertices;
        }
        if (e.dblclick)
            puts("Double click");
        break;

    case S2D_MOUSE_MOVE:
        //   puts("Mouse movement");
        //   printf("delta x: %i\ndelta y: %i\n", e.delta_x, e.delta_y);
        break;
    }
}

void on_key(S2D_Event e)
{

    switch (e.type)
    {
    case S2D_KEY_UP:
        printf("Key up: %s\n", e.key);
        if (!strcmp(e.key, "Escape"))
            S2D_Close(window);
        break;
    }
}

void render()
{
}

void update()
{
    static double accumulator = 0;

    // Different time mechanisms for Linux and Windows
#ifdef WIN32
    accumulator += g_Clock.Elapsed();
#else
    accumulator += g_Clock.Elapsed() / static_cast<double>(std::chrono::duration_cast<clock_freq>(std::chrono::seconds(1)).count());
#endif

    g_Clock.Start();

    accumulator = Clamp(0.0f, 0.1f, accumulator);
    while (accumulator >= dt)
    {
        if (!frameStepping)
            scene.Step();
        else
        {
            if (canStep)
            {
                scene.Step();
                canStep = false;
            }
        }
        accumulator -= dt;
    }

    g_Clock.Stop();

    scene.Render();
}

int main(int argc, char const *argv[])
{
    window = S2D_CreateWindow("Simple Physics Engine", 800, 700, update, render, S2D_RESIZABLE);
    window->viewport.mode = S2D_SCALE;
    window->on_key = on_key;
    window->on_mouse = on_mouse;
    PolygonShape poly1;
    poly1.SetBox(window->viewport.width, 1);
    Body *b1 = scene.Add(&poly1, 0, window->viewport.height-10);
    b1->SetStatic();
    b1->SetOrient(0);
    cout<<window->viewport.height<<"\n";
    PolygonShape poly2;
    poly1.SetBox(1, window->viewport.height);
    Body *b2 = scene.Add(&poly1, 10, 0);
    b2->SetStatic();
    b2->SetOrient(0);

    PolygonShape poly3;
    poly1.SetBox(1, window->viewport.height);
    Body *b3 = scene.Add(&poly1, window->viewport.width-10, 0);
    b3->SetOrient(0);

    b3->SetStatic();
    S2D_Show(window);
    return 0;
}