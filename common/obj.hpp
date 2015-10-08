void CleanVBO();

void createBufAndDraw(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals, std::vector<unsigned short> indices, GLuint shader, GLuint Texture, GLuint TextureID, int x, int y, int z)
{
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
 
	
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
 
	
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	
	
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

	glUseProgram(shader);
	
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0

	glUniform1i(TextureID, 0);

		GLuint vertexPosition_modelspaceID = glGetAttribLocation(shader, "vertexPosition_modelspace");
		GLuint vertexUVID = glGetAttribLocation(shader, "vertexUV");
		GLuint vertexNormal_modelspaceID = glGetAttribLocation(shader, "vertexNormal_modelspace"); 
		
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
 
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
 
		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			vertexNormal_modelspaceID,    // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 TranslationMatrix = glm::translate(mat4(), vec3(x,y,z)); // A bit to the left
		glm::mat4 ModelMatrix = TranslationMatrix; // * RotationMatrix * ScalingMatrix;
 
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		
		GLuint MatrixID = glGetUniformLocation(shader, "MVP");
		GLuint ViewMatrixID = glGetUniformLocation(shader, "V");
		GLuint ModelMatrixID = glGetUniformLocation(shader, "M");
		
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		
		glDrawElements(
				GL_TRIANGLES,      // mode
				indices.size(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
			
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &elementbuffer);
}

/*void CleanVBO()
{
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &elementbuffer);
}
*/

#include <string>
#include <glm/gtc/type_ptr.hpp>

class Obj
{
	public:
	float m;			// The mass value
	glm::vec3 pos;			// Position in space
	glm::vec3 aabb[8];
	glm::vec3 bsphere_center;
	glm::vec3 vel;			// Velocity
	glm::vec3 force;
	glm::vec3 size;
	float Rx, Ry, Rz;
	float frustum[6][4];
	float bsphere_radius;
	glm::vec3 Rot;
	std::vector<glm::vec3> vert;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> norm;
	std::vector<unsigned short> indicess;
	GLuint shaderr, Texture, TextureID;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
		
	//Obj(){}
	
	Obj(float xx, float yy, float zz,
				std::vector<glm::vec3> vertt,
				std::vector<glm::vec2> uvv,
				std::vector<glm::vec3> normm,
				std::vector<unsigned short> indicesss,
				GLuint Texturee)
	{
		pos.x = xx;
		pos.y = yy;
		pos.z = zz;
		
		size.x = 1.0f;
		size.y = 1.0f;
		size.z = 1.0f;
		/*
		vert = vertt;
		uv = uvv;
		norm = normm;
		indices = indicess;
		
		shader = shaderr;
		
		Texture = Texturee;
		TextureID = TextureIDd;

		tex = Texturee;
		*/

		/*shader = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );
		
		glUseProgram(shader);
		GLuint LightID = glGetUniformLocation(shader, "LightPosition_worldspace");
	
		glm::vec3 lightPos = glm::vec3(4,7,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		*/		
		shaderr = shader;
		
		Texture = Texturee;

		TextureID  = glGetUniformLocation(shaderr, "MyTextureSampler");
		
		vert = vertt;
		uv = uvv;
		norm = normm;
		indicess = indicesss;
			
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert[0], GL_STATIC_DRAW);
				
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
		
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);
		
		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicess.size() * sizeof(unsigned short), &indicess[0] , GL_STATIC_DRAW);
	}
	
	/*
	void Draw()
	{
		createBufAndDraw(vert, uv, norm, indices, shader, Texture, TextureID, x, y, z);
	}
	*/
	
	//лоад устарел его надо поправить, хотя я пока не вижу смысла его использовать
	/*
	void Load(std::vector<glm::vec3> vertices,
			std::vector<glm::vec2> uvs,
			std::vector<glm::vec3> normals,
			std::vector<unsigned short> indicess,
			//GLuint shaderr,
			std::string Texturee)
			//GLuint TextureIDd)
	{		
		//shader = shaderr;
		//TextureID = TextureIDd;
		//Texture = Texturee;
		tex = Texturee;

		shader = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );
		
		glUseProgram(shader);
		GLuint LightID = glGetUniformLocation(shader, "LightPosition_worldspace");
	
		glm::vec3 lightPos = glm::vec3(4,7,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
	
		Texture = loadDDS(tex.c_str());

		TextureID  = glGetUniformLocation(shader, "MyTextureSampler");
		
		vert = vertices;
		uv = uvs;
		norm = normals;
		indices = indicess;
	
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		
		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

		
	}
	*/
	
	void Unload()
	{
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &elementbuffer);
		glDeleteProgram(shaderr);
		glDeleteTextures(1, &Texture);
	}
	
	void DrawAt(float xx, float yy, float zz)
	{
		pos.x = xx;
		pos.y = yy;
		pos.z = zz;
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);
		
		glUseProgram(shaderr);
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		GLuint vertexPosition_modelspaceID = glGetAttribLocation(shaderr, "vertexPosition_modelspace");
		GLuint vertexUVID = glGetAttribLocation(shaderr, "vertexUV");
		GLuint vertexNormal_modelspaceID = glGetAttribLocation(shaderr, "vertexNormal_modelspace"); 
		//Rot.y += 3.14159f/2.0f * deltaTime;
		
		/*
		if(sp == true){
			Rot.y += 3.14159f/2.0f * deltaTime;
			if(Rot.y >= 6.22f) Rot.y = 0;
		}	
		if(sp == false) Rot.y += 0;
		*/
		glm::mat4 ScalingMatrix = glm::scale(size.x, size.y, size.z); //1.0f, 1.0f, 1.0f); //10.0f,10.0f,10.0f);
		//glm::mat4 RotationMatrix = eulerAngleYXZ(0.0f, 0.0f, 1.0f);//Rot.y,Rot.x,Rot.z);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 TranslationMatrix = glm::translate(mat4(), vec3(pos.x,pos.y,pos.z)); // A bit to the left
		glm::mat4 ModelMatrix = TranslationMatrix * ScalingMatrix; // * TranslationMatrix * RotationMatrix * ScalingMatrix;
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		GLuint MatrixID = glGetUniformLocation(shaderr, "MVP");
		GLuint ViewMatrixID = glGetUniformLocation(shaderr, "V");
		GLuint ModelMatrixID = glGetUniformLocation(shaderr, "M");
		
		// Send our transformation to the currently bound shaderr, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		
		// 1rst attribute buffer : vertices
		if(shader_v == 3)
			glEnableVertexAttribArray(0);
		else
			glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		///glDrawArrays(GL_QUADS, 0, vert.size());
		
		// 2nd attribute buffer : UVs
		if(shader_v == 3)
			glEnableVertexAttribArray(1);
		else
			glEnableVertexAttribArray(vertexUVID);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		
		// 3rd attribute buffer : normals
		if(shader_v == 3)
			glEnableVertexAttribArray(2);
		else
			glEnableVertexAttribArray(vertexNormal_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			vertexNormal_modelspaceID,    // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
			
		/**
		
		*/
		glDrawElements(
				GL_TRIANGLES,      // mode
				indicess.size(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
			
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
			
		lastTime = currentTime;
	}
	
	void ExtractFrustum()
	{
		//float   proj[16];
		//float   modl[16];
		float   clip[16];
		float   t;
	   
		//glm::mat4 tempMat = getProjectionMatrix();
		
		float proj[16] = {0.0};
		const float *pSource = (const float*)glm::value_ptr(getProjectionMatrix());
		for (int i = 0; i < 16; ++i)
			proj[i] = pSource[i];
		
		//tempMat = getViewMatrix();
		
		float modl[16] = {0.0};
		pSource = (const float*)glm::value_ptr(getViewMatrix());
		for(int i = 0; i < 16; i++)
			modl[i] = pSource[i];
	 
		/* Узнаем текущую матрицу PROJECTION 
		glGetFloatv( GL_PROJECTION_MATRIX, proj ); */
	 
		/* Узнаем текущую матрицу MODELVIEW 
		glGetFloatv( GL_MODELVIEW_MATRIX, modl ); */
	 
		/* Комбинируем матрицы(перемножаем) */
		clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
		clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
		clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
		clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
	 
		clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
		clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
		clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
		clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
	 
		clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
		clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
		clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
		clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
	 
		clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
		clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
		clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
		clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
	 
		/* Находим A, B, C, D для ПРАВОЙ плоскости */
		frustum[0][0] = clip[ 3] - clip[ 0];
		frustum[0][1] = clip[ 7] - clip[ 4];
		frustum[0][2] = clip[11] - clip[ 8];
		frustum[0][3] = clip[15] - clip[12];
	 
		/* Приводим уравнение плоскости к нормальному виду */
		t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
		frustum[0][0] /= t;
		frustum[0][1] /= t;
		frustum[0][2] /= t;
		frustum[0][3] /= t;
	 
		/* Находим A, B, C, D для ЛЕВОЙ плоскости */
		frustum[1][0] = clip[ 3] + clip[ 0];
		frustum[1][1] = clip[ 7] + clip[ 4];
		frustum[1][2] = clip[11] + clip[ 8];
		frustum[1][3] = clip[15] + clip[12];
	 
		/* Приводим уравнение плоскости к нормальному виду */
		t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
		frustum[1][0] /= t;
		frustum[1][1] /= t;
		frustum[1][2] /= t;
		frustum[1][3] /= t;
	 
		/* Находим A, B, C, D для НИЖНЕЙ плоскости */
		frustum[2][0] = clip[ 3] + clip[ 1];
		frustum[2][1] = clip[ 7] + clip[ 5];
		frustum[2][2] = clip[11] + clip[ 9];
		frustum[2][3] = clip[15] + clip[13];
	 
		/* Приводим уравнение плоскости к нормальному */
		t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
		frustum[2][0] /= t;
		frustum[2][1] /= t;
		frustum[2][2] /= t;
		frustum[2][3] /= t;
	 
		/* ВЕРХНЯЯ плоскость */
		frustum[3][0] = clip[ 3] - clip[ 1];
		frustum[3][1] = clip[ 7] - clip[ 5];
		frustum[3][2] = clip[11] - clip[ 9];
		frustum[3][3] = clip[15] - clip[13];
	 
		/* Нормальный вид */
		t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
		frustum[3][0] /= t;
		frustum[3][1] /= t;
		frustum[3][2] /= t;
		frustum[3][3] /= t;
	 
		/* ЗАДНЯЯ плоскость */
		frustum[4][0] = clip[ 3] - clip[ 2];
		frustum[4][1] = clip[ 7] - clip[ 6];
		frustum[4][2] = clip[11] - clip[10];
		frustum[4][3] = clip[15] - clip[14];
		
		/* Нормальный вид */
		t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
		frustum[4][0] /= t;
		frustum[4][1] /= t;
		frustum[4][2] /= t;
		frustum[4][3] /= t;
	 
		/* ПЕРЕДНЯЯ плоскость */
		frustum[5][0] = clip[ 3] + clip[ 2];
		frustum[5][1] = clip[ 7] + clip[ 6];
		frustum[5][2] = clip[11] + clip[10];
		frustum[5][3] = clip[15] + clip[14];
	 
		/* Нормальный вид */
		t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
		frustum[5][0] /= t;
		frustum[5][1] /= t;
		frustum[5][2] /= t;
		frustum[5][3] /= t;
	}
	
	bool SphereInFrustum( float x, float y, float z, float radius )
	{
		int p;

		for( p = 0; p < 6; p++ )
			if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= -radius )
				return false;
		return true;
	}
	
	void CreateBSphere(/**obj_type_ptr p_object*/)
	{
		glm::vec3 l_vector;
		
		//AABB creation
		CreateAABB();

		//Object center (It uses the extreme points of the AABB)
		bsphere_center.x = (aabb[6].x + aabb[0].x)/2;
		bsphere_center.y = (aabb[6].y + aabb[0].y)/2;
		bsphere_center.z = (aabb[6].z + aabb[0].z)/2;
		
		//Object radius (It uses the extreme points of the AABB)
		VectCreate(aabb[0], aabb[6], l_vector);
		bsphere_radius = VectLength(l_vector)/2;
	}
	
	float VectLength (glm::vec3 p_vector)
	{
		return (float)(sqrt(p_vector.x*p_vector.x + p_vector.y*p_vector.y + p_vector.z*p_vector.z));
	}
	
	void VectCreate (glm::vec3 p_start, glm::vec3 p_end, glm::vec3 &p_vector)
	{
		p_vector.x = p_end.x - p_start.x;
		p_vector.y = p_end.y - p_start.y;
		p_vector.z = p_end.z - p_start.z;
	}
	
	void CreateAABB()
	{
		int i;

		// At first store the first vertex into the first AABB vertex
		for (i=0; i<8; i++)
		{
			aabb[i].x = vert[0].x;
			aabb[i].y = vert[0].y;
			aabb[i].z = vert[0].z;
		}

		// Now get the 8 AABB vertex making some comparisons to get the 8 extreme vertices of the object
		for (i=1; i<vert.size(); i++)
		{
			if (vert[i].x < aabb[0].x) aabb[0].x = vert[i].x;// + pos.x;
			if (vert[i].y < aabb[0].y) aabb[0].y = vert[i].y;// + pos.y;
			if (vert[i].z < aabb[0].z) aabb[0].z = vert[i].z;// + pos.z;

			if (vert[i].x > aabb[1].x) aabb[1].x = vert[i].x;// + pos.x;
			if (vert[i].y < aabb[1].y) aabb[1].y = vert[i].y;// + pos.y;
			if (vert[i].z < aabb[1].z) aabb[1].z = vert[i].z;// + pos.z;

			if (vert[i].x > aabb[2].x) aabb[2].x = vert[i].x;// + pos.x;
			if (vert[i].y > aabb[2].y) aabb[2].y = vert[i].y;// + pos.y;
			if (vert[i].z < aabb[2].z) aabb[2].z = vert[i].z;// + pos.z;

			if (vert[i].x < aabb[3].x) aabb[3].x = vert[i].x;// + pos.x;
			if (vert[i].y > aabb[3].y) aabb[3].y = vert[i].y;// + pos.y;
			if (vert[i].z < aabb[3].z) aabb[3].z = vert[i].z;// + pos.z;

			if (vert[i].x < aabb[4].x) aabb[4].x = vert[i].x;// + pos.x;
			if (vert[i].y < aabb[4].y) aabb[4].y = vert[i].y;// + pos.y;
			if (vert[i].z > aabb[4].z) aabb[4].z = vert[i].z;// + pos.z;
		
			if (vert[i].x > aabb[5].x) aabb[5].x = vert[i].x;// + pos.x;
			if (vert[i].y < aabb[5].y) aabb[5].y = vert[i].y;// + pos.y;
			if (vert[i].z > aabb[5].z) aabb[5].z = vert[i].z;// + pos.z;

			if (vert[i].x > aabb[6].x) aabb[6].x = vert[i].x;// + pos.x;
			if (vert[i].y > aabb[6].y) aabb[6].y = vert[i].y;// + pos.y;
			if (vert[i].z > aabb[6].z) aabb[6].z = vert[i].z;// + pos.z;

			if (vert[i].x < aabb[7].x) aabb[7].x = vert[i].x;// + pos.x;
			if (vert[i].y > aabb[7].y) aabb[7].y = vert[i].y;// + pos.y;
			if (vert[i].z > aabb[7].z) aabb[7].z = vert[i].z;// + pos.z;	
			
		}
		
		FOR(8)
		{
			aabb[i] *= size;
			aabb[i] += pos;
		}
	}
	
	glm::vec3 rndVec3()
	{
		glm::vec3 n;
		do
		{
			n = glm::vec3(rand()%3-1, rand()%3-1, rand()%3-1);
		}
		while(VectLength(n) < 1);
		return n;
	}
	
	void CreateLand(int per)
	{
		int j = 0;
		glm::vec3 v;
		
		float d;
		
		FOR(per)
		{ 
			glm::vec3 n,m;
			n = rndVec3();
			m = rndVec3();
			
			FOR_j(vert.size())
			{
				glm::vec3& p = vert[j];
				v = p - n;
				d = glm::dot(n,v);
				
				if(d > 0)
				{
					p += m;
				}
				else
				{
					p -= m;
				}
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert[0], GL_STATIC_DRAW);
	}
	
};

void MapCubeToSphere( glm::vec3& vPosition )
{
    float x2 = vPosition.x * vPosition.x;
    float y2 = vPosition.y * vPosition.y;
    float z2 = vPosition.z * vPosition.z;

    vPosition.x = vPosition.x * sqrt( 1.0f - ( y2 * 0.5f ) - ( z2 * 0.5f ) + ( (y2 * z2) / 3.0f ) );
    vPosition.y = vPosition.y * sqrt( 1.0f - ( z2 * 0.5f ) - ( x2 * 0.5f ) + ( (z2 * x2) / 3.0f ) );
    vPosition.z = vPosition.z * sqrt( 1.0f - ( x2 * 0.5f ) - ( y2 * 0.5f ) + ( (x2 * y2) / 3.0f ) );
}

struct cube
{
	vector<glm::vec3> st_vert;
	vector<glm::vec2> st_uv;
	vector<glm::vec3> st_norm;
	vector<unsigned short> st_indices;
	
	bool operator+=( cube& that) {
		FOR(st_vert.size())
		{
			if(that.st_vert.size() > this->st_vert.size())
				this->st_vert.reserve(that.st_vert.size());
			this->st_vert[i] += that.st_vert[i];
		}
		FOR(st_uv.size())
		{
			if(that.st_uv.size() > this->st_uv.size())
				this->st_uv.reserve(that.st_uv.size());
			this->st_uv[i] += that.st_uv[i];
		}
		FOR(st_norm.size())
		{
			if(that.st_norm.size() > this->st_norm.size())
				this->st_norm.reserve(that.st_norm.size());
			this->st_norm[i] += that.st_norm[i];
		}
		FOR(that.st_indices.size())
		{
			if(that.st_indices.size() > this->st_indices.size())
				this->st_indices.reserve(that.st_indices.size());
			this->st_indices[i] += that.st_indices[i];
		}
		
		return this;
		//return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
	
};

cube CreatePlane(float radius, int s) //, int width, int height)
													/**
													std::vector<glm::vec3>& vert,
													std::vector<glm::vec2>& uv,
													std::vector<glm::vec3>& norm,
													std::vector<unsigned short>& indis
													)*/
{
	glm::vec3 vMinPosition( -1.0f, -1.0f, -1.0f );
	
	int width, height;
	width = height = s;
	
	std::vector<glm::vec3> vert(width*height);
	std::vector<glm::vec2> uv(width*height);
	std::vector<glm::vec3> norm(width*height);
	//std::vector<unsigned short> indis;
	cube cub;
	
	///vert.reserve(width*height);
	///norm.reserve(width*height);
	///uv.reserve(width*height);
	
	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < width; ++x )
		{
			glm::vec3 vPosition = vMinPosition;
			vPosition.x += (float)x / (float)(width-1) * 2.0f; // Умножить на 2.0f для переноса позиции с -1 на +1
			vPosition.y += (float)y / (float)(height-1) * 2.0f; // Умножить на 2.0f для переноса позиции с -1 на +1

			// Проекция позиции сетки на позицию сферы
			MapCubeToSphere( vPosition );

			// Нормаль - всего лишь вектор из центра сферы.
			glm::vec3 vNormal = glm::normalize(vPosition); //vPosition.Normal();

			// Экструдировать сферу по её радиусу
			vPosition *= radius;

			// Назначит в буффер вершин
			/*pVertexBuffer*/vert[ y * width + x ]/*.Position*/ = vPosition;

			/*pVertexBuffer*/norm[ y * width + x ]/*.Normal*/ = vNormal;
		}
	}
	
	for ( int j = 0; j < height; ++j )
	{
		for ( int i = 0; i < width; ++i )
		{
			uv[  j * width + i ].x = (float)i / (float)(height - 1);
			uv[  j * width + i ].y = (float)j / (float)(width - 1);
		}
	}
	
	/*
	for(int i = 0; i < vert.size() * 3; i += 3)
	{
		glm::vec3 n = (glm::cross( vert[i+1] - vert[i], vert[i+2] - vert[i] ));
		float sin_alpha = glm::length(n) / (glm::length(vert[i+1] - vert[i]) * glm::length(vert[i+2] - vert[i]));
		glm::vec3 normal = glm::normalize(n) * asin(sin_alpha);
		glm::vec3 norm1 = (dot(normal,position) < 0.0) ? normal : -normal;
		norm.push_back(norm1);
		//norm.push_back(normal);
		//norm.push_back(normal);
	}
	*/
	
	FOR(vert.size())
	{
		vert[i] = -vert[i];
	}
	
	cub.st_vert = vert;
	cub.st_uv = uv;
	cub.st_norm = norm;

	
	
	for(int i = 0; i < cub.st_vert.size()-s; i++)
	{
		static int j = 1;
		static int k = s-1;
		if(i == k) { k += s; j++; continue; }

		cub.st_indices.push_back(i);
		cub.st_indices.push_back(i+1);
		cub.st_indices.push_back(i+s);
		
		cub.st_indices.push_back(j);
		cub.st_indices.push_back(j+s);
		cub.st_indices.push_back(j+s-1);
		printf("Loading... %.1f%\n", (float)i*100.0f/(float)cub.st_vert.size());
		j++;
	}
	
	return cub;
}

