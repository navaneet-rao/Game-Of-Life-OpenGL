#include "GLScene.h"

int size = 500;

Life3d *life3d = new Life3d(size / 5, size / 5, size / 5);
Life *life = new Life(size, size);

int window_width;
int window_height;
int g_GLUTWindowHandle;
int g_ErrorCode;
float y_offset;
float x_offset;
float scal = 0.0f;

bool show_control_panel = false;
bool show_about_menu = false;
bool show_help_menu = false;

bool rot_all = true;
float rot_x = 0.1f;
float rot_y = 0.7f;
float rot_z = 0.3f;
float rot_angle = 0.1f;
bool b_rot = true;
bool sim = true;
bool shade = false;
int time_e = clock();

int last_mouse_x, last_mouse_y;
bool is_dragging = false;

float bg_color[4] = {0.156f, 0.172f, 0.203f, 1.00f}; // Default background color

float ob_color[4] = {(169.0f / 255.0f), (234.0f / 255.0f), (123.0f / 255.0f),
                     1.00f};

float ShadingColor[4] = {1.0, 2.0, 3.0, 4.0};

float simulation_speed = 30.0f; // Default simulation speed
int last_update_time = 0;

Scene g_current = scene1;

void GLScene(int argc, char *argv[]) { GLScene(900, 900, argc, argv); }

// Define arrays for key bindings and their descriptions
const char *keyBindings[] = {"SPACE", "R", "TAB", "W", "S", "A", "D", "C",
                             ",",     ".", "1-9", "!", "@", "#", "$"};

const char *descriptions[] = {"Pause/Play Simulation",
                              "Toggle 3D Rotation",
                              "Switch between 2D and 3D scenes",
                              "Move view up",
                              "Move view down",
                              "Move view left",
                              "Move view right",
                              "Toggle shading",
                              "Zoom out",
                              "Zoom in",
                              "Set grid size (100-900)",
                              "Set grid size to 1000",
                              "Set grid size to 2000",
                              "Set grid size to 3000",
                              "Set grid size to 4000"};

void GLScene(int x, int y, int argc, char *argv[]) {
  for (int i = 0; i < 100000; i++) {
    int x = rand() % size + 1;
    int y = rand() % size + 1;
    life->setLife(x, y, 1);
  }
  newlife3d();
  cout << glutGet(GLUT_ELAPSED_TIME) << endl;
  window_height = y;
  window_width = x;

  glutInit(&argc, argv);

  glutInitWindowPosition(30, 30);
  glutInitWindowSize(window_width, window_height);
  window_width = glutGet(GLUT_SCREEN_WIDTH);
  window_height = glutGet(GLUT_SCREEN_HEIGHT);

  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

  g_GLUTWindowHandle = glutCreateWindow("Conway's Game of Life");
  glutInitWindowSize(window_width, window_height);

  glutDisplayFunc(DisplayGL);
  glutKeyboardFunc(KeyboardGL);
  glutMouseFunc(MouseGL);        // Register the mouse function
  glutMotionFunc(MouseMotionGL); // Register the mouse motion function
  glutReshapeFunc(ReshapeGL);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();
  ImGui_ImplOpenGL3_Init();
  ImGui_ImplGLUT_Init();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  // glClearColor(0.156f, 0.172f, 0.203f, 1.00f); // BackGround Colour
  // glClearColor(1.00f, 1.00f, 1.00f, 1.00f);
  glClearColor(bg_color[0], bg_color[1], bg_color[2],
               bg_color[3]); // BackGround Colour
  glClearDepth(1.0f);
  glShadeModel(GL_SMOOTH);
}

void Cleanup() {

  if (g_GLUTWindowHandle != 0) {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    glutDestroyWindow(g_GLUTWindowHandle);
    g_GLUTWindowHandle = 0;
  }
  if (false) {
    exit(g_ErrorCode);
  }
}

void newLife() {
  // ew--calling a destructor
  life->~Life();
  life = new Life(size, size);
  for (int i = 0; i < .6 * size * size; i++) {
    int x = rand() % size + 1;
    int y = rand() % size + 1;
    life->setLife(x, y, 1);
  }
}

