#ifndef COURBES_LIBRARY_H
#define COURBES_LIBRARY_H

#include <iostream>
#include <new>
#include <cmath>
#include <GL/glut.h>

#include <armadillo>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define NMAX 100
//#define PI 3.1415926535

struct Point {
    float x,y;
    Point(float a=0, float b=0) {set(a,b);}
    void set(float a, float b) {x=a;y=b;}
};

extern Point Pts[NMAX];
extern int N;
extern arma::mat matCurve;
extern std::vector<arma::mat> matCurves;
extern double spline;    // spline parameter for Catmull Rom curve

extern arma::mat M_Bezier;
extern arma::mat M_BSpline;
extern arma::mat M_CatmullRom;
extern arma::mat T;


/* Curves creation */
void createBezierCubic();
void createBSplineCubic();
void createCatmullRomCubic();

/* Curves drawing */
void drawCurves();
void drawCurve();


#endif //COURBES_LIBRARY_H
