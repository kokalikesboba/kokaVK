#include "window.h"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
}

GlfwContext::GlfwContext()
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to init GLFW");
    }
}

GlfwContext::~GlfwContext()
{
    glfwTerminate();
}

Window::Window(unsigned int width, unsigned int height, const char *title)
{
    this->width = width; 
    this->height = height;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    windowPtr = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!windowPtr) {
        throw std::runtime_error("Failed to create a GLFW window");
    }

    glfwSetWindowSizeLimits(windowPtr, minWidth, minHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowUserPointer(windowPtr, this);
    glfwSetFramebufferSizeCallback(windowPtr, FbSizeCallback);
    glfwSetScrollCallback(windowPtr, ScrollCallback);

    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);

}

void Window::MakeContextCurrent() const
{   
    glfwMakeContextCurrent(windowPtr);
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(windowPtr);
}

void Window::PollEvents()
{
    glfwPollEvents();
    glfwGetWindowSize(windowPtr, &this->width, &this->height);
    glfwGetFramebufferSize(windowPtr, &this->fbWidth, &this->fbHeight);
}

void Window::RenameWindow(const char* title) const
{
    glfwSetWindowTitle(windowPtr, title);
}

void Window::EnableFullscreen() const
{
    glfwSetWindowMonitor(windowPtr, monitor, 0, 0, width, height, GLFW_DONT_CARE); 
}

void Window::DisableFullscreen() const
{
    glfwSetWindowMonitor(windowPtr, NULL, 0, 0, width, height, GLFW_DONT_CARE); 
}

void Window::VerticalSync(bool state) const
{
    glfwSwapInterval(state);
}

GLFWwindow *Window::GetWindowPtr() const
{
    // Constructor already checks if windowPtr is NULL
    return windowPtr;
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(windowPtr) != 0;
}

int Window::GetWidth() const
{
    return width;
}

int Window::GetHeight() const
{
    return height;
}

int Window::GetFbWidth() const
{
    return fbWidth;
}

int Window::GetFbHeight() const
{
    return fbHeight;
}

Window::~Window()
{
    if (windowPtr) {
        glfwDestroyWindow(windowPtr);
        windowPtr = nullptr;
    }
}

void Window::FbSizeCallback(GLFWwindow *win, int w, int h)
{
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    self->fbWidth = w;
    self->fbHeight = h;
}

void Window::ScrollCallback(GLFWwindow *win, double x, double y)
{
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    self->scrollX += x;
    self->scrollY += y;
}

void Window::ConsumeScroll(double& x, double& y)
{
    x = scrollX;
    y = scrollY;
    scrollX = 0.0;
    scrollY = 0.0;
}