void newlife3d() {
  // ew--calling a destructor
  life3d->~Life3d();
  life3d = new Life3d(size / 5, size / 5, size / 5);
  for (int i = 0; i < .05 * (size / 5) * (size / 5) * (size / 5); i++) {
    int x = rand() % size / 5 + 1;
    int y = rand() % size / 5 + 1;
    int z = rand() % size / 5 + 1;
    life3d->setLife(x, y, z, 1);
  }
}

void DisplayGL() {
  glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)window_width, (float)window_height);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGLUT_NewFrame();
  ImGui::NewFrame();

  // Render your scene
  if (g_current == scene1) {
    render();
  } else if (g_current == scene2) {
    render3d();
  }

  MainMenuBar();
  DisplayImGui();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glutSwapBuffers();
  glutPostRedisplay();
}

void MainMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    // Control Tab
    if (ImGui::BeginMenu("Settings")) {
      if (ImGui::MenuItem("Open Control Panel")) {
        show_control_panel = true;
      }
      ImGui::EndMenu();
    }

    // About Tab
    if (ImGui::BeginMenu("About")) {
      ImGui::MenuItem("Application Name: OpenGL ImGui Application");
      ImGui::MenuItem("Version: 1.0.0");
      ImGui::MenuItem("Author: Your Name");
      ImGui::EndMenu();
    }

    // Help Tab
    if (ImGui::BeginMenu("Help")) {
      if (ImGui::MenuItem("Show Key Bindings")) {
        show_help_menu = true;
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) {
        exit(0); // Exit the application
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void DisplayImGui() {
  if (show_control_panel) {

    static int currentTab = 0; // Track the current tab index

    ImGui::Begin("Control Panel", &show_control_panel);

    // Dropdown for selecting different sections
    const char *tabNames[] = {
        "Simulation Controls", "View Controls",    "3D Rotation",   "Shaders",
        "Grid Size",           "Simulation Speed", "Color Settings"};
    ImGui::Combo("##Tabs", &currentTab, tabNames, IM_ARRAYSIZE(tabNames));

    ImGui::Separator();

    // Display content based on the selected tab
    switch (currentTab) {
    case 0: // Simulation Controls
      ImGui::Text("Simulation Controls");
      ImGui::Separator();
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      if (ImGui::Button(sim ? "Pause" : "Play")) {
        sim = !sim;
      }
      ImGui::SameLine();
      if (ImGui::Button("Reset 2D Life")) {
        newLife();
      }
      ImGui::SameLine();
      if (ImGui::Button("Reset 3D Life")) {
        newlife3d();
      }
      ImGui::Checkbox("Rotate 3D", &b_rot);
      break;

    case 1: // View Controls
      ImGui::Text("View Controls");
      ImGui::Separator();
      if (ImGui::Button("Switch Scene")) {
        if (g_current == scene1) {
          glEnable(GL_DEPTH_TEST);
          glEnable(GL_LIGHTING);
          glEnable(GL_LIGHT0);
          g_current = scene2;
        } else if (g_current == scene2) {
          glDisable(GL_DEPTH_TEST);
          glDisable(GL_LIGHTING);
          glDisable(GL_LIGHT0);
          g_current = scene1;
        }
      }
      ImGui::SliderFloat("Zoom", &scal, -10.0f, 10.0f);
      ImGui::SliderFloat("X Offset", &x_offset, -5.0f, 5.0f);
      ImGui::SliderFloat("Y Offset", &y_offset, -5.0f, 5.0f);
      break;

    case 2: // 3D Rotation
      ImGui::Text("3D Rotation");
      ImGui::Separator();
      ImGui::SliderFloat("Rotation X", &rot_x, 0.0f, 1.0f);
      ImGui::SliderFloat("Rotation Y", &rot_y, 0.0f, 1.0f);
      ImGui::SliderFloat("Rotation Z", &rot_z, 0.0f, 1.0f);
      ImGui::SliderFloat("Rotation Angle", &rot_angle, 0.0f, 360.0f);
      ImGui::Checkbox("All axis rotate", &rot_all);
      break;

    case 3: // Other Settings
      ImGui::Text("Shaders");
      ImGui::Separator();
      ImGui::Checkbox("Shading", &shade);
      if (shade) {
        ImGui::ColorEdit4("Shading Color", ShadingColor);
      }
      break;

    case 4: // Grid Size
      ImGui::Text("Grid Size");
      ImGui::Separator();
      ImGui::Columns(4);
      if (ImGui::Button("200")) {
        size = 200;
        newLife();
        newlife3d();
      }
      ImGui::NextColumn();
      if (ImGui::Button("300")) {
        size = 300;
        newLife();
        newlife3d();
      }
      ImGui::NextColumn();
      if (ImGui::Button("400")) {
        size = 400;
        newLife();
        newlife3d();
      }
      ImGui::NextColumn();
      if (ImGui::Button("500")) {
        size = 500;
        newLife();
        newlife3d();
      }
      ImGui::NextColumn();
      if (ImGui::Button("600")) {
        size = 600;
        newLife();
        newlife3d();
      }
      ImGui::NextColumn();
      if (ImGui::Button("700")) {
        size = 700;
        newLife();
        newlife3d();
      }
      ImGui::NextColumn();
      if (ImGui::Button("800")) {
        size = 800;
        newLife();
        newlife3d();
      }
      ImGui::Columns(1);
      break;

    case 5: // Simulation Speed
      ImGui::Text("Simulation Speed");
      ImGui::Separator();
      ImGui::SliderFloat("Speed", &simulation_speed, 1.0f, 100.0f);
      break;

    case 6: // Color Settings
      ImGui::Text("Color Settings");
      ImGui::Separator();
      ImGui::Text("Background Color");
      ImGui::ColorEdit4("##BackgroundColor", bg_color);
      ImGui::Separator();
      ImGui::Text("Object Color");
      ImGui::ColorEdit4("##ObjectColor", ob_color);
      break;

    default:
      break;
    }

    ImGui::Separator();

    if (ImGui::Button("Reset All")) {
      size = 500;
      scal = 0.0f;
      x_offset = 0.0f;
      y_offset = 0.0f;
      rot_x = 0.1f;
      rot_y = 0.7f;
      rot_z = 0.3f;
      rot_angle = 0.1f;
      b_rot = true;
      sim = true;
      shade = false;
      newLife();
      newlife3d();
      glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
      glColor3f(ob_color[0], ob_color[1], ob_color[2]);
    }

    ImGui::End();
  }

  // Display the help menu with key bindings in a table format
  if (show_help_menu) {
    ImGui::Begin("key Bindings", &show_help_menu);
    // Iterate over key bindings and descriptions
    for (int i = 0; i < 15; i++) {
      ImGui::Separator();
      ImGui::Text("%s ---> %s", keyBindings[i], descriptions[i]);
      ImGui::Separator();
    }

    ImGui::End();
  }
}

/*void DisplayImGui() {*/
/*  ImGui::Begin("My Window");*/
/*  ImGui::Text("Hello World");*/
/*  ImGui::End();*/
/*}*/

void MouseGL(int button, int state, int x, int y) {

  ImGui_ImplGLUT_MouseFunc(button, state, x, y);

  if (button == GLUT_RIGHT_BUTTON) {
    if (state == GLUT_DOWN) {
      last_mouse_x = x;
      last_mouse_y = y;
      is_dragging = true;
    } else if (state == GLUT_UP) {
      is_dragging = false;
    }
  } else if (button == 3) { // Scroll up
    scal += 0.1f;
    glutPostRedisplay();
  } else if (button == 4) { // Scroll down
    scal -= 0.1f;
    glutPostRedisplay();
  }
}

void MouseMotionGL(int x, int y) {

  ImGui_ImplGLUT_MotionFunc(x, y);

  if (is_dragging) {
    int dx = x - last_mouse_x;
    int dy = y - last_mouse_y;
    x_offset += dx * 0.01f;
    y_offset -= dy * 0.01f;
    last_mouse_x = x;
    last_mouse_y = y;
    glutPostRedisplay();
  }
}

void KeyboardGL(unsigned char c, int x, int y) {

  ImGui_ImplGLUT_KeyboardFunc(c, x, y);

  if (c == ' ') {
    sim = !sim;
  }
  if (c == 'r') {
    b_rot = !(b_rot);
  }
  if (c == '	') {
    if (g_current == scene1) {
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      (g_current = scene2);
    } else if (g_current == scene2) {
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
      g_current = scene1;
    }
  }
  if (c == 'w') {
    y_offset = y_offset - 0.1;
  }

  if (c == 's') {
    y_offset += 0.1;
  }

  if (c == 'd') {
    x_offset -= 0.1;
  }

  if (c == 'a') {
    x_offset += 0.1;
  }
  if (c == ',') {
    scal -= 0.1f;
  }
  if (c == 'c') {
    shade = !shade;
  }

  if (c == '.') {
    scal += 0.1f;
  }
  if (c == '1') {
    size = 200;
    newLife();
    newlife3d();
  }

  if (c == '2') {
    size = 200;
    newLife();
    newlife3d();
  }
  if (c == '3') {
    size = 300;
    newLife();
    newlife3d();
  }
  if (c == '4') {
    size = 400;
    newLife();
    newlife3d();
  }
  if (c == '5') {
    size = 500;
    newLife();
    newlife3d();
  }
  if (c == '6') {
    size = 600;
    newLife();
    newlife3d();
  }
  if (c == '7') {
    size = 700;
    newLife();
    newlife3d();
  }
  if (c == '8') {
    size = 800;
    newLife();
    newlife3d();
  }
  if (c == '9') {
    size = 900;
    newLife();
  }
  if (c == '!') {
    size = 1000;
    newLife();
  }
  if (c == '@') {
    size = 2000;
    newLife();
  }
  if (c == '#') {
    size = 3000;
    newLife();
  }
  if (c == '$') {
    size = 4000;
    newLife();
  }
  if (c == '%') {
    size = 5000;
    newLife();
  }
}

void ReshapeGL(int w, int h) {
  // std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

  if (h == 0) // Prevent A Divide By Zero error
  {
    h = 1; // Making Height Equal One
  }

  window_width = w;
  window_height = h;

  glViewport(0, 0, window_width, window_height);

  // Setup the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)window_width / (GLdouble)window_height, 0.1,
                 100.0);

  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)window_width, (float)window_height);

  // render();
  glutPostRedisplay();
}

