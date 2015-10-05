// v 0.0.6
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

//#define DEBUG
//#define DEBUG_FRAMES

#define FOR(i, a) for(i = 0; i < a; i++)

// Include GLEW
#include <GL/glew.h>

#ifdef _WIN32
#include <glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif
GLFWwindow* window;


// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/euler_angles.hpp>
using namespace glm;

const int Texture_n = 3;

int w = 0, h = 0, chooseTex = 0, posX = 0, posY = 0, posZ = 0, shader_v = 0; 
int tgm = 0, tcl = 0;
GLuint shader, billShader, Texture[Texture_n];
GLuint CameraRight_worldspace_ID, 
		CameraUp_worldspace_ID,
		ViewProjMatrixID,
		BillboardPosID,
		BillboardSizeID,
		billboard_vertex_buffer;
GLfloat g_vertex_buffer_data[] = { 
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f
	};
float vertAngle = 0, horAngle = 0;
std::string model_path = "", title = "Default", texName[Texture_n], font;

std::vector<std::vector<glm::vec3> > vertices;
std::vector<std::vector<glm::vec2> > uvs;
std::vector<std::vector<glm::vec3> > normals;
std::vector<std::vector<unsigned short> > indices;

bool c_run = false;
bool c_on = true;

enum states {GAME, CONSOLE } st = GAME;

/*
#ifndef _WIN32
extern "C" {
#include <common/iniparser/iniparser.h>
}
#else
extern "C" {
#include <iniparser/src/iniparser.h>
#include <iniparser/src/dictionary.h>
}
#endif
*/


#include <iniparser/src/iniparser.h>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/vboindexer.hpp>
#include <common/objloader.hpp>
#include <common/text2D.hpp>
#include "commands.hpp"
#include <common/obj.hpp>

int parse_ini_file(char* ini_name);
void init();
void billboard_draw(GLuint shader, float x, float y, float z);

bool dist(int distt, glm::vec3 one)
{
	bool result = false;
	int rdist = distt - distt - distt;
	if(one.x >= distt) result = true;
	else if(one.x <= rdist) result = true;
	
	if(one.y >= distt) result = true;
	else if(one.y <= rdist) result = true;
	
	if(one.z >= distt) result = true;
	else if(one.z <= rdist) result = true;
	
	return result;
}


