#include "Life.h"
#include "Life3d.h"

#include "imgui.h"
#include "imgui_impl_glut.h"

#include <ctime>

#include <GL/glut.h>

using namespace life3;
using namespace ImGui;

#ifndef GLSCENE_H
#define GLSCENE_H

enum Scene { scene1, scene2 };

void GLScene(int argc, char *argv[]);
void GLScene(int, int, int argc, char *argv[]);
void Cleanup();

void newLife();
void newlife3d();

void DisplayGL();
void KeyboardGL(unsigned char c, int x, int y);
void ReshapeGL(int w, int h);
void MouseGL(int button, int state, int x, int y);
void MouseMotionGL(int x, int y);

void render();
void render3d();
#endif
