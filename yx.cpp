#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

struct Point {
    int x, y;
};

vector<Point> polygonPoints;
bool isPolygonClosed = false;

void yxFill() {
    if (!isPolygonClosed) return;

    int n = polygonPoints.size();
    int y_min = polygonPoints[0].y, y_max = polygonPoints[0].y;
    for (auto &p: polygonPoints) {
        y_min = min(y_min, p.y);
        y_max = max(y_max, p.y);
    }

    glBegin(GL_POINTS);
    for (int y = y_min; y <= y_max; y++) {
        vector<float> inter;
        for (int i = 0; i < n; i++) {
            Point p1 = polygonPoints[i];
            Point p2 = polygonPoints[(i+1)%n];
            if (p1.y > p2.y) {swap(p1, p2);}
            float scan_y = y + 0.5f;
            if (scan_y >= p1.y && scan_y < p2.y) {
                float x_inter = p1.x + (scan_y - p1.y) * (p2.x - p1.x) / float(p2.y - p1.y);
                inter.push_back(x_inter);
            }
        }

        sort(inter.begin(), inter.end());
        for (size_t i = 0; i + 1 < inter.size(); i += 2) {
            int x_start = int(ceil(inter[i] - 0.5f));
            int x_end = int(floor(inter[i + 1] - 0.5f));
            for (int x = x_start; x <= x_end; x++) {
                glVertex2i(x, y);
            }
        }
    }
    glEnd();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (isPolygonClosed && !polygonPoints.empty()) {
        glColor3f(4.8f, 5.9f, 0.8f);
        yxFill();
    }

    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (auto &p: polygonPoints) {
        glVertex2i(p.x, p.y);
    }
    glEnd();
    glFlush();
}

void mouse(int btn, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        y = 480 - y;
        if (btn == GLUT_LEFT_BUTTON) {
            if (!isPolygonClosed) {
                polygonPoints.push_back({x, y});
            }
        }
        else if (btn == GLUT_RIGHT_BUTTON) {
            if (!isPolygonClosed && polygonPoints.size() >= 3) {
                isPolygonClosed = true;
            }
        }
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE  | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("yxmf");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
} 