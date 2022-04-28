#include <windows.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

const double TWO_PI = 6.2831853;

GLsizei winWidth = 500, winHeight = 500;
GLuint regCirc;
//static values to keep track of our rotational speed, directional speed, boundaries, and direction
//our circle has diameter of 100, so adjust boundary appropriately in respect to window size
static GLfloat rotTheta = 0.0;
static GLfloat xPos = 0.0;
static GLfloat leftWall = -400.0;
static GLfloat rightWall = 400.0;
static GLboolean leftMove = TRUE;

class scrPt{
public:
    GLint x,y;
};

static void init (void)
{
scrPt circVertex;  GLdouble circTheta;  GLint k;

glClearColor (1.0, 1.0, 1.0, 0.0);

//Creates a larger green circle and smaller red circle inside, to show rotational direction
    regCirc = glGenLists (1);
    glNewList (regCirc, GL_COMPILE);  glColor3f (0.0, 0.8, 0.2);
    //Large Circle
    glBegin (GL_POLYGON);
    for (k = 0; k < 50; k++){
    circTheta = TWO_PI * k / 50;
    circVertex.x = 150 + 100 * cos (circTheta);  circVertex.y = 150 + 100 * sin (circTheta);  glVertex2i (circVertex.x, circVertex.y);
    }
    glEnd ( );
    //Smaller, inner circle
    glBegin(GL_POLYGON);
    for (k = 0; k < 50; k++){
    circTheta = TWO_PI * k / 50;
    glColor3f (1.0, 0.2, 0.2);
    circVertex.x = 100 + 25 * cos (circTheta);  circVertex.y = 100 + 25 * sin (circTheta);  glVertex2i (circVertex.x, circVertex.y);
    }
    glEnd();
    glEndList ( );
}

//Translate to move the circle
//Rotate then translate --> shape rotates in place
//Center of circle @ 150,150
void displayCircle (void)
{
glClear (GL_COLOR_BUFFER_BIT);

glPushMatrix ( );
//This Translate set to change xPos to values fluctuating between max and min
glTranslatef(xPos,0.0,0.0);
//Rotate the circle to simulate rolling, rotates about z-axis
glRotatef (rotTheta, 0.0, 0.0, 1.0);
//Center, Keep Circle on same X-Axis
glTranslatef(-150.0,-150.0,0);
glCallList (regCirc);
glPopMatrix ( );
glutSwapBuffers ( );  glFlush ( );
}


void moveCircle (void)
{
//if rotation goes over 360 degrees, use complimentary angle to maintain rotational direction
if (rotTheta > 360.0) {
        rotTheta -= 360.0;
}
//Moving Left, so X-values are increasingly negative and rotation is counter-clockwise
if (leftMove) {
        rotTheta += 1.0;
        xPos = xPos - 1;
        //if our X-value == leftWall boundary, we move right, hence FALSE and change rotational direction
        if (xPos < leftWall) {
                leftMove = FALSE;
                rotTheta -= 1.0;
        }
//Moving Right, X-values are increasingly positive and rotation is clockwise
}else{
     xPos = xPos + 1;
     rotTheta -= 1.0;
     //if our X-value == rightWall boundary, we move Left, so TRUE and change rotational direction
    if (xPos > rightWall){
            leftMove = TRUE;
            rotTheta += 1.0;
    }

}

glutPostRedisplay ( );
}



void winReshapeFcn (int newWidth, int newHeight)
{
glViewport (0, 0, (GLsizei) newWidth, (GLsizei) newHeight);  glMatrixMode (GL_PROJECTION);
glLoadIdentity ( );
gluOrtho2D (-500.0, 500.0, -500.0, 500.0);

glMatrixMode (GL_MODELVIEW);  glLoadIdentity ( );
glClear (GL_COLOR_BUFFER_BIT);
}



void MouseFcn (int button, int action, int x, int y)
{
    //Control for mouse buttons
    switch(button){
    case GLUT_LEFT_BUTTON:
        if (action == GLUT_DOWN)
            glutIdleFunc(moveCircle);
        break;
    case GLUT_RIGHT_BUTTON:
        if (action == GLUT_DOWN)
            exit(1);
        break;
    default:
        break;

    }

}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth,winHeight);
    glutInitWindowPosition(150,150);
    glutCreateWindow("Group Project - Part 2");


    init();
    glutDisplayFunc(displayCircle);
    glutReshapeFunc(winReshapeFcn);
    glutMouseFunc(MouseFcn);
    glutMainLoop();
    return 0;
}
