#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#define XMAX 1200
#define YMAX 700
#define SPEED 0.05
#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

#include<GL/glut.h>
#include <time.h>
using namespace std;

enum view {MENU, GAME, GAME_START, GAME_OVER};
view viewPage = MENU;
float xOne = 720, yOne = 0;
float xTwo = 720, yTwo = 300;
bool mButtonPressed = false;
bool laser1 = false;
bool keyStates[256] = {false};
float mouseX, mouseY;
bool laser1Dir[2] = {false};
GLint m_viewport[4];
float balloonX = -800.0f;
float balloonY = 0.0f;
float balloon2X= 0.0f;
float balloon2Y= -600.0f;
float balloonSpeedX= 0.5f;
float balloonSpeedY= 0.5f;
float balloonSpeed2X= 0.5f;
float balloonSpeed2Y= 0.5f;
bool win= false;

float balloonRadius = 40.0f;
float balloon2Radius = 40.0f;
int alienLife1 = 100;

void delay(float secs)
{
	float end = clock()/CLOCKS_PER_SEC + secs;
	while((clock()/CLOCKS_PER_SEC) < end);
}

void init()
{
	glClearColor(0.52,0.37,0.76,0);
	glColor3f(1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluOrtho2D(-1000,1000,-700,700);
	glMatrixMode(GL_MODELVIEW);
}

void displayRasterText(float x ,float y ,float z ,char *stringToDisplay) {
	glRasterPos3f(x, y, z);
	for(char* c = stringToDisplay; *c != '\0'; c++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , *c);
	}
}


void startScreenDisplay()
{
	glLineWidth(5);

	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-800 ,-500);
		glVertex2f(-800 ,500);
		glVertex2f(800 ,500);
		glVertex2f(800 ,-500);
	glEnd();

	glLineWidth(1);

	glColor3f(0.0, 0.95, 0.99);
	glBegin(GL_POLYGON);				//Start Game Button
		glVertex2f(-250 ,-50);
		glVertex2f(-250 ,40);
		glVertex2f(250 ,40);
		glVertex2f(250 ,-50);
	glEnd();

	glBegin(GL_POLYGON);				//Quit Button
		glVertex2f(-250 ,-300);
		glVertex2f(-250 ,-210);
		glVertex2f(250, -210);
		glVertex2f(250, -300);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	displayRasterText(-200 ,200 ,0.4 ,"WELCOME!");
	glColor3f(0.0, 0.0, 0.0);


	if(mouseX>=-200 && mouseX<=100 && mouseY>=-50 && mouseY<=40){
		glColor3f(0 ,0 ,1) ;
		if(mButtonPressed){
			viewPage = GAME;
			mButtonPressed = false;
			alienLife1 = 100;
		}
	} else
		glColor3f(0 , 0, 0);

    displayRasterText(-200 ,-30 ,0.4 ,"Start Game");


	if(mouseX>=-200 && mouseX<=100 && mouseY>=-170 && mouseY<=-100){
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			mButtonPressed = false;
			exit(0);
		}
	}
	else
		glColor3f(0 , 0, 0);
    displayRasterText(-100 ,-280 ,0.4 ,"Quit");

}

void DrawPlayer(){

	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex2d(xOne,yOne);
	glVertex2d(xOne,yOne+50);
	glVertex2d(xOne+50,yOne+50);
	glVertex2d(xOne+50,yOne);
	glEnd();

}

void DrawLaser(int x, int y, bool dir[]) {

	int xend = -XMAX, yend = y;
	if(dir[0])
		yend = YMAX;
	else if(dir[1])
		yend = -YMAX;
	glLineWidth(2.5);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(xend, yend);

	glEnd();
}

void checkLaser(int x, int y, int x1, int y1,int x2, int y2){
    if(balloonRadius<20.0 && balloon2Radius<20.0){
        balloonRadius= 40.0;
        balloon2Radius= 40.0;
        win= true;
        viewPage= GAME_OVER;
    }
    if(y==y1 && x>x1){
        cout<<"That's a hit "<<endl;

			if(balloonRadius>=20.0){
                balloonRadius/=1.05;
			}
    }

    if(y==y2 && x>x2){
        cout<<"That's a 2 hit "<<endl;

			if(balloon2Radius>=20.0){
                balloon2Radius/=1.05;
			}
    }
}

void checkPlayerContact(float X, float Y) {

	if((X<xOne+50 && Y<yOne+50 && X>xOne && Y>yOne)) {
			cout<<"Life reduced "<<endl;
			balloonRadius= 40.0;
            balloon2Radius= 40.0;
			viewPage=GAME_OVER;
	}
}

