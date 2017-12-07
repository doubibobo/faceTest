#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <gl\GL.h>
//#include <gl\glew.h>
#include <gl\glut.h>
#include <GL\glaux.h>

#pragma comment(lib, "GLAux.Lib")

#pragma warning(disable : 4996)

using namespace std;

// 相关数据声明
#define MAX 70000
#define max_rum 99999999

#define LEFT 1
#define RIGHT 2
#define SMALL 3
#define LARGE 4

// typedef float GLfloat;
// 定义纹理编号
GLuint texture[1];
GLfloat make_s_1 = 2.0;

int m, n;
int specialkey;
GLfloat max_x = -max_rum;
GLfloat min_x = max_rum;
GLfloat max_y = -max_rum;
GLfloat min_y = max_rum;
GLfloat max_z = -max_rum;
GLfloat min_z = max_rum;

// 点的结构体，存储点的信息
struct Point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat dx;
	GLfloat dy;
	GLfloat dz;
	int num;
};

// 边的结构体，存储边的信息
struct Triangle {
	int p1;
	int p2;
	int p3;
	GLfloat dx;
	GLfloat dy;
	GLfloat dz;
};

Point ma[MAX];
Triangle na[MAX];

GLfloat xrot = 0.0;
GLfloat yrot = 0.0;
GLfloat zrot = 0.0;

// 环境光参数
GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
// 漫射光参数
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
// 光源的位置
GLfloat lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };

// 加载位图，将位图读取到内存空间，实现纹理的创建
AUX_RGBImageRec *LoadBMP(char *Filename);
int LoadGLTextures();
int DrawGLScene(GLvoid);
void render();
void reshape(int width, int height);
void keyBoard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void createGLUTMenus();
void menusEvents(int options);
void init();

// 主函数相关设置
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Rabit Bunny");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	init();
	LoadGLTextures();
	DrawGLScene();
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyBoard);
	glutMouseFunc(mouse);
	glEnable(GL_DEPTH_TEST);
	createGLUTMenus();
	glutMainLoop();
	return 0;
}

AUX_RGBImageRec *LoadBMP(char *Filename) {
	FILE* file = NULL;
	if (!Filename) {
		cout << "文件不存在！" << endl;
		return NULL;
	}
	file = fopen(Filename, "r");
	if (file) {
		fclose(file);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}

// 根据加载的位图创建纹理
int LoadGLTextures() {
	int Status = FALSE;         // 指示纹理创建是否成功的标志
	AUX_RGBImageRec *TextureImage[1];     // 创建一个纹理图像数组，这里指定数组大小为6
	memset(TextureImage, 0, sizeof(void *) * 1);           // 初始化纹理图像数组，为其分配内存

	char *pictures[] = { // 创建一个位图名称数组，对应6幅位图
		"therabit.bmp"
	};
	// 遍历位图名称数组，根据位图名称分别生成
	for (int i = 0; i<1; i++){
		// 加载位图i成功，修改状态标志变量Status为TRUE
		if (TextureImage[i] = LoadBMP(pictures[i])) {
			Status = TRUE;
			glGenTextures(1, &texture[i]);     // 为第i个位图创建纹理
			glBindTexture(GL_TEXTURE_2D, texture[i]); // 将生成的纹理的名称绑定到指定的纹理上
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		// 释放位图数组占用的内存空间
		if (TextureImage[i]) {
			if (TextureImage[i]->data) {
				free(TextureImage[i]->data);
			}
				free(TextureImage[i]);
			}
	}
	return Status;          // 创建纹理并加载，返回成功或者失败的标志Status
}

int DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glRotatef(zrot, 0.0f, 0.0f, 1.0f);


	// Front Face
	glBindTexture(GL_TEXTURE_2D, texture[0]); //   选择第一个纹理texture[0]，进行贴纹理
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 8.0f); glVertex3f(-2.0f, 1.0f, 0.0f);
	glTexCoord2f(8.0f, 8.0f); glVertex3f(1500.0f, 1.0f, -6000.0f);
	glTexCoord2f(8.0f, 0.0f); glVertex3f(2000.0f, -1.0f, -6000.0f);
	glEnd();

	// Back Face
	//glBindTexture(GL_TEXTURE_2D, texture[1]); //   选择第二个纹理texture[1]，进行贴纹理
	//glBegin(GL_QUADS);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	//glEnd();

	// Top Face
	//glBindTexture(GL_TEXTURE_2D, texture[2]); //   选择第三个纹理texture[2]，进行贴纹理
	//glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	//glEnd();

	// Bottom Face
	//glBindTexture(GL_TEXTURE_2D, texture[3]); //   选择第四个纹理texture[3]，进行贴纹理
	//glBegin(GL_QUADS);
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	//glEnd();

	// Right face
	//glBindTexture(GL_TEXTURE_2D, texture[4]); //   选择第五个纹理texture[4]，进行贴纹理
	//glBegin(GL_QUADS);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	//glEnd();

	// Left Face
	//glBindTexture(GL_TEXTURE_2D, texture[5]); //   选择第六个纹理texture[5]，进行贴纹理
	//glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	//glEnd();

	GLfloat* planeCoefficients = (1, 0, 0, 0);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);


	xrot += 0.3f;
	yrot += 0.2f;
	zrot += 0.4f;
	return TRUE;
}


