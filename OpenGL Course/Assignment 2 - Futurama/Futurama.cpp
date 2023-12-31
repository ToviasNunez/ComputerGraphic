/* ---------------------------------------------------------------
   name:           Futurama.cpp
   purpose:        robot control 
   version:	   STARTER CODE
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#include "../GLIncludes.hpp"

#include <iostream>
#include <iomanip> 
#include <sstream>

#include "Futurama.hpp"
#include "../Context.hpp"
#include "../Input.hpp"
#include "Robot.hpp"

#include "../LightSource.h"

#define GLM_FORCE_RADIANS
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

// field of view
GLfloat Futurama::fov= 55;
GLfloat Futurama::cameraZ= 10;

mat4 Futurama::projectionMatrix, Futurama::orthographicProjectionMatrix, Futurama::viewMatrix;

LightSource Futurama::lightSource={
    // position
  glm::vec4(0, 50, 300, 1),
  // ambient color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // diffuse color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // specular color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
};

SceneGraph *Futurama::sceneGraph= NULL;
bool Futurama::bender= false;

const OpenGLApplication::Config Futurama::config(glm::uvec2(2, 1),
						Config::Profile::COMPATIBILITY,
						(Window::DOUBLE | Window::DEPTH | Window::RGB | Window::MULTISAMPLE),
						8,
						glm::uvec2(50, 50),
						uvec2(512, 512),
						"Computer Graphics Assignment 2 - Futurama");

void Futurama::init(){
  
  // set background color to black
  //  glClearColor(0.718,0.808,0.857,1.0);
  glClearColor(0,0,0,1.0);
  glPolygonMode(GL_FRONT, GL_FILL);
  
  // enable depth test (z-buffer)
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE0);
  
  // enable antialiasing
  glEnable(GL_MULTISAMPLE);
}

// add a scenegraph
void Futurama::addSceneGraph(SceneGraph *sceneGraph){
  
  sceneGraph->addLightSource(lightSource);
  Futurama::sceneGraph= sceneGraph;
}

// adjust to new window size
void Futurama::reshape(){
  
  // viewport
  glViewport(0, 0, (GLsizei) Context::window->width(), (GLsizei) Context::window->height());
  
  computeViewMatrix();
  computeProjectionMatrix();
  orthographicProjectionMatrix= ortho(0.0f, (float) Context::window->width(), (float) Context::window->height(), 0.0f, -0.1f, 0.1f);
}

void Futurama::computeViewMatrix(void){

  // position the camera looking down the
  // negative z-axis at the center of the
  // world coordinate system. z-Position
  // of camera is stored in Futurama::cameraZ

  // TODO: compute viewMatrix
  
  sceneGraph->setViewMatrix(viewMatrix);
}

void Futurama::computeProjectionMatrix(void){

  // reshaped window aspect ratio
  float aspect= (float) Context::window->width() / (float) Context::window->height();
  
  // compute near and far plane
  float nearPlane= cameraZ/10.0f; 
  float farPlane= cameraZ*10.0f;
  
  // fH = tan( (fovY / 2) / 180 * pi ) * zNear;
  float fH = glm::tan( fov / 360 * glm::pi<float>()) * nearPlane;
  float fW = fH * aspect;

  // TODO: compute projection matrix via glm::perspective
  projectionMatrix= glm::frustum( -fW, fW, -fH, fH, nearPlane, farPlane );
  
  sceneGraph->setProjectionMatrix(projectionMatrix);
}

// this is where the drawing happens
void Futurama::display(void){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw the scenegraph
  sceneGraph->traverse(mat4(1));

  drawCameraParameters();
  
  // display back buffer
  Context::window->swapBuffers();
}

void Futurama::drawCameraParameters(void){
  
  glColor3f(1,1,1);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(&orthographicProjectionMatrix[0][0]);

  Input::setFont("helvetica", 12);
  ostringstream s;
  s << "fov (f/F): " << fov << endl;
  Input::drawString(15, Context::window->width()-35, s.str());
  s.str("");
  s << "camera z (c/C): " << fixed << setprecision(2) << cameraZ << endl;
  Input::drawString(15, Context::window->width()-15, s.str());

  glPopMatrix();
}

// keyboard control
void Futurama::keyPressed(){

  // rotation units
  float step= 2.0;
  
  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(Input::keyboard.key){
    
  case 'q':
  case 'Q': exit(0);
  case 'x':
    sceneGraph->rotate(step, 0, 0);
    Context::window->redisplay();
    break;
  case 'X': 
    sceneGraph->rotate(-step, 0, 0);
    Context::window->redisplay();
    break;
  case 'y':
    sceneGraph->rotate(0, step, 0);
    Context::window->redisplay();
    break;
  case 'Y' :
    sceneGraph->rotate(0, -step, 0);
    Context::window->redisplay();
    break;  
  case 'z':
    sceneGraph->rotate(0, 0, step);
    Context::window->redisplay();
    break;
  case 'Z':
    sceneGraph->rotate(0, 0, -step);
    Context::window->redisplay();
    break;
  case 'f':
    if(fov>0) fov--;
    computeProjectionMatrix();
    Context::window->redisplay();
    break;
  case 'F': 
    if(fov<180) fov++;
    computeProjectionMatrix();
    Context::window->redisplay();
    break;
  case 'c':
    if(cameraZ>0) cameraZ-=0.1;
    computeViewMatrix();
    computeProjectionMatrix();
    Context::window->redisplay();
    break;
  case 'C':
    cameraZ+=0.1;
    computeViewMatrix();
    computeProjectionMatrix();
    Context::window->redisplay();
    break;
  case 'r': 
    sceneGraph->reset();
    Context::window->redisplay();
    break;
  case Keyboard::Key::SPECIAL:
    handleSpecialKeys();
    break;  
  default:
    break;
  }
}

// keyboard callback for special keys
// (arrow keys for node selection)
void Futurama::handleSpecialKeys(){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(Input::keyboard.code) {

  case Keyboard::Code::UP:
    sceneGraph->up();
    Context::window->redisplay();
    break;
  case Keyboard::Code::DOWN:
    sceneGraph->down();
    Context::window->redisplay();
    break;
  case Keyboard::Code::LEFT:
    sceneGraph->left();
    Context::window->redisplay();
    break;
  case Keyboard::Code::RIGHT:
    sceneGraph->right();
    Context::window->redisplay();
    break;
  default:
    break;
  }
}

vector< pair < int, string > > Futurama::menuEntries{{Menu::QUIT, "quit"},
						     {Menu::RESET, "reset"},
						     {Menu::BENDER, "toggle [b]ender"}};
						      
// mouse menu control
void Futurama::menu(int id){
  
  switch (id) {
  case Menu::QUIT: 
    exit(0);
 
  case Menu::RESET:
    sceneGraph->reset();
    Context::window->redisplay();
    break;
    
  default:
    break;
  }
}

int main(int argc, char** argv){

  // initialize OpenGL context
  OpenGLContext<Futurama>::init(&argc, argv);

  // some diagnostic output
  std::cout << "GPU: " << glGetString(GL_RENDERER) << ", OpenGL version: " << glGetString(GL_VERSION) << ", GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
  //  build the robot hierarchy (see robot.cpp)
  Node *root= Robot::build();
  
  //make scenegraph
  SceneGraph *sceneGraph= new SceneGraph(root);

  Futurama::addSceneGraph(sceneGraph);
 
  //start event loop
  InputManager<Futurama>::startEventLoop();
  
  return 0;
}

