#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
typedef struct rect {
	float x;
	float y;
	float width;
	float height;
} rect;
rect rectangle;
float angle = 30.0;
float dispx = 0.2;
float dispy = 0.1;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	rectangle.x = 0.2;
	rectangle.y = 0.4;
	rectangle.width = 0.6;
	rectangle.height = 0.2;
}
void drawAxes_world() {
	glLineWidth(3.0);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3i(0, 0, 0);
		glVertex3i(100, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 100, 0);
	glEnd();
}
void drawAxes_viewing() {
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glColor3f(0, 1, 1);
	glVertex3i(0, 0, 0);
	glVertex3i(100, 0, 0);
	glColor3f(1, 0, 1);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 100, 0);
	glEnd();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawAxes_viewing();
			glRotatef(angle, 0.0, 0.0, 1.0);
	gluLookAt(0.5, 0.5, 0.0, 0.5, 0.5, -1.0, 0.0, 1.0, 0.0);
	drawAxes_world();
			//glRotatef(angle, 0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(rectangle.x, rectangle.y);
		glVertex2f(rectangle.x, rectangle.y + rectangle.height);
		glVertex2f(rectangle.x + rectangle.width, rectangle.y + rectangle.height);
		glVertex2f(rectangle.x + rectangle.width, rectangle.y);
	glEnd();


	glColor3f(0, 0, 1);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex2f(0.5f, 0.5f);
	glEnd();

	glutSwapBuffers();
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
			//glTranslatef(dispx, dispy, 0.0);
	gluOrtho2D(-3, 3, -3, 3);
			//glTranslatef(dispx, dispy, 0.0);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}