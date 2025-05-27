#include "kindler.h"

void DrawPerson(ImDrawList*& drawlist)
{
    char buf[128] = "";
    for (int i = 0; i < levelvirtualnum; i++)
    {
        if (drawmode == 0) bcolor[i] = colors[i];
        else
        {
            float per = 1.0 * (i + 1) / (levelvirtualnum + 2);
            bcolor[i] = LerpColor(END0, START0, per);
        }
    }

    int cptr = -1;
    if ((!playing) && drawmode == 0)
    {
        if (arealist.size() > 0)
        {
            ImVec2 d = ~(arealist[0].pos - bstart);
            drawlist->AddLine(bstart * SCREENSCALE, (arealist[0].pos + d * linewidth * 0.5f) * SCREENSCALE, START0, linewidth * SCREENSCALE);
        }
        for (int i = 0; i < arealist.size(); i++)
        {
            if (arealist[i].ishead) cptr++;
            if (i + 1 < arealist.size())
            {
                ImVec2 d = ~(arealist[i + 1].pos - arealist[i].pos);
                drawlist->AddLine((arealist[i].pos - d * linewidth * 0.5f) * SCREENSCALE, (arealist[i + 1].pos + d * linewidth * 0.5f) * SCREENSCALE, bcolor[cptr], linewidth * SCREENSCALE);
            }
        }
        if (arealist.size() > 0)
        {
            ImVec2 d = ~(bend - arealist.back().pos);
            drawlist->AddLine((arealist.back().pos - d * linewidth * 0.5f) * SCREENSCALE, bend * SCREENSCALE, bcolor[cptr], linewidth * SCREENSCALE);
        }
    }
    cptr = -1;
    int tpsi = 0, tpei = arealist.size() - 1;
    for (int i = 0; i < arealist.size(); i++)
    {
        if (arealist[i].ishead) cptr++;
        if (playing)
        {
            float showing = timeup - playtime - arealist[i].tshow;
            if (showing > -EPS && showing < arealist[i].tlast + EPS)
            {
                if (drawmode == 3)
                {
                    if (arealist[i].ishead) tpei = i;
                    continue;
                }
                ImRect rect(arealist[i], scale);
                ImU32 tempcolor = bcolor[cptr];
                float restime = arealist[i].tlast - showing;
                if (restime < 10)
                {
                    unsigned tempA = static_cast<unsigned>(ceilf(sqrtf(restime / 10) * 255));
                    tempcolor &= 0x00FFFFFFu;
                    tempcolor |= tempA << IM_COL32_A_SHIFT;
                }
                drawlist->AddQuadFilled(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, tempcolor);
                int intrestime = static_cast<int>(ceil(restime));
                float fontbias = intrestime < 10 ? 12 : intrestime < 99 ? 24 : 30;
                ImGui::SetCursorPos(arealist[i].pos * SCREENSCALE - ImVec2(fontbias, 24) * SCREENSCALE);
                sprintf(buf, "%d", intrestime);
                ImGui::Text(buf);
            }
            else
            {
                if (drawmode == 3)
                {
                    if (arealist[i].ishead && i <= tpei) tpsi = i;
                    continue;
                }
            }
        }
        else
        {
            ImRect rect(arealist[i], scale);
            if (drawmode == 0) drawlist->AddQuadFilled(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, bcolor[cptr]);
            else if (drawmode == 2) drawlist->AddQuadFilled(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, bcolor[cptr] & TRANSPARENT);
            else if (drawmode == 3) drawlist->AddQuad(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, bcolor[cptr] & TRANSPARENT, sidewidth * SCREENSCALE);
            else if (arealist[i].ishead)
            {
                std::vector<ImVec2> bag, down, up;
                bag.push_back(rect.a); bag.push_back(rect.b); bag.push_back(rect.c); bag.push_back(rect.d);
                for (int j = i + 1; j < arealist.size(); j++)
                {
                    if (arealist[j].ishead) break;
                    ImRect re(arealist[j], scale);
                    bag.push_back(re.a); bag.push_back(re.b); bag.push_back(re.c); bag.push_back(re.d);
                }
                std::sort(bag.begin(), bag.end(), [](ImVec2 a, ImVec2 b) { return a.x < b.x; });
                down.push_back(bag[0]); down.push_back(bag[1]); up.push_back(bag[0]); up.push_back(bag[1]);
                for (int j = 2; j < bag.size(); j++)
                {
                    int t;
                    t = down.size() - 1;
                    while (t > 0)
                    {
                        if (Cro(down[t] - down[t - 1], bag[j] - down[t]) > 0) break;
                        down.pop_back();
                        t--;
                    }
                    down.push_back(bag[j]);
                    t = up.size() - 1;
                    while (t > 0)
                    {
                        if (Cro(up[t] - up[t - 1], bag[j] - up[t]) < 0) break;
                        up.pop_back();
                        t--;
                    }
                    up.push_back(bag[j]);
                }
                for (int j = 0; j < down.size() - 1; j++)
                {
                    ImVec2 d = ~(down[j + 1] - down[j]);
                    drawlist->AddLine((down[j] - d * sidewidth * 0.5f) * SCREENSCALE, (down[j + 1] + d * sidewidth * 0.5f) * SCREENSCALE, bcolor[cptr] & TRANSPARENT, sidewidth * SCREENSCALE);
                }
                for (int j = 0; j < up.size() - 1; j++)
                {
                    ImVec2 d = ~(up[j + 1] - up[j]);
                    drawlist->AddLine((up[j] - d * sidewidth * 0.5f) * SCREENSCALE, (up[j + 1] + d * sidewidth * 0.5f) * SCREENSCALE, bcolor[cptr] & TRANSPARENT, sidewidth * SCREENSCALE);
                }
            }
        }
    }
    ///////////////////////////
    ///////// rubbish /////////
    ///////////////////////////
    if (playing && drawmode == 3 && arealist.size() > 0)
    {
        tpei++;
        while (tpei < arealist.size())
        {
            if (!arealist[tpei].ishead) tpei++;
            else break;
        }
        cptr = -1;
        for (int i = 0; i < tpsi; i++)
            if (arealist[i].ishead) cptr++;
        for (int i = tpsi; i < tpei; i++)
        {
            if (!arealist[i].ishead) continue;
            cptr++;
            for (int j = i; j < tpei; j++)
            {
                if (arealist[j].ishead && j != i) break;
                float showing = timeup - playtime - arealist[j].tshow;
                ImRect rect(arealist[j], scale);
                float restime = arealist[j].tlast - showing;
                drawlist->AddQuad(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, bcolor[cptr] & TRANSPARENT, sidewidth * SCREENSCALE);
                int intrestime = static_cast<int>(ceil(restime));
                if (intrestime > 0)
                {
                    float fontbias = intrestime < 10 ? 12 : intrestime < 99 ? 24 : 30;
                    ImGui::PushStyleColor(ImGuiCol_Text, bcolor[cptr]);
                    ImGui::SetCursorPos(arealist[j].pos * SCREENSCALE - ImVec2(fontbias, 24) * SCREENSCALE);
                    sprintf(buf, "%d", intrestime);
                    ImGui::Text(buf);
                    ImGui::PopStyleColor(1);
                }
            }
        }
        cptr = -1;
        for (int i = 0; i < tpsi; i++)
            if (arealist[i].ishead) cptr++;
        std::vector<ImVec2> pre;
        if (tpsi == 0) pre.push_back(bstart);
        for (int i = tpsi; i < tpei; i++)
        {
            if (!arealist[i].ishead) continue;
            std::vector<ImVec2> tmp;
            tmp.push_back(arealist[i].pos);
            for (int j = i + 1; j < tpei; j++)
            {
                if (arealist[j].ishead) break;
                tmp.push_back(arealist[j].pos);
            }
            if (i == 0 || i != tpsi)
            {
                int tpi = 0, tpj = 0;
                float dmx = 9600000.0f;
                for (int ii = 0; ii < pre.size(); ii++)
                {
                    for (int jj = 0; jj < tmp.size(); jj++)
                    {
                        ImVec2 d = tmp[jj] - pre[ii];
                        if (dmx > Dot(d, d))
                        {
                            dmx = Dot(d, d);
                            tpi = ii;
                            tpj = jj;
                        }
                    }
                }
                ImVec2 d = ~(tmp[tpj] - pre[tpi]);
                if (i == 0) drawlist->AddLine((pre[tpi] - d * linewidth * 0.5f) * SCREENSCALE, (tmp[tpj] + d * linewidth * 0.5f) * SCREENSCALE, START1, linewidth * SCREENSCALE);
                else drawlist->AddLine((pre[tpi] - d * linewidth * 0.5f) * SCREENSCALE, (tmp[tpj] + d * linewidth * 0.5f) * SCREENSCALE, bcolor[cptr], linewidth * SCREENSCALE);
            }
            cptr++;
            pre = tmp;
        }
        if (tpei == arealist.size() && tpei != 0)
        {
            std::vector<ImVec2> tmp;
            tmp.push_back(bend);
            int tpi = 0, tpj = 0;
            float dmx = 9600000.0f;
            for (int ii = 0; ii < pre.size(); ii++)
            {
                for (int jj = 0; jj < tmp.size(); jj++)
                {
                    ImVec2 d = tmp[jj] - pre[ii];
                    if (dmx > Dot(d, d))
                    {
                        dmx = Dot(d, d);
                        tpi = ii;
                        tpj = jj;
                    }
                }
            }
            ImVec2 d = ~(tmp[tpj] - pre[tpi]);
            drawlist->AddLine((pre[tpi] - d * linewidth * 0.5f) * SCREENSCALE, (tmp[tpj] + d * linewidth * 0.5f) * SCREENSCALE, bcolor[cptr], linewidth * SCREENSCALE);
        }
    }
    ///////////////////////////
    ///////// rubbish /////////
    ///////////////////////////
    cptr = -1;
    if ((!playing) && drawmode > 0)
    {
        if (drawmode == 2)
        {
            if (arealist.size() > 0)
            {
                ImVec2 d = ~(arealist[0].pos - bstart);
                drawlist->AddLine(bstart * SCREENSCALE, (arealist[0].pos + d * linewidth * 0.5f) * SCREENSCALE, LerpColor(START1, 0.9f), linewidth * SCREENSCALE);
            }
            for (int i = 0; i < arealist.size(); i++)
            {
                if (arealist[i].ishead) cptr++;
                if (i + 1 < arealist.size())
                {
                    ImVec2 d = ~(arealist[i + 1].pos - arealist[i].pos);
                    drawlist->AddLine((arealist[i].pos - d * linewidth * 0.5f) * SCREENSCALE, (arealist[i + 1].pos + d * linewidth * 0.5f) * SCREENSCALE, LerpColor(bcolor[cptr], 0.9f), linewidth * SCREENSCALE);
                }
            }
            if (arealist.size() > 0)
            {
                ImVec2 d = ~(bend - arealist.back().pos);
                drawlist->AddLine((arealist.back().pos - d * linewidth * 0.5f) * SCREENSCALE, bend * SCREENSCALE, LerpColor(bcolor[cptr], 0.9f), linewidth * SCREENSCALE);
            }
        }
        else
        {
            std::vector<ImVec2> cent;
            if (drawmode == 3)
            {
                std::vector<ImVec2> tempre;
                int tpi, tpj;
                float dmx;
                tempre.push_back(bstart);
                for (int i = 0; i < arealist.size(); i++)
                {
                    if (arealist[i].ishead)
                    {
                        std::vector<ImVec2> tempnow;
                        tempnow.push_back(arealist[i].pos);
                        for (int j = i + 1; j < arealist.size(); j++)
                        {
                            if (arealist[j].ishead) break;
                            tempnow.push_back(arealist[j].pos);
                        }
                        tpi = tpj = 0;
                        dmx = 9600000.0f;
                        for (int ii = 0; ii < tempre.size(); ii++)
                        {
                            for (int jj = 0; jj < tempnow.size(); jj++)
                            {
                                ImVec2 d = tempnow[jj] - tempre[ii];
                                if (dmx > Dot(d, d))
                                {
                                    dmx = Dot(d, d);
                                    tpi = ii;
                                    tpj = jj;
                                }
                            }
                        }
                        if (i != 0) cent.push_back(tempre[tpi]);
                        cent.push_back(tempnow[tpj]);
                        tempre = tempnow;
                    }
                }
                tpi = 0;
                dmx = 9600000.0f;
                for (int i = 0; i < tempre.size(); i++)
                {
                    ImVec2 d = bend - tempre[i];
                    if (dmx > Dot(d, d))
                    {
                        dmx = Dot(d, d);
                        tpi = i;
                    }
                }
                if (tempre.size() > 0) cent.push_back(tempre[tpi]);
            }
            else // drawmode == 1
            {
                for (int i = 0; i < arealist.size(); i++)
                {
                    if (arealist[i].ishead)
                    {
                        int cnt = 1;
                        ImVec2 temp = arealist[i].pos;
                        for (int j = i + 1; j < arealist.size(); j++)
                        {
                            if (arealist[j].ishead) break;
                            cnt++;
                            temp = temp + arealist[j].pos;
                        }
                        cent.push_back(temp / cnt);
                        cent.push_back(temp / cnt);
                    }
                }
            }
            if (cent.size() > 0)
            {
                ImVec2 d = ~(cent[0] - bstart);
                drawlist->AddLine(bstart * SCREENSCALE, (cent[0] + d * linewidth * 0.5f) * SCREENSCALE, START1, linewidth * SCREENSCALE);
            }
            for (int i = 1; i < cent.size(); i += 2)
            {
                cptr++;
                if (i + 1 < cent.size())
                {
                    ImVec2 d = ~(cent[i + 1] - cent[i]);
                    drawlist->AddLine((cent[i] - d * linewidth * 0.5f) * SCREENSCALE, (cent[i + 1] + d * linewidth * 0.5f) * SCREENSCALE, bcolor[cptr], linewidth * SCREENSCALE);
                }
            }
            if (cent.size() > 0)
            {
                ImVec2 d = ~(bend - cent.back());
                drawlist->AddLine((cent.back() - d * linewidth * 0.5f) * SCREENSCALE, bend * SCREENSCALE, bcolor[cptr], linewidth * SCREENSCALE);
            }
        }
    }
}