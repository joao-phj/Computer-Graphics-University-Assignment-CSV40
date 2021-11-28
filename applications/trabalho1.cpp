/**
 * @file appImage.cpp
 * @brief Get maximum, minimum values and print image.
 * @author Ricardo Dutra da Silva
 */
#include <cg.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <iostream>
#include <vector>

/* Defines. */
#define MAX_NUM_POINTS 1000
#define WINDOW_SIZE 512
#define ROT_MOV 5.f
#define TRANS_MOV 0.03
#define SCALE_MOV_B 1.5
#define SCALE_MOV_S 0.75

/* Globals */
GLuint VAO, VBO, IBO;
int npoints = 0;
int triangles;

std::vector<GLfloat> vertices;
std::vector<unsigned int> indices;

int delay = 100;

int mode = 0;
int view = 1;

glm::mat4 trans = glm::mat4(1.0f);

float zAVG;

GLuint program;

void Init(cgMat2i);
void Display(void);
void Keyboard(unsigned char, int, int);
void keySpecial (int key, int x, int y);

float z = 0.0;
float x = 0.0;
float y = 0.0;

void Keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
		case 27:
        exit(0);
		case 'q':
				exit(0);
		case 't':
				std::cout<< "Entrando em Modo Translação" << std::endl;
				mode = 1;
				break;
		case 'r':
				std::cout<< "Entrando em Modo Rotação" << std::endl;
				mode = 2;
				break;
		case 's':
				mode = 3;
				break;
		case 'v':
				view *= -1;
				glutPostRedisplay();
				break;
			case 'a':
					{
						if(mode == 1) {
							trans = glm::translate(trans, glm::vec3(0.0, 0.0, TRANS_MOV));
						}
						else if(mode == 2) {
							trans = glm::rotate(trans, glm::radians(ROT_MOV), glm::vec3(0.0, 0.0, 1.0));
						}
						else if(mode == 3) {
							trans = glm::scale(trans, glm::vec3(1.0, 1.0, SCALE_MOV_B));
						}
						else {
							std::cout<< "Antes de prosseguir escolha uma transformação" << std::endl;
						}

						glutPostRedisplay();
					}
					break;
			case 'd':
					{
						if(mode == 1) {
							trans = glm::translate(trans, glm::vec3(0.0, 0.0, -TRANS_MOV));
						}
						else if(mode == 2) {
							trans = glm::rotate(trans, glm::radians(-ROT_MOV), glm::vec3(0.0, 0.0, 1.0));
						}
						else if(mode == 3) {
							trans = glm::scale(trans, glm::vec3(1.0, 1.0, SCALE_MOV_S));
						}
						else {
							std::cout<< "Antes de prosseguir escolha uma transformação" << std::endl;
						}

						glutPostRedisplay();
					}
					break;
    }
}

void keySpecial(int key, int x, int y) {
	switch(key){
		case GLUT_KEY_UP:
        {
					if(mode == 1) {
						trans = glm::translate(trans, glm::vec3(0.0, TRANS_MOV, 0.0));
					}
					else if(mode == 2) {
						trans = glm::rotate(trans, glm::radians(ROT_MOV), glm::vec3(0.0, 1.0, 0.0));
					}
					else if(mode == 3) {
						trans = glm::scale(trans, glm::vec3(1.0, SCALE_MOV_B, 1.0));
					}
					else {
						std::cout<< "Antes de prosseguir escolha uma transformação" << std::endl;
					}
          glutPostRedisplay();
        }
        break;
			case GLUT_KEY_DOWN:
	        {
						if(mode == 1) {
							trans = glm::translate(trans, glm::vec3(0.0, -TRANS_MOV, 0.0));
						}
						else if(mode == 2) {
							trans = glm::rotate(trans, glm::radians(-ROT_MOV), glm::vec3(0.0, 1.0, 0.0));
						}
						else if(mode == 3) {
							trans = glm::scale(trans, glm::vec3(1.0, SCALE_MOV_S, 1.0));
						}
						else {
							std::cout<< "Antes de prosseguir escolha uma transformação" << std::endl;
						}

	          glutPostRedisplay();
	        }
	        break;
			case GLUT_KEY_RIGHT:
	        {
						if(mode == 1) {
							trans = glm::translate(trans, glm::vec3(TRANS_MOV, 0.0, 0.0));
						}
						else if(mode == 2) {
							trans = glm::rotate(trans, glm::radians(ROT_MOV), glm::vec3(1.0, 0.0, 0.0));
						}
						else if(mode == 3) {
							trans = glm::scale(trans, glm::vec3(SCALE_MOV_B, 1.0, 1.0));
						}
						else {
							std::cout<< "Antes de prosseguir escolha uma transformação" << std::endl;
						}

	          glutPostRedisplay();
	        }
	        break;
			case GLUT_KEY_LEFT:
					{
						if(mode == 1) {
							trans = glm::translate(trans, glm::vec3(-TRANS_MOV, 0.0, 0.0));
						}
						else if(mode == 2) {
							trans = glm::rotate(trans, glm::radians(-ROT_MOV), glm::vec3(1.0, 0.0, 0.0));
						}
						else if(mode == 3) {
							trans = glm::scale(trans, glm::vec3(SCALE_MOV_S, 1.0, 1.0));
						}
						else {
							std::cout<< "Antes de prosseguir escolha uma transformação" << std::endl;
						}

						glutPostRedisplay();
					}
					break;
	}
}


