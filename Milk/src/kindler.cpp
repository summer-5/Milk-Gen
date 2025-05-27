#include "kindler.h"


void InitializeBorder()
{
    border.Addvert(ImVec2(10, 10));
    border.Addvert(ImVec2(REALX - 10, 10));
    border.Addvert(ImVec2(REALX - 10, REALY - 10));
    border.Addvert(ImVec2(10, REALY - 10));
    border.Addline(ImLine(border.vert[3], border.vert[0]));
    border.Addtri(ImTri(border.vert[0], border.vert[1], border.vert[2]));
    border.Addtri(ImTri(border.vert[0], border.vert[2], border.vert[3]));
    border.screenAcreage = (REALX - 20) * (REALY - 20);
    scale = sqrt(1.0f * border.screenAcreage / border.acreage);
    bstart = border.line[0].a;
    bend = border.line.back().a;
    Refind();
    return;
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    return;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCREENX = width;
    SCREENY = height;
    MENUY = SCREENY;
    float cxdy = 1.0f * (width - BUTTONMIN) / (height - INDEXMIN);
    if (cxdy > CXDY)
    {
        INDEXY = INDEXMIN;
        CANVASY = height - INDEXY;
        CANVASX = CANVASY * CXDY;
        INDEXX = CANVASX;
        BUTTONX = width - CANVASX - 15;
        MENUX = BUTTONX * BUTTON2MENU;
    }
    else
    {
        BUTTONX = BUTTONMIN;
        MENUX = BUTTONX * BUTTON2MENU;
        CANVASX = width - BUTTONX - 15;
        INDEXX = CANVASX;
        CANVASY = CANVASX / CXDY;
        INDEXY = height - CANVASY;
    }
    SCREENSCALE = 1.0f * CANVASX / REALX;
    //glViewport(0, 0, width, height);
    return;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse = ImVec2(xpos, ypos) / SCREENSCALE;
    if (signal) return;
    if (area->kk == 0)
    {
        mousehist = mouse;
        return;
    }
    Area temp = *area;
    temp.pos = mouse - temp.clickpos;
    if (temp.dirsolid && temp.kk >= 2)
    {
        ImVec2 fa = arealist[temp.kk - 2].pos;
        temp.pos = fa + Dot(temp.pos - fa, temp.dirc) * temp.dirc;
    }
    if (IsAinB(temp, 1)) mousehist = mouse;
    return;
}

void BuildStand(const ImVec2& mousepos, const bool& startruendfalse)
{
    ImVec2 aim(0, 0);
    if (startruendfalse) aim = bstart;
    else aim = bend;
    float mn = 1000000.0f;
    for (int i = 0; i < border.line.size(); i++)
    {
        ImVec2 temp(0, 0);
        ImVec2 d = ~(border.line[i].d);
        if (Dot(d, d) < EPS) continue;
        ImVec2 a = border.line[i].a;
        ImVec2 b = mousepos - a;
        a = a + Dot(b, d) * d;
        if (IsPinL(a, border.line[i], 0)) temp = a;
        else
        {
            temp = border.line[i].a;
            if (Dot(border.line[i].b - mousepos, border.line[i].b - mousepos) < Dot(border.line[i].a - mousepos, border.line[i].a - mousepos)) temp = border.line[i].b;
        }
        if (mn > Dot(temp - mousepos, temp - mousepos))
        {
            mn = Dot(temp - mousepos, temp - mousepos);
            aim = temp;
        }
    }
    if (startruendfalse) bstart = aim;
    else bend = aim;
    return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (signal)
    {
        press = false;
        return;
    }
    if (playing)
    {
        press = false;
        return;
    }
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        press = true;
        if (bordersetting)
        {
            if (mouse.x * SCREENSCALE < CANVASX && mouse.y * SCREENSCALE < CANVASY) newborder.Addvert(mouse);
        }
        if (barriersetting)
        {
            if (mouse.x * SCREENSCALE < CANVASX && mouse.y * SCREENSCALE < CANVASY) newbarrier.Addvert(mouse);
        }
        if (startsetting)
        {
            if (mouse.x * SCREENSCALE < CANVASX && mouse.y * SCREENSCALE < CANVASY) BuildStand(mouse, true);
        }
        if (endsetting)
        {
            if (mouse.x * SCREENSCALE < CANVASX && mouse.y * SCREENSCALE < CANVASY) BuildStand(mouse, false);
        }
    }
    if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) press = false;
    return;
}

