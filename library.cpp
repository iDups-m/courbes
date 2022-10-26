#include "library.h"

/* Curves creation */
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
