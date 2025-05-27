#include "kindler.h"

//std::vector<Playtime> schedulist;
//std::vector<Person> peoplist;
//std::vector<std::vector<bool>> crush;
//std::vector<int> occupeople;
//std::vector<int> levell;
int cnt = 0;
void DrawPeople(ImDrawList*& drawlist)
{
    int levellsize = levell.size();
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
    // update where're people
    for (int i = 0; i < peoplist.size(); i++)
    {
        int kk;
        kk = peoplist[i].klate;
        if (kk < levellsize - 1)
        {
            bool flag = true;
            if (kk == -1)
            {
                if (peoplist[i].tlate + EPS < schedulist[i].enter) flag = false;
            }
            else
            {
                if (peoplist[i].tlate + EPS < schedulist[i].late[kk]) flag = false;
            }
            if (kk != levellsize - 2 && kk != static_cast<int>(schedulist[i].late.size()) - 1)
            {
                if (occupeople[kk + 1] > 0) flag = false;
                else if (occupeople[kk + 1] == 0 && kk == peoplist[i].kearly) flag = false;
            }
            if (flag)
            {
                peoplist[i].klate++;
                peoplist[i].tlate = 0.0f;
                if (kk != -1)
                {
                    for (int j = 0; j < crush[kk].size(); j++)
                        if (crush[kk][j]) occupeople[j]--;
                }
            }
        }
        if (peoplist[i].klate >= static_cast<int>(schedulist[i].late.size())) peoplist[i].klate = levellsize - 1;
        peoplist[i].tlate += deltatime;

        kk = peoplist[i].kearly;
        if (kk < levellsize - 1)
        {
            bool flag = true;
            if (kk == -1)
            {
                if (peoplist[i].tearly + EPS < schedulist[i].enter) flag = false;
            }
            else
            {
                if (peoplist[i].tearly + EPS < schedulist[i].early[kk]) flag = false;
            }
            if (kk > peoplist[i].klate) flag = false;
            if (kk != levellsize - 2 && kk != static_cast<int>(schedulist[i].early.size()) - 1)
            {
                if (occupeople[kk + 1] >= 0) flag = false;
            }
            if (flag)
            {
                peoplist[i].kearly++;
                peoplist[i].tearly = 0.0f;
                if (kk != levellsize - 2 && kk != static_cast<int>(schedulist[i].early.size()) - 1)
                {
                    for (int j = 0; j < crush[kk + 1].size(); j++)
                        if (crush[kk + 1][j]) occupeople[j]++;
                }
            }
        }
        if (peoplist[i].kearly >= static_cast<int>(schedulist[i].early.size())) peoplist[i].kearly = levellsize - 1;
        peoplist[i].tearly += deltatime;
    }
    // draw
    bool drawstart = false, drawend = false;
    for (int i = 0; i < peoplist.size(); i++)
    {
        int kk = peoplist[i].klate;
        ImU32 tempcolor;
        if (kk == -1) break;
        if (peoplist[i].kearly == kk || peoplist[i].kearly >= levellsize - 1)
        {
            if (kk < levellsize - 1)
            {
                tempcolor = bcolor[kk];
                if (peoplist[i].kearly < levellsize - 1)
                {
                    if (peoplist[i].tearly > schedulist[i].early[kk] + EPS) tempcolor = COLOR1;
                }
                if (drawmode == 3) ImGui::PushStyleColor(ImGuiCol_Text, tempcolor);
                for (int j = levell[kk]; j < levell[kk + 1]; j++)
                {
                    if (j == 0) drawstart = true;
                    if (j == arealist.size() - 1) drawend = true;
                    ImRect rect(arealist[j], scale);
                    if (drawmode == 3) drawlist->AddQuad(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, tempcolor & TRANSPARENT, sidewidth * SCREENSCALE);
                    else drawlist->AddQuadFilled(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, tempcolor);
                    float fontbias = i < 10 ? 12 : i < 99 ? 24 : 30;
                    ImGui::SetCursorPos(arealist[j].pos * SCREENSCALE - ImVec2(fontbias, 24) * SCREENSCALE);
                    sprintf(buf, "%d", i);
                    ImGui::Text(buf);
                }
                if (drawmode == 3) ImGui::PopStyleColor(1);
                for (int j = levell[kk]; j < levell[kk + 1] - 1; j++)
                {
                    ImVec2 d = ~(arealist[j + 1].pos - arealist[j].pos);
                    drawlist->AddLine((arealist[j].pos - d * linewidth * 0.5f) * SCREENSCALE, (arealist[j + 1].pos + d * linewidth * 0.5f) * SCREENSCALE, tempcolor, linewidth * SCREENSCALE);
                }
            }
        }
        else
        {
            tempcolor = bcolor[kk + 1];
            if (drawmode == 3) ImGui::PushStyleColor(ImGuiCol_Text, tempcolor);
            for (int j = levell[kk + 1]; j < levell[kk + 2]; j++)
            {
                if (j == 0) drawstart = true;
                if (j == arealist.size() - 1) drawend = true;
                ImRect rect(arealist[j], scale);
                if (drawmode == 3) drawlist->AddQuad(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, tempcolor & TRANSPARENT, sidewidth * SCREENSCALE);
                else drawlist->AddQuadFilled(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, tempcolor);
                float fontbias = i < 10 ? 12 : i < 99 ? 24 : 30;
                ImGui::SetCursorPos(arealist[j].pos * SCREENSCALE - ImVec2(fontbias, 24) * SCREENSCALE);
                sprintf(buf, "%d", i);
                ImGui::Text(buf);
            }
            if (drawmode == 3) ImGui::PopStyleColor(1);
            for (int j = levell[kk + 1]; j < levell[kk + 2] - 1; j++)
            {
                ImVec2 d = ~(arealist[j + 1].pos - arealist[j].pos);
                drawlist->AddLine((arealist[j].pos - d * linewidth * 0.5f) * SCREENSCALE, (arealist[j + 1].pos + d * linewidth * 0.5f) * SCREENSCALE, tempcolor, linewidth * SCREENSCALE);
            }
            tempcolor = bcolor[kk];
            if (drawmode == 3) ImGui::PushStyleColor(ImGuiCol_Text, tempcolor);
            for (int j = levell[kk]; j < levell[kk + 1]; j++)
            {
                if (j == 0) drawstart = true;
                if (j == arealist.size() - 1) drawend = true;
                ImRect rect(arealist[j], scale);
                if (drawmode == 3) drawlist->AddQuad(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, tempcolor & TRANSPARENT, sidewidth * SCREENSCALE);
                else drawlist->AddQuadFilled(rect.a * SCREENSCALE, rect.b * SCREENSCALE, rect.c * SCREENSCALE, rect.d * SCREENSCALE, tempcolor);
                float fontbias = i < 10 ? 12 : i < 99 ? 24 : 30;
                ImGui::SetCursorPos(arealist[j].pos * SCREENSCALE - ImVec2(fontbias, 24) * SCREENSCALE);
                sprintf(buf, "%d", i);
                ImGui::Text(buf);
            }
            if (drawmode == 3) ImGui::PopStyleColor(1);
            for (int j = levell[kk]; j < levell[kk + 1]; j++)
            {
                ImVec2 d = ~(arealist[j + 1].pos - arealist[j].pos);
                drawlist->AddLine((arealist[j].pos - d * linewidth * 0.5f) * SCREENSCALE, (arealist[j + 1].pos + d * linewidth * 0.5f) * SCREENSCALE, tempcolor, linewidth * SCREENSCALE);
            }
        }
    }
    if (drawstart)
    {
        ImVec2 d = ~(arealist[0].pos - bstart);
        drawlist->AddLine(bstart * SCREENSCALE, (arealist[0].pos + d * linewidth * 0.5f)* SCREENSCALE, START1, linewidth * SCREENSCALE);
    }
    if (drawend)
    {
        ImVec2 d = ~(bend - arealist.back().pos);
        drawlist->AddLine((arealist.back().pos - d * linewidth * 0.5f) * SCREENSCALE, bend * SCREENSCALE, bcolor[levellsize - 2], linewidth * SCREENSCALE);
    }
    return;
}