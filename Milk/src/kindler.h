#ifndef KINDLER_H
#define KINDLER_H

#pragma once
#include "imgeometry.h"

constexpr int XMAX = 3840;
constexpr int YMAX = 2160;
constexpr int BUTTONMIN = 240;
constexpr int INDEXMIN = 247;
constexpr float BUTTON2MENU = 20.0f / 13.0f;
constexpr int REALX = 1000;
constexpr int BUTTONY = 90;
constexpr float BORDERPOINTSIZE = 10.0f;
constexpr float BORDERSIDEWIDTH = 5.0f;
constexpr int BORDERPOINTEDGENUM = 5;
constexpr float STANDSIZE = 20.0f;
constexpr int STANDPOINTEDGENUM = 4;

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

struct Person
{
	int kearly = -1;
	int klate = -1;
	float tearly = 0.0f;
	float tlate = 0.0f;
};

extern int DISPLAYX, DISPLAYY, SCREENX, SCREENY, BUTTONX;
extern int MENUX, MENUY, CANVASX, CANVASY, INDEXX, INDEXY;
extern float SCREENSCALE, CXDY, scale;
extern int REALY;

extern Area empty;
extern ImU32 colors[LEVELCALCULATEDMAX], bcolor[LEVELCALCULATEDMAX];
extern float px, pz, zy, endist, endeflex;
extern ImVec2 mouse, mousehist, seat, bstart, bend;
extern bool bordersetting, barriersetting, startsetting, endsetting, playing, dynamik;
extern Area* area;
extern std::vector<Area> arealist;
extern Border border, newborder, newbarrier;
extern std::vector<Border> barrierlist;
extern int barrierkk, levelvirtualnum;
extern unsigned char pixels[XMAX * YMAX][4];
extern bool press, presshist;
extern float playtime, playspeed, timeup, deltatime, tank;
extern int fpshist;
extern float PUTUP;
extern int PUTUPLITTLE;
extern int levelvirtualnum, drawmode, linewidth, sidewidth;
extern std::vector<Playtime> schedulist;
extern std::vector<Person> peoplist;
extern std::vector<std::vector<bool>> crush;
extern std::vector<int> occupeople;
extern std::vector<int> levell;

extern std::atomic<bool> signal;

void InitializeBorder();
void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
void BuildStand(const ImVec2&, const bool&);
void mouse_callback(GLFWwindow*, double, double);
void process_input(GLFWwindow*);
void ClickorDrag();
void SetBorder();
bool BuildBorder();
void SetStart();
void SetEnd();
void SetBarrier();
bool BuildBarrier();
void DeleteBarrier();
bool AddArea();
void DeleteArea();
bool SeefalseHoldtrue(const bool&);
void PlayNow();
void PlayDynamic(const std::string&);
void Repixel();
void Refind();
void ReCntLevels();

// draw
void DrawPerson(ImDrawList*&);
void DrawPeople(ImDrawList*&);
bool ImportPlay(const std::string&);

#endif