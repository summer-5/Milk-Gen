#include "arrange.h"

float UNITSIDE = 2.0f;
bool unitmap[UNX][UNY];
int pxnum = 0;
int pxdef = 0;
int thetamax = WINDING;
ImInt2 istart, iend;

ImInt2 vec2int(const ImVec2& z)
{
	int x = static_cast<int>(floorf(z.x / UNITSIDE));
	if (x < 0) x = 0;
	if (x >= UNX) x = UNX - 1;
	int y = static_cast<int>(floorf(z.y / UNITSIDE));
	if (y < 0) y = 0;
	if (y >= UNY) y = UNY - 1;
	return ImInt2(x, y);
}

ImVec2 int2vec(const ImInt2& z)
{
	return ImVec2(z.x * UNITSIDE + UNITSIDE / 2.0f, z.y * UNITSIDE + UNITSIDE / 2.0f);
}

int OnDist(const ImInt2& a, const ImInt2& b, const float& dist, const float& deflex)
{
	ImVec2 aa = int2vec(a);
	ImVec2 bb = int2vec(b);
	float distt = sqrtf(Dot(aa - bb, aa - bb)) / scale;
	if (distt < dist - NEARERTHRESH) return WINDING;
	if (distt > dist + FARERTHRESH) return WINDING;
	ImVec2 cc(cosf(-deflex * PI / 180.0f), sinf(-deflex * PI / 180.0f));
	ImVec2 dd = ~(bb - aa);
	int mpdeflex = static_cast<int>(roundf(acosf(Dot(cc, dd)) * 180.0f / PI));
	if (mpdeflex > 180) mpdeflex = 360 - mpdeflex;
	if (mpdeflex < 0) mpdeflex *= -1;
	return mpdeflex;
}

bool MakeU4Lrot(std::vector<Area>& arealist, std::vector<Units4Level>& levelist)
{	for (int i = 0; i < arealist.size(); i++)
	{
		if (arealist[i].ishead)
		{
			if (levelist.size() > 0) levelist.back().t = i - 1;
			levelist.emplace_back(arealist[i].deflex, arealist[i].dist, i, arealist.size() - 1);
		}
	}
	if (levelist.size() > LEVELCALCULATEDMAX) return false;
	int rotcnt = 0;
	for (int k = 0; k < levelist.size(); k++)
	{
		levelist[k].deflex += rotcnt;
		if (levelist[k].deflex >= 360.0f) levelist[k].deflex -= 360.0f;
		for (int i = levelist[k].h; i <= levelist[k].t; i++)
		{
			arealist[i].rot += rotcnt;
			if (arealist[i].rot >= 360.0f) arealist[i].rot -= 360.0f;
			arealist[i].deflex += rotcnt;
			if (arealist[i].deflex >= 360.0f) arealist[i].deflex -= 360.0f;
		}
		int theta = 0, mpcnt = 0;
		for (int t = 0; t < 360; t += 5)
		{
			std::vector<Area> debris;
			for (int i = levelist[k].h; i <= levelist[k].t; i++)
			{
				debris.push_back(arealist[i]);
				debris.back().rot += t;
				if (debris.back().rot >= 360.0f) debris.back().rot -= 360.0f;
				if (i != levelist[k].h)
				{
					debris.back().deflex += t;
					if (debris.back().deflex >= 360.0f) debris.back().deflex -= 360.0f;
				}
			}
			int tempmpcnt = 0;
			for (int i = 0; i < UNX; i++)
			{
				for (int j = 0; j < UNY; j++)
				{
					bool flag = true;
					debris[0].pos = int2vec(ImInt2(i, j));
					for (int a = 0; a < debris.size(); a++)
					{
						ImVec2 dirc(cosf(-debris[a].deflex * PI / 180.0f), sinf(-debris[a].deflex * PI / 180.0f));
						if (a > 0) debris[a].pos = debris[a - 1].pos + debris[a].dist * scale * dirc;
						if (!IsAinB(debris[a], 1))
						{
							flag = false;
							break;
						}
					}
					if (flag) tempmpcnt++;
				}
			}
			if (mpcnt < tempmpcnt)
			{
				mpcnt = tempmpcnt;
				theta = t;
			}
		}
		if (mpcnt == 0) return false;
		for (int i = levelist[k].h; i <= levelist[k].t; i++)
		{
			arealist[i].rot += theta;
			if (arealist[i].rot >= 360.0f) arealist[i].rot -= 360.0f;
			if (i != levelist[k].h)
			{
				arealist[i].deflex += theta;
				if (arealist[i].deflex >= 360.0f) arealist[i].deflex -= 360.0f;
			}
		}
		rotcnt = (rotcnt + theta) % 360;
	}
	endeflex += rotcnt;
	if (endeflex >= 360.0f) endeflex -= 360.0f;
	ImVec2 pre = bstart;
	for (int i = 0; i < arealist.size(); i++)
	{
		ImVec2 dirc(cosf(-arealist[i].deflex * PI / 180.0f), sinf(-arealist[i].deflex * PI / 180.0f));
		arealist[i].pos = pre + arealist[i].dist * scale * dirc;
		pre = arealist[i].pos;
	}
	return true;
}

