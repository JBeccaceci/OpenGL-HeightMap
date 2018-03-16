//
//  main.c
//  APPTest
//
//  Created by Juan Beccaceci on 30/5/17.
//  Copyright © 2017 Juan Beccaceci. All rights reserved.
//
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <GLUT/glut.h>
#include "Terrain.h"

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

//////////////////////////////////////////////
/// Camera test
//////////////////////////////////////////////
void SetCamera();

//////////////////////////////////////////////
/// Reshape callBack
//////////////////////////////////////////////
void ReshapeFunction(int w, int h);

//////////////////////////////////////////////
/// Initialize openGL
//////////////////////////////////////////////
void InitOpenGL();

//////////////////////////////////////////////
/// Render callBack
//////////////////////////////////////////////
void OpenGLDisplay();

int Width = 0;
int Height = 0;

//////////////////////////////////////////////
/// Camera test
//////////////////////////////////////////////
void SetCamera()
{
    float camyaw   = -90;
    float campitch = 0;
    float camroll  = 90;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //  Camera data
    GLdouble aspect    = (double)Width / Height,
    fov       = degreesToRadians(90),
    fovy      = 2 * atan2(tan(fov / 2), aspect),
    nearplane = 1e-2f,
    farplane  = 1000,
    ydist     = nearplane * tan(fovy / 2),
    xdist     = ydist * aspect;
    
    //  Set frustum
    glFrustum(-xdist, xdist, -ydist, ydist, nearplane, farplane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glRotatef(camroll, 0, 0, 1);
    glRotatef(campitch, -1, 0, 0);
    glRotatef(camyaw, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(1, -1, 1);
    
#if HEIGHT_MAP
    glTranslatef(-200, -100, -30);
#else
    glTranslatef(-160, 15, -50);
#endif
    
    //  Camera Pos: X (Adelante - Atras)
    //  Camera Pos: Y (Altura)
    //  Camera Pos: Z (Derecha - Izquierda)
}

//////////////////////////////////////////////
/// Reshape callBack
//////////////////////////////////////////////
void ReshapeFunction(int w, int h)
{
    Width  = w;
    Height = h;
    glViewport(0, 0, w, h);
}

//////////////////////////////////////////////
/// Initialize openGL
//////////////////////////////////////////////
void InitOpenGL()
{
    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
    glDisable(GL_FOG);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

//////////////////////////////////////////////
/// Render callBack
//////////////////////////////////////////////
void OpenGLDisplay()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //  Set camera
    SetCamera();
    
    //  Render terrain
    Terrain_Render();
    
    glutSwapBuffers();
}

int main(int argc, char * argv[])
{
    //  Create Window
    glutInitWindowSize(1024, 768);
    glutInit(&argc, argv);
    glutCreateWindow("OpenGL");
    glutFullScreen();
    
    //  Load terrain
    if (!Initialize_Terrain())
    {
        printf("Error al cargar el terreno");
        return 0;
    }
    
    //  Init OpenGL
    InitOpenGL();
    
    //  Reshape callBack
    glutReshapeFunc(ReshapeFunction);
    
    //  Display callBack
    glutDisplayFunc(OpenGLDisplay);
    
    //  Main loop
    glutMainLoop();
    
    return 0;
    
}
