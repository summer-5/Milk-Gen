#include "imgeometry.h"

ImVec2 operator+(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x + b.x, a.y + b.y);
}
ImVec2 operator-(const ImVec2& a, const ImVec2& b)
{
    return ImVec2(a.x - b.x, a.y - b.y);
}
ImVec2 operator*(const float& a, const ImVec2& b)
{
    return ImVec2(a * b.x, a * b.y);
}
ImVec2 operator*(const ImVec2& a, const float& b)
{
    return ImVec2(a.x * b, a.y * b);
}
ImVec2 operator/(const ImVec2& a, const float& b)
{
    return ImVec2(a.x / b, a.y / b);
}
ImVec2 operator~(const ImVec2& a)
{
    float len = sqrt(a.x * a.x + a.y * a.y);
    if (len > EPS) return a / len;
    else return a;
}
ImVec2 operator^(const ImLine& a, const ImLine& b)
{
    float cross = Cro(a.d, b.d);
    if (fabs(cross) < EPS) return ImVec2(-10000, -10000);
    ImVec2 d(Cro(a.a, a.d), Cro(b.a, b.d));
    ImVec2 dx(a.d.x, b.d.x);
    ImVec2 dy(a.d.y, b.d.y);
    ImVec2 ans = ImVec2(Cro(dx, d) / cross, Cro(dy, d) / cross);
    return ans;
}
ImInt2 operator+(const ImInt2& a, const ImInt2& b)
{
    return ImInt2(a.x + b.x, a.y + b.y);
}
ImInt2 operator-(const ImInt2& a, const ImInt2& b)
{
    return ImInt2(a.x - b.x, a.y - b.y);
}
ImInt2 operator*(const int& a, const ImInt2& b)
{
    return ImInt2(a * b.x, a * b.y);
}
ImInt2 operator*(const ImInt2& a, const int& b)
{
    return ImInt2(a.x * b, a.y * b);
}
ImInt2 operator/(const ImInt2& a, const int& b)
{
    if (b == 0) return ImInt2(-1, -1);
    else return ImInt2(a.x / b, a.y / b);
}
std::ostream& operator<<(std::ostream& os, const ImVec2& a)
{
    os << "(" << a.x << ", " << a.y << ")";
    return os;
}


float Cro(const ImVec2& a, const ImVec2& b)
{
    return a.x * b.y - a.y * b.x;
}

float Dot(const ImVec2& a, const ImVec2& b)
{
    return a.x * b.x + a.y * b.y;
}

std::ostream& operator<<(std::ostream& os, const ImInt2& a)
{
    os << "(" << a.x << ", " << a.y << ")";
    return os;
}

bool operator==(const ImInt2& a, const ImInt2& b)
{
    return a.x == b.x && a.y == b.y;
}

int Cro(const ImInt2& a, const ImInt2& b)
{
    return a.x * b.y - a.y * b.x;
}

int Dot(const ImInt2& a, const ImInt2& b)
{
    return a.x * b.x + a.y * b.y;
}

bool IsPinL(const ImVec2& a, const ImLine& l, const int& putup)
{
    float eps = EPS;
    if (putup == 1) eps += PUTUP;
    else if (putup == -1) eps -= PUTUP;
    if (a.x > l.a.x + eps && a.x > l.b.x + eps) return false;
    if (a.x < l.a.x - eps && a.x < l.b.x - eps) return false;
    if (a.y > l.a.y + eps && a.y > l.b.y + eps) return false;
    if (a.y < l.a.y - eps && a.y < l.b.y - eps) return false;
    return true;
}

