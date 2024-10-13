#include <ShaderGl.h>

unsigned int ShaderGL::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length , &length, message);
        qDebug() << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!";
        qDebug() << message;
        glDeleteShader(id);
    }

    return id;
    
}

int ShaderGL::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = ShaderGL::compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = ShaderGL::compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
}
