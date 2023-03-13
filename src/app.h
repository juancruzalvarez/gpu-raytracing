#ifndef APP_H
#define APP_H

#include "window.h"

class App {

public:

App();
~App();

void Run();

private:

Window *window_;


};


#endif