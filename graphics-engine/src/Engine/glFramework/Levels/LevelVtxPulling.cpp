#include "LevelVtxPulling.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Logging/Log.h>

struct VertexData
{
	glm::vec3 pos;
	glm::vec2 tc;
};

struct PerFrameData
{
	glm::mat4 mvp;
};

const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);

LevelVtxPulling::LevelVtxPulling()
{

}

LevelVtxPulling::~LevelVtxPulling()
{
	glDeleteBuffers(1, &dataIndices);
	glDeleteBuffers(1, &dataVertices);
	glDeleteBuffers(1, &perFrameDataBuffer);
	glDeleteVertexArrays(1, &vao);

	delete shaderVert, shaderGeom, shaderFrag, program;
}


bool LevelVtxPulling::init()
{
	shaderVert = new GLShader("res/rubber_duck/shaders/shader.vert");
	shaderGeom = new GLShader("res/rubber_duck/shaders/shader.geom");
	shaderFrag = new GLShader("res/rubber_duck/shaders/shader.frag");
	program = new GLProgram(*shaderVert, *shaderGeom, *shaderFrag);
	program->use();

	glCreateBuffers(1, &perFrameDataBuffer);
	glNamedBufferStorage(perFrameDataBuffer, kUniformBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer, 0, kUniformBufferSize);

	const char* filePath = "res/rubber_duck/scene.gltf";
	const aiScene* scene = aiImportFile(filePath, aiProcess_Triangulate);
	if (!scene || !scene->HasMeshes())
	{
		log_error("ERROR::ASSIMP::ERROR_LOADING::%s", filePath);
		return false;
	}

	const aiMesh* mesh = scene->mMeshes[0];
	std::vector<VertexData> vertices;
	for (unsigned i = 0; i != mesh->mNumVertices; i++)
	{
		const aiVector3D v = mesh->mVertices[i];
		const aiVector3D t = mesh->mTextureCoords[0][i];
		vertices.push_back({ glm::vec3(v.x, v.z, v.y), glm::vec2(t.x, t.y) });
	}
	for (unsigned i = 0; i != mesh->mNumFaces; i++)
	{
		for (unsigned j = 0; j != 3; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);
	}
	aiReleaseImport(scene);

	const size_t kSizeIndices = sizeof(unsigned int) * indices.size();
	const size_t kSizeVertices = sizeof(VertexData) * vertices.size();
	
	// Indicies
	glCreateBuffers(1, &dataIndices);
	glNamedBufferStorage(dataIndices, kSizeIndices, indices.data(), 0);
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexArrayElementBuffer(vao, dataIndices);
	
	// Verticies
	glCreateBuffers(1, &dataVertices);
	glNamedBufferStorage(dataVertices, kSizeVertices, vertices.data(), 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataVertices);

	// Texture
	int w, h, comp;
	const uint8_t* img = stbi_load("res/rubber_duck/textures/Duck_baseColor.png", &w, &h, &comp, 3);

	glCreateTextures(GL_TEXTURE_2D, 1, &tx);
	glTextureParameteri(tx, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(tx, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(tx, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(tx, 1, GL_RGB8, w, h);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTextureSubImage2D(tx, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTextures(0, 1, &tx);

	stbi_image_free((void *)img);

	return true;
}


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <Engine/Settings.h>

void LevelVtxPulling::update(float dt)
{
	Level::update(dt);

	const glm::mat4 m = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -1.5f)), (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 p = glm::perspective(45.0f, Settings::ratio, 0.1f, 1000.0f);

	const PerFrameData perFrameData = { p * m };
	glNamedBufferSubData(perFrameDataBuffer, 0, kUniformBufferSize, &perFrameData);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned>(indices.size()), GL_UNSIGNED_INT, nullptr);
}