void drawBalloon(float X,float Y, float R) {

    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.1415926 / 180.0f;
        float x = X + R * cosf(theta);
        float y = Y + R * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void idle() {

    balloonX += balloonSpeedX;
    balloonY += balloonSpeedY;
    balloon2X += balloonSpeed2X;
    balloon2Y += balloonSpeed2Y;

    if (balloonX + balloonRadius > 1000.0f || balloonX - balloonRadius < -1000.0f) {
        balloonSpeedX = -balloonSpeedX; // Reverse X direction
    }
    if (balloonY + balloonRadius > 700.0f || balloonY - balloonRadius < -700.0f) {
        balloonSpeedY = -balloonSpeedY; // Reverse Y direction
    }

    if (balloon2X + balloonRadius > 1000.0f || balloon2X - balloonRadius < -1000.0f) {
        balloonSpeed2X = -balloonSpeed2X; // Reverse X direction
    }
    if (balloon2Y + balloonRadius > 700.0f || balloon2Y - balloonRadius < -700.0f) {
        balloonSpeed2Y = -balloonSpeed2Y; // Reverse Y direction
    }

    glutPostRedisplay();
}


void gameScreenDisplay()
{
    DrawPlayer();

    drawBalloon(balloonX, balloonY, balloonRadius);
    drawBalloon(balloon2X, balloon2Y, balloon2Radius);
    glColor3f(1.0, 1.0, 1.0);
    displayRasterText(-550, -550, 0.0,"Press ENTER to start the game");
    glColor3f(0.8,1,0);
    displayRasterText(-325, 425, 0.0,"Shooting Game");
    displayRasterText(-300, 350, 0.0,"Instructions");
    glColor3f(1.0, 1.0, 1.0);

    displayRasterText(-500, 200, 0.0,"Key");
    displayRasterText(0,200,0, "Operation");
    displayRasterText(-500, 50, 0.0,"a");
    displayRasterText(0,50,0, "Move left");
    displayRasterText(-500, -50, 0.0,"d");
    displayRasterText(0,-50,0, "Move right");
    displayRasterText(-500, -150, 0.0,"w");
    displayRasterText(0,-150,0, "Move up");
    displayRasterText(-500, -250, 0.0,"x");
    displayRasterText(0,-250,0, "Move down");
    displayRasterText(-500, -350, 0.0,"s");
    displayRasterText(0,-350,0, "Shoot");

}
void gameStartDisplay()
{
    DrawPlayer();
    drawBalloon(balloonX, balloonY, balloonRadius);
    drawBalloon(balloon2X, balloon2Y, balloon2Radius);
    checkPlayerContact(balloonX, balloonY);
    checkPlayerContact(balloon2X, balloon2Y);
    if(laser1) {
        DrawLaser(xOne, yOne, laser1Dir);
        checkLaser(xOne, yOne, balloonX, balloonY,balloonY, balloon2Y);
    }
}

void gameOverDisplay()
{
	glLineWidth(5);

	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-800 ,-500);
		glVertex2f(-800 ,500);
		glVertex2f(800 ,500);
		glVertex2f(800 ,-500);
	glEnd();

	glLineWidth(1);

	glColor3f(0.0, 0.95, 0.99);
	glBegin(GL_POLYGON);				//Start Game Button
		glVertex2f(-250 ,-50);
		glVertex2f(-250 ,40);
		glVertex2f(250 ,40);
		glVertex2f(250 ,-50);
	glEnd();

	glBegin(GL_POLYGON);				//Quit Button
		glVertex2f(-250 ,-300);
		glVertex2f(-250 ,-210);
		glVertex2f(250, -210);
		glVertex2f(250, -300);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	if(win==false){
        displayRasterText(-200 ,200 ,0.4 ,"GAME OVER!");}

	else{
        displayRasterText(-200 ,200 ,0.4 ,"YOU WON!");
        balloonRadius= 40.0;
	}
	glColor3f(0.0, 0.0, 0.0);

	if(mouseX>=-200 && mouseX<=100 && mouseY>=-50 && mouseY<=40){
		glColor3f(0 ,0 ,1) ;
		if(mButtonPressed){
			viewPage = GAME;
			mButtonPressed = false;
			alienLife1 = 100;
		}
	} else
		glColor3f(0 , 0, 0);

    displayRasterText(-200 ,-30 ,0.4 ,"New Game");
		if(mouseX>=-200 && mouseX<=100 && mouseY>=-170 && mouseY<=-100){
		glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			mButtonPressed = false;
			exit(0);
		}
	}
	else
		glColor3f(0 , 0, 0);
    displayRasterText(-100 ,-280 ,0.4 ,"Quit");

}


void mouseClick(int buttonPressed ,int state ,int x, int y) {

	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else
		mButtonPressed = false;
	glutPostRedisplay();
}

void keyOperations(){

    if(keyStates[13] == true && viewPage == GAME) {
		viewPage = GAME_START;
	}
    if(keyStates['s'] == true) {
			laser1 = true;
		}
		else {
			laser1 = false;
			if(keyStates['d'] == true) xOne+=SPEED;
			if(keyStates['a'] == true) xOne-=SPEED;
			if(keyStates['w'] == true) yOne+=SPEED;
			if(keyStates['x'] == true) yOne-=SPEED;
		}
}

void display()
{
    keyOperations();
	glClear(GL_COLOR_BUFFER_BIT);

    if(viewPage==MENU){
	startScreenDisplay();}
	else if(viewPage==GAME){
        gameScreenDisplay();
	}
	else if(viewPage==GAME_START){
        gameStartDisplay();
	}
	else if(viewPage==GAME_OVER){
        gameOverDisplay();
	}
	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}


void passiveMotionFunc(int x,int y) {

	mouseX = float(x)/(m_viewport[2]/1000.0)-600.0;
	mouseY = -(float(y)/(m_viewport[3]/700.0)-350.0);

	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = true;
	glutPostRedisplay();
}

void refresh() {
	glutPostRedisplay();
}

void keyReleased(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 500);
    glutCreateWindow("Shooting Game");
    init();
    glutIdleFunc(refresh);
    glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutPassiveMotionFunc(passiveMotionFunc);
	glutMouseFunc(mouseClick);
	glGetIntegerv(GL_VIEWPORT ,m_viewport);
	glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutMainLoop();
}
