#ifndef JSONBRIDGE_H
#define JSONBRIDGE_H
#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include "nlohmann/json.hpp"

constexpr float PI = 3.1415926f;

struct Oriarea
{
    int kk = 0;
    float sizex = 5.0f;
    float sizey = 5.0f;
    float dist = 0.0f;
    float deflex = 0.0f;
    float rot = 0.0f;
    int tshow = 0;
    int tlast = 10;
    bool ishead = true;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Oriarea, kk, sizex, sizey, dist, deflex, rot, tshow, tlast, ishead)
};
struct Playtime
{
    int kk = 0;
    std::vector<float> early;
    std::vector<float> late;
    float enter = 0.0f;
    Playtime() {}
    Playtime(int K, float E) :kk(K), enter(E) {}
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Playtime, kk, early, late, enter)
};

bool ExportArea(const std::string&, const std::vector<Oriarea>&);
bool TooNB(const float&, const float&);
bool Righto(const float&);

#endif