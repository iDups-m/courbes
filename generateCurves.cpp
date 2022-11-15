#include "library.h"

int window;
int mp=-1, droite=0, gauche=0;

int N=0;
Point Pts[NMAX];
arma::mat matCurve;
std::vector<arma::mat> matCurves;
double spline = 0.5;    // spline parameter for Catmull Rom curve

arma::mat M_Bezier;
arma::mat M_BSpline;
arma::mat M_CatmullRom;
arma::mat T;

enum TypeCurve {
    bezier = 0,
    bspline,
    catmullrom
} type_curve = bezier; // default


void initContext(){
    M_Bezier = {
        {-1, 3, -3, 1},
        {3, -6, 3, 0},
        {-3, 3, 0, 0},
        {1, 0, 0, 0}
    };

    M_BSpline = {
        {-1, 3, -3, 1},
        {3, -6, 3, 0},
        {-3, 0, 3, 0},
        {1, 4, 1, 0}
    };

    M_CatmullRom = {
        {-1, 3, -3, 1},
        {2, -5, 4, -1},
        {-1, 0, 1, 0},
        {0, 2, 0, 0}
    };

    T = {
        {0, 0, 0, 1}
    };

    float t = 0.1;
    for(int i=1; t<1; t+=0.01){
        arma::mat matLine = {pow (t, 3.0), pow (t, 2.0), pow (t, 1.0), 1};

        T.insert_rows (i, matLine);
        ++i;
    }
}

void Trace()
{
	glBegin(GL_POINTS);
	for (int i=0;i<N;i++){
		glVertex2f(Pts[i].x,Pts[i].y);
	}
	glEnd();
}

void
main_reshape(int width,  int height) 
{
	GLint viewport[4];
    glViewport(0, 0, width, height);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0, viewport[2], 0.0, viewport[3], -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void
main_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glColor3f(0.0,1.0,0.0);
    glPointSize(3.0);
	glInitNames();
	glPushName(1);
	for (int i=0;i<N;i++){
		glLoadName(i);
		glBegin(GL_POINTS);
		glVertex2f(Pts[i].x,Pts[i].y);
		glEnd();	
	}

    switch (type_curve) {
        case bezier :
            if(N==4 || (N>4 && N%3 == 1)){
                createBezierCubic();
            }
            break;
        case bspline :
            if(N > 3){
                createBSplineCubic();
            }
            break;
        case catmullrom :
            if(N > 3){
                createCatmullRomCubic();
            }
            break;
        default:
            break;
    }

    drawCurves();

    // Bezier :
    /*if(N==4 || (N>4 && N%3 == 1)){
        createBezierCubic();
    }
    drawCurves();*/
    /*if(N%4 == 0){
        createBezierCubic();
        drawCurve();
    }*/

    // B-Spline :
    /*if(N > 3){
        createBSplineCubic();
    }
    drawCurves();*/

    // Catmull-Rom :
    /*if(N > 3){
        createCatmullRomCubic();
    }
    drawCurves();*/

	glutPostRedisplay();
	
    glutSwapBuffers();
}



void specialKeyListener(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            spline += 0.01;
            break;

        case GLUT_KEY_DOWN:
            if(spline > 0.01){
                spline -= 0.01;
            }
            break;
    }
}


void Mouse(int button, int state, int x, int y) {
	GLint viewport[4];

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glGetIntegerv(GL_VIEWPORT, viewport);

	if(button == GLUT_LEFT_BUTTON) {
		droite = 0; gauche = 1;
			glColor3f(0.0,1.0,0.0);
			glPointSize(3.0);
			glInitNames();
			glPushName(1);

            Pts[N].x = x;
            Pts[N].y = viewport[3]-y;

			glLoadName(N);
			glBegin(GL_POINTS);
				glVertex2f(Pts[N].x,Pts[N].y);
			glEnd();

			if(state == GLUT_UP) N++;
		
			glutPostRedisplay();
			
		}

	if(button == GLUT_RIGHT_BUTTON) {
		gauche = 0; droite = 1;
		if(state == GLUT_DOWN) {
			GLuint *selectBuf = new GLuint[200];
			GLuint *ptr;
			GLint hits;

			glSelectBuffer(200, selectBuf);
			glRenderMode(GL_SELECT);

			glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPickMatrix(x, viewport[3]-y, 5.0, 5.0, viewport);
			glOrtho(0.0, viewport[2], 0.0, viewport[3], -50.0, 50.0);

			glColor3f(0.0,1.0,0.0);
			glPointSize(3.0);
			glInitNames();
			glPushName(1);

			for (int i = 0;i<N;i++) {
				glLoadName(i);
				glBegin(GL_POINTS);
				glVertex2f(Pts[i].x,Pts[i].y);
				glEnd();			
			}
				
			glPopMatrix();
			glFlush();

			hits = glRenderMode(GL_RENDER);
			if(hits) {
				ptr = (GLuint *)selectBuf;
				ptr += 3;
				mp = *ptr;
			}
		}

		if(state == GLUT_UP)
			mp = -1;

		main_reshape(viewport[2], viewport[3]);
		glutPostRedisplay();
	}
}

void Motion(int x, int y) {
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	
	if ((droite) &&(mp != -1)) {
		int i= mp;
        Pts[i].x = x;
        Pts[i].y = viewport[3]-y;
		Trace();
		glutPostRedisplay();
	}
}

static void menu (int item)
{
    type_curve = static_cast<TypeCurve> (item);
    glutPostRedisplay ();
}

int main (int argc, char** argv)
{

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);
    initContext();
    
    window = glutCreateWindow("Curves");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
  	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
    glutSpecialFunc(specialKeyListener);

    // Menu to choose type of curve
    glutCreateMenu (menu);
    glutAddMenuEntry ("Bezier", bezier);
    glutAddMenuEntry ("B-Spline", bspline);
    glutAddMenuEntry ("Catmull-Rom", catmullrom);
    glutAttachMenu (GLUT_RIGHT_BUTTON); // right click to open menu

	glutPostRedisplay();  
    glutMainLoop();
    
    return 0;
}
