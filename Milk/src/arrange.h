#ifndef ARRANGE_H
#define ARRANGE_H

#pragma once
#include "imgeometry.h"

constexpr int UNX = 50;
constexpr int UNY = 50;
constexpr int WINDING = 360000000;
constexpr int THETAMAX = 24;
constexpr float NEARERTHRESH = 1.0f;
constexpr float FARERTHRESH = 5.0f;
constexpr int SEEKMAX = 256;

struct Units4Level
{
	ImInt2 posh;
	ImInt2 posd;
	std::vector<ImInt2> q;
	float deflex = 0.0f;
	float dist = 0.0f;
	int h = 0;
	int t = 1;
	int* mp[2][UNX];
	mutable bool copied = false;
	Units4Level()
	{
		copied = false;
		for (int i = 0; i < UNX; i++)
		{
			mp[0][i] = new int[UNY];
			mp[1][i] = new int[UNY];
		}
	}
	Units4Level(float De, float Di, int H, int T)
	{
		deflex = De; dist = Di; h = H; t = T;
		copied = false;
		for (int i = 0; i < UNX; i++)
		{
			mp[0][i] = new int[UNY];
			mp[1][i] = new int[UNY];
		}
	}
	Units4Level(const Units4Level& ori)
	{
		posh = ori.posh; q = ori.q; deflex = ori.deflex; dist = ori.dist; h = ori.h; t = ori.t;
		for (int i = 0; i < UNX; i++)
		{
			mp[0][i] = ori.mp[0][i];
			mp[1][i] = ori.mp[1][i];
		}
		copied = false;
		ori.copied = true;
	}
	~Units4Level() 
	{ 
		if (!copied)
		{
			for (int i = 0; i < UNX; i++)
			{
				delete[] mp[0][i];
				delete[] mp[1][i];
			}
		}
	}
	Units4Level& operator=(const Units4Level&) = delete;

};

ImInt2 vec2int(const ImVec2& z);
ImVec2 int2vec(const ImInt2& z);

extern ImVec2 bstart, bend;
extern float scale, endeflex, endist;
extern std::mt19937 gen;

extern std::atomic<float> makepercent;
extern std::atomic<bool> makefinish;

void RearrangeAreas(std::vector<Area>&);
bool MakeU4Lrot(std::vector<Area>&, std::vector<Units4Level>&);
bool MakeU4Lq(const std::vector<Area>&, std::vector<Units4Level>&);
void MakeU4Lmp(const std::vector<Area>&, std::vector<Units4Level>&);
int OnDist(const ImInt2&, const ImInt2&, const float&, const float&);
bool MakeU4Lpos(const std::vector<Area>&, std::vector<Units4Level>&, const int&);
bool MakeU4LposReverse(const std::vector<Area>&, std::vector<Units4Level>&, const int&);

#endif