bool MakeU4Lq(const std::vector<Area>& arealist, std::vector<Units4Level>& levelist)
{
	for (int k = 0; k < levelist.size(); k++)
	{
		std::vector<Area> debris;
		bool flag1 = true;
		for (int i = levelist[k].h; i <= levelist[k].t; i++)
			debris.push_back(arealist[i]);
		for (int i = 0; i < UNX; i++)
		{
			for (int j = 0; j < UNY; j++)
			{
				bool flag2 = false;
				debris[0].pos = int2vec(ImInt2(i, j));
				for (int a = 0; a < debris.size(); a++)
				{
					ImVec2 dirc(cosf(-debris[a].deflex * PI / 180.0f), sinf(-debris[a].deflex * PI / 180.0f));
					if (a > 0) debris[a].pos = debris[a - 1].pos + debris[a].dist * scale * dirc;
					if (!IsAinB(debris[a], 1))
					{
						flag2 = true;
						break;
					}
				}
				if (flag2)
				{
					levelist[k].mp[0][i][j] = levelist[k].mp[1][i][j] = -1;
					continue;
				}
				levelist[k].mp[0][i][j] = levelist[k].mp[1][i][j] = WINDING;
				if (flag1)
				{
					flag1 = false;
					for (int ii = 0; ii < UNX; ii++)
					{
						for (int jj = 0; jj < UNY; jj++)
						{
							ImVec2 temp = int2vec(ImInt2(ii, jj));
							for (int a = 0; a < debris.size(); a++)
							{
								if (IsPinA(temp, debris[a], 0))
								{
									levelist[k].q.push_back(ImInt2(ii - i, jj - j));
									break;
								}
							}
						}
					}
					levelist[k].posd = vec2int(debris.back().pos) - vec2int(debris[0].pos);
				}
			}
		}
	}
	for (int k = 0; k < levelist.size(); k++)
	{
		bool flag1 = true, flag2 = true;
		if (k > 0)
		{
			for (int i = 0; i < UNX; i++)
			{
				for (int j = 0; j < UNY; j++)
				{
					bool flag3 = true;
					if (levelist[k].mp[0][i][j] < 0) continue;
					for (int ii = 0; ii < UNX; ii++)
					{
						for (int jj = 0; jj < UNY; jj++)
						{
							if (levelist[k - 1].mp[0][ii][jj] < 0) continue;
							bool flag4 = false;
							for (int a = levelist[k - 1].h; a <= levelist[k - 1].t; a++)
							{
								Area ara = arealist[a];
								ara.pos = ara.pos + int2vec(ImInt2(ii, jj)) - arealist[levelist[k - 1].h].pos;
								for (int b = levelist[k].h; b <= levelist[k].t; b++)
								{
									Area arb = arealist[b];
									arb.pos = arb.pos + int2vec(ImInt2(i, j)) - arealist[levelist[k].h].pos;
									if (IsAonA(ara, arb, 0))
									{
										flag4 = true;
										break;
									}
								}
								if (flag4) break;
							}
							if (!flag4) flag1 = flag3 = false;
						}
					}
					if (flag3) levelist[k].mp[0][i][j] = levelist[k].mp[1][i][j] = -1;
				}
			}
		}
		else flag1 = false;
		if (k < levelist.size() - 1)
		{
			for (int i = 0; i < UNX; i++)
			{
				for (int j = 0; j < UNY; j++)
				{
					if (levelist[k].mp[0][i][j] < 0) continue;
					bool flag3 = true;
					for (int ii = 0; ii < UNX; ii++)
					{
						for (int jj = 0; jj < UNY; jj++)
						{
							if (levelist[k + 1].mp[0][ii][jj] < 0) continue;
							bool flag4 = false;
							for (int a = levelist[k + 1].h; a <= levelist[k + 1].t; a++)
							{
								Area ara = arealist[a];
								ara.pos = ara.pos + int2vec(ImInt2(ii, jj)) - arealist[levelist[k + 1].h].pos;
								for (int b = levelist[k].h; b <= levelist[k].t; b++)
								{
									Area arb = arealist[b];
									arb.pos = arb.pos + int2vec(ImInt2(i, j)) - arealist[levelist[k].h].pos;
									if (IsAonA(ara, arb, 0))
									{
										flag4 = true;
										break;
									}
								}
								if (flag4) break;
							}
							if (!flag4) flag2 = flag3 = false;
						}
					}
					if (flag3) levelist[k].mp[0][i][j] = levelist[k].mp[1][i][j] = -1;
				}
			}
		}
		else flag2 = false;
		if (flag1 || flag2) return false;
		makepercent = 0.5f * (k + 1) / levelist.size();
	}
	return true;
}