cube CreateQuad(glm::vec3 origin, glm::vec3 w, glm::vec3 len)
{
	cube poly;
	
	poly.st_vert.push_back(origin);
	poly.st_vert.push_back(origin + len);
	poly.st_vert.push_back(origin + len + w);
	poly.st_vert.push_back(origin + w);
	
	FOR(poly.st_vert.size())
	{
		glm::vec3 normal = glm::normalize(poly.st_vert[i]);
		poly.st_norm.push_back(normal);
	}
	
	poly.st_indices.push_back(0);
	poly.st_indices.push_back(1);
	poly.st_indices.push_back(2);
	
	poly.st_indices.push_back(0);
	poly.st_indices.push_back(2);
	poly.st_indices.push_back(3);
	
	
	FOR(poly.st_vert.size())
	{
		poly.st_uv.push_back(glm::vec2(rand()%2, rand()%2));
	}
	
	return poly;
}

cube CreateCube(glm::vec3 width, glm::vec3 length, glm::vec3 height)
{
	glm::vec3 temp;
	temp.x = -width.x * 0.5f;
	temp.y = -width.y * 0.5f;
	temp.z = -width.z * 0.5f;
	
	glm::vec3 corner0 = temp;
	
	temp.x = length.x * 0.5f;
	temp.y = length.y * 0.5f;
	temp.z = length.z * 0.5f;
	
	corner0 -= length;
	
	temp.x = height.x * 0.5f;
	temp.y = height.y * 0.5f;
	temp.z = height.z * 0.5f;
	
	corner0 -= height;
	
	temp.x = width.x * 0.5f;
	temp.y = width.y * 0.5f;
	temp.z = width.z * 0.5f;
	
	glm::vec3 corner1 = width;
	
	temp.x = length.x * 0.5f;
	temp.y = length.y * 0.5f;
	temp.z = length.z * 0.5f;
	
	corner1 += length;
	
	temp.x = height.x * 0.5f;
	temp.y = height.y * 0.5f;
	temp.z = height.z * 0.5f;

	corner1 += height;
	
	//glm::vec3 corner0 = -width.x*0.5f - length*0.5f - height*0.5f;
    //glm::vec3 corner1 = width*0.5f + length*0.5f + height*0.5f;

    cube combine[6]; // = new CombineInstance[6];
    combine[0] = CreateQuad(corner0, length, width);
    combine[1] = CreateQuad(corner0, width, height);
    combine[2] = CreateQuad(corner0, height, length);
    combine[3] = CreateQuad(corner1, -width, -length);
    combine[4] = CreateQuad(corner1, -height, -width);
    combine[5] = CreateQuad(corner1, -length, -height);
	
	/*
	for(int i = 1; i < 6; i++)
	{
		combine[0] += combine[i];  //<- тут надо индексы складывать
	}
	*/
	
	for(int i = 1; i < 6; i++)
	{
		for(int j = 0; j < combine[i].st_indices.size(); j++)
		{
			combine[0].st_indices.push_back(combine[i].st_indices[j]);
		}
		
		for(int j = 0; j < combine[i].st_vert.size(); j++)
		{
			combine[0].st_vert.push_back(combine[i].st_vert[j]);
		}
		
		for(int j = 0; j < combine[i].st_uv.size(); j++)
		{
			combine[0].st_uv.push_back(combine[i].st_uv[j]);
		}
		
		for(int j = 0; j < combine[i].st_norm.size(); j++)
		{
			combine[0].st_norm.push_back(combine[i].st_norm[j]);
		}
	}
	
	/*
	vector<glm::vec3> t_vert, t_norm;
	vector<glm::vec2> t_uv;
	
	indexVBO(combine[0].st_vert,
			 combine[0].st_uv,
			 combine[0].st_norm,
			 combine[0].st_indices,
			 t_vert,
			 t_uv,
			 t_norm);
	combine[0].st_vert = t_vert;
	combine[0].st_uv = t_uv;
	combine[0].st_norm = t_norm;
	*/
	
	return combine[0];
}

