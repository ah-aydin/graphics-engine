#include "LevelCubemap.h"

#ifdef GRAPHICS_API_OPENGL

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/version.h>
#include <stdexcept>

#include <Logging/Log.h>
#include <Engine/Utils/Bitmap.h>
#include <Engine/Utils/Cubemap.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <Engine/Settings.h>

#include <Engine/glApi/Rendering/Shaders/GLShader.h>

struct VertexData
{
	glm::vec3 pos;
	glm::vec3 n;
	glm::vec2 tc;
};

struct PerFrameData
{
	glm::mat4 model;
	glm::mat4 mvp;
	glm::vec4 cameraPos;
};

const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);

namespace kbb::glApi
{
	LevelCubemap::LevelCubemap()
	{
		GLShader shaderDuckVert("res/rubber_duck/shaders/cubemap.vert");
		GLShader shaderDuckFrag("res/rubber_duck/shaders/cubemap.frag");
		programDuck = new GLProgram(shaderDuckVert, shaderDuckFrag);
		programDuck->use();

		GLShader shaderCubeVert("res/cubemap/shader.vert");
		GLShader shaderCubeFrag("res/cubemap/shader.frag");
		programCube = new GLProgram(shaderCubeVert, shaderCubeFrag);

		glCreateBuffers(1, &perFrameDataBuffer);
		glNamedBufferStorage(perFrameDataBuffer, kUniformBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer, 0, kUniformBufferSize);

		const char* filePath = "res/rubber_duck/scene.gltf";
		const aiScene* scene = aiImportFile(filePath, aiProcess_Triangulate);
		if (!scene || !scene->HasMeshes())
		{
			log_error("ERROR::ASSIMP::ERROR_LOADING::%s", filePath);
			throw std::runtime_error("Failed to load assimp model");
		}

		const aiMesh* mesh = scene->mMeshes[0];
		std::vector<VertexData> vertices;
		for (unsigned i = 0; i != mesh->mNumVertices; i++)
		{
			const aiVector3D v = mesh->mVertices[i];
			const aiVector3D n = mesh->mNormals[i];
			const aiVector3D t = mesh->mTextureCoords[0][i];
			vertices.push_back({ glm::vec3(v.x, v.z, v.y), glm::vec3(n.x, n.y, n.z), glm::vec2(t.x, t.y) });
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
		{
			const char* path = "res/rubber_duck/textures/Duck_baseColor.png";
			int w, h, comp;
			const uint8_t* img = stbi_load("res/rubber_duck/textures/Duck_baseColor.png", &w, &h, &comp, 3);
			if (!img)
			{
				log_error("ERROR::STB::IMAGE_LOAD::%s", path);
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &txDuck);
			glTextureParameteri(txDuck, GL_TEXTURE_MAX_LEVEL, 0);
			glTextureParameteri(txDuck, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(txDuck, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureStorage2D(txDuck, 1, GL_RGB8, w, h);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(txDuck, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);
			glBindTextures(0, 1, &txDuck);

			stbi_image_free((void*)img);
		}

		{
			const char* path = "res/cubemap/piazza_bologni_1k.hdr";
			int w, h, comp;
			const float* img = stbi_loadf(path, &w, &h, &comp, 3);
			if (!img)
			{
				log_error("ERROR::STB::IMAGE_LOAD::%s", path);
			}
			Bitmap in(w, h, comp, eBitmapFormat_Float, img);
			Bitmap out = convertEquirectangularMapToVerticalCross(in);
			//stbi_write_hdr("screenshot.hdr", out.getWidth(), out.getHeight(), out.getComp(), reinterpret_cast<const float*>(out.m_data.data()));
			Bitmap cubemap = convertVerticalCrossToCubeMapFaces(out);

			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &txCubemap);
			glTextureParameteri(txCubemap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(txCubemap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameteri(txCubemap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureParameteri(txCubemap, GL_TEXTURE_BASE_LEVEL, 0);
			glTextureParameteri(txCubemap, GL_TEXTURE_MAX_LEVEL, 0);
			glTextureParameteri(txCubemap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(txCubemap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureStorage2D(txCubemap, 1, GL_RGB32F, cubemap.getWidth(), cubemap.getHeight());
			const uint8_t* data = cubemap.m_data.data();

			for (unsigned i = 0; i != 6; ++i)
			{
				glTextureSubImage3D(txCubemap, 0, 0, 0, i, cubemap.getWidth(), cubemap.getHeight(), 1, GL_RGB, GL_FLOAT, data);
				data += cubemap.getWidth() * cubemap.getHeight() * cubemap.getComp() * Bitmap::getBytesPerComponent(cubemap.getFormat());
			}
			glBindTextures(1, 1, &txCubemap);

			stbi_image_free((void*)img);
		}
	}

	LevelCubemap::~LevelCubemap()
	{
		glDeleteBuffers(1, &dataIndices);
		glDeleteBuffers(1, &dataVertices);
		glDeleteBuffers(1, &perFrameDataBuffer);
		glDeleteVertexArrays(1, &vao);
		glDeleteTextures(1, &txDuck);
		glDeleteTextures(1, &txCubemap);

		delete programCube, programDuck;
	}

	void LevelCubemap::update(double dt)
	{
		const glm::mat4 p = glm::perspective(45.0f, Settings::ratio, 0.1f, 1000.0f);

		{
			const glm::mat4 m = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -1.5f)), (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

			programDuck->use();
			const PerFrameData perFrameData = { m, p * m, glm::vec4(0.0f) };
			glNamedBufferSubData(perFrameDataBuffer, 0, kUniformBufferSize, &perFrameData);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned>(indices.size()), GL_UNSIGNED_INT, nullptr);
		}

		{
			const glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
			const PerFrameData perFrameData = { m, p * m, glm::vec4(0.0f) };
			glNamedBufferSubData(perFrameDataBuffer, 0, kUniformBufferSize, &perFrameData);
			programCube->use();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}
#endif