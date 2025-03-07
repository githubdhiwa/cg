#include <GL/glut.h>
#include <vector>
#include <iostream>

struct Point {int x, y;};
struct TestPoint {int x, y; bool inside;};
bool isPolygonClosed = false;
std::vector<Point> polygonPoints;
std::vector<TestPoint> testPoints;

bool isInside(int x, int y, std::vector<Point>& poly) {
    int count = 0, n = poly.size();
    for (int i = 0; i < n; i++) {
        Point a = poly[i], b = poly[(i + 1) % n];
        if ((a.y > y) != (b.y > y)) {
            double ix = a.x + (y - a.y) * (b.x - a.x) / (b.y - a.y);
            if (x < ix) count++;
        }
    }
    return (count % 2 == 1);
}

void display() {
    if (!isPolygonClosed) {glClear(GL_COLOR_BUFFER_BIT);}
    if (!polygonPoints.empty() && isPolygonClosed) {
        glColor3f(4.8f, 5.8f, 0.8f);
        glPointSize(1);
        glBegin(GL_POINTS);
        for (int y = 0; y < 480; y++) {
            for (int x = 0; x < 640; x++) {
                if (isInside(x, y, polygonPoints)) {
                    glVertex2i(x, y);
                }
            }
        }
        glEnd();
    }

    if (!polygonPoints.empty()) {
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_LOOP);
        for (auto &p: polygonPoints) {
            glVertex2i(p.x, p.y);
        }
        glEnd();
    }

    glPointSize(5);
    for (auto &p: testPoints) {
        glColor3f(p.inside ? 0 : 1, p.inside ? 1 : 0, 0);
        glBegin(GL_POINTS);
        glVertex2i(p.x, p.y);
        glEnd(); 
    }
    glFlush();
}

void mouse(int btn, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        y = 480 - y;

        if (btn == GLUT_LEFT_BUTTON) {
            if (!isPolygonClosed) {
                polygonPoints.push_back({x, y});
            }
            else {
                bool inside = isInside(x, y, polygonPoints);
                testPoints.push_back({x, y, inside});
            }
        } else if (btn == GLUT_RIGHT_BUTTON) {
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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("io");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
}