void ClickorDrag()
{
    if (signal)
    {
        presshist = press;
        return;
    }
    if (bordersetting || barriersetting || startsetting || endsetting)
    {
        presshist = press;
        return;
    }
    if (press && mouse.x * SCREENSCALE < CANVASX && mouse.y * SCREENSCALE < CANVASY)
    {
        if (presshist)
        {
            if (area->kk != 0) area->pos = mousehist - area->clickpos;
            if (area->dirsolid && area->kk >= 2)
            {
                ImVec2 fa = arealist[area->kk - 2].pos;
                area->pos = fa + Dot(area->pos - fa, area->dirc) * area->dirc;
            }
        }
        else
        {
            int kk = area->kk;
            bool temp = area->dirsolid;
            area->dirsolid = false;
            area = &empty;
            for (int i = arealist.size() - 1; i >= 0; i--)
            {
                if (IsPinA(mouse, arealist[i], 0))
                {
                    area = &arealist[i];
                    area->clickpos = mouse - area->pos;
                    if (area->kk == kk) area->dirsolid = temp;
                    break;
                }
            }
        }
    }
    presshist = press;
    if (arealist.size() == 0) return;
    int cnt = 2;
    pz = 0.0f;
    ImVec2 ll, ee;
    ll= arealist[0].pos - bstart;
    ee = ImVec2(cosf(-arealist[0].deflex * PI / 180), sinf(-arealist[0].deflex * PI / 180));
    pz += acosf(Dot(ll, ee) / sqrt(Dot(ll, ll))) / PI * 180;
    ll = bend - arealist.back().pos;
    ee = ImVec2(cosf(-endeflex * PI / 180), sinf(-endeflex * PI / 180));
    pz += acosf(Dot(ll, ee) / sqrt(Dot(ll, ll))) / PI * 180;
    for (int i = 1; i < arealist.size(); i++)
    {
        if (arealist[i].ishead)
        {
            ImVec2 l = arealist[i].pos - arealist[i - 1].pos;
            ImVec2 e(cosf(-arealist[i].deflex * PI / 180), sinf(-arealist[i].deflex * PI / 180));
            pz += acosf(Dot(l, e) / sqrt(Dot(l, l))) / PI * 180;
            cnt++;
        }
    }
    pz /= cnt;
    return;
}

void Repixel()
{
    if (signal) return;
    if (bordersetting || barriersetting || startsetting || endsetting) return;
    px = zy = 0.0f;
    int white = 0;
    for (int i = 0; i < CANVASX * CANVASY; i++)
    {
        Pixcol pixcol(static_cast<int>(pixels[i][0]), static_cast<int>(pixels[i][1]), static_cast<int>(pixels[i][2]), static_cast<int>(pixels[i][3]));
        if (pixcol.IsColorful()) px += 1.0f;
        if (pixcol.IsWhite()) white += 1;
    }
    for (int i = 0; i < arealist.size(); i++)
        zy += arealist[i].size.x * arealist[i].size.y;
    zy *= scale * scale * SCREENSCALE * SCREENSCALE / (px + white);
    px /= px + white;
    if (px < 0.0f) px = 0.0f;
    if (px > 1.0f) px = 1.0f;
    if (zy < 0.0f) zy = 0.0f;
    return;
}

