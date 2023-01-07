#pragma once

namespace Bagel {
	//Holds a generic name for each data type shaders can hold. This is needed as the same data types will have different names in different APIS
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4, 
		Mat3, Mat4,
		Bool
	};

	//Returns the size in bytes of a ShaderDataType
	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::None:		return 0;
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		BG_CORE_ASSERT(false, "Unknown ShaderDataType!");

		return 0;
	}

	static ShaderDataType StringToDataType(const std::string& typeName) {
		if (typeName == "float") return ShaderDataType::Float;
		else if (typeName == "vec2") return ShaderDataType::Float2;
		else if (typeName == "vec3") return ShaderDataType::Float3;
		else if (typeName == "vec4") return ShaderDataType::Float4;
		else if (typeName == "int") return ShaderDataType::Int;
		else if (typeName == "ivec2") return ShaderDataType::Int2;
		else if (typeName == "ivec3") return ShaderDataType::Int3;
		else if (typeName == "ivec4") return ShaderDataType::Int4;
		else if (typeName == "mat3") return ShaderDataType::Mat3;
		else if (typeName == "mat4") return ShaderDataType::Mat4;
		else if (typeName == "bool") return ShaderDataType::Bool;
		else if (typeName == "sampler2D") return ShaderDataType::Int;

		BG_CORE_ASSERT(false, "Unknown Type!");

		return ShaderDataType::None;
	}

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), 
			Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		//Returns the amount of components this data type has. E.g. a Float3 has 3 components.
		uint32_t GetComponentCount() const {
			switch (Type) {
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
			}

			BG_CORE_ASSERT(false, "Unknown ShaderDataType!");

			return 0;
		}
	};

	//Abstract class for representing a BufferLayout across different APIs
	class BufferLayout {
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: _elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		//Getters
		inline const std::vector<BufferElement>& GetElements() const { return _elements; }
		inline const uint32_t GetStride() const { return _stride; }

		//Iterator Functions. Allows the use of for range loops 
		std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
		std::vector<BufferElement>::iterator end() { return _elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

	private:
		void CalculateOffsetsAndStride() {
			//Rest stride in the event of this Buffer being reused
			_stride = 0;

			//Offset will hold the accumulated offset in bytes
			uint32_t offset = 0;
			for (auto& elem : _elements) {
				elem.Offset = offset;
				offset += elem.Size;
				_stride += elem.Size;
			}
		}

		std::vector<BufferElement> _elements;
		uint32_t _stride = 0;
	};

	//Abstract class for representing a VertexBuffer across different APIs
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		//Handles creating a VertexBuffer in the desired API
		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	//Abstract class for representing a IndexBuffer across different APIs
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		//Handles creating a IndexBuffer in the desired API
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

	};
}