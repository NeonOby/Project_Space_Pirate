#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "TinyXML/tinyxml.h"
USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    EGLView* eglView = EGLView::getInstance();
    eglView->setViewName("SpacePirate");
    eglView->setFrameSize(1240, 720);


    return Application::getInstance()->run();
}