bool IsLawayL(const ImLine& a, const ImLine& b, const int& putup)
{
    float eps = EPS;
    if (putup != 0) eps += PUTUP;
    int flag1, flag2, flag3, flag4;
    float cross;
    flag1 = flag2 = flag3 = flag4 = 0;
    cross = Cro(a.a - b.a, b.d);
    if (cross > eps) flag1 = 1;
    else if (cross < -eps) flag1 = -1;
    cross = Cro(a.b - b.a, b.d);
    if (cross > eps) flag2 = 1;
    else if (cross < -eps) flag2 = -1;
    cross = Cro(b.a - a.a, a.d);
    if (cross > eps) flag3 = 1;
    else if (cross < -eps) flag3 = -1;
    cross = Cro(b.b - a.a, a.d);
    if (cross > eps) flag4 = 1;
    else if (cross < -eps) flag4 = -1;
    if (putup >= 0)
    {
        if (flag1 * flag2 == -1 && flag3 * flag4 == -1) return false;
    }
    else
    {
        if (flag1 * flag2 <= 0 && flag3 * flag4 <= 0) return false;
    }
    return true;
}
bool IsPinT(const ImVec2& a, const ImTri& t, const int& putup)
{
    float eps = EPS;
    if (putup == 1) eps += PUTUP;
    else if (putup == -1) eps -= PUTUP;
    if (Cro(t.a - a, t.ab) < -eps) return false;
    if (Cro(t.b - a, t.bc) < -eps) return false;
    if (Cro(t.c - a, t.ca) < -eps) return false;
    return true;
}
bool IsAinB(const Area& a, const int& putup)
{
    ImRect aa(a, scale);
    if (!IsPinB(aa.a, border, putup)) return false;
    if (!IsPinB(aa.b, border, putup)) return false;
    if (!IsPinB(aa.c, border, putup)) return false;
    if (!IsPinB(aa.d, border, putup)) return false;
    for (int i = 0; i < border.line.size(); i++)
    {
        if (IsPinA(border.line[i].a, a, -putup)) return false;
        if (!IsLawayL(border.line[i], aa.ab, putup)) return false;
        if (!IsLawayL(border.line[i], aa.bc, putup)) return false;
        if (!IsLawayL(border.line[i], aa.cd, putup)) return false;
        if (!IsLawayL(border.line[i], aa.da, putup)) return false;
    }
    for (int i = 0; i < barrierlist.size(); i++)
    {
        if (IsPinB(aa.a, barrierlist[i], -putup)) return false;
        if (IsPinB(aa.b, barrierlist[i], -putup)) return false;
        if (IsPinB(aa.c, barrierlist[i], -putup)) return false;
        if (IsPinB(aa.d, barrierlist[i], -putup)) return false;
        for (int j = 0; j < barrierlist[i].line.size(); j++)
        {
            if (IsPinA(barrierlist[i].line[j].a, a, -putup)) return false;
            if (!IsLawayL(barrierlist[i].line[j], aa.ab, putup)) return false;
            if (!IsLawayL(barrierlist[i].line[j], aa.bc, putup)) return false;
            if (!IsLawayL(barrierlist[i].line[j], aa.cd, putup)) return false;
            if (!IsLawayL(barrierlist[i].line[j], aa.da, putup)) return false;
        }
    }
    return true;
}

bool IsPinB(const ImVec2& a, const Border& b, const int& putup)
{
    for (int i = 0; i < b.tri.size(); i++)
        if (IsPinT(a, b.tri[i], putup)) return true;
    return false;
}

bool IsPinB(const ImVec2& a, const int& putup)
{
    if (!IsPinB(a, border, putup)) return false;
    for (int i = 0; i < barrierlist.size(); i++)
        if (IsPinB(a, barrierlist[i], -putup)) return false;
    return true;
}

bool IsPinA(const ImVec2& a, const Area& ar, const int& putup)
{
    float eps = EPS;
    if (putup == 1) eps += PUTUP;
    else if (putup == -1) eps -= PUTUP;
    ImRect r(ar, scale);
    if (Cro(r.a - a, r.ab.d) < -eps) return false;
    if (Cro(r.b - a, r.bc.d) < -eps) return false;
    if (Cro(r.c - a, r.cd.d) < -eps) return false;
    if (Cro(r.d - a, r.da.d) < -eps) return false;
    return true;
}

bool IsAonA(const Area& a, const Area& b, const int& putup)
{
    ImRect aa(a, scale);
    ImRect bb(b, scale);
    if (IsPinA(aa.a, b, putup)) return true;
    if (IsPinA(aa.b, b, putup)) return true;
    if (IsPinA(aa.c, b, putup)) return true;
    if (IsPinA(aa.d, b, putup)) return true;
    if (IsPinA(bb.a, a, putup)) return true;
    if (IsPinA(bb.b, a, putup)) return true;
    if (IsPinA(bb.c, a, putup)) return true;
    if (IsPinA(bb.d, a, putup)) return true;
    if (!IsLawayL(aa.ab, bb.ab, -putup)) return true; if (!IsLawayL(aa.ab, bb.bc, -putup)) return true; if (!IsLawayL(aa.ab, bb.cd, -putup)) return true;
    if (!IsLawayL(aa.bc, bb.ab, -putup)) return true; if (!IsLawayL(aa.bc, bb.bc, -putup)) return true; if (!IsLawayL(aa.bc, bb.cd, -putup)) return true;
    if (!IsLawayL(aa.cd, bb.ab, -putup)) return true; if (!IsLawayL(aa.cd, bb.bc, -putup)) return true; if (!IsLawayL(aa.cd, bb.cd, -putup)) return true;
    return false;
}

