#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <algorithm>
using namespace std;

struct Point { int x, y; };
struct TestPoint { int x, y; bool inside; };

vector<Point> polygonPoints;
vector<TestPoint> testPoints;
bool polygonClosed = false;

int windingNumber(int x, int y, const vector<Point>& poly) {
    int wn = 0, n = poly.size();
    for (int i = 0; i < n; i++) {
        Point a = poly[i], b = poly[(i + 1) % n];
        if(a.y <= y) {
            if(b.y > y && (b.x - a.x) * (y - a.y) - (x - a.x) * (b.y - a.y) > 0)
                wn++;
        } else {
            if(b.y <= y && (b.x - a.x) * (y - a.y) - (x - a.x) * (b.y - a.y) < 0)
                wn--;
        }
    }
    return wn;
}

void init() {
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (polygonClosed && !polygonPoints.empty()) {
        glColor3f(5.8f, 6.8f, 0.8f);
        glPointSize(1);
        glBegin(GL_POINTS);
        for (int y = 0; y < 480; y++)
            for (int x = 0; x < 640; x++)
                if (windingNumber(x, y, polygonPoints) != 0)
                    glVertex2i(x, y);
        glEnd();
    }
    if (!polygonPoints.empty()) {
        glColor3f(0, 0, 0);
        glBegin(GL_LINE_LOOP);
        for (auto &p : polygonPoints)
            glVertex2i(p.x, p.y);
        glEnd();
    }
    glPointSize(5);
    for (auto &tp : testPoints) {
        glColor3f(tp.inside ? 0 : 1, tp.inside ? 1 : 0, 0);
        glBegin(GL_POINTS);
        glVertex2i(tp.x, tp.y);
        glEnd();
    }
    glFlush();
}

void mouse(int btn, int state, int x, int y) {
    y = 480 - y;
    if (state == GLUT_DOWN) {
        if (!polygonClosed) {
            if (btn == GLUT_LEFT_BUTTON)
                polygonPoints.push_back({ x, y });
            else if (btn == GLUT_RIGHT_BUTTON && polygonPoints.size() >= 3)
                polygonClosed = true;
        } else {
            bool inside = windingNumber(x, y, polygonPoints) != 0;
            cout << "Winding Number test for point (" << x << ", " << y << "): " 
                 << (inside ? "Inside" : "Outside") << "\n";
            testPoints.push_back({ x, y, inside });
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Winding Number Test");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
