#include "bgpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Bagel/Renderer/RenderCommand.h"

#include <glm/gtx/transform.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace Bagel {
	struct Renderer2DData {
		Ref<Bagel::VertexArray> QuadVertexArray;
		Ref<Bagel::Shader> TextureShader;

		Ref<Bagel::Texture2D> WhiteTexture;
	};

	static Renderer2DData* _sData;

	void Renderer2D::Init()
	{
		BG_PROFILE_FUNCTION();

		_sData = new Renderer2DData();
		_sData->TextureShader = Shader::Create("Assets/Shaders/TextureShader.glsl");

		_sData->QuadVertexArray = VertexArray::Create();
		
		//Vertex Pos (X, Y, Z). Texture coordinate (U, V)
		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	//Bottom Left
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	//Bottom Right
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,	//Top Right
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f	//Top Left
		};
		
		uint32_t squareIndices[6] = {
			0,1,2,
			3,0,2
		};
		
		_sData->QuadVertexArray = VertexArray::Create();
		
		Ref<VertexBuffer> pSquareVertexBuffer;
		pSquareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		BufferLayout squareLayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TextureCoordinate"}
		};
		pSquareVertexBuffer->SetLayout(squareLayout);
		_sData->QuadVertexArray->AddVertexBuffer(pSquareVertexBuffer);
		
		Ref<IndexBuffer> pSquareIndexBuffer;
		pSquareIndexBuffer = IndexBuffer::Create(squareIndices, 6);
		_sData->QuadVertexArray->SetIndexBuffer(pSquareIndexBuffer);

		_sData->WhiteTexture = Texture2D::Create(1,1);

		uint32_t whiteTexData = 0xffffffff;
		_sData->WhiteTexture->SetData(&whiteTexData, sizeof(whiteTexData));
	}

	void Renderer2D::Shutdown()
	{
		BG_PROFILE_FUNCTION();
		delete _sData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		BG_PROFILE_FUNCTION();
		_sData->TextureShader->Bind();
		_sData->TextureShader->UploadUniformMat4("u_ViewProjection", camera.ViewProj());
		_sData->TextureShader->UploadUniformInt("u_Texture", 0);
	}
	
	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const float& rotation, const glm::vec4& color)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const float& rotation, const glm::vec4& color)
	{
		BG_PROFILE_FUNCTION();
		_sData->TextureShader->UploadUniformFloat4("u_Color", color);
		_sData->WhiteTexture->Bind(0);

		const static glm::mat4 baseMat = glm::mat4(1.0f);

		glm::mat4 modelMat = glm::translate(baseMat, pos) *
			glm::rotate(baseMat, glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(baseMat, { size.x, size.y, 0.0f });

		_sData->TextureShader->UploadUniformMat4("u_Model", modelMat);

		_sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_sData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const float& rotation, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad({ pos.x,pos.y, 0.0f }, size, rotation, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const float& rotation, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		BG_PROFILE_FUNCTION();
		_sData->TextureShader->UploadUniformFloat4("u_Color", color);
		texture->Bind(0);
		_sData->TextureShader->UploadUniformInt("u_Texture", 0);

		const static glm::mat4 baseMat = glm::mat4(1.0f);
		glm::mat4 modelMat = glm::translate(baseMat, pos) *
			glm::rotate(baseMat, glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(baseMat, { size.x, size.y, 0.0f });

		_sData->TextureShader->UploadUniformMat4("u_Model", modelMat);

		_sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_sData->QuadVertexArray);

		texture->Unbind();
	}
}