// v 0.0.4
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

//#define DEBUG
//#define DEBUG_FRAMES

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
using namespace glm;

int w = 0, h = 0, chooseTex = 0, posX = 0, posY = 0, posZ = 0; 
int tgm = 0, tcl = 0;
float vertAngle = 0, horAngle = 0;
std::string model_path = "", title = "Default", texName[3], font;

bool c_run = false;
bool c_on = true;

enum states {GAME, CONSOLE } st = GAME;

#ifndef _WIN32
extern "C" {
#include <common/iniparser/iniparser.h>
}
#else
#include <common/iniparser/iniparser.h>
#endif

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/text2D.hpp>
#include "commands.hpp"
#include <common/obj.hpp>

int parse_ini_file(char* ini_name);

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
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ(model_path.c_str(), vertices, uvs, normals);

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
	
	std::vector<Obj*> objs;
	std::vector<Obj*>::iterator oIT;
	
	objs.push_back(	new Obj(0.0f, 0.0f, 0.0f,
			indexed_vertices,
			indexed_uvs,
			indexed_normals,
			indices,
			texName[chooseTex]));


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
			indexed_vertices,
			indexed_uvs,
			indexed_normals,
			indices,
			texName[chooseTex]));
			coord = false;
		}

		if(send == true)
		{
			objs.push_back(	new Obj(position.x, position.y, position.z,
			indexed_vertices,
			indexed_uvs,
			indexed_normals,
			indices,
			texName[chooseTex]));
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
			
			a->ExtractFrustum();

			if(a->SphereInFrustum(a->pos.x, a->pos.y, a->pos.z, 8.0f))
			{
				tcl++;
				a->DrawAt(a->pos.x, a->pos.y, a->pos.z);
			}
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		
		//char text[256];
		sprintf(text,"%.2f ", verticalAngle );
		printText2D(text, 10, 560, 20);
		
		sprintf(text,"%.2f ", horizontalAngle );
		printText2D(text, 10, 520, 20);
		
		sprintf(text,"tgm %i ", tgm );
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
	
	#ifdef _WIN32
	path = iniparser_getstring(ini, "Level:model", NULL);
	#else
	path = const_cast<char*>(iniparser_getstring(ini, "Level:model", NULL));
	#endif
	model_path = path;
	#ifdef DEBUG
	printf("path = %s\nmodel_path = %s\n", path, model_path.c_str());
	#endif
	
	#ifdef _WIN32
	path = iniparser_getstring(ini, "Global:title", NULL);
	#else
	path = const_cast<char*>(iniparser_getstring(ini, "Global:title", NULL));
	#endif
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
	#ifdef _WIN32
	texBuff[0] = iniparser_getstring(ini, "Level:t1", NULL);
	#else
	texBuff[0] = const_cast<char*>(iniparser_getstring(ini, "Level:t1", NULL));
	#endif
	texName[0] = texBuff[0];
	#ifdef DEBUG
	printf("texBuff = %s\ntexName = %s\n", texBuff[0], texName[0].c_str());
	#endif
	
	#ifdef _WIN32
	texBuff[1] = iniparser_getstring(ini, "Level:t2", NULL);
	#else
	texBuff[1] = const_cast<char*>(iniparser_getstring(ini, "Level:t2", NULL));
	#endif
	texName[1] = texBuff[1];
	#ifdef DEBUG
	printf("texBuff = %s\ntexName = %s\n", texBuff[1], texName[1].c_str());
	#endif
	
	#ifdef _WIN32
	texBuff[2] = iniparser_getstring(ini, "Level:t3", NULL);
	#else
	texBuff[2] = const_cast<char*>(iniparser_getstring(ini, "Level:t3", NULL));
	#endif
	texName[2] = texBuff[2];
	#ifdef DEBUG
	printf("texBuff = %s\ntexName = %s\n", texBuff[2], texName[2].c_str());
	#endif
	
	#ifdef _WIN32
	path = iniparser_getstring(ini, "Global:font", NULL);
	#else
	path = const_cast<char*>(iniparser_getstring(ini, "Global:font", NULL));
	#endif
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
	
	
    iniparser_freedict(ini);
    return 0 ;
}
