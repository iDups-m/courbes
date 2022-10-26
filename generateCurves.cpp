#include <iostream>
#include <new>
#include <math.h>
#include <GL/glut.h>

#include <armadillo>

//const float PI = 3.1415926535;

// variables globales pour OpenGL
int window,width,height;
const int NMAX = 100;
int N = 0;

int mp=-1,droite=0,gauche=0;

struct Point {
	float x,y;
	Point(float a=0, float b=0) {set(a,b);}
	void set(float a, float b) {x=a;y=b;}
};

Point Pts[NMAX];
arma::mat matCurve;
std::vector<arma::mat> matCurves;
double spline = 0.5;    // spline parameter for Catmull Rom curve

void createBezierCubic(){
	float t = 0.0;

	arma::mat M = {
		{-1, 3, -3, 1},
		{3, -6, 3, 0},
		{-3, 3, 0, 0},
		{1, 0, 0, 0}
	};


    arma::mat P = {
        {Pts[N-4].x, Pts[N-4].y},
        {Pts[N-3].x, Pts[N-3].y},
        {Pts[N-2].x, Pts[N-2].y},
        {Pts[N-1].x, Pts[N-1].y}
    };

	arma::mat T = {
		{0, 0, 0, 1}
	};

	t+=0.01;
	for(int i=1; t<1; t+=0.01){
		arma::mat matLine = {pow (t, 3.0), pow (t, 2.0), pow (t, 1.0), 1};

		T.insert_rows (i, matLine);
		++i;
	}

    //matCurve = T * M * P;
    matCurves.push_back(T * M * P);
}

void createBSplineCubic(){
    float t = 0.0;

    arma::mat M = {
            {-1, 3, -3, 1},
            {3, -6, 3, 0},
            {-3, 0, 3, 0},
            {1, 4, 1, 0}
    };


    arma::mat P = {
            {Pts[N-4].x, Pts[N-4].y},
            {Pts[N-3].x, Pts[N-3].y},
            {Pts[N-2].x, Pts[N-2].y},
            {Pts[N-1].x, Pts[N-1].y}
    };

    arma::mat T = {
            {0, 0, 0, 1}
    };

    t+=0.01;
    for(int i=1; t<1; t+=0.01){
        arma::mat matLine = {pow (t, 3.0), pow (t, 2.0), pow (t, 1.0), 1};

        T.insert_rows (i, matLine);
        ++i;
    }

    //matCurve = T * (M/6) * P;
    matCurves.push_back(T * (M/6) * P);
}

void createCatmullRomCubic(){
    float t = 0.0;

    arma::mat M = {
            {-1, 3, -3, 1},
            {2, -5, 4, -1},
            {-1, 0, 1, 0},
            {0, 2, 0, 0}
    };


    arma::mat P = {
            {Pts[N-4].x, Pts[N-4].y},
            {Pts[N-3].x, Pts[N-3].y},
            {Pts[N-2].x, Pts[N-2].y},
            {Pts[N-1].x, Pts[N-1].y}
    };

    arma::mat T = {
            {0, 0, 0, 1}
    };

    t+=0.01;
    for(int i=1; t<1; t+=0.01){
        arma::mat matLine = {pow (t, 3.0), pow (t, 2.0), pow (t, 1.0), 1};

        T.insert_rows (i, matLine);
        ++i;
    }

    std::cout << "spline parameter : " << spline << std::endl;

    //matCurve = T * (M * spline) * P;
    matCurves.push_back(T * (M * spline) * P);
}

void drawCurve(){
	glBegin(GL_LINE_STRIP);
	int nbRow = matCurve.n_rows;
	for(int i=0; i<nbRow; i++){
		glVertex2f(matCurve(i, 0), matCurve(i, 1));
	}
	glEnd();
}
void drawCurves(){
    for(auto matBezier : matCurves){
        glBegin(GL_LINE_STRIP);
        int nbRow = matBezier.n_rows;
        for(int i=0; i<nbRow; i++){
            glVertex2f(matBezier(i, 0), matBezier(i, 1));
        }
        glEnd();
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
    if(N > 3){
        createCatmullRomCubic();
    }
    drawCurves();

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

int main (int argc, char** argv)
{

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);
    
    window = glutCreateWindow("Select");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
  	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
    glutSpecialFunc(specialKeyListener);

	glutPostRedisplay();  
    glutMainLoop();
    
    return 0;
}