int main()
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	
	int status;
	
	status = parse_ini_file("Game.ini");
	if(status != 0)
	{
		printf("Error parse Game.ini file\n");
		system("pause");
		return 0;
	}
	if(shader_v == 3){
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}else{
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	}
	window = glfwCreateWindow( w, h, title.c_str(), NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		#ifdef DEBUG
		printf("Trying to restart with another arguments - width = 1600 | height = 900\nw = %i\nh = %i\n", w, h);
		#endif
		system("pause");
		#ifdef DEBUG
		window = glfwCreateWindow( 1600, 900, title.c_str(), NULL, NULL);
		if(window == NULL)
		{
			fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
			system("pause");
			glfwTerminate();
			return -1;
		}
		#endif
		#ifndef DEBUG
		glfwTerminate();
		return -1;
		#endif
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, w/2, h/2);

	

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	billShader = LoadShaders( "Billboard.vertexshader", "Billboard.fragmentshader" );

	CameraRight_worldspace_ID  = glGetUniformLocation(billShader, "CameraRight_worldspace");
	CameraUp_worldspace_ID  = glGetUniformLocation(billShader, "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(billShader, "VP");
	BillboardPosID = glGetUniformLocation(billShader, "BillboardPos");
	BillboardSizeID = glGetUniformLocation(billShader, "BillboardSize");
	
	
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
	
	init();	
		
	std::vector<Obj*> objs;
	std::vector<Obj*>::iterator oIT;
		
	objs.push_back(	new Obj(0.0f, 0.0f, 0.0f,
			vertices[0], //indexed_vertices,
			uvs[0], 	 //indexed_uvs,
			normals[0],  //indexed_normals,
			indices[0],
			Texture[0])); //texName[chooseTex]));

	position = glm::vec3(posX, posY, posZ);
	verticalAngle = vertAngle;
	horizontalAngle = horAngle;
	
	initText2D( font.c_str() );

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	char text[256];

	do{
		
		// Measure speed
		#ifdef DEBUG
		#ifdef DUBUG_FRAMES
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
			printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
		#endif
		#endif

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(st == GAME){
		computeMatricesFromInputs();
		}

		if(st == CONSOLE){
			console_update();
		}

		if(coord == true)
		{
			objs.push_back(	new Obj(oX, oY, oZ,
			vertices[0],
			uvs[0],
			normals[0],
			indices[0],
			Texture[texID])); //texName[chooseTex]);
			coord = false;
		}

		if(send == true)
		{
			objs.push_back(	new Obj(position.x, position.y, position.z,
			vertices[0],
			uvs[0],
			normals[0],
			indices[0],
			Texture[0]));
			send = false;
		}

		if(rm == true)
		{
			objs.erase(objs.begin()+ix);
			rm = false;
		}

		for(oIT = objs.begin(); oIT != objs.end(); oIT++)
		{
			Obj *a = *oIT;
			tcl = 0;
			glm::vec3 distt = a->pos - position;

			a->ExtractFrustum();

			if(a->SphereInFrustum(a->pos.x, a->pos.y, a->pos.z, 20.0f) && dist(100, distt) == false)
			{
				tcl++;
				a->DrawAt(a->pos.x, a->pos.y, a->pos.z);
			}
			else //if(dist(100, distt))
			{
				billboard_draw(billShader, a->pos.x, a->pos.y, a->pos.z);
			}
		}

		//char text[256];
		sprintf(text,"%.2f ", verticalAngle );
		printText2D(text, 10, 560, 20);

		sprintf(text,"%.2f ", horizontalAngle );
		printText2D(text, 10, 520, 20);
		
		//glm::vec3 dis = objs[0]->pos - position;
		sprintf(text,"tgm =  %f ", tgm );
		printText2D(text, 10, 480, 20);

		sprintf(text,"tcl %i ", tcl );
		printText2D(text, 10, 440, 20);
		
		sprintf(text,"pX %.2f ", position.x);
		printText2D(text, 150, 560, 20);
		
		sprintf(text,"pY %.2f ", position.y);
		printText2D(text, 150, 520, 20);
		
		sprintf(text,"pZ %.2f ", position.z);
		printText2D(text, 150, 480, 20);
		
		#ifndef DEBUG
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
			// printf and reset
			//sprintf(text, "%.2f ", 1000.0/double(nbFrames));
			//printText2D(text, 10, 480, 20);
			nbFrames = 0;
			lastTime += 1.0;
		}

			sprintf(text, "%.0f ", 1000.0/double(nbFrames));
			printText2D(text, 10, 480, 20);
		#endif		
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
	for(oIT = objs.begin(); oIT != objs.end(); oIT++)
		{
			Obj *a = *oIT;
			delete a;
		}
	glDeleteVertexArrays(1, &VertexArrayID);
		glDeleteProgram(billShader);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}


