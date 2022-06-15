import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

from scipy.interpolate import make_interp_spline

WIDTH = 750
HEIGHT = 750

class Graph:
    def __init__(self) -> None:
        self.points = np.array([[], []])

        self.k = 3

        self.minX = -1
        self.maxX = 1

        self.minY = -1
        self.maxY = 1

    def draw(self):
        self.drawGrid()
        self.drawSpline()
        self.draw_points()
        self.drawString(-0.9, 0.9, f"k = {self.k}", (0.7, 0.5, 0.7), GLUT_BITMAP_TIMES_ROMAN_24)

    def drawGrid(self):
        glBegin(GL_LINES)
        glColor4d(0.1, 0.1, 0.1, 1)

        glVertex2d(-1, 0)
        glVertex2d(1, 0)

        glVertex2d(0, -1)

        glVertex2d(0, 1)

        x = np.linspace(self.minX, self.maxX, 11)
        y = np.linspace(self.minY, self.maxY, 11)

        pos = np.linspace(-1, 1, 11)
        for i in pos:
            glVertex2d(i, -0.01)
            glVertex2d(i, 0.01)

            glVertex2d(-0.01, i)
            glVertex2d(0.01, i)

        glEnd()

        def format(fl):
            if abs(fl) < 0.000001:
                return "0"
            if abs(fl) >= 0.02:
                return f"{fl:.2f}"
            else:
                return f"{fl:.2e}"

        for i, p in enumerate(pos):
            if i != 5:
                self.drawString(p - 0.02, -0.03, format(x[i]))
            self.drawString(0.03, p + 0.0, format(y[i]))

    def drawString(self, x, y, text, color=(0, 1, 1), font=GLUT_BITMAP_TIMES_ROMAN_10):
        glColor3f(*color)
        glRasterPos2f(x, y)
        glutBitmapString(font, text.encode('ascii'))

    def draw_points(self):
        glColor4d(0.1, 0.1, 1, 1)
        glEnable(GL_PROGRAM_POINT_SIZE)
        glPointSize(5)
        glBegin(GL_POINTS)
        self.drawPoints(self.points[0], self.points[1])
        glEnd()

    def addPoint(self, x, y):
        x = (self.maxX - self.minX) * x / WIDTH + self.minX
        y = self.maxY - (self.maxY - self.minY) * y / HEIGHT
        points = np.c_[self.points, np.array([[x], [y]])]
        points = points[:, points[0].argsort()]
        _, unique_ind = np.unique(points[0], return_index=True)

        self.points = points[:, unique_ind]

    def drawSpline(self):
        glColor4d(0.1, 0.1, 0.1, 1)

        try:
            splineB = make_interp_spline(self.points[0], self.points[1], k=self.k)  # , bc_type="clamped")
            # splineB = scipy_bspline(self.points.T, degree=self.k)

            x = np.linspace(self.minX, self.maxX, 256)
            y = splineB(x)

            glBegin(GL_LINE_STRIP)
            self.drawPoints(x, y)
            glEnd()

            glBegin(GL_LINE_STRIP)
            self.drawPoints(x, y)
            glEnd()

        except Exception as e:
            print(e)

            glBegin(GL_LINE_STRIP)
            self.drawPoints(self.points[0], self.points[1])
            glEnd()
            self.drawString(-0.8, -0.8, "Can't create B-spline! Add more points.", (1, 0, 0))

    def drawPoints(self, x, y):
        x = (x - self.minX) * 2 / (self.maxX - self.minX) - 1
        y = (y - self.minY) * 2 / (self.maxY - self.minY) - 1

        for x_i, y_i in zip(x, y):
            glVertex2d(x_i, y_i)

    def scaleX(self):
        return self.maxX - self.minX

    def scaleY(self):
        return self.maxY - self.minY


graph = Graph()


def draw():
    glClearColor(0.5, 0.5, 0.5, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)

    graph.draw()

    glutSwapBuffers()


def mouseClick(button, state, x, y):
    if button == GLUT_LEFT_BUTTON and state == GLUT_DOWN:
        addPoint(x, y)


def addPoint(x, y):
    global graph

    graph.addPoint(x, y)

    glutPostRedisplay()


def keyPressed(key, x, y):
    global graph
    print(key)

    if key == b'c':
        graph.points = np.array([[], []])
    elif key == b'3':
        graph.k = 3
    elif key == b'5':
        graph.k = 5
    elif key == b'7':
        graph.k = 7
    elif key == b'-':
        m = 0.6
        xCenter = graph.minX + graph.scaleX() / 2

        graph.minX = xCenter - m * graph.scaleX()
        graph.maxX = xCenter + m * graph.scaleX()

        yCenter = graph.minY + graph.scaleY() / 2

        graph.minY = yCenter - m * graph.scaleY()
        graph.maxY = yCenter + m * graph.scaleY()

    elif key == b'+':
        m = 0.4
        xCenter = graph.minX + graph.scaleX() / 2

        graph.minX = xCenter - m * graph.scaleX()
        graph.maxX = xCenter + m * graph.scaleX()

        yCenter = graph.minY + graph.scaleY() / 2

        graph.minY = yCenter - m * graph.scaleY()
        graph.maxY = yCenter + m * graph.scaleY()

    glutPostRedisplay()


def specialKeyPressed(key, x, y):
    global graph

    if key == GLUT_KEY_LEFT:
        graph.minX -= 0.15 * graph.scaleX()
        graph.maxX -= 0.15 * graph.scaleX()

    elif key == GLUT_KEY_RIGHT:
        graph.minX += 0.15 * graph.scaleX()
        graph.maxX += 0.15 * graph.scaleX()

    elif key == GLUT_KEY_UP:
        graph.minY += 0.15 * graph.scaleY()
        graph.maxY += 0.15 * graph.scaleY()

    elif key == GLUT_KEY_DOWN:
        graph.minY -= 0.15 * graph.scaleY()
        graph.maxY -= 0.15 * graph.scaleY()

    glutPostRedisplay()


if __name__ == '__main__':
    glutInit(sys.argv)

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)
    glutInitWindowSize(WIDTH, HEIGHT)
    glutInitWindowPosition(10, 10)
    glutCreateWindow(b"RR graphics")

    glutDisplayFunc(draw)
    glutMouseFunc(mouseClick)

    glutKeyboardFunc(keyPressed)
    glutSpecialFunc(specialKeyPressed)

    glutMainLoop()
