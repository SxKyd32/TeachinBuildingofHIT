// beautiful-HZAU.cpp: �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <cstdlib>
#include <GL/glut.h> 
#include "math3d.h"  //Ϊʵ��ƽ��ͶӰ�ķ�������Ӱ�������ͷ�ļ�
#include <iostream>
#include <GL/gl.h>
using namespace std;
#pragma warning(disable:4996) 

//�����������������
GLuint texGround;
GLuint texTop;
GLuint texDown;

// �任����Ͷ����Ӱ������ƽ��ͶӰ����Ӱ��
M3DMatrix44f shadowMat;

GLfloat w = 700;
GLfloat h = 700;

//��ת�ĽǶ�
double y = -20.0;

//������ɫ���
int p = 0;

double cub[8][3];             // ���峤����İ˸�����
double tri[6][3];              // ��������������������

#define _USE_MATH_DEFINES 
#define BMP_Header_Length 54L  //����ͼ���������ڴ���е�ƫ����,�����������

void setTri(double x, double y, double z, double x1, double y1, double z1) {  //Ϊ������Ķ������鸳ֵ
	tri[0][0] = x;
	tri[0][1] = y;
	tri[0][2] = z;     //  ��1

	tri[1][0] = x;
	tri[1][1] = y;
	tri[1][2] = z + z1;   //  ��2

	tri[5][0] = x;
	tri[5][1] = y + y1;
	tri[5][2] = z + z1 / 2;   //  ��3

	for (int i = 0; i < 3; i++) {  //�����������������������
		if (i == 0) {
			tri[3][i] = tri[0][i] + x1;
			tri[2][i] = tri[1][i] + x1;
			tri[4][i] = tri[5][i] + x1;
		}
		else {
			tri[3][i] = tri[0][i];
			tri[2][i] = tri[1][i];
			tri[4][i] = tri[5][i];
		}
	}
}
void setCubiod(double x, double y, double z, double x1, double y1, double z1) {  //Ϊ������Ķ������鸳ֵ
	cub[0][0] = x;
	cub[0][1] = y;
	cub[0][2] = z;        //  ��1

	cub[1][0] = x;
	cub[1][1] = y;
	cub[1][2] = z + z1;     //��2

	cub[2][0] = x + x1;
	cub[2][1] = y;
	cub[2][2] = z + z1;     //  ��3

	cub[3][0] = x + x1;
	cub[3][1] = y;
	cub[3][2] = z;        //  ��4
	for (int i = 0; i < 4; i++) {  // ���µ���Ļ���������������
		for (int j = 0; j < 3; j++) {
			if (j == 1)
				cub[i + 4][j] = cub[i][j] + y1;
			else
				cub[i + 4][j] = cub[i][j];
		}
	}
}
void drawTri() {  //��������������Ϊ��¥����
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);  //Ϊ����0���÷�����
	glVertex3f(tri[0][0], tri[0][1], tri[0][2]);
	glVertex3f(tri[1][0], tri[1][1], tri[1][2]);
	glVertex3f(tri[2][0], tri[2][1], tri[2][2]);
	glVertex3f(tri[3][0], tri[3][1], tri[3][2]);
	glEnd();   //     �µ�

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);  //Ϊ����1���÷�����
	glVertex3f(tri[1][0], tri[1][1], tri[1][2]);
	glVertex3f(tri[0][0], tri[0][1], tri[0][2]);
	glVertex3f(tri[5][0], tri[5][1], tri[5][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);  //Ϊ����7���÷�����
	glVertex3f(tri[4][0], tri[4][1], tri[4][2]);
	glVertex3f(tri[3][0], tri[3][1], tri[3][2]);
	glVertex3f(tri[2][0], tri[2][1], tri[2][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);  //Ϊ����5���÷�����
	glVertex3f(tri[5][0], tri[5][1], tri[5][2]);
	glVertex3f(tri[4][0], tri[4][1], tri[4][2]);
	glVertex3f(tri[2][0], tri[2][1], tri[2][2]);
	glVertex3f(tri[1][0], tri[1][1], tri[1][2]);
	glEnd();   //     ǰ��

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(tri[0][0], tri[0][1], tri[0][2]);
	glVertex3f(tri[3][0], tri[3][1], tri[3][2]);
	glVertex3f(tri[4][0], tri[4][1], tri[4][2]);
	glVertex3f(tri[5][0], tri[5][1], tri[5][2]);
	glEnd();   //     ����

}

void drawCuboid() {  //���Ƴ�����
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(cub[0][0], cub[0][1], cub[0][2]);
	glVertex3f(cub[1][0], cub[1][1], cub[1][2]);
	glVertex3f(cub[2][0], cub[2][1], cub[2][2]);
	glVertex3f(cub[3][0], cub[3][1], cub[3][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(cub[1][0], cub[1][1], cub[1][2]);
	glVertex3f(cub[0][0], cub[0][1], cub[0][2]);
	glVertex3f(cub[4][0], cub[4][1], cub[4][2]);
	glVertex3f(cub[5][0], cub[5][1], cub[5][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(cub[7][0], cub[7][1], cub[7][2]);
	glVertex3f(cub[6][0], cub[6][1], cub[6][2]);
	glVertex3f(cub[2][0], cub[2][1], cub[2][2]);
	glVertex3f(cub[3][0], cub[3][1], cub[3][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(cub[5][0], cub[5][1], cub[5][2]);
	glVertex3f(cub[6][0], cub[6][1], cub[6][2]);
	glVertex3f(cub[2][0], cub[2][1], cub[2][2]);
	glVertex3f(cub[1][0], cub[1][1], cub[1][2]);
	glEnd();   //     ǰ��

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(cub[0][0], cub[0][1], cub[0][2]);
	glVertex3f(cub[3][0], cub[3][1], cub[3][2]);
	glVertex3f(cub[7][0], cub[7][1], cub[7][2]);
	glVertex3f(cub[4][0], cub[4][1], cub[4][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(cub[4][0], cub[4][1], cub[4][2]);
	glVertex3f(cub[7][0], cub[7][1], cub[7][2]);
	glVertex3f(cub[6][0], cub[6][1], cub[6][2]);
	glVertex3f(cub[5][0], cub[5][1], cub[5][2]);
	glEnd();   //     ����
}

void setFloor() {
	//��ӵ�������
	glEnable(GL_TEXTURE_2D);  //��������
	glBindTexture(GL_TEXTURE_2D, texGround);  //������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50, 11, 500);  //������������������
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50, 11, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(550, 11, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(550, 11, 500);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void setXiaohui() {
	//����У������
	glEnable(GL_TEXTURE_2D);  //��������
	glBindTexture(GL_TEXTURE_2D, texTop);  //������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(240, 371, 470);  //������������������
	glTexCoord2f(0.0f, 1.0f); glVertex3f(240, 371, 250);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(450, 371, 250);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(450, 371, 470);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void setXiaoxun() {
	//���Уѵ����
	glEnable(GL_TEXTURE_2D);  //��������
	glBindTexture(GL_TEXTURE_2D, texDown);  //������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50, 11, 600);  //������������������
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50, 11, 500);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(550, 11, 500);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(550, 11, 600);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawBoard(int shadow) {  //����ָʾ��
	if (shadow == 0) {  //����ѡ�������ɫ
		glColor3f(0.823, 0.412, 0.118);
	}
	else {
		glColor3f(0.0, 0.0, 0.0);
	}
	//����ָʾ��
	setCubiod(60, 10, 350, 15, 100, 15);
	drawCuboid();
	setCubiod(65, 50, 366, 5, 50, 60);
	drawCuboid();
	setCubiod(60, 10, 427, 15, 100, 15);
	drawCuboid();
}

// ��ʼ����Ⱦ��ΪӰ������׼��
void setShadow()
{
	float direction[] = { 100.0, 100.0, 100.0 };  //�ѵ��Դ��λ�ø���һ���Ա�ʹ��
	// ��Ϊ������������
	M3DVector3f points[3] = { { 1160.0f, 12.0f, 390.0f },
	{ 70, 12, 390 },
	{ 70, 12, 400 } };
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// ������ɫ����
	glEnable(GL_COLOR_MATERIAL);

	// ������ɫֵ���ò�������
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// ���屣��ƽ�淽�̲����ľ���
	M3DVector4f vPlaneEquation;
	// ����ƽ�淽��
	m3dGetPlaneEquation(vPlaneEquation, points[0], points[1], points[2]);
	// ����ͶӰ�ڵ����Ӱ��
	m3dMakePlanarShadowMatrix(shadowMat, vPlaneEquation, direction);
	// �������޳�
	glEnable(GL_NORMALIZE);
}

void drawMain(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -300, 0);  //Ϊ��ԭ���������ƶ�һ��
	glRotatef(20, 1.0, 0.0, 0.0);  //ʵ����תЧ��
	glRotatef(y, 0.0, 1.0, 0.0);

	glTranslatef(-370, 0, -370);  //б���ƶ������ᣬʹ�û���λ������
	glScalef(1.5, 1.5, 1.5);

	//���ƻ�������
	glBegin(GL_POLYGON);
	setCubiod(-50, 0, 0, 600, 10, 500);
	glColor3f(0, 0, 0);
	drawCuboid();  //���Ƴ�������Ϊ����

	setFloor();

	//����Уѵ������
	glBegin(GL_POLYGON);
	setCubiod(-50, 0, 500, 600, 10, 100);
	glColor3f(0, 0, 0);
	drawCuboid();

	setXiaoxun();

	//���ƴ�¥ǰһ����
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(240, 10, 250, 12, 350, 220);  //��
	drawCuboid();
	setCubiod(438, 10, 250, 12, 350, 220);  //��
	drawCuboid();
	setCubiod(250, 10, 450, 20, 350, 20);  //ǰ1
	drawCuboid();
	setCubiod(320, 10, 450, 40, 350, 20);  //ǰ2
	drawCuboid();
	setCubiod(410, 10, 450, 30, 350, 20);  //ǰ3
	drawCuboid();
	setCubiod(270, 10, 450, 140, 40, 20);  //ǰ4
	drawCuboid();
	setCubiod(270, 100, 450, 140, 40, 20);  //ǰ5
	drawCuboid();
	setCubiod(270, 190, 450, 140, 40, 20);  //ǰ6
	drawCuboid();
	setCubiod(270, 280, 450, 140, 30, 20);  //ǰ7
	drawCuboid();

	//���ƴ�¥ǰһ���ֵ�¥��
	glColor3f(0.1, 0.2, 0.2);
	setCubiod(240, 360, 250, 210, 10, 220);
	drawCuboid();

	setXiaohui();

	//���ƴ�¥��һ����
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(50, 10, 30, 12, 350, 220);
	drawCuboid();     //��ǽ��
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(438, 10, 30, 12, 350, 220);
	drawCuboid();     //��ǽ��
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(62, 10, 30, 376, 350, 12);
	drawCuboid();     //��ǽ��
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(62, 10, 235, 188, 50, 15);
	drawCuboid();     //ǰǽ��1
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(62, 60, 235, 50, 50, 15);
	drawCuboid();     //ǰǽ��2
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(162, 60, 235, 88, 50, 15);
	drawCuboid();     //ǰǽ��3
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(62, 110, 235, 188, 250, 15);
	drawCuboid();     //ǰǽ��4

	//���ƴ�¥��һ���ֵĴ���
	glColor4f(0.0, 0.0, 1.0, 0.25);
	setCubiod(112, 60, 235, 50, 50, 12);
	drawCuboid();
	//���ƴ�¥ǰһ���ֵĴ���
	glColor4f(0.0, 0.0, 1.0, 0.15);
	setCubiod(270, 50, 455, 50, 50, 10);
	drawCuboid();
	glColor4f(0.0, 0.0, 1.0, 0.15);
	setCubiod(360, 50, 455, 50, 50, 10);
	drawCuboid();
	glColor4f(0.0, 0.0, 1.0, 0.15);
	setCubiod(270, 140, 455, 50, 50, 10);
	drawCuboid();
	glColor4f(0.0, 0.0, 1.0, 0.15);
	setCubiod(360, 140, 455, 50, 50, 10);
	drawCuboid();
	glColor4f(0.0, 0.0, 1.0, 0.15);
	setCubiod(270, 230, 455, 50, 50, 10);
	drawCuboid();
	glColor4f(0.0, 0.0, 1.0, 0.15);
	setCubiod(360, 230, 455, 50, 50, 10);
	drawCuboid();
	glColor4f(0.0, 0.0, 1.0, 0.15);
	setCubiod(270, 310, 455, 50, 50, 10);
	drawCuboid();
	glColor4f(0.0, 0.0, 1.0, 0.15);
	setCubiod(360, 310, 455, 50, 50, 10);
	drawCuboid();

	//���ƴ�¥¥��
	glColor3f(0.2, 0.3, 0.3);
	setTri(50, 360, 30, 400, 70, 220);
	drawTri();

	//����ָʾ��
	drawBoard(0);
	//����ƽ��ͶӰ������ָʾ�Ƶ�Ӱ��Ч��
	glPushMatrix();
	// ��ͶӰ�������
	glMultMatrixf((GLfloat*)shadowMat);
	drawBoard(1);
	glPopMatrix();

	glFlush();  // ˢ��
	glutSwapBuffers();//��Ҫ����swapbuffer����
}

void myMouse(int button, int state, int xx, int yy) {	//��꽻��
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {		//������ʹ����˳ʱ��ת��10��
			y -= 10;
			glutPostRedisplay();
		}
		else if (button == GLUT_RIGHT_BUTTON) {		//����Ҽ��ı䱳����ɫ
			if (p == 0)
			{
				p = 1;
				glClearColor(0.6, 0.3, 0.3, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);
				glFlush();
			}
			else
			{
				p = 0;
				glClearColor(0.4, 0.6, 0.6, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);
				glFlush();
			}
		}
	}
	return;
}

void setLight(void)  //���ù�Դ
{
	float direction[] = { 100.0, 100.0, 100.0, 0.0 };  //���Դ��λ��
	float intensity[] = { 1.0, 1.0, 1.0, 1.0 };  //������ǿ��
	float ambient[] = { 0.5, 0.5, 0.5, 1.0 };  //������ǿ��

	glEnable(GL_LIGHTING);  //��������

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);  //���ù���ģ�ͣ�����ȫ�ֻ������RGBAǿ��

	glEnable(GL_LIGHT0);  //��������0
	glLightfv(GL_LIGHT0, GL_POSITION, direction);  //���ù���0��λ��
	glLightfv(GL_LIGHT0, GL_DIFFUSE, intensity);  //���ù�Դ�Ĺ��������ǿ��

	glEnable(GL_COLOR_MATERIAL);        // ������������
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLineWidth(5);  //�����߶ο��
	glClearColor(0.4, 0.6, 0.6, 0.0);      //���屳����ɫ

	glEnable(GL_BLEND); // �򿪻��
	glEnable(GL_DEPTH_TEST); // ����Ȳ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // ����Դ����alphaͨ��ֵ�İ�͸����Ϻ���

	glMatrixMode(GL_PROJECTION);     // �ӽ�
	glOrtho(-w, w, -w, w, -w, w); // 
}

//�������ר��
//�ж�һ�������ǲ���2����������
int isTwo(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

//��ȡ����
GLuint loadTexture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;
	FILE* pFile;
	errno_t err;

	if ((err = fopen_s(&pFile, file_name, "rb")) != 0)
	{
		printf("%d", err);
		printf("�޷��򿪴��ļ�\n");            //����򲻿���������򲻿�
		exit(0);                               //��ֹ����
	}

	// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// �����������ֽ��������ڴ�
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// ��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// �Ծɰ汾�ļ��ݣ����ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
	// ��ͼ���߳�����OpenGL�涨�����ֵ��Ҳ����
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!isTwo(width)
			|| !isTwo(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// ����ÿ����Ҫ���ֽ��������ֽ���
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// �����ڴ�
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// ������������
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// ����һ���µ�������
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// ���µ������������������������
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������
	free(pixels);
	return texture_ID;
}




int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("�������ѧ¥");
	setLight();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);    // ��������

	texGround = loadTexture("Green.bmp");  //���ص�������
	texTop = loadTexture("xiaohui.bmp");   //����У������
	texDown = loadTexture("xiaoxun.bmp");  //����Уѵ����

	glutDisplayFunc(drawMain);
	setShadow();
	glutMouseFunc(myMouse);
	glutMainLoop();
	system("pause");
	return 0;
}