void MakeU4Lmp(const std::vector<Area>& arealist, std::vector<Units4Level>& levelist)
{
	for (int i = 0; i < UNX; i++)
		for (int j = 0; j < UNY; j++)
			if (levelist[0].mp[0][i][j] >= 0) levelist[0].mp[0][i][j] = OnDist(istart, ImInt2(i, j), levelist[0].dist, levelist[0].deflex);
	for (int k = 1; k < levelist.size(); k++)
	{
		for (int i = 0; i < UNX; i++)
		{
			for (int j = 0; j < UNY; j++)
			{
				if (levelist[k].mp[0][i][j] >= 0)
				{
					for (int ii = 0; ii < UNX; ii++)
					{
						for (int jj = 0; jj < UNY; jj++)
						{
							if (levelist[k - 1].mp[0][ii][jj] >= 0)
							{
								int temp = OnDist(ImInt2(ii, jj) + levelist[k - 1].posd, ImInt2(i, j), levelist[k].dist, levelist[k].deflex) + levelist[k - 1].mp[0][ii][jj];
								if (levelist[k].mp[0][i][j] > temp)
								{
									bool flag = false;
									for (int a = levelist[k - 1].h; a <= levelist[k - 1].t; a++)
									{
										Area ara = arealist[a];
										ara.pos = ara.pos + int2vec(ImInt2(ii, jj)) - arealist[levelist[k - 1].h].pos;
										for (int b = levelist[k].h; b <= levelist[k].t; b++)
										{
											Area arb = arealist[b];
											arb.pos = arb.pos + int2vec(ImInt2(i, j)) - arealist[levelist[k].h].pos;
											if (IsAonA(ara, arb, 0))
											{
												flag = true;
												break;
											}
										}
										if (flag) break;
									}
									if (flag) continue;
									levelist[k].mp[0][i][j] = temp;
								}
							}
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < UNX; i++)
		for (int j = 0; j < UNY; j++)
			if (levelist.back().mp[1][i][j] >= 0) levelist.back().mp[1][i][j] = OnDist(ImInt2(i, j) + levelist.back().posd, iend, endist, endeflex);
	for (int k = levelist.size() - 2; k >= 0; k--)
	{
		for (int i = 0; i < UNX; i++)
		{
			for (int j = 0; j < UNY; j++)
			{
				if (levelist[k].mp[1][i][j] >= 0)
				{
					for (int ii = 0; ii < UNX; ii++)
					{
						for (int jj = 0; jj < UNY; jj++)
						{
							if (levelist[k + 1].mp[1][ii][jj] >= 0)
							{
								int temp = OnDist(ImInt2(i, j) + levelist[k].posd, ImInt2(ii, jj), levelist[k + 1].dist, levelist[k + 1].deflex) + levelist[k + 1].mp[1][ii][jj];
								if (levelist[k].mp[1][i][j] > temp)
								{
									bool flag = false;
									for (int a = levelist[k + 1].h; a <= levelist[k + 1].t; a++)
									{
										Area ara = arealist[a];
										ara.pos = ara.pos + int2vec(ImInt2(ii, jj)) - arealist[levelist[k + 1].h].pos;
										for (int b = levelist[k].h; b <= levelist[k].t; b++)
										{
											Area arb = arealist[b];
											arb.pos = arb.pos + int2vec(ImInt2(i, j)) - arealist[levelist[k].h].pos;
											if (IsAonA(ara, arb, 0))
											{
												flag = true;
												break;
											}
										}
										if (flag) break;
									}
									if (flag) continue;
									levelist[k].mp[1][i][j] = temp;
								}
							}
						}
					}
				}
			}
		}
	}
	return;
}

bool MakeU4Lpos(const std::vector<Area>& arealist, std::vector<Units4Level>& levelist, const int& kk)
{
	int mpdeflex = 0;
	ImInt2 pre = istart;
	for (int i = 0; i < UNX; i++)
		for (int j = 0; j < UNY; j++)
			unitmap[i][j] = false;
	ImInt2 tempos[LEVELCALCULATEDMAX];
	bool flag1 = true;
	for (int k = 0; k < levelist.size(); k++)
	{
		ImInt2 temp = pre;
		int tempcnt = 0;
		int tempdeflex = WINDING;
		for (int i = 0; i < UNX; i++)
		{
			for (int j = 0; j < UNY; j++)
			{
				if (levelist[k].mp[0][i][j] < 0) continue;
				if (k > 0)
				{
					bool flag2 = false;
					for (int a = levelist[k - 1].h; a <= levelist[k - 1].t; a++)
					{
						Area ara = arealist[a];
						ara.pos = ara.pos + int2vec(tempos[k - 1]) - arealist[levelist[k - 1].h].pos;
						for (int b = levelist[k].h; b <= levelist[k].t; b++)
						{
							Area arb = arealist[b];
							arb.pos = arb.pos + int2vec(ImInt2(i, j)) - arealist[levelist[k].h].pos;
							if (IsAonA(ara, arb, 0))
							{
								flag2 = true;
								break;
							}
						}
						if (flag2) break;
					}
					if (flag2) continue;
				}
				int temptempdeflex = mpdeflex + OnDist(pre, ImInt2(i, j), levelist[k].dist, levelist[k].deflex) + levelist[k].mp[1][i][j];
				if (temptempdeflex > thetamax) continue;
				int temptempcnt = 0;
				for (int m = 0; m < levelist[k].q.size(); m++)
				{
					ImInt2 tempq = ImInt2(i, j) + levelist[k].q[m];
					if (tempq.x >= 0 && temp.x < UNX && tempq.y >= 0 && tempq.y < UNY)
						if (!unitmap[tempq.x][tempq.y]) temptempcnt++;
				}
				if (((kk >> k) & 1) == 0)
				{
					if (tempdeflex > temptempdeflex || (tempdeflex == temptempdeflex && tempcnt < temptempcnt))
					{
						tempdeflex = temptempdeflex;
						tempcnt = temptempcnt;
						temp = ImInt2(i, j);
					}
				}
				else
				{
					if (tempcnt < temptempcnt || (tempcnt == temptempcnt && tempdeflex > temptempdeflex))
					{
						tempdeflex = temptempdeflex;
						tempcnt = temptempcnt;
						temp = ImInt2(i, j);
					}
				}
				flag1 = false;
			}
		}
		mpdeflex += OnDist(pre, temp, levelist[k].dist, levelist[k].deflex);
		tempos[k] = temp;
		pre = temp + levelist[k].posd;
		for (int m = 0; m < levelist[k].q.size(); m++)
		{
			ImInt2 tempq = temp + levelist[k].q[m];
			if (tempq.x >= 0 && temp.x < UNX && tempq.y >= 0 && tempq.y < UNY)
				unitmap[tempq.x][tempq.y] = true;
		}
	}
	if (flag1) return false;
	mpdeflex += levelist.back().mp[1][tempos[levelist.size() - 1].x][tempos[levelist.size() - 1].y];
	int mpcnt = 0;
	for (int i = 0; i < UNX; i++)
		for (int j = 0; j < UNY; j++)
			if (unitmap[i][j]) mpcnt++;
	if (pxnum < mpcnt || (pxnum == mpcnt && pxdef < mpdeflex))
	{
		pxnum = mpcnt;
		pxdef = mpdeflex;
		for (int k = 0; k < levelist.size(); k++)
			levelist[k].posh = tempos[k];
	}
	return true;
}

bool MakeU4LposReverse(const std::vector<Area>& arealist, std::vector<Units4Level>& levelist, const int& kk)
{
	int mpdeflex = 0;
	ImInt2 las = iend;
	for (int i = 0; i < UNX; i++)
		for (int j = 0; j < UNY; j++)
			unitmap[i][j] = false;
	ImInt2 tempos[LEVELCALCULATEDMAX];
	bool flag = true;
	for (int k = levelist.size() - 1; k >= 0; k--)
	{
		ImInt2 temp = las;
		int tempcnt = 0;
		int tempdeflex = WINDING;
		float ondistdist, ondistdeflex;
		if (k + 1 == levelist.size())
		{
			ondistdist = endist;
			ondistdeflex = endeflex;
		}
		else
		{
			ondistdist = levelist[k + 1].dist;
			ondistdeflex = levelist[k + 1].deflex;
		}
		for (int i = 0; i < UNX; i++)
		{
			for (int j = 0; j < UNY; j++)
			{
				if (levelist[k].mp[1][i][j] < 0) continue;
				if (k < levelist.size() - 1)
				{
					bool flag2 = false;
					for (int a = levelist[k + 1].h; a <= levelist[k + 1].t; a++)
					{
						Area ara = arealist[a];
						ara.pos = ara.pos + int2vec(tempos[k + 1]) - arealist[levelist[k + 1].h].pos;
						for (int b = levelist[k].h; b <= levelist[k].t; b++)
						{
							Area arb = arealist[b];
							arb.pos = arb.pos + int2vec(ImInt2(i, j)) - arealist[levelist[k].h].pos;
							if (IsAonA(ara, arb, 0))
							{
								flag2 = true;
								break;
							}
						}
						if (flag2) break;
					}
					if (flag2) continue;
				}
				int temptempdeflex = mpdeflex + OnDist(ImInt2(i, j) + levelist[k].posd, las, ondistdist, ondistdeflex) + levelist[k].mp[0][i][j];
				if (temptempdeflex > thetamax) continue;
				int temptempcnt = 0;
				for (int m = 0; m < levelist[k].q.size(); m++)
				{
					ImInt2 tempq = ImInt2(i, j) + levelist[k].q[m];
					if (tempq.x >= 0 && temp.x < UNX && tempq.y >= 0 && tempq.y < UNY)
						if (!unitmap[tempq.x][tempq.y]) temptempcnt++;
				}
				if (((kk >> k) & 1) == 0)
				{
					if (tempdeflex > temptempdeflex || (tempdeflex == temptempdeflex && tempcnt < temptempcnt))
					{
						tempdeflex = temptempdeflex;
						tempcnt = temptempcnt;
						temp = ImInt2(i, j);
					}
				}
				else
				{
					if (tempcnt < temptempcnt || (tempcnt == temptempcnt && tempdeflex > temptempdeflex))
					{
						tempdeflex = temptempdeflex;
						tempcnt = temptempcnt;
						temp = ImInt2(i, j);
					}
				}
				flag = false;
			}
		}
		mpdeflex += OnDist(temp + levelist[k].posd, las, ondistdist, ondistdeflex);
		tempos[k] = las = temp;
		for (int m = 0; m < levelist[k].q.size(); m++)
		{
			ImInt2 tempq = temp + levelist[k].q[m];
			if (tempq.x >= 0 && temp.x < UNX && tempq.y >= 0 && tempq.y < UNY)
				unitmap[tempq.x][tempq.y] = true;
		}
	}
	if (flag) return false;
	mpdeflex += levelist[0].mp[0][tempos[0].x][tempos[0].y];
	int mpcnt = 0;
	for (int i = 0; i < UNX; i++)
		for (int j = 0; j < UNY; j++)
			if (unitmap[i][j]) mpcnt++;
	if (pxnum < mpcnt || (pxnum == mpcnt && pxdef < mpdeflex))
	{
		pxnum = mpcnt;
		pxdef = mpdeflex;
		for (int k = 0; k < levelist.size(); k++)
			levelist[k].posh = tempos[k];
	}
	return true;
}

void RearrangeAreas(std::vector<Area>& arealist)
{
	std::vector<Units4Level> levelist;
	if (!MakeU4Lrot(arealist, levelist)) return;
	if (!MakeU4Lq(arealist, levelist)) return;
	istart = vec2int(bstart);
	iend = vec2int(bend);
	MakeU4Lmp(arealist, levelist);
	thetamax = THETAMAX * (levelist.size() + 1);
	int thetamaxmin = WINDING;
	for (int i = 0; i < UNX; i++)
		for (int j = 0; j < UNY; j++)
			if (levelist[0].mp[0][i][j] > 0)
				if (thetamaxmin > levelist[0].mp[0][i][j] + levelist[0].mp[1][i][j]) thetamaxmin = levelist[0].mp[0][i][j] + levelist[0].mp[1][i][j];
	while (thetamax < thetamaxmin)
		thetamax += 10 * (levelist.size() + 1);
	pxnum = 0;
	bool flag = true, thisfinish = false;;
	int finishcnt = 0;
	std::vector<int> ranseek;
	if ((1 << levelist.size()) <= SEEKMAX)
	{
		for (int i = 0; i < (1 << levelist.size()); i++)
			ranseek.push_back(i);
	}
	else
	{
		std::uniform_int_distribution<> dis(0, (1 << levelist.size()) - 1);
		std::map<int,int> mp;
		for (int i = 0; i < SEEKMAX; i++)
		{
			int ran = dis(gen);
			while (mp.find(ran) != mp.end())
				ran = dis(gen);
			mp.emplace(ran, 0);
			ranseek.push_back(ran);
		}
	}
	for (int kk = 0; kk < ranseek.size(); kk++)
	{
		if (MakeU4Lpos(arealist, levelist, ranseek[kk])) flag = false;
		finishcnt++;
		makepercent = 0.5f + 0.25f * finishcnt / ranseek.size();
		thisfinish = makefinish;
		if (thisfinish) return;
		if (MakeU4LposReverse(arealist, levelist, ranseek[kk])) flag = false;
		finishcnt++;
		makepercent = 0.5f + 0.25f * finishcnt / ranseek.size();
		thisfinish = makefinish;
		if (thisfinish) return;
	}
	if (flag) return;
	for (int i = 0; i < levelist.size(); i++)
	{
		arealist[levelist[i].h].pos = int2vec(levelist[i].posh);
		for (int j = levelist[i].h + 1; j <= levelist[i].t; j++)
		{
			ImVec2 dirc(cosf(-arealist[j].deflex * PI / 180.0f), sinf(-arealist[j].deflex * PI / 180.0f));
			arealist[j].pos = arealist[j - 1].pos + scale * arealist[j].dist * dirc;
		}
	}
	return;
}