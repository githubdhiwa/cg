#include <GL/glut.h>
#include <iostream>
#include <vector>

std::vector<std::pair<int, int>> polygonPoints;
bool isPolygonClosed = false;

void bfill(int x, int y, float* bc, float* fc) {
    float color[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);

    if ((color[0] != bc[0] || color[1] != bc[1] || color[2] != bc[2]) && 
        (color[0] != fc[0] || color[1] != fc[1] || color[2] != fc[2])) {
        
        glColor3fv(fc);
        glBegin(GL_POINTS);
            glVertex2i(x, y);
        glEnd();
        glFlush();
        
        bfill(x + 2, y, bc, fc);
        bfill(x - 2, y, bc, fc);
        bfill(x, y + 2, bc, fc);
        bfill(x, y - 2, bc, fc);
    }
}

void display() {
    if (!isPolygonClosed) {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glLineWidth(3);
    glPointSize(2);
    glColor3f(1, 0, 0);

    if (!polygonPoints.empty()) {
        glBegin(GL_LINE_LOOP);
        for (auto &p: polygonPoints) {
            glVertex2i(p.first, p.second);
        }
        glEnd();
    }
    glFlush();
}

void mouse(int btn, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        y = 480 - y;

        if (btn == GLUT_LEFT_BUTTON) {
            if(!isPolygonClosed) {
                polygonPoints.push_back({x, y});
            } else {
                float bc[] = {1.0f, 0.0f, 0.0f};
                float fc[] = {0.0f, 0.0f, 1.0f};
                bfill(x, y, bc, fc);
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
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Boundary Fill");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
}