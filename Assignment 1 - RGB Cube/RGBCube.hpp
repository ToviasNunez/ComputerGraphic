/* ----------------------------------------------------------------
   name:           RGBCube.hpp
   purpose:        computer graphics assignment 1 - rotations
   version:	   STARTER CODE
   TODO:           add menu item for RESET
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#pragma once

#include <string>
#include <vector>
#include <utility>

#include "glm/glm.hpp"

#include "Context.hpp"
#include "Input.hpp"
#include "TriangleMesh.hpp"
#include "GLSL.hpp"
#include "Application.hpp"


class RGBCube : public OpenGLApplication{
  
  typedef OpenGLContext<RGBCube> Context;
  typedef InputManager<RGBCube> Input;
  typedef InputManager<RGBCube> Output;

public:
  
  static const OpenGLApplication::Config config;

  //initialization
  static void init(void);
  // display
  static void display(void);
  // adjust to new window size
  static void reshape();
  // keyboard callback
  static void keyPressed(void);
  // menu handling
  static void menu(int value);
  
  // menu entries                                 
  static std::vector< std::pair< int, std::string > > menuEntries;
  
private:
  
  // reset transformations
  static void reset(void);

  static void drawParameters(void);

  static float fov, cameraZ;
  
  static glm::mat4 orthographicProjectionMatrix, projectionMatrix, viewMatrix, modelMatrix, rotationMatrix;

  static glsl::Shader rgbShader;

  static TriangleMesh cube;
  
  static const std::string models[];

  static bool euler;
  static float yAngle;
  static float xAngle;
  static float zAngle;
  struct Menu{
    enum Item{
      QUIT,
      EULER,
      RESET,// TODO: Add menu item for RESET
    };
  };
};
