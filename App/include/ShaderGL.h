#pragma once

#include <QOpenGLFunctions>

#include <iostream>

class ShaderGL
{
public:
    static unsigned int compileShader(unsigned int type, const std::string& source);

    static int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};