#include"Renderer.h"
#include"MyLog.h"
#include<fstream>
#include<string>
#include<iostream>
#include"../stb_image/stb_image.h"

namespace my_engine{
	VertexBuffer::VertexBuffer(const float* position,unsigned int size)
	{
		CheckGL(glGenBuffers(1, &m_RenderId));
		CheckGL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderId));
		CheckGL(glBufferData(GL_ARRAY_BUFFER, size, position, GL_STATIC_DRAW));
	}
	VertexBuffer::~VertexBuffer()
	{
	}
	void VertexBuffer::bind()const
	{
		CheckGL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderId));
	}
	void VertexBuffer::unbind()const
	{
		CheckGL(glBindBuffer(GL_ARRAY_BUFFER,0));
	}


	IndexBuffer::IndexBuffer(const unsigned int* index, unsigned int size)
	{
		CheckGL(glGenBuffers(1, &m_Renderer));
		CheckGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer));
		CheckGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, index, GL_STATIC_DRAW));
	}
	IndexBuffer::~IndexBuffer()
	{
	}
	void IndexBuffer::bind()const
	{
		CheckGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer));
	}
	void IndexBuffer::unbind()const
	{
		CheckGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
	VertexArray::VertexArray():layout_stride(0)
	{
		CheckGL(glGenVertexArrays(1, &m_RenderId));
		CheckGL(glBindVertexArray(m_RenderId));
		
		
	}
	VertexArray::~VertexArray()
	{
	}
	void VertexArray::bind() const
	{
		CheckGL(glBindVertexArray(m_RenderId));
	}
	void VertexArray::unbind() const
	{
		CheckGL(glBindVertexArray(0));
	}
	void VertexArray::addLayout()
	{
		unsigned int offset = 0;
		for (unsigned int i = 0; i<layout.size(); i++) {
			auto get_layout = layout[i];
			CheckGL(glEnableVertexAttribArray(i));
			CheckGL(glVertexAttribPointer(i, get_layout.count, get_layout.type, get_layout.normalized, layout_stride, (const void*)offset));
			offset += get_layout.count * get_layout.get_offset();
		}
	}
	
	Shader::Shader(const std::string& filepath):filepath(filepath)
	{
		CheckGL(Vs_Id = glCreateShader(GL_VERTEX_SHADER));
		CheckGL(Fs_Id = glCreateShader(GL_FRAGMENT_SHADER));
		Dealwith_ShaderSource(filepath);

		int success;
		char infoLog[512];
		CheckGL(glGetShaderiv(Vs_Id, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			CheckGL(glGetShaderInfoLog(Vs_Id, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		CheckGL(glGetShaderiv(Fs_Id, GL_COMPILE_STATUS, &success));
		if (!success) {
			CheckGL(glGetShaderInfoLog(Fs_Id, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		CheckGL(Program_Id = glCreateProgram());
		CheckGL(glAttachShader(Program_Id, Vs_Id));
		CheckGL(glAttachShader(Program_Id, Fs_Id));
		CheckGL(glLinkProgram(Program_Id));

		CheckGL(glGetProgramiv(Program_Id, GL_LINK_STATUS, &success));
		if (!success) {
			CheckGL(glGetProgramInfoLog(Program_Id, 512, NULL, infoLog));
			std::cout << "Linked Error" << std::endl;
		}
		CheckGL(glUseProgram(Program_Id));
		CheckGL(glDeleteShader(Vs_Id));
		CheckGL(glDeleteShader(Fs_Id));
	}

	Shader::~Shader()
	{

	}

	void Shader::bind()
	{
		CheckGL(glUseProgram(Program_Id));
	}

	void Shader::unbind()
	{
		CheckGL(glUseProgram(0));
	}

	void Shader::Dealwith_ShaderSource(const std::string& file)
	{
		std::string VertexShader,FragemntShader;
		std::fstream shadersource(file);
		if (shadersource.is_open()) {
			std::string line;
			int shadertype = -1;
			while(std::getline(shadersource, line)){
				//std::cout << line << std::endl;
				if(line.find("#shader")!=std::string::npos)
				{
					if (line.find("vertex")!=std::string::npos) {
						shadertype = 0;
						std::cout << "set vertexshader type\n";
					}
					else if (line.find("fragment")!=std::string::npos) {
						shadertype = 1;
						std::cout << "set fragment type\n";
					}
				}
				else 
				{
					if (shadertype == -1) {
						std::cout << "this line is not shadersource\n";
					}
					else if (shadertype == 0) {
						VertexShader.append(line);
						VertexShader.append("\n");
						std::cout << "vertexshader append\n";
					}
					else if (shadertype == 1) {
						FragemntShader.append(line);
						FragemntShader.append("\n");
						std::cout << "fragmentshader append\n";
					}
				}
			}
			std::cout <<"this is a test" << VertexShader << std::endl;
			std::cout << "this is another test" << FragemntShader << std::endl;
			const char* vs = VertexShader.c_str();
			const char* fs = FragemntShader.c_str();
			CheckGL(glShaderSource(Vs_Id, 1, &vs, nullptr));
			CheckGL(glShaderSource(Fs_Id, 1, &fs, nullptr));
			
		}
		else {
			std::cout << "file exist\n";
		}

		CheckGL(glCompileShader(Vs_Id));
		CheckGL(glCompileShader(Fs_Id));

	}

	void Shader::SetUniform1f(const std::string& valuename, float value)
	{
		CheckGL(glUniform1f(getuniformlocation(valuename), value));
	}

	void Shader::SetUniform1i(const std::string&valuename, int value)
	{
		CheckGL(glUniform1i(getuniformlocation(valuename), value));
	}

	int Shader::getuniformlocation(const std::string& valuename)
	{
		if (location_cache.find(valuename) != location_cache.end()) {
			return location_cache[valuename];
		}
		else{
			int location;
			CheckGL(location = glGetUniformLocation(Program_Id, valuename.c_str()));
			location_cache.emplace(valuename, location);
			return location;
		}
	}

	Texture::Texture(const std::string& filename):width(0),height(0),bmp(0)
	{
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &bmp, 4);
		CheckGL(glGenTextures(1, &m_renderId));
		CheckGL(glBindTexture(GL_TEXTURE_2D, m_renderId));
		CheckGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		CheckGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		CheckGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		CheckGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		CheckGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		//CheckGL(glActiveTexture(GL_TEXTURE0));
		//CheckGL(glGenerateMipmap(GL_TEXTURE_2D));
		if (data) {
			stbi_image_free(data);
		}
	}

	Texture::~Texture()
	{
		CheckGL(glDeleteTextures(1, &m_renderId));
	}

	void Texture::bind(unsigned int solt)
	{
		CheckGL(glActiveTexture(GL_TEXTURE0 + solt));
		CheckGL(glBindTexture(GL_TEXTURE_2D, m_renderId));
	}

	void Texture::unbind()
	{
		CheckGL(glBindTexture(GL_TEXTURE_2D, 0));
	}

}