// beautiful-HZAU.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cstdlib>
#include <GL/glut.h> 
#include "math3d.h"  //为实现平面投影的方法产生影子引入的头文件
#include <iostream>
#include <GL/gl.h>
using namespace std;
#pragma warning(disable:4996) 

//定义三个纹理对象编号
GLuint texGround;
GLuint texTop;
GLuint texDown;

// 变换矩阵投射阴影，用于平面投影产生影子
M3DMatrix44f shadowMat;

GLfloat w = 700;
GLfloat h = 700;

//旋转的角度
double y = -20.0;

//背景颜色标记
int p = 0;

double cub[8][3];             // 定义长方体的八个顶点
double tri[6][3];              // 定义三棱柱的六个顶点

#define _USE_MATH_DEFINES 
#define BMP_Header_Length 54L  //保存图像数据在内存块中的偏移量,方便加载纹理

void setTri(double x, double y, double z, double x1, double y1, double z1) {  //为梯形体的顶点数组赋值
	tri[0][0] = x;
	tri[0][1] = y;
	tri[0][2] = z;     //  左1

	tri[1][0] = x;
	tri[1][1] = y;
	tri[1][2] = z + z1;   //  左2

	tri[5][0] = x;
	tri[5][1] = y + y1;
	tri[5][2] = z + z1 / 2;   //  左3

	for (int i = 0; i < 3; i++) {  //在左侧面测基础上生成其他面
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
void setCubiod(double x, double y, double z, double x1, double y1, double z1) {  //为长方体的顶点数组赋值
	cub[0][0] = x;
	cub[0][1] = y;
	cub[0][2] = z;        //  下1

	cub[1][0] = x;
	cub[1][1] = y;
	cub[1][2] = z + z1;     //下2

	cub[2][0] = x + x1;
	cub[2][1] = y;
	cub[2][2] = z + z1;     //  下3

	cub[3][0] = x + x1;
	cub[3][1] = y;
	cub[3][2] = z;        //  下4
	for (int i = 0; i < 4; i++) {  // 在下底面的基础上生成其他面
		for (int j = 0; j < 3; j++) {
			if (j == 1)
				cub[i + 4][j] = cub[i][j] + y1;
			else
				cub[i + 4][j] = cub[i][j];
		}
	}
}
void drawTri() {  //绘制三棱柱，作为大楼顶部
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);  //为顶点0设置法向量
	glVertex3f(tri[0][0], tri[0][1], tri[0][2]);
	glVertex3f(tri[1][0], tri[1][1], tri[1][2]);
	glVertex3f(tri[2][0], tri[2][1], tri[2][2]);
	glVertex3f(tri[3][0], tri[3][1], tri[3][2]);
	glEnd();   //     下底

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);  //为顶点1设置法向量
	glVertex3f(tri[1][0], tri[1][1], tri[1][2]);
	glVertex3f(tri[0][0], tri[0][1], tri[0][2]);
	glVertex3f(tri[5][0], tri[5][1], tri[5][2]);
	glEnd();   //     左面

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);  //为顶点7设置法向量
	glVertex3f(tri[4][0], tri[4][1], tri[4][2]);
	glVertex3f(tri[3][0], tri[3][1], tri[3][2]);
	glVertex3f(tri[2][0], tri[2][1], tri[2][2]);
	glEnd();   //     右面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);  //为顶点5设置法向量
	glVertex3f(tri[5][0], tri[5][1], tri[5][2]);
	glVertex3f(tri[4][0], tri[4][1], tri[4][2]);
	glVertex3f(tri[2][0], tri[2][1], tri[2][2]);
	glVertex3f(tri[1][0], tri[1][1], tri[1][2]);
	glEnd();   //     前面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(tri[0][0], tri[0][1], tri[0][2]);
	glVertex3f(tri[3][0], tri[3][1], tri[3][2]);
	glVertex3f(tri[4][0], tri[4][1], tri[4][2]);
	glVertex3f(tri[5][0], tri[5][1], tri[5][2]);
	glEnd();   //     后面

}

