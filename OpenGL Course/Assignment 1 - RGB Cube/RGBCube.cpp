/* ----------------------------------------------------------------
   name:           RGBCube.hpp
   purpose:        computer graphics assignment 1 - rotations
   version:	   STARTER CODE
   TODO:           see README.md
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#include <string>
#include <iostream>
#include <sstream>

#include "../GLIncludes.hpp"

#include "RGBCube.hpp"

#include "../glm/gtx/rotate_vector.hpp"

using namespace std;
using namespace glsl;


float RGBCube::yAngle = glm::radians(0.0f);
float RGBCube::xAngle = glm::radians(0.0f);
float RGBCube::zAngle = glm::radians(0.0f);

const RGBCube::Config RGBCube::config(// OpenGL version
				      glm::uvec2(2, 1),
				      // Core or Compatibility context?
				      OpenGLApplication::Config::Profile::COMPATIBILITY,
				      // request double buffered (front/back) buffer
				      (Window::DOUBLE |
				       // request depth buffer
				       Window::DEPTH |
				       // request a color window
				       Window::RGB |
				       // request antialiazing
				       Window::MULTISAMPLE),
				      // requested number of samples for multisampling
				      8,
				      // window position
				      glm::uvec2(50, 50),
				      // initial window size
				      glm::uvec2(500, 500),
				      // title to be shown in window decoration
				      "Assignment 1 - RGB cube");




Shader RGBCube::rgbShader;

// transformation matrices
mat4 RGBCube::modelMatrix= mat4(1);
mat4 RGBCube::rotationMatrix= mat4(1);
mat4 RGBCube::viewMatrix= mat4(1);
mat4 RGBCube::projectionMatrix= mat4(1);
mat4 RGBCube::orthographicProjectionMatrix= mat4(1);

// camera setup
// distance of camera to object
GLfloat RGBCube::cameraZ=3;
// field of view
float RGBCube::fov= 45.0; 

// a triangle mesh defining a cube
TriangleMesh RGBCube::cube;

bool RGBCube::euler= true;

// init callback
// called once when initializing OpenGLContext 
void RGBCube::init(void){

  glClearColor(0.2, 0.2, 0.2, 1);
    
  // load rgb shader
  rgbShader.loadVertexShader("shaders/rgb-cube.vert");
  rgbShader.compileVertexShader();
  rgbShader.loadFragmentShader("shaders/rgb-cube.frag");
  rgbShader.compileFragmentShader();
  rgbShader.bindVertexAttrib("position", TriangleMesh::attribPosition);
  rgbShader.link();
  
  cube.load("meshes/cube.off");
}

// reshape callback
// called by InputManager whenever the window was resized by the user
void RGBCube::reshape(){
  
  // Set the viewport to be the entire window
  glViewport(0, 0, Context::window->width(), Context::window->height());
  
  // near and far plane
  float nearPlane= cameraZ/10.0;
  float farPlane= cameraZ*10.0;

  float aspect =  ((float) Context::window->width() / (float) Context::window->height());
  projectionMatrix= perspective(radians(fov), aspect, nearPlane, farPlane);
  orthographicProjectionMatrix= ortho(0.0f, (float) Context::window->width(), (float) Context::window->height(), 0.0f, -0.1f, 0.1f);
  
  //position the camera at (0,0,cameraZ) looking down the
  //negative z-axis at (0,0,0)
  viewMatrix= lookAt(vec3(0.0, 0.0, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

  Context::window->redisplay();
}

// display callback
// called by InputManager whenever a window refresh was requested
void RGBCube::display(void){

   //-------------------2. Tiefenpuffer

//glClearDepth(0);
glClearDepth(1);
glDepthFunc(GL_LESS);
glEnable(GL_DEPTH_TEST);
//glEnable(GL_GREATER);
  // initialize back buffer with background color (set in init)
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 //------------------------------3. Drehung  und  6.Euler Winkel---------------------------

    mat4 rotationMatrix_ = mat4(1.0f); 

     if (euler) {
     // apply the rotation to the model matrix
    rotationMatrix_ *= glm::rotate(mat4(0.5f),yAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix_ *= glm::rotate(mat4(0.5f),xAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix_ *= glm::rotate(mat4(0.5f),zAngle, glm::vec3(0.0f, 0.0f, 1.0f));
     }

     modelMatrix = rotationMatrix_;
  
   

  // reset model matrix each frame
  //mat4 modelMatrix= mat4(1);
  
  // draw cube
  rgbShader.bind();
  rgbShader.setUniform("modelViewProjectionMatrix", projectionMatrix*viewMatrix*modelMatrix);
  cube.draw();
  rgbShader.unbind();
  
  drawParameters();
  
  // exchange front and back buffer
  Context::window->swapBuffers();
}

// reset rotations
void RGBCube::reset(void){

  RGBCube::yAngle = glm::radians(1.0f);
  RGBCube::xAngle = glm::radians(1.0f);
  RGBCube::zAngle = glm::radians(1.0f);
  Context::window->redisplay(); // Ruft die display-Funktion auf, um die Änderungen anzuzeigen
  // TODO: reset euler angles and rotationMatrix
}

// keyboard callback
// called by InputManager whenever a key is pressed
void RGBCube::keyPressed(void){
  
switch (Input::keyboard.key){
    
        case 'x':
            // Rotation um die X-Achse im Uhrzeigersinn um 5 Grad
            xAngle += glm::radians(5.0f);
             Context::window->redisplay();
            break;
        case 'X':
            // Rotation um die X-Achse gegen den Uhrzeigersinn um 5 Grad
            xAngle -= glm::radians(5.0f);
             Context::window->redisplay();
            break;
        case 'y':    //---------------------------------4. Tastaturinput:---------------------
            yAngle += glm::radians(5.0f); // Inkrementieren um 5 Grad gegen den Uhrzeigersinn
            Context::window->redisplay();
            break;
        case 'Y':
            yAngle -= glm::radians(5.0f); // Dekrementieren um 5 Grad im Uhrzeigersinn
           Context::window->redisplay();
            break;
        case 'z':
            // Rotation um die Z-Achse im Uhrzeigersinn um 5 Grad
            zAngle += glm::radians(5.0f);
            Context:: window->redisplay();
            break;
        case 'Z':
            // Rotation um die Z-Achse gegen den Uhrzeigersinn um 5 Grad
            zAngle -= glm::radians(5.0f);
             Context::window->redisplay();
            break;
      case 'r':
      case 'R':
            reset(); // Ruft die reset-Funktion auf, wenn die 'r'-Taste gedrückt wird
            Context::window->redisplay();
            break; 

  case 'q':
  case 'Q':
    exit(EXIT_SUCCESS);
    break;

case 'e':
    euler= !euler;
    Context::window->redisplay();
    break;
       
   // TODO: increment euler angles / update rotationMatrix
  default:
    return; 
  }
}
//-----------------------------5. Menüs (1 Punkt)
// association of human-readable text to menu entries
vector< pair< int, string > > RGBCube::menuEntries= {{Menu::QUIT, "[q]uit"},
						     {Menu::EULER, "[e]uler"},
                   {Menu::RESET, "[r]eset"},
						     // TODO: add menu entry for reset
};

// menu callback
// called when right mouse button is pressed
void RGBCube::menu(int value){
  
  switch(value){
  case Menu::QUIT:
    exit(0);
  case Menu::EULER:
    euler= !euler;
     Context::window->redisplay();
     break;
     case Menu::RESET:
     reset();
      Context::window->redisplay();
     break;
     // TODO: reset rotations
  default: return; 
  }
}

void RGBCube::drawParameters(void){
  
  glColor3f(1,1,1);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(&orthographicProjectionMatrix[0][0]);

  Output::setFont("helvetica", 12);
  ostringstream s;
  s << "euler: " << (euler? "true" : "false") << endl;
  Output::drawString(15, Context::window->height()-15, s.str());
  s.str("");
  s << "rotate with x/X, y/Y, z/Z keys" << endl;
  Output::drawString(Context::window->width()-180, Context::window->height()-15, s.str());
  glPopMatrix();
}

int main(int argc, char** argv){

  // initialization of OpenGL Context
  OpenGLContext<RGBCube>::init(&argc, argv);

  // some diagnostic output
  std::cout << "GPU: " << glGetString(GL_RENDERER) << ", OpenGL version: " << glGetString(GL_VERSION) << ", GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
   
  // start event loop
  InputManager<RGBCube>::startEventLoop();
  
  // in c++, main must return an integer value
  // 0 means everything went fine
  return 0;
}
