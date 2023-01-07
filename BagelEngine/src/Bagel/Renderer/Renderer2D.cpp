#include "bgpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Bagel/Renderer/RenderCommand.h"

#include <glm/gtx/transform.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace Bagel {
	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TextureSclaing;

		//Tex ID
		//MaskID
		//Light map data?
	};

	struct Renderer2DData {
		const uint32_t MAX_QUADS = 10000;
		const uint32_t MAX_VERTICES = MAX_QUADS * 4;
		const uint32_t MAX_INDICES = MAX_QUADS * 6;
		static const uint32_t MAX_TEXTURE_SLOTS = 32;

		Ref<Bagel::VertexArray> QuadVertexArray;
		Ref<Bagel::VertexBuffer> QuadVertexBuffer;
		Ref<Bagel::Shader> Shader;
		Ref<Bagel::Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> TextureSlots;
		uint32_t TextureSlotIndex = 1; //0 = white texture;
	};

	static Renderer2DData _sData;



	void Renderer2D::Init()
	{
		BG_PROFILE_FUNCTION();

		_sData.Shader = Shader::Create("Assets/Shaders/TextureShader.glsl");
		_sData.Shader->Bind();


		_sData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexData = 0xffffffff;
		_sData.WhiteTexture->SetData(&whiteTexData, sizeof(whiteTexData));
		_sData.TextureSlots[0] = _sData.WhiteTexture;

		_sData.QuadVertexArray = VertexArray::Create();

		int32_t samplers[_sData.MAX_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < _sData.MAX_TEXTURE_SLOTS; i++) {
			samplers[i] = i;
		}
		_sData.Shader->UploadUniformIntArray("u_Textures", samplers, _sData.MAX_TEXTURE_SLOTS);

		_sData.QuadVertexBuffer = VertexBuffer::Create(_sData.MAX_VERTICES * sizeof(QuadVertex));
		BufferLayout quadLayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TextureCoordinate"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TextureScaling"}
		};
		_sData.QuadVertexBuffer->SetLayout(quadLayout);
		_sData.QuadVertexArray->AddVertexBuffer(_sData.QuadVertexBuffer);

		_sData.QuadVertexBufferBase = new QuadVertex[_sData.MAX_VERTICES];

		uint32_t* quadIndices = new uint32_t[_sData.MAX_INDICES];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < _sData.MAX_INDICES; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> pQuadIB;
		pQuadIB = IndexBuffer::Create(quadIndices, _sData.MAX_INDICES);
		_sData.QuadVertexArray->SetIndexBuffer(pQuadIB);
		delete[] quadIndices;
	}

	void Renderer2D::Shutdown()
	{
		BG_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		BG_PROFILE_RENDERER_FUNCTION();
		_sData.Shader->Bind();
		_sData.Shader->UploadUniformMat4("u_ViewProjection", camera.ViewProj());
		//_sData.Shader->UploadUniformIntArray("u_Textures", 0, _sData.TextureSlotIndex);

		_sData.TextureSlotIndex = 1; //0 is white texture

		_sData.QuadIndexCount = 0;
		_sData.QuadVertexBufferPtr = _sData.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		BG_PROFILE_RENDERER_FUNCTION();

		uint32_t dataSize = (uint8_t*)_sData.QuadVertexBufferPtr - (uint8_t*)_sData.QuadVertexBufferBase;
		_sData.QuadVertexBuffer->SetData(_sData.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		BG_PROFILE_RENDERER_FUNCTION();

		for (uint32_t i = 0; i < _sData.TextureSlotIndex; i++) {
			_sData.TextureSlots[i]->Bind(i);
		}

		_sData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_sData.QuadVertexArray, _sData.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const float& rotation, const glm::vec4& color)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, size, rotation, color);
	}

	//TODO: Batch this
	void Renderer2D::DrawQuad(const Transform& trans, const glm::vec4& color)
	{
		BG_PROFILE_RENDERER_FUNCTION();
		_sData.Shader->UploadUniformFloat4("u_Color", color);
		_sData.WhiteTexture->Bind(0);

		_sData.Shader->UploadUniformMat4("u_Model", trans.transformMat);

		_sData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_sData.QuadVertexArray);
	}

	//TODO: Batch this
	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const float& rotation, const glm::vec4& color)
	{
		BG_PROFILE_RENDERER_FUNCTION();
		_sData.Shader->UploadUniformFloat4("u_Color", color);
		_sData.WhiteTexture->Bind(0);

		const static glm::mat4 baseMat = glm::mat4(1.0f);

		glm::mat4 modelMat = glm::translate(baseMat, pos) *
			glm::rotate(baseMat, glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(baseMat, { size.x, size.y, 0.0f });

		_sData.Shader->UploadUniformMat4("u_Model", modelMat);

		_sData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_sData.QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const float& rotation, const Ref<Texture2D>& texture, const glm::vec4& color, const float& textureScaling)
	{
		DrawQuad({ pos.x,pos.y, 0.0f }, size, rotation, texture, color, textureScaling);
	}

	//TODO: Batch this
	void Renderer2D::DrawQuad(const Transform& trans, const Ref<Texture2D>& texture, const glm::vec4& color, const float& textureScaling)
	{
		BG_PROFILE_RENDERER_FUNCTION();
		//_sData.Shader->UploadUniformMat4("u_Model", trans.transformMat);
		_sData.Shader->UploadUniformFloat4("u_Color", color);

		texture->Bind(0);
		_sData.Shader->UploadUniformInt("u_Texture", 0);
		_sData.Shader->UploadUniformFloat("u_TextureScaling", textureScaling);

		_sData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_sData.QuadVertexArray);
	}

	//TODO: Batch this
	void Renderer2D::DrawQuad(const Transform& trans, const DrawInfo& drawInfo)
	{
		BG_PROFILE_RENDERER_FUNCTION();

		_sData.Shader->UploadUniformMat4("u_Model", trans.transformMat);

		Ref<Texture2D> t = drawInfo.texture == nullptr ? _sData.WhiteTexture : drawInfo.texture;
		t->Bind(0);
		_sData.Shader->UploadUniformInt("u_Texture", 0);

		_sData.Shader->UploadUniformFloat4("u_Color", drawInfo.color);
		_sData.Shader->UploadUniformFloat("u_TextureScaling", drawInfo.textureScale);

		_sData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_sData.QuadVertexArray);
		t->Unbind();
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const float& rotation, const Ref<Texture2D>& texture, const glm::vec4& color, const float& textureScaling)
	{
		BG_PROFILE_RENDERER_FUNCTION();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < _sData.TextureSlotIndex; i++) {
			if (*_sData.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)_sData.TextureSlotIndex;
			_sData.TextureSlots[_sData.TextureSlotIndex] = texture;
			_sData.TextureSlotIndex++;
		}

		//BL
		_sData.QuadVertexBufferPtr->Position = pos;
		_sData.QuadVertexBufferPtr->Color = color;
		_sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		_sData.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
		_sData.QuadVertexBufferPtr->TextureSclaing = textureScaling;
		_sData.QuadVertexBufferPtr++;

		//BR
		_sData.QuadVertexBufferPtr->Position = {pos.x + size.x, pos.y, pos.z};
		_sData.QuadVertexBufferPtr->Color = color;
		_sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		_sData.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		_sData.QuadVertexBufferPtr->TextureSclaing = textureScaling;
		_sData.QuadVertexBufferPtr++;

		//TR
		_sData.QuadVertexBufferPtr->Position = {pos.x + size.x, pos.y + size.y, pos.z};
		_sData.QuadVertexBufferPtr->Color = color;
		_sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		_sData.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		_sData.QuadVertexBufferPtr->TextureSclaing = textureScaling;
		_sData.QuadVertexBufferPtr++;

		//TL
		_sData.QuadVertexBufferPtr->Position = {pos.x, pos.y + size.y, pos.z};
		_sData.QuadVertexBufferPtr->Color = color;
		_sData.QuadVertexBufferPtr->TexIndex = textureIndex;
		_sData.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		_sData.QuadVertexBufferPtr->TextureSclaing = textureScaling;
		_sData.QuadVertexBufferPtr++;

		_sData.QuadIndexCount += 6;

		//_sData.Shader->UploadUniformFloat4("u_Color", color);
		//texture->Bind(0);
		//_sData.Shader->UploadUniformInt("u_Texture", 0);
		//_sData.Shader->UploadUniformFloat("u_TextureScaling", textureScaling);
		//const static glm::mat4 baseMat = glm::mat4(1.0f);
		//glm::mat4 modelMat = glm::translate(baseMat, pos) *
		//	glm::rotate(baseMat, glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
		//	glm::scale(baseMat, { size.x, size.y, 0.0f });
		//
		//_sData.Shader->UploadUniformMat4("u_Model", modelMat);
		//
		//_sData.QuadVertexArray->Bind();
		//RenderCommand::DrawIndexed(_sData.QuadVertexArray);

		texture->Unbind();

	}
}