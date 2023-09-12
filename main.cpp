// CPT205 Assessment 2 3D project
// Author: Shengkai Sun
// ID: 1928915

#define FREEGLUT_STATIC
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#define checkImageWidth 64
#define checkImageHeight 64

#include <GL/freeglut.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

// Angles of rotation
static GLfloat yRot = 0.0f;
static GLfloat xRot = 0.0f;

bool normal = true;

GLfloat X0 = 0.0f, Y0 = 30.0f, Z0 = 200.0f;
GLfloat zoom = 1.0f;
GLfloat xRef = 0.0f, yRef = 0.0f, zRef = 0.0f;
GLfloat xUp = 0.0f, yUp = 1.0f, zUp = 0.0f;

GLfloat xAt = xRef, yAt = yRef, zAt = zRef;
GLfloat eyeX = X0, eyeY = Y0, eyeZ = Z0;
GLfloat eyeXu = xUp, eyeYu = yUp, eyeZu = zUp;

GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLint imageWidth0, imageWidth1, imageWidth2, imageWidth3, imageWidth4, imageWidth5, imageWidth6, imageWidth7;
GLint imageHeight0, imageHeight1, imageHeight2, imageHeight3, imageHeight4, imageHeight5, imageHeight6, imageHeight7;
GLint pixelLength0, pixelLength1, pixelLength2, pixelLength3, pixelLength4, pixelLength5, pixelLength6, pixelLength7;
GLfloat windowHeight = 600, windowWidth = 800;
vector<GLubyte*> p; // Similar to GLubyte* for program 3 in Week 11 but for 2 images (so a vector)
GLuint texture[9];
GLfloat hourAngle = 0.0f;
GLfloat minAngle = 0.0f;
GLfloat b1Rot = 0.0f, b2Rot = 15.0f;
bool s1 = true, s2 = true;
// define a 64*64*3 plane for checkerboard
GLubyte checkImage[checkImageWidth][checkImageHeight][3];

// make a checkerboard
void makeCheckImage() {
	int i, j, c;
	for (i = 0; i < checkImageWidth; i++) {
		for (j = 0; j < checkImageHeight; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255; // bitwise AND and XOR operations,
			checkImage[i][j][0] = (GLubyte)c; // which are faster than normal calculations
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
		}
	}
}

void readImage(const char path[256], GLint& imageWidth, GLint& imageHeight, GLint& pixelLength) {
	GLubyte* pixelData;
	FILE* pFile;
	fopen_s(&pFile, path, "rb");
	if (pFile == nullptr)
		exit(0);
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&imageWidth, sizeof(imageWidth), 1, pFile);
	fread(&imageHeight, sizeof(imageHeight), 1, pFile);
	pixelLength = imageWidth * 3;
	while (pixelLength % 4 != 0)
		pixelLength++;
	pixelLength *= imageHeight;
	pixelData = (GLubyte*)malloc(pixelLength);
	if (pixelData == nullptr)
		exit(0);
	fseek(pFile, 54, SEEK_SET);
	fread(pixelData, pixelLength, 1, pFile);
	p.push_back(pixelData); // Similar to glDrawPixels for program 3 in Week 11
	fclose(pFile);
}

