#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#define XMAX 1200
#define YMAX 700
#define SPACESHIP_SPEED 0.05
#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

#include<GL/glut.h>

#include <time.h>
void delay(float secs)
{
	float end = clock()/CLOCKS_PER_SEC + secs;
	while((clock()/CLOCKS_PER_SEC) < end);
}


using namespace std;

enum view {MENU, GAME, GAME_START, GAMEOVER};
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
float d=0;

float balloonRadius = 40.0f;
int alienLife1 = 100;



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


	//glutPostRedisplay();

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

void checkLaserContact(int x, int y, bool dir[], int xp, int yp, bool player1) {
	int xend = -XMAX, yend = y;
	xp += 8; yp += 8; // moving circle slightly up to fix laser issue
	if(dir[0])
		yend = YMAX;
	else if(dir[1])
		yend = -YMAX;

	// Here we find out if the laser(line) intersects with spaceship(circle)
	// by solving the equations for the same and finding the discriminant of the
	// quadratic equation obtained
	float m = (float)(yend - y) / (float)(xend - x);
	float k = y - m * x ;
	int r = 50; // approx radius of the spaceship

	//calculating value of b, a, and c needed to find discriminant
	float b = 2 * xp - 2 * m * (k - yp);
	float a = 1 + m * m;
	float c = xp * xp + (k - yp) * (k - yp) - r * r;

	d = (b * b - 4 * a * c); // discriminant for the equation
	//printf();
	if((d>=0)) {
		if(player1){
			cout<<"Out "<<endl;
			balloonRadius/=1.1;
			delay(0.2);}

	}
}

void drawBalloon(float X,float Y) {

    glColor3f(0.7f, 0.7f, 0.7f); // Set color to red
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.1415926 / 180.0f;
        float x = X + balloonRadius * cosf(theta);
        float y = Y + balloonRadius * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void idle() {
    // Update balloon position
    balloonX += balloonSpeedX;
    balloonY += balloonSpeedY;
    balloon2X += balloonSpeed2X;
    balloon2Y += balloonSpeed2Y;

    // Check for collision with window boundaries
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


void DisplayHealthBar1() {
	char temp1[40];
	glColor3f(1 ,1 ,1);
    sprintf(temp1,"  LIFE = %d",alienLife1);

	displayRasterText(-1100 ,600 ,0.4 ,temp1);
	glColor3f(1 ,0 ,0);
}


void gameScreenDisplay()
{
    DrawPlayer();

    drawBalloon(balloonX, balloonY);
    drawBalloon(balloon2X, balloon2Y);
    DisplayHealthBar1();
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
    drawBalloon(balloonX, balloonY);
    drawBalloon(balloon2X, balloon2Y);
    if(laser1) {
        DrawLaser(xOne, yOne, laser1Dir);
        checkLaserContact(xOne, yOne, laser1Dir, balloonX, balloonY, true);
        checkLaserContact(xOne, yOne, laser1Dir, balloon2X, balloon2Y, true);
    }
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
			if(keyStates['d'] == true) xOne+=SPACESHIP_SPEED;
			if(keyStates['a'] == true) xOne-=SPACESHIP_SPEED;
			if(keyStates['w'] == true) yOne+=SPACESHIP_SPEED;
			if(keyStates['x'] == true) yOne-=SPACESHIP_SPEED;
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
