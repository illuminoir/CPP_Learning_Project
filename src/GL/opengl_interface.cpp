#include "opengl_interface.hpp"

std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::_V2::system_clock::duration> current_time;


namespace GL {

void handle_error(const std::string& prefix, const GLenum err)
{
    if (err != GL_NO_ERROR)
    {
        throw std::runtime_error { prefix + std::string { ": " } +
                                   std::string { reinterpret_cast<const char*>(gluErrorString(err)) } };
    }
}

void keyboard(unsigned char key, int, int)
{
    const auto iter = keystrokes.find(key);
    if (iter != keystrokes.end())
    {
        (iter->second)();
    }
}

void toggle_fullscreen()
{
    if (fullscreen)
    {
        glutPositionWindow(10, 10);
        glutReshapeWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    }
    else
    {
        glutFullScreen();
    }

    handle_error("Toggle fullscreen");
    fullscreen = !fullscreen;
}

void change_zoom(const float factor)
{
    zoom *= factor;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom, zoom, -zoom, zoom, 0.0f, 1.0f); // left, right, bottom, top, near, far
    handle_error("Zoom");
}

void reshape_window(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom, zoom, -zoom, zoom, 0.0f, 1.0f); // left, right, bottom, top, near, far
    handle_error("Cannot reshape window");
}

void display(void)
{
    // sort the displayables by their z-coordinate
    std::sort(display_queue.begin(), display_queue.end(), disp_z_cmp {});
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom, zoom, -zoom, zoom, 0.0f, 1.0f); // left, right, bottom, top, near, far
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    for (const auto& item : display_queue)
    {
        item->display();
    }
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}
void timer(const int step)
{
    auto start_time = std::chrono::system_clock::now();
    std::chrono::duration<float> dDelta_time = (start_time - current_time);
    std::chrono::milliseconds mDelta_time = std::chrono::duration_cast<std::chrono::milliseconds>(dDelta_time);

    float delta_time = mDelta_time.count();

    if(delta_time < 1000/60)
    {
        delta_time = 1000/60;
    }

    //for (auto& item : move_queue)
    for(auto it = move_queue.begin() ; it != move_queue.end(); it++)
    {

        if(!((*it)->move(std::min(delta_time/1000, (1000/60.f)))))
        {
            auto casted_item = dynamic_cast<const GL::Displayable*>(*it);
            it = GL::move_queue.erase(it);
            if(std::find(GL::display_queue.begin(), GL::display_queue.end(), casted_item) != GL::display_queue.end()) {
                GL::display_queue.erase(std::remove(GL::display_queue.begin(), GL::display_queue.end(), casted_item));
            }
            delete(*it);
        }
    }

    //auto end_time = std::chrono::system_clock::now();
    current_time = std::chrono::system_clock::now();

    glutPostRedisplay();
    glutTimerFunc(1000u / ticks_per_sec, timer, step + 1);
}
/*
void timer(const int step)
{

    auto delta_time = clock::now() - time_start;
    time_start = clock::now();



    for (auto& item : move_queue)
    {
        if(item->move())
        {
            GL::display_queue.erase(std::remove(GL::display_queue.begin(), GL::display_queue.end(), item));
            GL::move_queue.erase(std::remove(GL::move_queue.begin(), GL::move_queue.end(), item));
            delete(item);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1000u * ticks_per_sec, timer, step + 1);
}
*/
void init_gl(int argc, char** argv, const char* title)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    glutCreateWindow(title);
    // glutFullScreen();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glEnable(GL_DEPTH_TEST);
    // The following two lines enable semi transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape_window);

    handle_error("Cannot init OpenGL");
}

void loop()
{
    glutTimerFunc(100, timer, 0);
    glutMainLoop();
}

void exit_loop()
{
    glutLeaveMainLoop();
}

} // namespace GL