void myInit() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);
	readImage("marble.bmp", imageWidth0, imageHeight0, pixelLength0);
	readImage("floor.bmp", imageWidth1, imageHeight1, pixelLength1);
	readImage("carpet.bmp", imageWidth2, imageHeight2, pixelLength2);
	readImage("wall.bmp", imageWidth3, imageHeight3, pixelLength3);
	readImage("wall2.bmp", imageWidth4, imageHeight4, pixelLength4);
	readImage("xuenv.bmp", imageWidth5, imageHeight5, pixelLength5);
	readImage("qianji.bmp", imageWidth6, imageHeight6, pixelLength6);
	readImage("window.bmp", imageWidth7, imageHeight7, pixelLength7);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set pixel storage modes (in the memory)
	glGenTextures(9, &texture[0]); // number of texture names to be generated and
	// a pointer to the first element in the array of texture names
	glBindTexture(GL_TEXTURE_2D, texture[0]); // target to which texture is bound and
	// the name of the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth0, imageHeight0, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth1, imageHeight1, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth2, imageHeight2, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	makeCheckImage();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
		&checkImage[0][0][0]);
	// set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth3, imageHeight3, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[3]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth4, imageHeight4, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[4]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth5, imageHeight6, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[5]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth6, imageHeight6, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[6]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth7, imageHeight7, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[7]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void myIdleFunc() {
	if (s1 && s2) { // ball 2 fall
		b2Rot -= 0.1f;
		if (b2Rot <= 0.0f)
			s1 = false;  // ball 1 raise
	}
	else if (!s1 && s2) {
		b1Rot -= 0.1f;
		if (b1Rot <= -15.0f)
			s2 = false;
	}
	else if (!s1 && !s2) {
		b1Rot += 0.1f;
		if (b1Rot >= 0.0f)
			s1 = true;
	}
	else if (s1 & !s2) {
		b2Rot += 0.1f;
		if (b2Rot >= 15.0f)
			s2 = true;
	}

	hourAngle -= 0.2 / 5; // incremental angle per frame while 60 frames/second (so very fast)
	minAngle -= 0.2;
	glutPostRedisplay();
}

// Change the view volume and viewport. This is called when the window is resized.
void changeSize(int newWidth, int newHeight) {
	if (newHeight == 0)
		newHeight = 1;
	glViewport(0, 0, newWidth, newHeight); // Set viewport to window dimensions
}

// Setting up lighting and material parameters for enhanced rendering effect.
// This topic will be covered later on in the module so please skip this for now.
void lightingAndMaterials() {
	// Light parameters and coordinates
	GLfloat whiteLight[] = { 0.45f, 0.45f, 0.45f, 1.0f };
	GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat lightPos[] = { 100.0f, 100.0f, -100.0f, 0.0f };
	glEnable(GL_DEPTH_TEST); // Hidden surface removal
	glEnable(GL_LIGHTING); // Enable lighting
	// Setup and enable light0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; // Directional light
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

	glEnable(GL_COLOR_MATERIAL); // Enable colour tracking
	// Set material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
}

void drawFloor() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glTranslatef(0.0, -64.0, 0.0);
	glScalef(250.0, 1.0, 150.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, 1.01, 1.01);
	glTexCoord2f(0.0, 10.0);
	glVertex3f(-1.01, 1.01, -1.01);
	glTexCoord2f(5.0, 10.0);
	glVertex3f(1.01, 1.01, -1.01);
	glTexCoord2f(5.0, 0.0);
	glVertex3f(1.01, 1.01, 1.01);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

void drawCeil() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glTranslatef(0.0, 238.0, 0.0);
	glScalef(250.0, 1.0, 150.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, -1.01, 1.01);
	glTexCoord2f(0.0, 10.0);
	glVertex3f(-1.01, -1.01, -1.01);
	glTexCoord2f(5.0, 10.0);
	glVertex3f(1.01, -1.01, -1.01);
	glTexCoord2f(5.0, 0.0);
	glVertex3f(1.01, -1.01, 1.01);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

void drawWall(GLuint tex, GLfloat size) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glTranslatef(0.0f, 88.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(size, 150, 1.0);
	glutSolidCube(2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, -1.01, 1.01);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.01, 1.01, 1.01);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.01, 1.01, 1.01);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.01, -1.01, 1.01);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

// Draw the chess board
void drawChessBoard() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0.0f, 2.0f, 0.0f);
	glScalef(18.0f, 1.0f, 18.0f);
	glutSolidCube(2);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, 1.01, 1.01);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.01, 1.01, -1.01);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.01, 1.01, -1.01);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.01, 1.01, 1.01);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
}

// Draw Chess Pieces
void drawPieces(GLfloat color[3]) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor3f(color[0], color[1], color[2]);
	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);
	glPushMatrix();
	glTranslatef(0.0f, 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.8, 0.5, 30, 30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 2.0f, 0.0f);
	glRotatef(90, -1, 0, 0);
	glutSolidCone(0.6, 2, 30, 30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 4.0f, 0.0f);
	glutSolidSphere(0.4, 30, 30);
	glPopMatrix();
	glPopMatrix();
	glPopAttrib();
}