void render() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float y_t = 0.0f;
  float x_t = 0.0f;
  float off = 500 / (float)size * 0.01f;

  glTranslatef(-5.0f + x_offset, -5.0f + y_offset, -9.0f + scal);

  if (shade == false) {
    glColor3f(ob_color[0], ob_color[1], ob_color[2]);
  }

  glBegin(GL_QUADS);

  for (int i = 0; i < size; i++) {
    x_t = 0.0f;
    for (int j = 0; j < size; j++) {
      if (life->getLifeform(j + 1, i + 1) == 1) {

        if (shade == true) {
          glColor3f(ShadingColor[0] * ((float)i / (float)size),
                    ShadingColor[1] * ((float)j / (float)size),
                    ShadingColor[2]);
        }

        glVertex2f(x_t - off, y_t + off);
        glVertex2f(x_t + off, y_t + off);
        glVertex2d(x_t + off, y_t - off);
        glVertex2d(x_t - off, y_t - off);
      }

      x_t += (500 / (float)size) * 0.02f;
    }
    y_t += (500 / (float)size) * 0.02f;
  }

  glEnd();

  glPopMatrix();

  // Calculate elapsed time since last update
  int current_time = glutGet(GLUT_ELAPSED_TIME);
  int elapsed_time = current_time - last_update_time;

  // Check if enough time has passed to update the simulation
  if (sim &&
      elapsed_time >= 1000 / simulation_speed) { // Adjust 1000 to match your
                                                 // desired update rate
    last_update_time = current_time;
    life->update(); // Update the simulation
  }
}

