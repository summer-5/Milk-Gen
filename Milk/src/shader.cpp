#include "shader.h"

//Shader::Shader(const char *vert_shader_path, const char *frag_shader_path) {
Shader::Shader(const char* vert_shader_code_cstr, const char* frag_shader_code_cstr) {
    /*std::ifstream vert_shader_file;
    std::ifstream frag_shader_file;
    std::string vert_shader_code;
    std::string frag_shader_code;

    vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vert_shader_file.open(vert_shader_path);
        frag_shader_file.open(frag_shader_path);

        std::stringstream vert_shader_stream;
        std::stringstream frag_shader_stream;
        vert_shader_stream << vert_shader_file.rdbuf();
        frag_shader_stream << frag_shader_file.rdbuf();

        vert_shader_file.close();
        frag_shader_file.close();

        vert_shader_code = vert_shader_stream.str();
        frag_shader_code = frag_shader_stream.str();
    } catch (std::ifstream::failure e) {
        std::cerr << "[E] Failed to read shader file." << std::endl;
    }

    const char *vert_shader_code_cstr = vert_shader_code.c_str();
    const char *frag_shader_code_cstr = frag_shader_code.c_str();*/
    
    // vertex shader
    uint32_t vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_code_cstr, nullptr);
    glCompileShader(vert_shader);
    check_compile_errors(vert_shader, vert_type);

    // fragment shader
    uint32_t frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_code_cstr, nullptr);
    glCompileShader(frag_shader);
    check_compile_errors(frag_shader, frag_type);

    // shader program
    id = glCreateProgram();
    glAttachShader(id, vert_shader);
    glAttachShader(id, frag_shader);
    glLinkProgram(id);
    check_compile_errors(id, prog_type);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

Shader::~Shader() = default;

void Shader::check_compile_errors(uint32_t shader, int type) {
    int32_t success;
    char info_log[512];

    if (type != prog_type) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            std::cerr << "[E] Shader compilation failed : " << info_log << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, info_log);
            std::cerr << "[E] Shader program linking failed : " << info_log << std::endl;
        }
    }
}

void Shader::use_program() {
    glUseProgram(id);
}

void Shader::delete_program() {
    glDeleteProgram(id);
}

uint32_t Shader::get_id() {
    return id;
}
