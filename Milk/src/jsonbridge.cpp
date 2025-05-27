#include "jsonbridge.h"

bool RealImportArea(const std::string& filename)
{
	std::string filepath = "./" + filename + ".json";
	std::ifstream fin(filepath);
	if (!fin.is_open()) return false;
	std::vector<Oriarea> orilist;
	nlohmann::json js;
	fin >> js;
	if (!js.contains("orilist")) return false;
	if (!js.contains("endeflex")) return false;
	if (!js.contains("endist")) return false;
	orilist = js["orilist"].get<std::vector<Oriarea>>();
	if (orilist.size() == 0) return false;
	endeflex = js["endeflex"];
	endist = js["endist"];
	bordersetting = barriersetting = startsetting = endsetting = false;
	area = &empty;
	while (arealist.size() > 0)
		arealist.pop_back();
	for (int i = 0; i < orilist.size(); i++)
	{
		Area ar;
		ar.kk = orilist[i].kk;
		ar.size = ImVec2(orilist[i].sizex, orilist[i].sizey);
		ar.rot = orilist[i].rot;
		ar.tshow = orilist[i].tshow;
		ar.tlast = orilist[i].tlast;
		ar.ishead = orilist[i].ishead;
		ar.deflex = orilist[i].deflex;
		ar.dist = orilist[i].dist;
		ImVec2 dirc(cosf(-orilist[i].deflex * PI / 180.0f), sinf(-orilist[i].deflex * PI / 180.0f));
		dirc = dirc * orilist[i].dist * scale;
		if (i == 0) ar.pos = bstart + dirc;
		else ar.pos = arealist[i - 1].pos + dirc;
		arealist.push_back(ar);
	}
	ImVec2 range(0.0f, 0.0f);
	for (int i = 0; i < border.line.size(); i++)
	{
		if (range.x < border.line[i].a.x) range.x = border.line[i].a.x;
		if (range.y < border.line[i].a.y) range.y = border.line[i].a.y;
	}
	UNITSIDE = range.x / UNX;
	if (UNITSIDE < range.y / UNY) UNITSIDE = range.y / UNY;
	RearrangeAreas(arealist);
	ReCntLevels();
	int k;
	for (k = 0; k < arealist.size(); k++)
		if (!IsAinB(arealist[k], 1)) break;
	if (k != arealist.size())
	{
		while (!arealist[k].ishead) 
			k--;
		while (arealist.size() > k)
			arealist.pop_back();
	}
	return true;
}

bool ImportArea(const std::string& filename)
{
	bool res = RealImportArea(filename);
	signal = false;
	return res;
}

bool ExportArea(const std::string& filename)
{
	if (arealist.size() == 0) return false;
	std::string filepath = "./" + filename + ".json";
	std::ofstream fout(filepath);
	if (!fout.is_open()) return false;
	bordersetting = barriersetting = startsetting = endsetting = false;
	area = &empty;	
	std::vector<Oriarea> orilist;
	nlohmann::json js;
	for (int i = 0; i < arealist.size(); i++)
	{
		Oriarea ori;
		ori.kk = arealist[i].kk;
		ori.sizex = arealist[i].size.x;
		ori.sizey = arealist[i].size.y;
		ori.rot = arealist[i].rot;
		ori.tshow = arealist[i].tshow;
		ori.tlast = arealist[i].tlast;
		ori.ishead = arealist[i].ishead;
		ImVec2 d(0, 0);
		if (i == 0) d = arealist[i].pos - bstart;
		else d = arealist[i].pos - arealist[i - 1].pos;
		ori.dist = sqrtf(Dot(d, d)) / scale;
		if (ori.dist < EPS) ori.deflex = 0.0f;
		else ori.deflex = atan2f(-d.y, d.x) / PI * 180.0f;
		if (ori.deflex < 0.0f) ori.deflex += 360.0f;
		orilist.push_back(ori);
	}
	js["orilist"] = orilist;
	ImVec2 d = bend - arealist.back().pos;
	float deflex = 0.0f;
	if (Dot(d, d) > EPS) deflex = atan2f(-d.y, d.x) / PI * 180.0f;
	if (deflex < 0.0f) deflex += 360.0f;
	js["endist"] = sqrtf(Dot(d, d)) / scale;
	js["endeflex"] = deflex;
	fout << js.dump(4);
	return true;
}