void render3d() {
  if (size < 801) {
    float y_t = 0.0f;
    float x_t = 0.0f;
    float z_t = 0.0f;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (shade == false) {
      GLfloat ColourImGui[] = {ob_color[0], ob_color[1], ob_color[2], 1.f};
      glMaterialfv(GL_FRONT, GL_DIFFUSE, ColourImGui);
    }
    /* clear color and depth buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTranslatef(0.0f + x_offset, 0.0f + y_offset, -7.0f + scal);

    if (rot_all) {
      glRotatef(rot_angle / 3, rot_x, rot_y, rot_z);
      glRotatef(rot_angle / 3, rot_z, rot_y, rot_x);
      glRotatef(rot_angle / 3, rot_x, rot_z, rot_y);
    } else {
      glRotatef(rot_angle / 3, rot_x, rot_y, rot_z);
    }

    glTranslatef(-2.0f, -2.0f, -2.0f);

    glBegin(GL_QUADS);
    float sz = 2.0f * 5.0f / size;
    for (int i = 0; i < size / 5; i++) {
      y_t = 0.0f;
      for (int j = 0; j < size / 5; j++) {
        x_t = 0.0f;
        for (int k = 0; k < size / 5; k++) {
          if (life3d->getLifeform(k + 1, j + 1, i + 1) == 1) {

            if (shade == true) {
              GLfloat green[] = {5.0f * ((float)i / (float)size),
                                 5.0f * ((float)j / (float)size),
                                 5.0f * ((float)k / (float)size)};
              glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
            }

            glNormal3f(0.0F, 0.0F, 1.0F);
            glVertex3f(sz + x_t, sz + y_t, sz + z_t);
            glVertex3f(-sz + x_t, sz + y_t, sz + z_t);
            glVertex3f(-sz + x_t, -sz + y_t, sz + z_t);
            glVertex3f(sz + x_t, -sz + y_t, sz + z_t);

            glNormal3f(0.0F, 0.0F, -1.0F);
            glVertex3f(-sz + x_t, -sz + y_t, -sz + z_t);
            glVertex3f(-sz + x_t, sz + y_t, -sz + z_t);
            glVertex3f(sz + x_t, sz + y_t, -sz + z_t);
            glVertex3f(sz + x_t, -sz + y_t, -sz + z_t);

            glNormal3f(0.0F, 1.0F, 0.0F);
            glVertex3f(sz + x_t, sz + y_t, sz + z_t);
            glVertex3f(sz + x_t, sz + y_t, -sz + z_t);
            glVertex3f(-sz + x_t, sz + y_t, -sz + z_t);
            glVertex3f(-sz + x_t, sz + y_t, sz + z_t);

            glNormal3f(0.0F, -1.0F, 0.0F);
            glVertex3f(-sz + x_t, -sz + y_t, -sz + z_t);
            glVertex3f(sz + x_t, -sz + y_t, -sz + z_t);
            glVertex3f(sz + x_t, -sz + y_t, sz + z_t);
            glVertex3f(-sz + x_t, -sz + y_t, sz + z_t);

            glNormal3f(1.0F, 0.0F, 0.0F);
            glVertex3f(sz + x_t, sz + y_t, sz + z_t);
            glVertex3f(sz + x_t, -sz + y_t, sz + z_t);
            glVertex3f(sz + x_t, -sz + y_t, -sz + z_t);
            glVertex3f(sz + x_t, sz + y_t, -sz + z_t);

            glNormal3f(-1.0F, 0.0F, 0.0F);
            glVertex3f(-sz + x_t, -sz + y_t, -sz + z_t);
            glVertex3f(-sz + x_t, -sz + y_t, sz + z_t);
            glVertex3f(-sz + x_t, sz + y_t, sz + z_t);
            glVertex3f(-sz + x_t, sz + y_t, -sz + z_t);
          }
          x_t += sz * 2.0f;
        }
        y_t += sz * 2.0f;
      }
      z_t += sz * 2.0f;
    }

    glEnd();

    // Calculate elapsed time since last update
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    int elapsed_time = current_time - last_update_time;

    // Check if enough time has passed to update the simulation
    if (sim &&
        elapsed_time >= 1000 / simulation_speed) { // Adjust 1000 to match your
                                                   // desired update rate
      last_update_time = current_time;
      life3d->update(); // Update the simulation
    }

    if (b_rot) {
      rot_angle++;
      rot_x = ((int)((rot_x + 1.0f) * 10.0f) % 10) / 10.0f;
      rot_y = ((int)((rot_y + 1.0f) * 10.0f) % 10) / 10.0f;
      rot_z = ((int)((rot_z + 1.0f) * 10.0f) % 10) / 10.0f;
    }
    glPopMatrix();
  }
}
