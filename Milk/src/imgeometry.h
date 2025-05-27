#ifndef IMGEOMETRY_H
#define IMGEOMETRY_H

#pragma once
#include "transformation.h"

#define EPS 0.001f

constexpr int LEVELCALCULATEDMAX = 30;
constexpr int ACREAGE = 900;
constexpr ImU32 TRANSPARENT = IM_COL32(255, 255, 255, 127);
constexpr ImU32 BLUE0 = IM_COL32(64, 86, 141, 255);
constexpr ImU32 BLUE1 = IM_COL32(204, 213, 240, 255);
constexpr ImU32 RED0 = IM_COL32(141, 64, 86, 255);
constexpr ImU32 RED1 = IM_COL32(240, 204, 213, 255);
constexpr ImU32 DARK0 = IM_COL32(26, 26, 26, 255);
constexpr ImU32 DARK1 = IM_COL32(100, 100, 100, 255);
constexpr ImU32 COLOR0 = IM_COL32(255, 255, 255, 255);
constexpr ImU32 COLOR1 = IM_COL32(255, 0, 0, 255);
constexpr ImU32 COLOR2 = IM_COL32(0, 255, 0, 255);
constexpr ImU32 COLOR3 = IM_COL32(0, 0, 255, 255);
constexpr ImU32 START0 = IM_COL32(51, 230, 255, 255);
constexpr ImU32 START1 = IM_COL32(153, 255, 255, 255);
constexpr ImU32 END0 = IM_COL32(255, 0, 153, 255);
constexpr ImU32 END1 = IM_COL32(255, 153, 180, 255);

ImVec2 operator+(const ImVec2& a, const ImVec2& b);
ImVec2 operator-(const ImVec2& a, const ImVec2& b);
ImVec2 operator*(const float& a, const ImVec2& b);
ImVec2 operator*(const ImVec2& a, const float& b);
ImVec2 operator/(const ImVec2& a, const float& b);
ImVec2 operator~(const ImVec2& a);
std::ostream& operator<<(std::ostream& os, const ImVec2& a);
float Cro(const ImVec2&, const ImVec2&);
float Dot(const ImVec2&, const ImVec2&);