void Display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int transformLoc = glGetUniformLocation(program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(),  &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	if(view == 1) {
		glDrawArrays(GL_POINTS, 0, npoints);
	}
	else if (view == -1) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices[0], GL_STATIC_DRAW);
		glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, (void*)0);
	}

	glFlush();
}

void Init(cgMat2i img)
{
	triangles = (img->height-1) * (img->width-1) * 2;

  /* Print min and max values. */
	printf("%d\n", cgMatMinValue2i(img));
	printf("%d\n", cgMatMaxValue2i(img));

  for (int r = 0; r < img->height; r++)
  {
    for (int c = 0; c < img->height; c++)
    {
      vertices.push_back((float)((float)r-img->width/2)/(img->width/2));
      vertices.push_back((float)((float)c-img->height/2)/(img->height/2));
      vertices.push_back((float)((float)img->val[r][c]-zAVG)/(zAVG));
      vertices.push_back(1.0);

			//std::cout<< "x: " << vertices[4*npoints] << ", y: " << vertices[(4*npoints)+1] << ", z: " << vertices[(4*npoints)+2] << std::endl;

      npoints++;
    }
  }

	std::cout<< "Height: " << img->height << std::endl;
	std::cout<< "Width: " << img->width << std::endl;
	std::cout<< "Points: " << npoints << std::endl;
	std::cout<< "Triangles: " << triangles << std::endl;


	for(int i=0; i<img->width * img->height; i++) {
		if(i%(img->width) != img->width-1) {
			indices.push_back(i);
			indices.push_back(i+1);
			//std::cout<< "Horizontal_Top: " << i << " -> " << i+1 << "| x: " << vertices[4*i] << ", y: " << vertices[(4*i)+1] << ", z: " << vertices[(4*i)+2] << std::endl;
		}

		if(i/(img->height) != img->height-1) {
			indices.push_back(i);
			indices.push_back(i+img->width);
			//std::cout<< "Vertical_Left: " << i << " -> " << i+img->width << "| x: " << vertices[4*i] << ", y: " << vertices[(4*i)+1] << ", z: " << vertices[(4*i)+2] << std::endl;
		}

		if(i%(img->height) != img->height-1 && i/(img->width) != img->width-1) {
			indices.push_back(i);
			indices.push_back(i+img->width+1);
			//std::cout<< "Diagonal_TL_BR: " << i << " -> " << i+img->width+1 << "| x: " << vertices[4*i] << ", y: " << vertices[(4*i)+1] << ", z: " << vertices[(4*i)+2] << std::endl;
		}
		//printf("\n");
	}


  glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

  /* Create vertex buffer object (VBO). */
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &IBO);

  /* Copy data to VBO. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	program = LoadShaders("simple3.vert", "simple.frag");
  glUseProgram(program);

  glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv)
{

	/* Check input arguments. */
	if (argc != 2)
	{
		fprintf(stderr, "%s image.pgm\n", argv[0]);
		exit(1);
	}

	/* Read image. */
	cgMat2i img  = cgReadPGMImage(argv[1]);

	if (img == NULL)
	{
		fprintf(stdout, "FAILURE.\n");
		return EXIT_FAILURE;
	}

	zAVG = cgMatMaxValue2i(img) - cgMatMinValue2i(img);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);

	if (glewInit())
       	{
		fprintf(stderr, "Unable to initialize GLEW ... exiting.");
		exit(EXIT_FAILURE);
	}

  Init(img);

	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(keySpecial);
	glutDisplayFunc(Display);


  glutMainLoop();

  //cgFreeMat2i(img);
}