int parse_ini_file(char * ini_name)
{
    dictionary  *   ini ;

    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        return -1 ;
    }
    iniparser_dump(ini, stderr);
	
	char* path;

	vertAngle = iniparser_getdouble(ini, "Player:vertAngle", -1.0);
	#ifndef _WIN32
	vertAngle -= 3.07f;
	#endif
	#ifdef DEBUG
	printf("vertAngle = %f\n", vertAngle);
	#endif
	
	horAngle = iniparser_getdouble(ini, "Player:horAngle", -1.0);
	#ifndef _WIN32
	horAngle -= 4.10f;
	#endif
	#ifdef DEBUG
	printf("horAngle = %f\n", horAngle);
	#endif
	
	path = const_cast<char*>(iniparser_getstring(ini, "Level:model", NULL));
	model_path = path;
	#ifdef DEBUG
	printf("path = %s\nmodel_path = %s\n", path, model_path.c_str());
	#endif
	
	path = const_cast<char*>(iniparser_getstring(ini, "Global:title", NULL));
	title = path;
	#ifdef DEBUG
	printf("path = %s\ntitle = %s\n", path, title.c_str());
	#endif
	
	w = iniparser_getint(ini, "Global:intW", -1);
	h = iniparser_getint(ini, "Global:intH", -1);
	#ifdef DEBUG
	printf("w = %i\nh = %i\n", w, h);
	#endif
	
	char* texBuff[3];
	texBuff[0] = const_cast<char*>(iniparser_getstring(ini, "Level:t1", NULL));
	texName[0] = texBuff[0];
	#ifdef DEBUG
	printf("texBuff = %s\ntexName = %s\n", texBuff[0], texName[0].c_str());
	#endif
	
	texBuff[1] = const_cast<char*>(iniparser_getstring(ini, "Level:t2", NULL));
	texName[1] = texBuff[1];
	#ifdef DEBUG
	printf("texBuff = %s\ntexName = %s\n", texBuff[1], texName[1].c_str());
	#endif
	
	texBuff[2] = const_cast<char*>(iniparser_getstring(ini, "Level:t3", NULL));
	texName[2] = texBuff[2];
	#ifdef DEBUG
	printf("texBuff = %s\ntexName = %s\n", texBuff[2], texName[2].c_str());
	#endif
	
	path = const_cast<char*>(iniparser_getstring(ini, "Global:font", NULL));
	font = path;
	#ifdef DEBUG
	printf("font = %s\n", font.c_str());
	#endif
	
	chooseTex = iniparser_getint(ini, "Global:nTexture", -1);
	#ifdef DEBUG
	printf("chooseTex = %i\n", chooseTex);
	#endif

	posX = iniparser_getint(ini, "Player:pX", -1);
	#ifdef DEBUG
	printf("Player.x = %i\n", posX);
	#endif
	
	posY = iniparser_getint(ini, "Player:pY", -1);
	#ifdef DEBUG
	printf("Player.y = %i\n", posY);
	#endif

	posZ = iniparser_getint(ini, "Player:pZ", -1);
	#ifdef DEBUG
	printf("Player.z = %i\n", posZ);
	#endif

	c_on = iniparser_getboolean(ini, "Global:console", -1);
	#ifdef DEBUG
	printf("Console = %d\n", c_on);
	#endif
	
	shader_v = iniparser_getint(ini, "Global:shader", -1);
	#ifdef DEBUG
	printf("Shader version = %i\n", shader_v);
	#endif
	
    iniparser_freedict(ini);
    return 0 ;
}


void init()
{
	int i = 0;
	/*
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec2> uvs_;
	std::vector<glm::vec3> normals_;
	bool res = loadOBJ(model_path.c_str(), vertices_, uvs_, normals_);

	std::vector<unsigned short> indicess;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices_, uvs_, normals_, indicess, indexed_vertices, indexed_uvs, indexed_normals);
	*/
	
	std::vector<unsigned short> indicess;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	
	loadOBJ(model_path.c_str(), indexed_vertices, indexed_uvs, indexed_normals, indicess);
	
	vertices.push_back(indexed_vertices);
	uvs.push_back(indexed_uvs);
	normals.push_back(indexed_normals);
	indices.push_back(indicess);
	
	shader = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	FOR(i, Texture_n)
	{
		Texture[i] = loadDDS(texName[i].c_str());
	}
	
	glUseProgram(shader);
	GLuint LightID = glGetUniformLocation(shader, "LightPosition_worldspace");
	
	glm::vec3 lightPos = glm::vec3(4,7,4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
}


void billboard_draw(GLuint shader, float x, float y, float z)
{
	glUseProgram(shader);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
			GLuint TextureID  = glGetUniformLocation(shader, "MyTextureSampler");
	glUniform1i(TextureID, 0);
			
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
			
	glUniform3f(BillboardPosID, x, y, z); // The billboard will be just above the cube
	glUniform2f(BillboardSizeID, 1.0f, /*0.125f*/ 1.0f);     // and 1m*12cm, because it matches its 256*32 resolution
		
	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
		

	// Draw the billboard !
	// This draws a triangle_strip which looks like a quad.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableVertexAttribArray(0);
}