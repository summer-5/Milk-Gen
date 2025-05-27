#ifndef JSONBRIDGE_H
#define JSONBRIDGE_H

#pragma once
#include "kindler.h"
#include "arrange.h"

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


struct Outscene
{
    std::string name = "border";
    std::vector<float> posx;
    std::vector<float> posy;
    Outscene() {}
    Outscene(const bool& bordertruebarrierfalse) 
    {
        if (bordertruebarrierfalse) name = "border";
        else name = "barrier";
    }
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Outscene, name, posx, posy)
};

extern float UNITSIDE;

bool ImportArea(const std::string&);
bool RealImportArea(const std::string&);
bool ExportArea(const std::string&);
bool ImportScene(const std::string&);
bool ExportScene(const std::string&);

#endif