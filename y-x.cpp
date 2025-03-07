#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

struct Point {
    int x, y;
};

struct edgeRec {
    int y_min, y_max;
    float x;
    float dx;
};

vector<Point> polygonPoints;
bool isPolygonClosed = false;

void yxFill() {
    if (!isPolygonClosed) return;

    int n = polygonPoints.size();
    int min_y = polygonPoints[0].y, max_y = polygonPoints[0].y;
    for (auto &p: polygonPoints) {
        min_y = min(min_y, p.y);
        max_y = max(max_y, p.y);
    }

    vector<edgeRec> edgeTable;
    for (int i = 0; i < n; i++) {
        Point p1 = polygonPoints[i];
        Point p2 = polygonPoints[(i + 1) % n];
        if (p1.y == p2.y) continue;
        if (p1.y > p2.y) {swap(p1, p2);}
        edgeRec edge;
        edge.y_min = p1.y;
        edge.y_max = p2.y;
        edge.x = p1.x;
        edge.dx = float(p2.x - p1.x) / float(p2.y - p1.y);
        edgeTable.push_back(edge);
    }

    sort(edgeTable.begin(), edgeTable.end(), [](const edgeRec& a, const edgeRec& b) {
        return a.y_min < b.y_min;
    });


    vector<edgeRec> AEL;
    int et_index = 0;
    for (int y = min_y; y < max_y; y++) {
        while (et_index < edgeTable.size() && edgeTable[et_index].y_min == y) {
            AEL.push_back(edgeTable[et_index]);
            et_index++;
        }

        for (int i = AEL.size() - 1; i >= 0; i--) {
            if (y >= AEL[i].y_max) {
                AEL.erase(AEL.begin() + i);
            }
        }

        sort(AEL.begin(), AEL.end(), [](const edgeRec&a, const edgeRec&b) {
            return a.x < b.x;
        });

        glBegin(GL_POINTS);
        for (size_t i = 0; i < AEL.size() - 1; i += 2) {
            int x_start = int(ceil(AEL[i].x));
            int x_end = int(floor(AEL[i + 1].x));
            for (int x = x_start; x <= x_end; x++) {
                glVertex2i(x, y);
            }   
        }
        glEnd();

        for (auto& e: AEL) {
            e.x += e.dx;
        }
    }
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
    glutCreateWindow("y-x");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
} 