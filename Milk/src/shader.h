#ifndef SHADER_H
#define SHADER_H

#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Eigen/Dense"
#include "nlohmann/json.hpp"

constexpr double PI = 3.1415926535;

#define vert_type 0
#define frag_type 1
#define prog_type 2

class Shader {
public:
    Shader(const char *vert_shader_path, const char *frag_shader_path);
    ~Shader();
    void use_program();
    void delete_program();
    uint32_t get_id();
private:
    uint32_t id;
    void check_compile_errors(uint32_t shader, int type);
};

#endif // SHADER_H