ImVec2 AAholdAB(const Area& a, const Area& b)
{
    Area aa = a, bb = b;
    float dist = 0.0f;
    // AA not move
    ImLine L1(aa.pos, bb.pos);
    if (Dot(L1.d, L1.d) < EPS) L1 = ImLine(aa.pos + ImVec2(1, 0), bb.pos);
    bb.pos = a.pos;
    ImRect A1(aa, scale), B1(bb, scale);
    ImVec2 verts1[4] = { B1.a,B1.b,B1.c,B1.d };
    ImLine lines1[4] = { A1.ab,A1.bc,A1.cd,A1.da };
    for (int i = 0; i < 4; i++)
    {
        float mn = CANVASX + CANVASY;
        for (int j = 0; j < 4; j++)
        {
            float temp = CANVASX + CANVASY;
            float cross = Cro(L1.d, lines1[j].d);
            if (cross > EPS)
            {
                ImLine l(verts1[i], verts1[i] + L1.d);
                ImVec2 Q = l ^ lines1[j];
                temp = Dot(Q - verts1[i], L1.d) / sqrt(Dot(L1.d, L1.d));
            }
            else if (cross > -EPS)
            {
                if (Cro(verts1[i] - L1.a, L1.d) > EPS) temp = 0;
            }
            if (mn > temp) mn = temp;
        }
        if (dist < mn) dist = mn;
    }
    // AB not move
    aa = a; bb = b;
    ImLine L2(bb.pos, aa.pos);
    if (Dot(L2.d, L2.d) < EPS) L2 = ImLine(bb.pos, aa.pos + ImVec2(1, 0));
    aa.pos = b.pos;
    ImRect A2(aa, scale), B2(bb, scale);
    ImVec2 verts2[4] = { A1.a,A1.b,A1.c,A1.d };
    ImLine lines2[4] = { B1.ab,B1.bc,B1.cd,B1.da };
    for (int i = 0; i < 4; i++)
    {
        float mn = CANVASX + CANVASY;
        for (int j = 0; j < 4; j++)
        {
            float temp = CANVASX + CANVASY;
            float cross = Cro(L2.d, lines2[j].d);
            if (cross > EPS)
            {
                ImLine l(verts2[i], verts2[i] + L2.d);
                ImVec2 Q = l ^ lines2[j];
                temp = Dot(Q - verts2[i], L2.d) / sqrt(Dot(L2.d, L2.d));
            }
            else if (cross > -EPS)
            {
                if (Cro(verts2[i] - L2.a, L2.d) > EPS) temp = 0;
            }
            if (mn > temp) mn = temp;
        }
        if (dist < mn) dist = mn;
    }
    // ans
    return b.pos + L2.d / sqrt(Dot(L2.d, L2.d)) * dist;
}

ImVec2 AAseeAB(const Area& a, const Area& b)
{
    ImVec2 ba = a.pos - b.pos;
    ImVec2 l(cosf(-a.deflex * PI / 180), sinf(-a.deflex * PI / 180));
    float dist = Dot(ba, l);
    return b.pos + l * dist;
}

ImU32 LerpColor(const ImU32& h, const ImU32& t, const float& per)
{
    int hr = (h & 0x000000FFu) >> IM_COL32_R_SHIFT;
    int hg = (h & 0x0000FF00u) >> IM_COL32_G_SHIFT;
    int hb = (h & 0x00FF0000u) >> IM_COL32_B_SHIFT;
    int tr = (t & 0x000000FFu) >> IM_COL32_R_SHIFT;
    int tg = (t & 0x0000FF00u) >> IM_COL32_G_SHIFT;
    int tb = (t & 0x00FF0000u) >> IM_COL32_B_SHIFT;
    int lr = static_cast<int>(roundf(hr * per + tr * (1.0f - per)));
    int lg = static_cast<int>(roundf(hg * per + tg * (1.0f - per)));
    int lb = static_cast<int>(roundf(hb * per + tb * (1.0f - per)));
    return IM_COL32(lr, lg, lb, 255);
}

ImU32 LerpColor(const ImU32& h, const float& per)
{
    int hr = (h & 0x000000FFu) >> IM_COL32_R_SHIFT;
    int hg = (h & 0x0000FF00u) >> IM_COL32_G_SHIFT;
    int hb = (h & 0x00FF0000u) >> IM_COL32_B_SHIFT;
    int lr = static_cast<int>(roundf(hr * per));
    int lg = static_cast<int>(roundf(hg * per));
    int lb = static_cast<int>(roundf(hb * per));
    return IM_COL32(lr, lg, lb, 255);
}