//
//  SoftwareRenderer
//

#pragma once

#include <X11/Xlib.h>
#include "Application.hpp"
#include "Window.hpp"

namespace demo
{
    class WindowLinux: public Window
    {
    public:
        WindowLinux(Application& initApplication);
        virtual ~WindowLinux();
        virtual bool init(int argc, const char** argv) override;

        void draw();
        void didResize(int width, int height);

        inline Display* getDisplay() const { return display; }
        inline ::Window getWindow() const { return window; }
        inline Atom getProtocolsAtom() const { return protocolsAtom; }
        inline Atom getDeleteAtom() const { return deleteAtom; }

    private:
        Visual* visual;
        int depth;
        Display* display;
        ::Window window;
        Atom protocolsAtom;
        Atom deleteAtom;
        GC gc;
    };
}