void drawTable() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// desk surface
	glPushMatrix(); // Save the matrix state and perform rotations
	glScalef(50, 2, 35);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(2);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Top
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, 1.01, 1.01);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.01, 1.01, -1.01);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.01, 1.01, -1.01);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.01, 1.01, 1.01);
	glEnd();

	// Bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, -1.01, 1.01);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.01, -1.01, -1.01);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.01, -1.01, -1.01);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.01, -1.01, 1.01);
	glEnd();

	// Front
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, -1.01, 1.01);
	glTexCoord2f(0.0, 0.5);
	glVertex3f(-1.01, 1.01, 1.01);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(1.01, 1.01, 1.01);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(1.01, -1.01, 1.01);
	glEnd();

	// Back
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, -1.01, -1.01);
	glTexCoord2f(0.0, 0.5);
	glVertex3f(-1.01, 1.01, -1.01);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(1.01, 1.01, -1.01);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(1.01, -1.01, -1.01);
	glEnd();

	// Left
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, -1.01, -1.01);
	glTexCoord2f(0.0, 0.5);
	glVertex3f(-1.01, 1.01, -1.01);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(-1.01, 1.01, 1.01);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(-1.01, -1.01, 1.01);
	glEnd();

	// Right
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.01, -1.01, -1.01);
	glTexCoord2f(0.0, 0.5);
	glVertex3f(1.01, 1.01, -1.01);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(1.01, 1.01, 1.01);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(1.01, -1.01, 1.01);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutWireCube(2.01);
	glPopMatrix();
	// leg-1
	glPushMatrix();
	glTranslatef(-43.0f, -22.0f, 28.0f);
	glScalef(2, 20, 2);

	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(2);

	glColor3f(0.7f, 0.7f, 0.7f);
	glutWireCube(2);
	glPopMatrix();
	// leg-2
	glPushMatrix();
	glTranslatef(43.0f, -22.0f, 28.0f);
	glScalef(2, 20, 2);

	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(2);

	glColor3f(0.7f, 0.7f, 0.7f);
	glutWireCube(2);
	glPopMatrix();
	// leg-3
	glPushMatrix();
	glTranslatef(43.0f, -22.0f, -28.0f);
	glScalef(2, 20, 2);

	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(2);

	glColor3f(0.7f, 0.7f, 0.7f);
	glutWireCube(2);
	glPopMatrix();
	// leg-4
	glPushMatrix();
	glTranslatef(-43.0f, -22.0f, -28.0f);
	glScalef(2, 20, 2);

	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(2);

	glColor3f(0.7f, 0.7f, 0.7f);
	glutWireCube(2);
	glPopMatrix();

	glPopAttrib();
}

void drawCarpet() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix(); // Save the matrix state and perform rotations
	glTranslatef(0.0f, -43.99f, 0.0f);
	glScalef(65, 1, 45);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCube(2);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, 1.01, 1.01);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.01, 1.01, -1.01);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.01, 1.01, -1.01);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.01, 1.01, 1.01);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

void drawTeaSet() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLfloat teaSetSpe[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, teaSetSpe);
	glColor3f(1.0f, 1.0f, 1.0f);
	// Teapot
	glPushMatrix();
	glTranslatef(40.0f, 6.0f, 10.0f);
	glRotatef(135, 0, 1, 0);
	glutSolidTeapot(5);
	// Teacups
	glPushMatrix();
	glTranslatef(0.0f, -2.0f, -15.0f);
	glRotatef(135, 0, -1, 0);
	glutSolidTeacup(5);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -15.0f);
	glutSolidTeacup(5);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopAttrib();
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
}

void drawClock() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// Clock body
	glutSolidCylinder(10, 1, 60, 60);
	// Hour hand
	glColor3f(0.0f, 0.0f, 0.0f);
	glutWireCylinder(10, 1, 60, 60);
	glPushMatrix();
	glRotatef(minAngle, 0, 0, 1);
	glTranslatef(0, 3.5, 1);
	glScalef(1, 8, 1);
	glutSolidCube(1);
	glPopMatrix();
	// Minute hand
	glPushMatrix();
	glRotatef(hourAngle, 0, 0, 1);
	glTranslatef(0, 2.3, 1);
	glScalef(1, 5, 1);
	glutSolidCube(1);
	glPopMatrix();
	glPopAttrib();
}

