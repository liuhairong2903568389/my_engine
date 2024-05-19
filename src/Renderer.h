#pragma once
#include<vector>
#include<iostream>
#include<unordered_map>
#include"GLEW/glew.h"

namespace my_engine {
	struct layout {
		unsigned int count;
		unsigned int type;
		unsigned int normalized;
		unsigned int get_offset() {
			switch (type) {
			case GL_FLOAT: return  4;
			case GL_UNSIGNED_INT:  4;
			case GL_UNSIGNED_BYTE:  1;
			}
			return 0;
		}
	};

	class Renderer {

	};
	class VertexBuffer {
	public:
		VertexBuffer(const float *,unsigned int size);
		~VertexBuffer();
		void bind()const;
		void unbind()const;
	private:
		unsigned int m_RenderId;
	};

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		template<typename T>
		void layout_push(unsigned int count) {
			std::cout << "test" << std::endl;
		}
		template<>
		void layout_push<float>(unsigned int count) {
			layout.push_back({ count,GL_FLOAT,GL_FALSE });
			layout_stride += count * 4;
		}
		template<>
		void layout_push<unsigned int>(unsigned int count) {
			layout.push_back({ count,GL_UNSIGNED_INT,GL_FALSE });
			layout_stride += count * 4;
		}
		template<>
		void layout_push<unsigned char>(unsigned int count) {
			layout.push_back({ count,GL_UNSIGNED_BYTE,GL_FALSE });
			layout_stride += count * 1;

		}
		void bind()const;
		void unbind()const;
		void addLayout();

	private:
		unsigned int m_RenderId;
		std::vector<layout> layout;
		unsigned int layout_stride;
	};
	class IndexBuffer {
	public:
		IndexBuffer(const unsigned int*, unsigned int size);
		~IndexBuffer();
		void bind()const;
		void unbind()const;
	private:
		unsigned int m_Renderer;
		
	};
	class Shader {
	public:
		Shader(const std::string& filepath);
		~Shader();
		void bind();
		void unbind();
		void Dealwith_ShaderSource(const std::string& filepath);
		void SetUniform1f(const std::string&,float);
		void SetUniform1i(const std::string&, int);

	private:
		unsigned int Vs_Id,Fs_Id,Program_Id;
		std::string filepath;
		std::unordered_map<std::string, int> location_cache;
		int getuniformlocation(const std::string&);
		
	};
	class Texture {
	public:
		Texture(const std::string&);
		~Texture();
		void bind(unsigned int solt = 0);
		void unbind();

	private:
		unsigned int m_renderId;
		int width, height,bmp;

	};
	
}