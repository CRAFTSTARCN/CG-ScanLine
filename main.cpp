#include "Window.h"

int main(int, char**) {
    initGLFW(3,3);

    Window win(900,900,"Scan-Line, press S to go");
    win.initWindow();
    win.setUpRendProp();
    win.mainLoop();

    return 0;
}
