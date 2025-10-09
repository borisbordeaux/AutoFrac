#ifndef AUTOFRAC_WINDOW_H
#define AUTOFRAC_WINDOW_H

class GLFWwindow;

class Window {
public:
    Window();
    ~Window();

    bool shouldClose() const;
    void swapBuffers() const;
    void pollEvents() const;

    inline GLFWwindow* window() const { return m_window; }

private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    GLFWwindow* m_window;
};

#endif //AUTOFRAC_WINDOW_H