// Balls for Newton Put
void balls() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLfloat ball_diff[] = { 1.0f, 1.0f, 0.0f, 0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ball_diff);
	glColor3f(0.0f, 0.0f, 0.0f);

	glPushMatrix();  // ball
	glTranslatef(-3.0f, 16.0f, 0.0f);
	glRotatef(b1Rot, 0, 0, 1);
	glTranslatef(0.0f, -11.0f, 0.0f);
	glutSolidSphere(1.5, 30, 30);

	glPushMatrix(); //rope
	glTranslatef(0.0f, 5.0f, 0.0f);
	glScalef(0.1, 11, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 0.0f);
	glutSolidSphere(1.5, 30, 30);

	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 0.0f);
	glScalef(0.1, 11, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0f, 16.0f, 0.0f);
	glRotatef(b2Rot, 0, 0, 1);
	glTranslatef(0.0f, -11.0f, 0.0f);
	glutSolidSphere(1.5, 30, 30);

	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 0.0f);
	glScalef(0.1, 11, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopAttrib();
}

// Draw Newton put
void drawNewtonPut() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	// Pan
	glTranslatef(0, 0, 0);
	glPushMatrix();
	glScalef(20, 2, 15);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCube(1);
	glPopMatrix();
	// pillars
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(-8, 16, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.5, 15, 30, 30);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(8, 16, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.5, 15, 30, 30);
	glPopMatrix();

	// Rod
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(-8, 16, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidCylinder(0.5, 16, 30, 30);
	glPopMatrix();

	// balls
	balls();
	glPopMatrix();
	glPopAttrib();
}

// Draw pictures
void drawPicture(GLuint tex) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(1.73, 1.0, 1.0);
	glutSolidCube(2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.01, -1.01, 1.01);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.01, 1.01, 1.01);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.01, 1.01, 1.01);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.01, -1.01, 1.01);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

// Draw Chair
void drawChair() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// seat
	glColor3f(0.64f, 0.40f, 0.19f);
	glPushMatrix();
	glTranslatef(0, 15, 0);
	glScalef(50, 5, 50);
	glutSolidCube(1);
	glColor3f(0.4f, 0.4f, 0.4f);
	glutWireCube(1);
	glPopMatrix();

	// legs
	glColor3f(0.64f, 0.40f, 0.19f);
	glPushMatrix();
	glTranslatef(-22, 0, 22);
	glScalef(2, 30, 2);
	glutSolidCube(1);
	glColor3f(0.4f, 0.4f, 0.4f);
	glutWireCube(1);
	glPopMatrix();


	glColor3f(0.64f, 0.40f, 0.19f);
	glPushMatrix();
	glTranslatef(-22, 0, -22);
	glScalef(2, 30, 2);
	glutSolidCube(1);
	glColor3f(0.4f, 0.4f, 0.4f);
	glutWireCube(1);
	glPopMatrix();

	glColor3f(0.64f, 0.40f, 0.19f);
	glPushMatrix();
	glTranslatef(22, 0, 22);
	glScalef(2, 30, 2);
	glutSolidCube(1);
	glColor3f(0.4f, 0.4f, 0.4f);
	glutWireCube(1);
	glPopMatrix();

	glColor3f(0.64f, 0.40f, 0.19f);
	glPushMatrix();
	glTranslatef(22, 0, -22);
	glScalef(2, 30, 2);
	glutSolidCube(1);
	glColor3f(0.4f, 0.4f, 0.4f);
	glutWireCube(1);
	glPopMatrix();

	// Back of Chair
	glColor3f(0.64f, 0.40f, 0.19f);
	glPushMatrix();
	glTranslatef(-22.0f, 43.0f, 0.0f);
	glScalef(5, 50, 50);
	glutSolidCube(1);
	glColor3f(0.4f, 0.4f, 0.4f);
	glutWireCube(1);
	glPopMatrix();

	glPopAttrib();
}