bool ImportScene(const std::string& filename)
{
	std::string filepath = "./" + filename + ".json";
	std::ifstream fin(filepath);
	if (!fin.is_open()) return false;
	bordersetting = barriersetting = startsetting = endsetting = false;
	area = &empty;
	nlohmann::json js;
	fin >> js;
	if (!js.contains("border")) return false;
	if (!js.contains("barriers")) return false;
	if (!js.contains("startx")) return false;
	if (!js.contains("starty")) return false;
	if (!js.contains("endx")) return false;
	if (!js.contains("endy")) return false;
	Outscene inborder;
	inborder = js["border"];
	std::vector<Outscene> inbarriers;
	inbarriers = js["barriers"].get<std::vector<Outscene>>();
	float startx = js["startx"];
	float starty = js["starty"];
	float endx = js["endx"];
	float endy = js["endy"];
	float maxx = 0.0f, maxy = 0.0f;
	for (int i = 0; i < inborder.posx.size(); i++)
		if (maxx < inborder.posx[i]) maxx = inborder.posx[i];
	for (int i = 0; i < inborder.posy.size(); i++)
		if (maxy < inborder.posy[i]) maxy = inborder.posy[i];
	scale = REALX / maxx;
	if (scale > REALY / maxy) scale = REALY / maxy;
	SetBorder();
	for (int i = 0; i < inborder.posx.size(); i++)
		newborder.Addvert(ImVec2(inborder.posx[i] * scale, REALY - inborder.posy[i] * scale));
	BuildBorder();
	SetStart();
	BuildStand(ImVec2(startx * scale, REALY - starty * scale), true);
	SetEnd();
	BuildStand(ImVec2(endx * scale, REALY - endy * scale), false);
	SetEnd();
	for (int i = 0; i < inbarriers.size(); i++)
	{
		SetBarrier();
		for (int j = 0; j < inbarriers[i].posx.size(); j++)
			newbarrier.Addvert(ImVec2(inbarriers[i].posx[j] * scale, REALY - inbarriers[i].posy[j] * scale));
		BuildBarrier();
	}
	return true;
}

bool ExportScene(const std::string& filename)
{
	std::string filepath = "./" + filename + ".json";
	std::ofstream fout(filepath);
	if (!fout.is_open()) return false;
	bordersetting = barriersetting = startsetting = endsetting = false;
	area = &empty;
	nlohmann::json js;
	Outscene outborder(true);
	for (int i = 0; i < border.line.size(); i++)
	{
		outborder.posx.push_back(border.line[i].a.x / scale);
		outborder.posy.push_back((REALY - border.line[i].a.y) / scale);
	}
	js["startx"] = bstart.x / scale;
	js["starty"] = (REALY - bstart.y) / scale;
	js["endx"] = bend.x / scale;
	js["endy"] = (REALY - bend.y) / scale;
	js["border"] = outborder;
	std::vector<Outscene> outbarriers;
	for (int i = 0; i < barrierlist.size(); i++)
	{
		Outscene outbarrier(false);
		for (int j = 0; j < barrierlist[i].line.size(); j++)
		{
			outbarrier.posx.push_back(barrierlist[i].line[j].a.x / scale);
			outbarrier.posy.push_back((REALY - barrierlist[i].line[j].a.y) / scale);
		}
		outbarriers.push_back(outbarrier);
	}
	js["barriers"] = outbarriers;
	fout << js.dump(4);
	return true;
}