void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 将当前坐标系以a(1.0, 0.0, 0.0)向量为旋转轴，旋转xrot度
	glRotated(xrot, 1.0, 0.0, 0.0);
	glRotated(yrot, 0.0, 1.0, 0.0);
	glRotated(zrot, 0.0, 0.0, 1.0);

	for (int i = 0; i < n; i++) {
		// 开始画三角形
		glBegin(GL_TRIANGLES);
		// 设置顶点的法线向量
		glNormal3f(ma[na[i].p1].dx, ma[na[i].p1].dy, ma[na[i].p1].dz);
		glVertex3f(ma[na[i].p1].x, ma[na[i].p1].y, ma[na[i].p1].z);
		glNormal3f(ma[na[i].p2].dx, ma[na[i].p2].dy, ma[na[i].p2].dz);
		glVertex3f(ma[na[i].p2].x, ma[na[i].p2].y, ma[na[i].p2].z);
		glNormal3f(ma[na[i].p3].dx, ma[na[i].p3].dy, ma[na[i].p3].dz);
		glVertex3f(ma[na[i].p3].x, ma[na[i].p3].y, ma[na[i].p3].z);
	}
	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	// 把视景体截取的图像按照怎样的高和宽显示到屏幕上
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(left, right, bottom, top, near, far)， left表示视景体左面的坐标，right表示右面的坐标，bottom表示下面的，top表示上面的。这个函数简单理解起来，就是一个物体摆在那里，你怎么去截取他
	glOrtho(min_x*make_s_1, max_x*make_s_1, min_y*make_s_1, max_y*make_s_1, min_z*make_s_1, max_z*make_s_1);
}

void keyBoard(unsigned char key, int x, int y) {
	int mod;
	switch (key)
	{
	case 'q':
	case 'Q':
		xrot = 0.0f;
		yrot = -10.0f;
		zrot = 0.0f;
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		xrot = 0.0f;
		yrot = 10.0f;
		zrot = 0.0f;
		glutPostRedisplay();
		break;
	case 'a':
	case 'A':
		xrot = -10.0f;
		yrot = 0.0f;
		zrot = 0.0f;
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':
		xrot = 10.0f;
		yrot = 0.0f;
		zrot = 0.0f;
		glutPostRedisplay();
		break;
	case 'w':
	case 'W':
		xrot = 0.0f;
		yrot = 0.0f;
		zrot = -10.0f;
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		xrot = 0.0f;
		yrot = 0.0f;
		zrot = 10.0f;
		glutPostRedisplay();
		break;
	case 'r':
	{
		mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_ALT) {
			xrot = 0.0f;
			yrot = 0.0f;
			zrot = 10.0f;
			glutPostRedisplay();
		}
		break;
	}
	case 'o':
	{
		mod = glutGetModifiers();
		if (mod == (GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT)) {
			xrot = 0.0f;
			yrot = 0.0f;
			zrot = 10.0f;
			glutPostRedisplay();
		}
		break;
	}
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	specialkey = glutGetModifiers();
	if ((state == GLUT_DOWN) && (specialkey == GLUT_ACTIVE_ALT)) {
		if (button == GLUT_LEFT_BUTTON) {
			xrot = 0.0f;
			yrot = 0.0f;
			zrot = 10.0f;
			glutPostRedisplay();
		}
	}
}