// Draw scene
void renderScene() {
	GLfloat fAspect;
	fAspect = (GLfloat)windowWidth / (GLfloat)windowHeight;
	glMatrixMode(GL_PROJECTION); // Reset coordinate system
	glLoadIdentity();
	gluPerspective(60.0f, fAspect, 1.0, 6000.0); // Set viewing volume
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX * zoom, eyeY * zoom, eyeZ * zoom, xAt, yAt, zAt, eyeXu, eyeYu, eyeZu);
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Perform the depth test to render multiple objects in the correct order of Z-axis value
	glEnable(GL_DEPTH_TEST); // Hidden surface removal

	glPushMatrix();
	glRotatef(yRot, 0, 1, 0);
	glRotatef(xRot, 1, 0, 0);

	// Floor
	drawFloor();
	// Ceil
	drawCeil();
	// Walls
	GLfloat size[] = { 250.0f, 150.0f };

	glPushMatrix(); // Back
	glTranslatef(0, 0, -152);
	drawWall(texture[4], size[0]);
	glPopMatrix();

	glPushMatrix(); // Left
	glTranslatef(251, 0, 0);
	glRotatef(-90, 0, 1, 0);
	drawWall(texture[5], size[1]);
	glPopMatrix();

	glPushMatrix(); // Right
	glTranslatef(-251, 0, 0);
	glRotatef(90, 0, 1, 0);
	drawWall(texture[5], size[1]);
	glPopMatrix();

	// Window
	glPushMatrix();
	glTranslatef(-250, 80, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(50, 70, 1);
	drawPicture(texture[8]);
	glPopMatrix();

	// Pictures
	glPushMatrix();
	glTranslatef(-160, 80, -151);
	glScalef(30, 30, 1);
	drawPicture(texture[6]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(250, 80, 0);
	glRotatef(-90, 0, 1, 0);
	glScalef(60, 60, 1);
	drawPicture(texture[7]);
	glPopMatrix();

	// Clock
	glPushMatrix();
	glTranslatef(160, 80, -151);
	glScalef(3, 3, 1);
	drawClock();
	glPopMatrix();

	// Table
	glPushMatrix();
	glTranslatef(-50.0f, -19.0f, -20.0f);
	drawTable();
	// Carpet
	drawCarpet();
	// Newton Put
	glPushMatrix();
	glTranslatef(30.0f, 3.0f, -15.0f);
	drawNewtonPut();
	glPopMatrix();
	// Chair
	glPushMatrix();
	glTranslatef(100, -35, 30);
	glScalef(0.7, 0.7, 0.7);
	glRotatef(135, 0, -1, 0);
	drawChair();
	glPopMatrix();
	// Tea set
	drawTeaSet();
	glPushMatrix();
	glTranslatef(-10.0f, 0.0f, 0.0f);
	GLfloat white[] = { 1.0f, 1.0f, 1.0f };
	GLfloat black[] = { 0.0f, 0.0f, 0.0f };
	// Chess Board
	drawChessBoard();
	// Pieces
	glPushMatrix();
	glTranslatef(-2.3f, 0.0f, -2.3f);
	drawPieces(black);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0f, 5.1f, -3.0f);
	glRotatef(155, -5, -5, -4);
	drawPieces(white);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix(); // Restore the matrix state

	glPopMatrix();
	glutSwapBuffers();
}

// Respond to arrow keys
void specialKeys(int key, int x, int y) {
	if (normal) {
		if (key == GLUT_KEY_RIGHT)
			if (yRot < 55)
				yRot += 5.0f;
		if (key == GLUT_KEY_LEFT)
			if (yRot > -55)
				yRot -= 5.0f;
		if (key == GLUT_KEY_UP)
			if (xRot > -20)
				xRot -= 5.0f;
		if (key == GLUT_KEY_DOWN)
			if (xRot < 90)
				xRot += 5.0f;
		yRot = (GLfloat)((const int)yRot % 360);
		xRot = (GLfloat)((const int)xRot % 360);
		glutPostRedisplay(); // Refresh the window
	}
}