void Refind()
{
    Area ar;
    int x = (REALX - 1) / 2;
    int y = REALY / 2;
    for (int i = 1; i <= REALX - x - 1 + y; i++)
    {
        for (int a = 0; a < i; a++)
        {
            int b = i - a, tx, ty;
            tx = x + b; ty = y + a;
            if (tx < REALX && ty < REALY)
            {
                ar.pos = ImVec2(tx, ty);
                if (IsAinB(ar, 1))
                {
                    seat = ar.pos;
                    return;
                }
            }
            tx = x - a; ty = y + b;
            if (tx >= 0 && ty < REALY)
            {
                ar.pos = ImVec2(tx, ty);
                if (IsAinB(ar, 1))
                {
                    seat = ar.pos;
                    return;
                }
            }
            tx = x - b; ty = y - a;
            if (tx >= 0 && ty >= 0)
            {
                ar.pos = ImVec2(tx, ty);
                if (IsAinB(ar, 1))
                {
                    seat = ar.pos;
                    return;
                }
            }
            tx = x + a; ty = y - b;
            if (tx < REALX && ty >= 0)
            {
                ar.pos = ImVec2(tx, ty);
                if (IsAinB(ar, 1))
                {
                    seat = ar.pos;
                    return;
                }
            }
        }
    }
    seat = ImVec2(-10000, -10000);
    return;
}

void SetBorder()
{
    barriersetting = startsetting = endsetting = false;
    area->dirsolid = false;
    area = &empty;
    bordersetting = true;
    newborder.Reset(border.acreage, border.screenAcreage);
    return;
}

bool BuildBorder()
{
    if (!bordersetting) return false;
    bordersetting = false;
    int num = newborder.vert.size();
    if (num < 3) return false;
    newborder.Addline(ImLine(newborder.line.back().b, newborder.line[0].a));
    float sign = 0.0f;
    for (int i = 0; i < num; i++)
        sign += Cro(newborder.vert[i], newborder.vert[(i + 1) % num]);
    if (sign < 0)
    {
        for (int i = 0; i < num / 2; i++)
        {
            ImVec2 temp = newborder.vert[i];
            newborder.vert[i] = newborder.vert[num - i - 1];
            newborder.vert[num - i - 1] = temp;
        }
    }
    for (int i = 0; i < num; i++)
    {
        ImVec2 a = newborder.vert[i];
        ImVec2 b = newborder.vert[(i + 1) % num];
        ImVec2 c = newborder.vert[(i + 2) % num];
        ImTri t(a, b, c);
        float temp = fabs(Cro(t.ab, t.bc));
        if (temp < EPS)
        {
            newborder.Deletevert((i + 1) % num, num);
            num--;
            i--;
        }
    }
    bool flag = true;;
    while (flag && num >= 3)
    {
        flag = false;
        for (int i = 0; i < num; i++)
        {
            ImVec2 a = newborder.vert[i];
            ImVec2 b = newborder.vert[(i + 1) % num];
            ImVec2 c = newborder.vert[(i + 2) % num];
            ImTri t(a, b, c);
            float temp = Cro(t.ab, t.bc);
            if (temp > EPS)
            {
                bool flagflag = true;
                for (int j = 0; j < num; j++)
                {
                    if (i == j || (i + 1) % num == j || (i + 2) % num == j) continue;
                    ImVec2 p = newborder.vert[j];
                    if (IsPinT(p, t, 0))
                    {
                        flagflag = false;
                        break;
                    }
                }
                if (flagflag)
                {
                    newborder.Addtri(t);
                    newborder.Deletevert((i + 1) % num, num);
                    num--;
                    flag = true;
                }
            }
        }
    }
    if ((!flag) || newborder.tri.size() < 1) return false;
    float temp = 0.0f;
    for (int i = 0; i < newborder.tri.size(); i++)
        temp += Cro(newborder.tri[i].ab, newborder.tri[i].bc) / 2.0;
    if (temp < 100.0f) return false;
    newborder.screenAcreage = temp;
    newborder.acreage = static_cast<int>(roundf(newborder.screenAcreage / scale / scale));
    border.Reset(border.acreage, border.screenAcreage);
    for (int i = 0; i < newborder.line.size(); i++)
        border.Addline(newborder.line[i]);
    for (int i = 0; i < newborder.tri.size(); i++)
        border.Addtri(newborder.tri[i]);
    border = newborder;
    while (arealist.size() > 0)
        arealist.pop_back();
    ReCntLevels();
    while (barrierlist.size() > 0)
        barrierlist.pop_back();
    bstart = border.line[0].a;
    bend = border.line.back().a;
    Refind();
    return true;
}

