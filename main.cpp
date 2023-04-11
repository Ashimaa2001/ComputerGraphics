#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#ifdef APPLE
#include<openGL/openGL.h>
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

using namespace std;

float ballX = 0.1f;
float ballY = -0.2f;
float ballZ = -1.0f;

float faceX= 0.1f;
float faceY= -0.11f;
float faceZ= -1.0f;

float eye1_X = 0.3f;
float eye1_Y = 0.0f;
float eye1_Z = -1.0f;

static int flag=1;

void drawBall(float X,float Y,float Z, float rad) {


    glTranslatef(X,Y,Z); //moving it toward the screen a bit on creation
    glutSolidSphere (rad, 30,50);//eate ball.

       glEnd();


}

void keyPress(int key, int x, int y)
{
    if(key==GLUT_KEY_RIGHT)
    {ballX += 0.05f;
        faceX+=0.05f;}
    if(key==GLUT_KEY_LEFT){
        ballX  -= 0.05f;
        faceX-=0.05f;}
    if(key==GLUT_KEY_UP){
        ballY += 0.05f;
        faceY+=0.05f;}
    if(key==GLUT_KEY_DOWN){
        ballY -= 0.05f;
        faceY-=0.05f;}


            glutPostRedisplay();
        }
        void initRendering()
        {
            //glEnable(GL_DEPTH_TEST);

            glClearColor(1.0, 1.0, 0.0, 1.0);

            // making picture color green (in RGB mode), as middle argument is 1.0
            glColor3f(0.0, 1.0, 0.0);

            // breadth of picture boundary is 1 pixel
            glPointSize(10);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            // setting window dimension in X- and Y- direction
            //gluOrtho2D(-780, 780, -420, 420);
            gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
        }


        //Called when the window is resized
        void handleResize(int w, int h) {
            //Tell OpenGL how to convert from coordinates to pixel values
            glViewport(0, 0, w, h);

            glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

            //Set the camera perspective
            glLoadIdentity(); //Reset the camera
            gluPerspective(45.0,                  //The camera angle
                           (double)w / (double)h, //The width-to-height ratio
                           1.0,                   //The near z clipping coordinate
                           200.0);                //The far z clipping coordinate
        }

        void drawScene()
        {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_MODELVIEW);

            glLoadIdentity();
            glColor3f(0.4, 0.4, 0.4);
            drawBall(faceX, faceY, faceZ, 0.03);
            glColor3f(1.0, 0.0, 0.0);
            drawBall(ballX, ballY, ballZ, 0.1);

            glutSwapBuffers();
        }

        void update(int value) {
            if(flag)
            {
            ballX += 0.001f;
               if(ballX>0.3)
               {
                   flag=0;

               }
            }
            if (!flag)
            {
                ballX -= 0.001f;
                if(ballX<-0.3)
               {
                   flag=1;

               }
            }

            glutPostRedisplay(); 
        }

        int main(int argc,char** argv)
        {
            glutInit(&argc,argv);

            glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);

            glutInitWindowSize(400,400);

            glutCreateWindow("Moving Circle");

            initRendering();

            glutDisplayFunc(drawScene);

            glutSpecialFunc(keyPress);
            glutReshapeFunc(handleResize);
            //autorot();

            glutTimerFunc(25, update, 0);
            glFlush();
            glutMainLoop();

            return(0);
        }