void drawCuboid() {  //绘制长方体
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(cub[0][0], cub[0][1], cub[0][2]);
	glVertex3f(cub[1][0], cub[1][1], cub[1][2]);
	glVertex3f(cub[2][0], cub[2][1], cub[2][2]);
	glVertex3f(cub[3][0], cub[3][1], cub[3][2]);
	glEnd();   //     下面

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(cub[1][0], cub[1][1], cub[1][2]);
	glVertex3f(cub[0][0], cub[0][1], cub[0][2]);
	glVertex3f(cub[4][0], cub[4][1], cub[4][2]);
	glVertex3f(cub[5][0], cub[5][1], cub[5][2]);
	glEnd();   //     左面

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(cub[7][0], cub[7][1], cub[7][2]);
	glVertex3f(cub[6][0], cub[6][1], cub[6][2]);
	glVertex3f(cub[2][0], cub[2][1], cub[2][2]);
	glVertex3f(cub[3][0], cub[3][1], cub[3][2]);
	glEnd();   //     右面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(cub[5][0], cub[5][1], cub[5][2]);
	glVertex3f(cub[6][0], cub[6][1], cub[6][2]);
	glVertex3f(cub[2][0], cub[2][1], cub[2][2]);
	glVertex3f(cub[1][0], cub[1][1], cub[1][2]);
	glEnd();   //     前面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(cub[0][0], cub[0][1], cub[0][2]);
	glVertex3f(cub[3][0], cub[3][1], cub[3][2]);
	glVertex3f(cub[7][0], cub[7][1], cub[7][2]);
	glVertex3f(cub[4][0], cub[4][1], cub[4][2]);
	glEnd();   //     后面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(cub[4][0], cub[4][1], cub[4][2]);
	glVertex3f(cub[7][0], cub[7][1], cub[7][2]);
	glVertex3f(cub[6][0], cub[6][1], cub[6][2]);
	glVertex3f(cub[5][0], cub[5][1], cub[5][2]);
	glEnd();   //     上面
}

void setFloor() {
	//添加地面纹理
	glEnable(GL_TEXTURE_2D);  //开启纹理
	glBindTexture(GL_TEXTURE_2D, texGround);  //绑定纹理
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50, 11, 500);  //纹理坐标与绘制坐标绑定
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50, 11, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(550, 11, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(550, 11, 500);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void setXiaohui() {
	//增加校徽纹理
	glEnable(GL_TEXTURE_2D);  //开启纹理
	glBindTexture(GL_TEXTURE_2D, texTop);  //绑定纹理
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(240, 371, 470);  //纹理坐标与绘制坐标绑定
	glTexCoord2f(0.0f, 1.0f); glVertex3f(240, 371, 250);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(450, 371, 250);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(450, 371, 470);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void setXiaoxun() {
	//添加校训纹理
	glEnable(GL_TEXTURE_2D);  //开启纹理
	glBindTexture(GL_TEXTURE_2D, texDown);  //绑定纹理
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50, 11, 600);  //纹理坐标与绘制坐标绑定
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50, 11, 500);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(550, 11, 500);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(550, 11, 600);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawBoard(int shadow) {  //绘制指示牌
	if (shadow == 0) {  //根据选择决定颜色
		glColor3f(0.823, 0.412, 0.118);
	}
	else {
		glColor3f(0.0, 0.0, 0.0);
	}
	//绘制指示牌
	setCubiod(60, 10, 350, 15, 100, 15);
	drawCuboid();
	setCubiod(65, 50, 366, 5, 50, 60);
	drawCuboid();
	setCubiod(60, 10, 427, 15, 100, 15);
	drawCuboid();
}

// 初始化渲染，为影子做好准备
void setShadow()
{
	float direction[] = { 100.0, 100.0, 100.0 };  //把点光源的位置复制一下以便使用
	// 作为地面计算的三点
	M3DVector3f points[3] = { { 1160.0f, 12.0f, 390.0f },
	{ 70, 12, 390 },
	{ 70, 12, 400 } };
	//启用深度测试
	glEnable(GL_DEPTH_TEST);

	// 启用颜色跟踪
	glEnable(GL_COLOR_MATERIAL);

	// 按照颜色值设置材质属性
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// 定义保存平面方程参数的矩阵
	M3DVector4f vPlaneEquation;
	// 计算平面方程
	m3dGetPlaneEquation(vPlaneEquation, points[0], points[1], points[2]);
	// 计算投影在地面的影子
	m3dMakePlanarShadowMatrix(shadowMat, vPlaneEquation, direction);
	// 启用面剔除
	glEnable(GL_NORMALIZE);
}

void drawMain(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -300, 0);  //为了原点先向下移动一点
	glRotatef(20, 1.0, 0.0, 0.0);  //实现旋转效果
	glRotatef(y, 0.0, 1.0, 0.0);

	glTranslatef(-370, 0, -370);  //斜向移动坐标轴，使得画面位于正中
	glScalef(1.5, 1.5, 1.5);

	//绘制基础方块
	glBegin(GL_POLYGON);
	setCubiod(-50, 0, 0, 600, 10, 500);
	glColor3f(0, 0, 0);
	drawCuboid();  //绘制长方体作为地面

	setFloor();

	//绘制校训衬托区
	glBegin(GL_POLYGON);
	setCubiod(-50, 0, 500, 600, 10, 100);
	glColor3f(0, 0, 0);
	drawCuboid();

	setXiaoxun();

	//绘制大楼前一部分
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(240, 10, 250, 12, 350, 220);  //左
	drawCuboid();
	setCubiod(438, 10, 250, 12, 350, 220);  //右
	drawCuboid();
	setCubiod(250, 10, 450, 20, 350, 20);  //前1
	drawCuboid();
	setCubiod(320, 10, 450, 40, 350, 20);  //前2
	drawCuboid();
	setCubiod(410, 10, 450, 30, 350, 20);  //前3
	drawCuboid();
	setCubiod(270, 10, 450, 140, 40, 20);  //前4
	drawCuboid();
	setCubiod(270, 100, 450, 140, 40, 20);  //前5
	drawCuboid();
	setCubiod(270, 190, 450, 140, 40, 20);  //前6
	drawCuboid();
	setCubiod(270, 280, 450, 140, 30, 20);  //前7
	drawCuboid();

	//绘制大楼前一部分的楼顶
	glColor3f(0.1, 0.2, 0.2);
	setCubiod(240, 360, 250, 210, 10, 220);
	drawCuboid();

	setXiaohui();

	//绘制大楼后一部分
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(50, 10, 30, 12, 350, 220);
	drawCuboid();     //左墙壁
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(438, 10, 30, 12, 350, 220);
	drawCuboid();     //右墙壁
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(62, 10, 30, 376, 350, 12);
	drawCuboid();     //后墙壁
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(62, 10, 235, 188, 50, 15);
	drawCuboid();     //前墙壁1
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(62, 60, 235, 50, 50, 15);
	drawCuboid();     //前墙壁2
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(162, 60, 235, 88, 50, 15);
	drawCuboid();     //前墙壁3
	glColor3f(1.0, 1.0, 1.0);
	setCubiod(62, 110, 235, 188, 250, 15);
	drawCuboid();     //前墙壁4

	//绘制大楼后一部分的窗户
	glColor4f(0.0, 0.0, 1.0, 0.25);
	setCubiod(112, 60, 235, 50, 50, 12);
	drawCuboid();
	//绘制大楼前一部分的窗户
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

	//绘制大楼楼顶
	glColor3f(0.2, 0.3, 0.3);
	setTri(50, 360, 30, 400, 70, 220);
	drawTri();

	//绘制指示牌
	drawBoard(0);
	//采用平面投影法产生指示牌的影子效果
	glPushMatrix();
	// 与投影矩阵相乘
	glMultMatrixf((GLfloat*)shadowMat);
	drawBoard(1);
	glPopMatrix();

	glFlush();  // 刷新
	glutSwapBuffers();//还要加上swapbuffer函数
}