void SetStart()
{
    if (startsetting)
    {
        startsetting = false;
        return;
    }
    bordersetting = barriersetting = endsetting = false;
    area->dirsolid = false;
    area = &empty;
    startsetting = true;
    return;
}

void SetEnd()
{
    if (endsetting)
    {
        endsetting = false;
        return;
    }
    bordersetting = barriersetting = startsetting = false;
    area->dirsolid = false;
    area = &empty;
    endsetting = true;
    return;
}

void SetBarrier()
{
    bordersetting = startsetting = endsetting = false;
    area->dirsolid = false;
    area = &empty;
    barriersetting = true;
    newbarrier.Reset(0, 0);
    return;
}

bool BuildBarrier()
{
    if (!barriersetting) return false;
    barriersetting = false;
    int num = newbarrier.vert.size();
    if (num < 3) return false;
    newbarrier.Addline(ImLine(newbarrier.line.back().b, newbarrier.line[0].a));
    float sign = 0.0f;
    for (int i = 0; i < num; i++)
        sign += Cro(newbarrier.vert[i], newbarrier.vert[(i + 1) % num]);
    if (sign < 0)
    {
        for (int i = 0; i < num / 2; i++)
        {
            ImVec2 temp = newbarrier.vert[i];
            newbarrier.vert[i] = newbarrier.vert[num - i - 1];
            newbarrier.vert[num - i - 1] = temp;
        }
    }
    for (int i = 0; i < num; i++)
    {
        ImVec2 a = newbarrier.vert[i];
        ImVec2 b = newbarrier.vert[(i + 1) % num];
        ImVec2 c = newbarrier.vert[(i + 2) % num];
        ImTri t(a, b, c);
        float temp = fabs(Cro(t.ab, t.bc));
        if (temp < EPS)
        {
            newbarrier.Deletevert((i + 1) % num, num);
            num--;
            i--;
        }
    }
    bool flag = true;;
    while (flag && num >= 3)
    {
        flag = false;
        for (int i = 0; i < num; i++)
        {
            ImVec2 a = newbarrier.vert[i];
            ImVec2 b = newbarrier.vert[(i + 1) % num];
            ImVec2 c = newbarrier.vert[(i + 2) % num];
            ImTri t(a, b, c);
            float temp = Cro(t.ab, t.bc);
            if (temp > EPS)
            {
                bool flagflag = true;
                for (int j = 0; j < num; j++)
                {
                    if (i == j || (i + 1) % num == j || (i + 2) % num == j) continue;
                    ImVec2 p = newbarrier.vert[j];
                    if (IsPinT(p, t, 0))
                    {
                        flagflag = false;
                        break;
                    }
                }
                if (flagflag)
                {
                    newbarrier.Addtri(t);
                    newbarrier.Deletevert((i + 1) % num, num);
                    num--;
                    flag = true;
                }
            }
        }
    }
    for (int i = 0; i < arealist.size(); i++)
    {
        ImRect re(arealist[i], scale);
        if (IsPinB(re.a, newbarrier, -1)) return false;
        if (IsPinB(re.b, newbarrier, -1)) return false;
        if (IsPinB(re.c, newbarrier, -1)) return false;
        if (IsPinB(re.d, newbarrier, -1)) return false;
        for (int j = 0; j < newbarrier.line.size(); j++)
            if (IsPinA(newbarrier.line[j].a, arealist[i], -1)) return false;
    }
    if ((!flag) || newbarrier.tri.size() < 1) return false;

    float temp = 0.0f;
    for (int i = 0; i < newbarrier.tri.size(); i++)
        temp += Cro(newbarrier.tri[i].ab, newbarrier.tri[i].bc) / 2.0;
    if (temp < 100.0f) return false;
    newbarrier.screenAcreage = temp;
    newbarrier.acreage = static_cast<int>(roundf(newbarrier.screenAcreage / scale / scale));
    barrierlist.push_back(newbarrier);
    barrierkk = barrierlist.size();
    Refind();
    return true;
}

void DeleteBarrier()
{
    if (barrierkk == 0) return;
    for (int i = barrierkk - 1; i < barrierlist.size() - 1; i++)
        barrierlist[i] = barrierlist[i + 1];
    barrierlist.pop_back();
    barrierkk = 0;
    return;
}