// Respond to char keys
void keyboardInput(unsigned char key, int x, int y) {
	if (key == 'q' || key == 'Q') {  // Go back to normal mode
		zoom = 1.0;
		eyeX = 0.0, eyeY = 30.0, eyeZ = 200.0;
		eyeXu = 0, eyeYu = 1, eyeZu = 0;
		xAt = 0, yAt = 0, zAt = 0;
		xRot = 0.0, yRot = 0.0;
		normal = true;
	}
	if (key == '1' || key == '!') { // Watch the table
		zoom = 1.0;
		eyeX = -50, eyeY = 100, eyeZ = -20;
		xAt = -50, yAt = 0, zAt = -20;
		eyeXu = 0, eyeYu = 0, eyeZu = -1;
		xRot = 0.0, yRot = 0.0;
		normal = false;
	}
	if (key == '2' || key == '@') { // Watch the window
		zoom = 1.0;
		eyeX = -55, eyeY = 80, eyeZ = 0;
		xAt = -250, yAt = 80, zAt = 0;
		eyeXu = 0, eyeYu = 1, eyeZu = 0;
		xRot = 0.0, yRot = 0.0;
		normal = false;
	}
	if (key == '3' || key == '#') { // Watch the first picture
		zoom = 1.0;
		eyeX = -160, eyeY = 80, eyeZ = 0;
		xAt = -160, yAt = 80, zAt = -151;
		eyeXu = 0, eyeYu = 1, eyeZu = 0;
		xRot = 0.0, yRot = 0.0;
		normal = false;
	}
	if (key == '4' || key == '$') { // Watch the second picture
		zoom = 1.0;
		eyeX = 55, eyeY = 80, eyeZ = 0;
		xAt = 250, yAt = 80, zAt = 0;
		eyeXu = 0, eyeYu = 1, eyeZu = 0;
		xRot = 0.0, yRot = 0.0;
		normal = false;
	}
	if (key == '5' || key == '%') { // Watch the clock
		zoom = 1.0;
		eyeX = 160, eyeY = 80, eyeZ = 0;
		xAt = 160, yAt = 80, zAt = -151;
		eyeXu = 0, eyeYu = 1, eyeZu = 0;
		xRot = 0.0, yRot = 0.0;
		normal = false;
	}
	if (key == 27)
		exit(0);  //Exit the program.
	if (normal) {
		if (key == 'o' || key == 'O')
			if (zoom < 1.3)
				zoom += 0.1; //Zoom out
		if (key == 'i' || key == 'I')
			if (zoom > 0.1)
				zoom -= 0.1; //Zoom in
	}
	glutPostRedisplay();
}

void menu(int item) //mouse and keyboard can control the display altogether
{
	keyboardInput((unsigned char)item, 0, 0);
}

int main(int argc, char* argv[]) {
	int ops, zoo;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 3D Project");
	glutReshapeFunc(changeSize);
	glutIdleFunc(myIdleFunc);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboardInput);
	// menu items
	ops = glutCreateMenu(menu);
	glutAddMenuEntry("Show Table (1)", '1');
	glutAddMenuEntry("Show Window (2)", '2');
	glutAddMenuEntry("Show Picture 1 (3)", '3');
	glutAddMenuEntry("Show Picture 2 (4)", '4');
	glutAddMenuEntry("Show Clock (5)", '5');
	glutAddMenuEntry("Back to normal vision (q)", 'q');
	zoo = glutCreateMenu(menu);
	glutAddMenuEntry("Zoom in (i)", 'i');
	glutAddMenuEntry("Zoom increase (o)", 'o');
	glutCreateMenu(menu);
	glutAddMenuEntry("3D Project", '\0');
	glutAddMenuEntry(" ", '\0');
	glutAddSubMenu("Operations ", ops);
	glutAddSubMenu("Zoom ", zoo);
	glutAddMenuEntry(" ", '\0');
	glutAddMenuEntry("Quit (Esc)", '\033');
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	myInit();
	glutDisplayFunc(renderScene);
	lightingAndMaterials();
	glutMainLoop();
	return 0;
}