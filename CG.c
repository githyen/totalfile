#define GL_SILENCE_DEPRECATION

#include<glut.h>
#include<stdlib.h>
#include<math.h>
#if 1

float angle = 0;
float xView = 0, yView = 2, zView = 7;
float lx = 0, ly = 0, lz = -1;

int xOrigin = -1, yOrigin = -1, zOrigin = -1;

float deltaAngle = 0;

void changeSize(int width, int height);
void drawSnowMan();
void display(void);
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

void mouseMove(int x, int y);
void mouseButton(int button, int state, int x, int y);

void changeSize(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	float aspect = width / height;
	gluPerspective(45, aspect, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan()
{   	//Draw head
	glColor3f(1, 1, 1);
	glTranslatef(0, 0.75, 0);
	glutWireSphere(0.75, 10, 10);
	glTranslatef(0, 1, 0);
	glutSolidSphere(0.5, 10, 10);
	glPushMatrix();
	//Draw eyes
	glTranslatef(0.1, 0, 0.23);
	glColor3f(0, 0, 1);
	glutSolidSphere(0.25, 10, 10);
	glTranslatef(-0.2, 0, 0);
	glutSolidSphere(0.25, 10, 10);
	//Draw nose
	glPopMatrix();
	glTranslatef(0, -0.1, 0.75);
	glColor3f(1, 0, 0);
	glutSolidCube(0.1);
	glutSolidCone(0.025, 0.25, 4, 4);	

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(xView, yView, zView,xView + lx, yView + ly, zView + lz,0, 1, 0);
	glRotatef(angle, 0, 1, 0);
	glColor3f(1, 0, 0);

	glLineWidth(15);

	glBegin(GL_LINES); // 객체 불러와서 시작
	glColor3f(1, 1, 1);
	glVertex3f(1.3, 1, 0); //팔과 다리
	glVertex3f(-1, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(-1.3, 1, 0);
	glEnd();

	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);	// 몸체
	glVertex3f(-100, 0, -100);
	glVertex3f(-100, 0, 100);
	glVertex3f(100, 0, 100);
	glVertex3f(100, 0, -100);
	glEnd();

	drawSnowMan();
	glutSwapBuffers();

}
void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	}
}
void processSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		angle -= 0.5;
		break;

	case GLUT_KEY_RIGHT:
		angle += 0.5;
		break;

	case GLUT_KEY_UP:
		zView -= 0.5;
		break;

	case GLUT_KEY_DOWN:
		zView += 0.5;
		break;
	}
}

void mouseMove(int x, int y)
{
	if (xOrigin >= 0 || zOrigin >= 0)
	{
		deltaAngle = acos(((float)xOrigin * x + yOrigin * y) / (sqrt((float)xOrigin * xOrigin + yOrigin * yOrigin) * sqrt((float)x * x + y * y)));
		if (x <= xOrigin)
		{
			angle -= deltaAngle * 100;
			xOrigin = x;
			yOrigin = y;
		}
		else
		{
			angle += deltaAngle * 100;
			xOrigin = x;
			yOrigin = y;
		}
	}
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			xOrigin = x;
			yOrigin = y;
		}
		else
		{
			xOrigin = -1;
			yOrigin = -1;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(680, 680);
	glutCreateWindow("SM");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutIdleFunc(display);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 0;
}
#endif