void myMouse(int button, int state, int xx, int yy) {	//鼠标交互
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {		//鼠标左键使场景顺时针转动10度
			y -= 10;
			glutPostRedisplay();
		}
		else if (button == GLUT_RIGHT_BUTTON) {		//鼠标右键改变背景颜色
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

void setLight(void)  //设置光源
{
	float direction[] = { 100.0, 100.0, 100.0, 0.0 };  //点光源的位置
	float intensity[] = { 1.0, 1.0, 1.0, 1.0 };  //漫反射强度
	float ambient[] = { 0.5, 0.5, 0.5, 1.0 };  //环境光强度

	glEnable(GL_LIGHTING);  //开启光照

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);  //设置光照模型，设置全局环境光的RGBA强度

	glEnable(GL_LIGHT0);  //开启光照0
	glLightfv(GL_LIGHT0, GL_POSITION, direction);  //设置光照0的位置
	glLightfv(GL_LIGHT0, GL_DIFFUSE, intensity);  //设置光源的光的漫反射强度

	glEnable(GL_COLOR_MATERIAL);        // 开启材质设置
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLineWidth(5);  //设置线段宽度
	glClearColor(0.4, 0.6, 0.6, 0.0);      //定义背景颜色

	glEnable(GL_BLEND); // 打开混合
	glEnable(GL_DEPTH_TEST); // 打开深度测试
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // 基于源象素alpha通道值的半透明混合函数

	glMatrixMode(GL_PROJECTION);     // 视角
	glOrtho(-w, w, -w, w, -w, w); // 
}

//添加纹理专用
//判断一个整数是不是2的整数次幂
int isTwo(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

//读取纹理
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
		printf("无法打开此文件\n");            //如果打不开，就输出打不开
		exit(0);                               //终止程序
	}

	// 读取文件中图像的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// 读取像素数据
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 对旧版本的兼容，如果图像的宽度和高度不是的整数次方，则需要进行缩放
	// 若图像宽高超过了OpenGL规定的最大值，也缩放
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!isTwo(width)
			|| !isTwo(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// 计算每行需要的字节数和总字节数
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// 分配内存
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// 进行像素缩放
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// 分配一个新的纹理编号
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
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
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
	free(pixels);
	return texture_ID;
}




int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("哈工大教学楼");
	setLight();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);    // 启用纹理

	texGround = loadTexture("Green.bmp");  //加载地面纹理
	texTop = loadTexture("xiaohui.bmp");   //加载校徽纹理
	texDown = loadTexture("xiaoxun.bmp");  //加载校训纹理

	glutDisplayFunc(drawMain);
	setShadow();
	glutMouseFunc(myMouse);
	glutMainLoop();
	system("pause");
	return 0;
}

