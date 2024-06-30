#include "Life.h"
#include "World.h"

#include "time.h"
#include <iostream>

// #include <stdlib.h>
// #include <Windows.h>

#include "GLScene.h"

using std::cout;
using std::endl;
using std::malloc;

int main(int argc, char *argv[]) {
  GLScene(argc, argv);
  glutMainLoop();
}
