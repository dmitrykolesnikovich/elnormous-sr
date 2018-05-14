//
//  SoftwareRenderer
//

#include "Window.hpp"

Window::Window()
{
}

Window::~Window()
{
}

bool Window::init(int argc, const char** argv)
{
    renderer.init(width, height);

    return true;
}

const sr::Buffer& Window::render()
{
    renderer.clear();
    renderer.draw();
    return renderer.getFrameBuffer();
}

void Window::onResize()
{
    renderer.resize(width, height);
}