cube CreateOcto(float radius)
{
	cube cub;
	// Верх
    glm::vec3 v[6];
	v[0] = glm::vec3(0.0f, -radius, 0.0f);
    // Многоугольник посередине
    v[1] = glm::vec3(-radius, 0.0f, 0.0f);
    v[2] = glm::vec3(0.0f, 0.0f, -radius);
    v[3] = glm::vec3(+radius, 0.0f, 0.0f);
    v[4] = glm::vec3(0.0f, 0.0f, +radius);
    // Низ
    v[5] = glm::vec3(0.0f, radius, 0.0f);
	
	FOR(6)
	{
		cub.st_vert.push_back(v[i]);
	}
	
	FOR(cub.st_vert.size())
	{
		cub.st_norm.push_back(glm::normalize(cub.st_vert[i]));
	}
	
	FOR(cub.st_vert.size())
	{
		cub.st_uv.push_back(glm::vec2(rand()%2, rand()%2));
	}
	
	cub.st_indices.push_back(0);
	cub.st_indices.push_back(1);
	cub.st_indices.push_back(2);
	
	cub.st_indices.push_back(0);
	cub.st_indices.push_back(2);
	cub.st_indices.push_back(3);
	
	cub.st_indices.push_back(0);
	cub.st_indices.push_back(3);
	cub.st_indices.push_back(4);
	
	cub.st_indices.push_back(0);
	cub.st_indices.push_back(4);
	cub.st_indices.push_back(1);
	
	cub.st_indices.push_back(5);
	cub.st_indices.push_back(2);
	cub.st_indices.push_back(1);
	
	cub.st_indices.push_back(5);
	cub.st_indices.push_back(3);
	cub.st_indices.push_back(2);
	
	cub.st_indices.push_back(5);
	cub.st_indices.push_back(4);
	cub.st_indices.push_back(3);
	
	cub.st_indices.push_back(5);
	cub.st_indices.push_back(1);
	cub.st_indices.push_back(4);
	
    return cub;
}