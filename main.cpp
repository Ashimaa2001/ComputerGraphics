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


float ball[3]={0.1f,-0.2f,-1.0f};

float face[3]={0.1f, -0.11f, -1.0f};

float eye1[3]={0.12f, 0.032f,-1.0f};

float eye2[3]={0.05f, -0.09f,-1.0f};

static int flag=1;

void drawBall(float X,float Y,float Z, float rad) {

    glTranslatef(X,Y,Z); //moving it toward the screen a bit on creation
    glutSolidSphere (rad, 30,50);//eate ball.
    glEnd();

}

void keyPress(int key, int x, int y)
{
    if(key==GLUT_KEY_RIGHT)
    {ball[0] += 0.05f;
        face[0]+=0.05f;
        eye1[0]+=0.05f;
        eye2[0]+=0.05f;}
    if(key==GLUT_KEY_LEFT){
        ball[0]  -= 0.05f;
        face[0]-=0.05f;
        eye1[0]-=0.05f;
        eye2[0]-=0.05f;}
    if(key==GLUT_KEY_UP){
        ball[1] += 0.05f;
        face[1]+=0.05f;
        eye1[1]+=0.05f;
        eye2[1]+=0.05f;}
    if(key==GLUT_KEY_DOWN){
        ball[1] -= 0.05f;
        face[1]-=0.05f;
        eye1[1]-=0.05f;
        eye2[1]-=0.05f;}

    glutPostRedisplay();
}

void initRendering()
{
        glClearColor(1.0, 1.0, 0.0, 1.0);
        glColor3f(0.0, 1.0, 0.0);

        glPointSize(10);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
        }


void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
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
    drawBall(face[0], face[1], face[2], 0.03f);

    glColor3f(1.0, 0.0, 0.0);
    drawBall(ball[0], ball[1], ball[2], 0.1f);

    glColor3f(0.0, 0.0, 0.0);
    drawBall(eye1[0], eye1[1], eye1[2], 0.01f);
    drawBall(eye2[0], eye2[1], eye2[2], 0.015f);

    glutSwapBuffers();
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("Road Crossing Game");
    initRendering();
    glutDisplayFunc(drawScene);
    glutSpecialFunc(keyPress);
    glutReshapeFunc(handleResize);
    glFlush();
    glutMainLoop();

    return(0);
}