bool AddArea()
{
    bordersetting = barriersetting = startsetting = endsetting = false;
    if (seat.x < -1) return false;
    Area newarea;
    newarea.pos = seat;
    newarea.kk = arealist.size() + 1;
    arealist.push_back(newarea);
    area = &(arealist.back());
    ReCntLevels();
    return true;
}

void DeleteArea()
{
    if (area->kk == 0) return;
    for (int i = area->kk; i > 0 && i < arealist.size(); i++)
    {
        arealist[i - 1] = arealist[i];
        arealist[i - 1].kk = i;
    }
    arealist.pop_back();
    if (arealist.size() > 0) arealist[0].ishead = true;
    area = &empty;
    ReCntLevels();
    return;
}

bool SeefalseHoldtrue(const bool& sorh)
{
    int i = area->kk - 1;
    if (i <= 0) return false;
    if (sorh)
    {
        ImVec2 hold = AAholdAB(arealist[i], arealist[i - 1]);
        Area temp = *area;
        temp.pos = hold;
        if (IsAinB(temp, 1)) area->pos = hold;
        else return false;
    }
    else
    {
        ImVec2 see = AAseeAB(arealist[i], arealist[i - 1]);
        Area temp = *area;
        temp.pos = see;
        if (IsAinB(temp, 1))
        {
            if (Dot(see - area->pos, see - area->pos) > EPS) area->dirsolid = false;
            area->pos = see;
        }
        else return false;
    }
    return true;
}

void PlayNow()
{
    bordersetting = barriersetting = startsetting = endsetting = false;
    area = &empty;
    playtime = timeup;
    playing = true;
    dynamik = false;
    return;
}

void PlayDynamic(const std::string& filename)
{
    bordersetting = barriersetting = startsetting = endsetting = false;
    area = &empty;
    if (arealist.size() <= 0) return;
    if (!ImportPlay(filename)) return;
    levell.clear();
    for (int i = 0; i < arealist.size(); i++)
        if (arealist[i].ishead) levell.push_back(i);
    levell.push_back(arealist.size());
    occupeople.clear();
    occupeople.resize(levell.size() - 1, -1);
    crush.clear();
    crush.resize(levell.size() - 1, std::vector<bool>(levell.size() - 1, false));
    for (int i = 0; i < levell.size() - 1; i++)
    {
        for (int j = 0; j < i - 1; j++)
        {
            for (int a = levell[i]; a < levell[i + 1]; a++)
            {
                for (int b = levell[j]; b < levell[j + 1]; b++)
                {
                    if (IsAonA(arealist[a], arealist[b], 0))
                    {
                        crush[i][j] = crush[j][i] = true;
                        break;
                    }
                }
                if (crush[i][j]) break;
            }
        }
        crush[i][i] = true;
    }
    playtime = timeup;
    playing = true;
    dynamik = true;
    return;
}

void ReCntLevels()
{
    levelvirtualnum = 0;
    for (int i = 0; i < arealist.size(); i++)
        if (arealist[i].ishead) levelvirtualnum++;
    return;
}

bool ImportPlay(const std::string& filename)
{
    std::string filepath = "./" + filename + ".json";
    std::ifstream fin(filepath);
    if (!fin.is_open()) return false;
    nlohmann::json js;
    fin >> js;
    if (!js.contains("schedulist")) return false;
    schedulist = js["schedulist"].get<std::vector<Playtime>>();
    for (int i = 0; i < schedulist.size(); i++)
    {
        if (schedulist[i].early.size() != schedulist[i].late.size()) return false;
        for (int j = 0; j < schedulist[i].early.size(); j++)
        {
            if (schedulist[i].early[j] > schedulist[i].late[j])
            {
                float temp = schedulist[i].early[j];
                schedulist[i].early[j] = schedulist[i].late[j];
                schedulist[i].late[j] = temp;
            }
        }
    }
    peoplist.clear();
    peoplist.resize(schedulist.size());
    std::sort(schedulist.begin(), schedulist.end(), [](Playtime a, Playtime b) {return a.kk < b.kk; });
    return true;
}