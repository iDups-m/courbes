#include "library.h"

/* Curves creation */
void createBezierCubic(){

    arma::mat P = {
            {Pts[N-4].x, Pts[N-4].y},
            {Pts[N-3].x, Pts[N-3].y},
            {Pts[N-2].x, Pts[N-2].y},
            {Pts[N-1].x, Pts[N-1].y}
    };

    matCurves.push_back(T * M_Bezier * P);
}

void createBSplineCubic(){

    arma::mat P = {
            {Pts[N-4].x, Pts[N-4].y},
            {Pts[N-3].x, Pts[N-3].y},
            {Pts[N-2].x, Pts[N-2].y},
            {Pts[N-1].x, Pts[N-1].y}
    };

    matCurves.push_back(T * (M_BSpline/6) * P);
}

void createCatmullRomCubic(){

    arma::mat P = {
            {Pts[N-4].x, Pts[N-4].y},
            {Pts[N-3].x, Pts[N-3].y},
            {Pts[N-2].x, Pts[N-2].y},
            {Pts[N-1].x, Pts[N-1].y}
    };

    std::cout << "spline parameter : " << spline << std::endl;

    matCurves.push_back(T * (M_CatmullRom * spline) * P);
}

/* Curves render - one or several curves */
void drawCurve(void){
    glBegin(GL_LINE_STRIP);
    int nbRow = matCurve.n_rows;
    for(int i=0; i<nbRow; i++){
        glVertex2f(matCurve(i, 0), matCurve(i, 1));
    }
    glEnd();
}
void drawCurves(void){
    for(auto matBezier : matCurves){
        glBegin(GL_LINE_STRIP);
        int nbRow = matBezier.n_rows;
        for(int i=0; i<nbRow; i++){
            glVertex2f(matBezier(i, 0), matBezier(i, 1));
        }
        glEnd();
    }
}
