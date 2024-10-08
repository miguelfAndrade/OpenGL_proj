#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>

#include "Renderer.h"

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {}

	// Receives a type and a count to add the VertexBufferElement
	void Push(unsigned int type, unsigned int count) {
		if(!count) {
			std::cout << "[!ERROR!] VertexBufferLayout::Push(unsigned int type, unsigned int count): count not initialized!\n";
			return;
		}

		switch (type)
		{
			case GL_FLOAT: 
				m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
				m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
				break;
			case GL_UNSIGNED_INT: 
				m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
				m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
				break;
			case GL_UNSIGNED_BYTE: 
				m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
				m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
				break;
			default:
				break;
		}
		return;
	}

	/*template<typename T> void PushT(unsigned) = delete;

	template<>
	void PushT<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void PushT<unsigned int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void PushT<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}*/

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