struct ImInt2
{
    int x = 0;
    int y = 0;
    ImInt2() {}
    ImInt2(int xx, int yy) : x(xx), y(yy) {}
};
struct ImLine
{
    ImVec2 a;
    ImVec2 b;
    ImVec2 d;
    ImLine() {};
    ImLine(const ImVec2& A, const ImVec2& B) : a(A), b(B), d(B - A) {}
};
struct ImTri
{
    ImVec2 a;
    ImVec2 b;
    ImVec2 c;
    ImVec2 ab;
    ImVec2 bc;
    ImVec2 ca;
    ImTri(const ImVec2& A, const ImVec2& B, const ImVec2& C) : a(A), b(B), c(C), ab(B - A), bc(C - B), ca(A - C) {}
};
struct Area
{
    int kk = 0;
    ImVec2 pos = ImVec2(0, 0);
    ImVec2 clickpos = ImVec2(0, 0);
    ImVec2 size = ImVec2(5, 5);
    float deflex = 0.0f;
    float dist = 5.0f; // constraint but not fact
    float rot = 0.0f;
    int tshow = 0;
    int tlast = 10;
    bool ishead = true;
    bool dirsolid = false;
    ImVec2 dirc = ImVec2(1, 0);
};
struct ImRect
{
    ImVec2 a;
    ImVec2 b;
    ImVec2 c;
    ImVec2 d;
    ImLine ab;
    ImLine bc;
    ImLine cd;
    ImLine da;
    ImRect(const Area& ar, const float& sc)
    {
        float rot = ar.rot * PI / 180;
        float ta = atanf(ar.size.y / ar.size.x) - rot;
        float tb = PI - ta - rot * 2;
        float tc = ta + PI;
        float td = tb + PI;
        float r = sc * sqrt(ar.size.x * ar.size.x + ar.size.y * ar.size.y) / 2;
        a = ImVec2(r * cosf(ta), r * sinf(ta)) + ar.pos;
        b = ImVec2(r * cosf(tb), r * sinf(tb)) + ar.pos;
        c = ImVec2(r * cosf(tc), r * sinf(tc)) + ar.pos;
        d = ImVec2(r * cosf(td), r * sinf(td)) + ar.pos;
        ab = ImLine(a, b);
        bc = ImLine(b, c);
        cd = ImLine(c, d);
        da = ImLine(d, a);
    }
};
struct Border
{
    std::vector<ImVec2> vert;
    std::vector<ImTri> tri;
    std::vector<ImLine> line;
    int acreage = ACREAGE;
    int screenAcreage = 0;
    void Reset(const int& ac, const int& scAc) 
    { 
        vert.clear(); line.clear(); tri.clear();
        acreage = ac; screenAcreage = scAc; 
    }
    void Addvert(const ImVec2& ve)
    {
        if (vert.size() > 0) Addline(ImLine(vert.back(), ve));
        vert.push_back(ve);
    }
    void Deletevert(const int& dnum, const int& num)
    {
        for (int i = dnum; i < num - 1; i++)
            vert[i] = vert[i + 1];
        vert.pop_back();
    }
    void Addline(const ImLine& li)
    {
        line.push_back(li);
    }
    void Addtri(const ImTri& tr)
    {
        tri.push_back(tr);
    }
};
struct Pixcol
{
    int r;
    int g;
    int b;
    int a;
    Pixcol(const int& R, const int& G, const int& B, const int& A) : r(R), g(G), b(B), a(A) {}
    bool SameAs(const Pixcol& other)
    {
        if (r == other.r && g == other.g && b == other.b && a == other.a) return true;
        else return false;
    }
    bool IsWhite()
    {
        if (r > 215 && g > 215 && b > 215 && a > 250) return true;
        else return false;
    }
    bool IsColorful()
    {
        if ((r - g) * (r - g) + (g - b) * (g - b) + (r - b) * (r - b) > 4500) return true;
        else return false;
    }
    bool IsDark()
    {
        if (r < 64 && g < 64 && b < 64) return true;
        else return false;
    }
};

ImInt2 operator+(const ImInt2& a, const ImInt2& b);
ImInt2 operator-(const ImInt2& a, const ImInt2& b);
ImInt2 operator*(const int& a, const ImInt2& b);
ImInt2 operator*(const ImInt2& a, const int& b);
ImInt2 operator/(const ImInt2& a, const int& b);
bool operator==(const ImInt2& a, const ImInt2& b);
ImVec2 operator^(const ImLine& a, const ImLine& b);
std::ostream& operator<<(std::ostream& os, const ImInt2& a);
int Cro(const ImInt2&, const ImInt2&);
int Dot(const ImInt2&, const ImInt2&);

extern float PUTUP, scale;
extern int CANVASX, CANVASY;
extern Border border;
extern std::vector<Border> barrierlist;

bool IsLawayL(const ImLine&, const ImLine&, const int&);
bool IsPinT(const ImVec2&, const ImTri&, const int&);
bool IsPinL(const ImVec2&, const ImLine&, const int&);
bool IsAinB(const Area&, const int&); // in boder and not in barrier
bool IsPinB(const ImVec2&, const Border&, const int&); // in a border or a barrier
bool IsPinB(const ImVec2&, const int&); // in the border without barrier 
bool IsPinA(const ImVec2&, const Area&, const int&);
bool IsAonA(const Area&, const Area&, const int&);
ImVec2 AAholdAB(const Area& a, const Area& b);
ImVec2 AAseeAB(const Area& a, const Area& b);
ImU32 LerpColor(const ImU32&, const ImU32&, const float&);
ImU32 LerpColor(const ImU32&, const float&);

#endif