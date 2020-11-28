#pragma once
#include "../stdafx.h"

class Shader
{
public:
    unsigned int ID;
    ~Shader();
public:
    void Load(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);
    void Use();

    template<typename T>
    void Set(const std::string& name, const T& value) const;
private:
    void CheckCompileErrors(GLuint shader, std::string type);
};