void createGLUTMenus() {
	int menu, submenu;
	submenu = glutCreateMenu(menusEvents);
	glutAddMenuEntry("smaller", SMALL);
	glutAddMenuEntry("larger", LARGE);

	menu = glutCreateMenu(menusEvents);
	glutAddMenuEntry("rotate_left", LEFT);
	glutAddMenuEntry("rotate_right", RIGHT);
	glutAddSubMenu("S_L", submenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menusEvents(int options) {
	switch (options)
	{
	case LEFT:
		xrot = 0.0f;
		yrot = -45.0f;
		zrot = 0.0f;
		glutPostRedisplay();
		break;
	case RIGHT:
		xrot = 0.0f;
		yrot = 45.0f;
		zrot = 0.0f;
		glutPostRedisplay();
		break;
	case LARGE:
		if (make_s_1 >= 1.6) make_s_1 = make_s_1 - 0.1;
		else cout << "已经是可以缩放的最大形态了" << endl;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(min_x*make_s_1, max_x*make_s_1, min_y*make_s_1, max_y*make_s_1, min_z*make_s_1, max_z*make_s_1);
		glutPostRedisplay();
		break;
	case SMALL:
		if (make_s_1 <= 2.4) make_s_1 = make_s_1 + 0.1;
		else cout << "已经是可以缩放的最小形态了" << endl;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(min_x*make_s_1, max_x*make_s_1, min_y*make_s_1, max_y*make_s_1, min_z*make_s_1, max_z*make_s_1);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void init() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);	
	glEnable(GL_LIGHT1);					
	
	int i;
	ifstream sin("bunny.vt");
	sin >> m >> n;
	for (i = 0; i < m; i++) {
		sin >> ma[i].x >> ma[i].y >> ma[i].z;
		ma[i].dx = 0;
		ma[i].dy = 0;
		ma[i].dz = 0;
		ma[i].num = 0;
		if (ma[i].x < min_x) min_x = ma[i].x;
		if (ma[i].x > max_x) max_x = ma[i].x;
		if (ma[i].y < min_y) min_y = ma[i].y;
		if (ma[i].y > max_y) max_y = ma[i].y;
		if (ma[i].z < min_z) min_z = ma[i].z;
		if (ma[i].z > max_z) max_z = ma[i].z;
	}
	for (i = 1; i <= n; i++) {
		sin >> na[i].p1 >> na[i].p2 >> na[i].p3;
		GLfloat a1, b1, c1, a2, b2, c2;
		a1 = ma[na[i].p2].x - ma[na[i].p1].x;
		b1 = ma[na[i].p2].y - ma[na[i].p1].y;
		c1 = ma[na[i].p2].z - ma[na[i].p1].z;
		a2 = ma[na[i].p2].x - ma[na[i].p3].x;
		b2 = ma[na[i].p2].y - ma[na[i].p3].y;
		c2 = ma[na[i].p2].z - ma[na[i].p3].z;
		na[i].dx = b1*c2 - b2*c1;
		na[i].dy = c1*a2 - a1*c2;
		na[i].dz = a1*b2 - a2*b1;
		ma[na[i].p1].num++;
		ma[na[i].p1].dx += na[i].dx;
		ma[na[i].p1].dy += na[i].dy;
		ma[na[i].p1].dz += na[i].dz;
		ma[na[i].p2].num++;
		ma[na[i].p2].dx += na[i].dx;
		ma[na[i].p2].dy += na[i].dy;
		ma[na[i].p2].dz += na[i].dz;
		ma[na[i].p3].num++;
		ma[na[i].p3].dx += na[i].dx;
		ma[na[i].p3].dy += na[i].dy;
		ma[na[i].p3].dz += na[i].dz;
	}
	for (i = 0; i<m; i++) {
		ma[i].dx = ma[i].dx / ma[i].num;
		ma[i].dy = ma[i].dy / ma[i].num;
		ma[i].dz = ma[i].dz / ma[i].num;
		GLfloat d = sqrt(ma[i].dx*ma[i].dx + ma[i].dy*ma[i].dy + ma[i].dz*ma[i].dz);
		ma[i].dx = ma[i].dx / d;
		ma[i].dy = ma[i].dy / d;
		ma[i].dz = ma[i].dz / d;
	}
}