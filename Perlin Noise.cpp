#include <stdlib.h>
#include <glut.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define B 0x100        //256  
#define BM 0xff         //255  
#define N 0x1000        //4096  
#define NP 12           // 2^N   
#define NM 0xfff  

static int p[B + B + 2];
static float g2[B + B + 2][2];

int start = 1;

static void init(void);

#define s_curve(t) ( t * t * t * (t * (t * 6. - 15.) + 10.) )  
#define lerp(t, a, b) ( a + t * (b - a) )  
#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + N; \
	b0 = ((int)t) & BM; \
	b1 = (b0 + 1) & BM; \
	r0 = t - (int)t; \
	r1 = r0 - 1.;

float noise2(float vec[2])
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, u, v, t;
	register int i, j;
	if (start) {
		start = 0;
		init();
	}
	setup(0, bx0, bx1, rx0, rx1);
	setup(1, by0, by1, ry0, ry1);
	i = p[bx0];
	j = p[bx1];

	b00 = p[i + by0];
	b10 = p[j + by0];
	b01 = p[i + by1];
	b11 = p[j + by1];

	sx = s_curve(rx0);
	sy = s_curve(ry0);

#define at2(rx,ry) ( rx * q[0] + ry * q[1] )  
	q = g2[b00]; u = at2(rx0, ry0);
	q = g2[b10]; v = at2(rx1, ry0);
	a = lerp(sx, u, v);

	q = g2[b01]; u = at2(rx0, ry1);
	q = g2[b11]; v = at2(rx1, ry1);
	b = lerp(sx, u, v);

	return lerp(sy, a, b);

}

static void init(void)
{
	int i, j, k;
	for (i = 0; i < B; i++) {
		p[i] = i;
		for (j = 0; j < 2; j++)
			g2[i][j] = (float)((rand() % (B + B)) - B) / B;
	}

	while (--i) {
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}
	for (i = 0; i < B + 2; i++) {
		p[B + i] = p[i];
		for (j = 0; j < 2; j++)
			g2[B + i][j] = g2[i][j];
	}
}

void drawScene()
{
	float d1 = -300.0, d2 = -300.0;
	const float size = 0.1f;
	for (int i = 0; i < 100; i++) {
		d1 = -300;
		for (int j = 0; j < 100; j++) {
			float vec[2] = { d1,d2 };
			float color = (noise2(vec) + 1) / 4;
			glColor3f(color, color, color);
			glRectf(i*size, j*size, i*size + size, j*size + size);
			d1 += 0.1;
		}
		d2 += 0.1;
	}
}
void drawScene2()
{
	float d1 = -300.0, d2 = -300.0;
	const float size = 0.1f;
	for (int i = 0; i < 100; i++) {
		d1 = -300;
		for (int j = 0; j < 100; j++) {
			float vec[2] = { d1,d2 };
			float vec1[2] = { d1 * 2,d2 * 2 };
			float vec2[2] = { d1 * 4,d2 * 4 };
			float vec3[2] = { d1 * 8,d2 * 8 };
			
			float color = (noise2(vec)+noise2(vec1)/2+noise2(vec2)/4 +noise2(vec3)/8+ 1) / 4;
			glColor3f(color, color, color);
			glRectf(i*size, j*size, i*size + size, j*size + size);
			d1 += 0.1;
		}
		d2 += 0.1;
	}
}
void reshape(int width, int height)
{
	if (height == 0)height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();              
	glOrtho(0, 10, 0, 10, -100, 100); 
	glMatrixMode(GL_MODELVIEW);  
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  
	drawScene();
	glutSwapBuffers();
}

void redraw2() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawScene2();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	srand(time(nullptr));
	glutInit(&argc, argv);     
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(400, 400);         
	int windowHandle = glutCreateWindow("Perlin Noise");     
	glutDisplayFunc(redraw);           
	glutReshapeFunc(reshape);              
    

	srand(time(nullptr));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(400, 400);
	int windowHandle2 = glutCreateWindow("Perlin Noise2");
	glutDisplayFunc(redraw2);
	glutReshapeFunc(reshape);
	glutMainLoop();
}