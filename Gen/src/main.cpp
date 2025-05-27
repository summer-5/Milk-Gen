#include "jsonbridge.h"

float endist = 5.0f;
float endeflex = 0.0f;
std::vector<float> f45, f5565, f68, f0360, f15, f2050;
std::vector<int> i711;
std::random_device rd;
std::mt19937 gen(std::random_device{}());

bool ExportArea(const std::string& filename, const std::vector<Oriarea>& arealist)
{
	std::string filepath = "./" + filename + ".json";
	std::ofstream fout(filepath);
	if (!fout.is_open()) return false;
	nlohmann::json js;
	js["orilist"] = arealist;
	js["endist"] = endist;
	js["endeflex"] = endeflex;
	fout << js.dump(4);
	return true;
}

bool ExportPlay(const std::string& filename, const std::vector<Playtime>& schedulist)
{
	std::string filepath = "./" + filename + ".json";
	std::ofstream fout(filepath);
	if (!fout.is_open()) return false;
	nlohmann::json js;
	js["schedulist"] = schedulist;
	fout << js.dump(4);
	return true;
}

bool TooNB(const float& a, const float& b)
{
	float ax = cosf(a * PI / 180.0f);
	float ay = sinf(a * PI / 180.0f);
	float bx = cosf(b * PI / 180.0f);
	float by = sinf(b * PI / 180.0f);
	if (ax * bx + ay * by > 0) return true;
	if (ax * bx + ay * by < -0.5f) return true;
	else return false;
}

bool Righto(const float& a)
{
	return cosf(a * PI / 180.0f) > 0;
}

void GenArea()
{
	std::uniform_real_distribution<float> df45(4.0f, 4.5f);
	std::uniform_real_distribution<float> df5565(5.5f, 6.0f);
	std::uniform_real_distribution<float> df68(6.0f, 8.0f);
	std::uniform_real_distribution<float> df0360(0.0f, 360.0f);
	std::uniform_int_distribution<int> di711(14, 19);
	for (int i = 0; i < 10000; i++)
	{
		f45.push_back(df45(gen));
		f5565.push_back(df5565(gen));
		f68.push_back(df68(gen));
		f0360.push_back(df0360(gen));
		i711.push_back(di711(gen));
	}
	for (int k = 0; k < 9; k++)
	{
		int n = 20;
		std::vector<Oriarea> arealist;
		for (int i = 0; i < n; i++)
		{
			int m = 1;
			for (int j = 0; j < m; j++)
			{
				Oriarea area;
				if (j == 0)
				{
					area.ishead = true;
					area.dist = f68.back();
					f68.pop_back();
				}
				else
				{
					area.ishead = false;
					area.dist = f5565.back();
					f5565.pop_back();
				}
				area.deflex = f0360.back();
				f0360.pop_back();
				if (i == 0 && j == 0)
				{
					while (!Righto(area.deflex))
					{
						area.deflex = f0360.back();
						f0360.pop_back();
					}
				}
				if (j == 2)
				{
					while (TooNB(area.deflex, arealist.back().deflex))
					{
						area.deflex = f0360.back();
						f0360.pop_back();
					}
				}
				area.kk = arealist.size() + 1;
				area.rot = f0360.back();
				f0360.pop_back();
				area.sizex = f45.back();
				f45.pop_back();
				area.sizey = f45.back();
				f45.pop_back();
				area.tlast = 3;
				area.tshow = i * 3;
				arealist.push_back(area);
			}
		}
		endist = f68.back();
		f68.pop_back();
		endeflex = f0360.back();
		f0360.pop_back();
		while (Righto(endeflex))
		{
			endeflex = f0360.back();
			f0360.pop_back();
		}
		std::string str = "levellist" + std::to_string(k + 1);
		ExportArea(str, arealist);
	}
	std::cout << "Finish gen area!" << std::endl;
}

void GenPlay()
{
	std::uniform_real_distribution<float> df15(1.0f, 5.0f);
	std::uniform_real_distribution<float> df2050(20.0f, 50.0f);
	for (int i = 0; i < 10000; i++)
	{
		f15.push_back(df15(gen));
		f2050.push_back(df2050(gen));
	}
	for (int k = 0; k < 5; k++)
	{
		int n = 10;
		std::vector<Playtime> playlist;
		for (int i = 0; i < n; i++)
		{
			int m = 20;
			float tpe = 0.0f;
			if (i != 0)
			{

				tpe = f2050.back() + playlist[i - 1].enter;
				f2050.pop_back();
			}
			Playtime playtime(i, tpe);
			for (int j = 0; j < m; j++)
			{
				playtime.early.push_back(f15.back());
				f15.pop_back();
				playtime.late.push_back(f15.back());
				f15.pop_back();
			}
			playlist.push_back(playtime);
		}
		std::string str = "playlist" + std::to_string(k + 1);
		ExportPlay(str, playlist);
	}
	std::cout << "Finish gen play!" << std::endl;
}

int main()
{
	std::string str;
	while (std::cin >> str)
	{
		if (str == "area") GenArea();
		else if (str == "play") GenPlay();
		else std::cout << "Invalid instruction?" << std::endl;
	}
	std::cout << "Safe finish." << std::endl;
	return 0;
}