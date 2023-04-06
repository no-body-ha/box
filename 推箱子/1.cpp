#include<stdio.h>
#include<easyx.h>
#include<conio.h>//获取键盘信息
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
enum number
{
	SPACE = 0,//空地
	Wall = 1,//墙壁
	BOX = 2,//箱子
	AIM = 3,//目的地
	PERSON = 4,//小人
	BOX_AIM = 5,//箱子在目的地上 2+3=5
	PERSON_AIM = 7,//小人在目的地上 4+3=7
};
//地图
int num = 0;//当前关卡数 默认为第一关
char f;

int MAP[3][8][8]
{
	//第一关
	{
		{0,0,1,1,1,0,0,0},
		{0,0,1,3,1,0,0,0},
		{0,0,1,0,1,1,1,1},
		{1,1,1,2,0,2,3,1},
		{1,3,0,2,4,1,1,1},
		{1,1,1,1,2,1,0,0},
		{0,0,0,1,3,1,0,0},
		{0,0,0,1,1,1,0,0},
	},
	//第二关
	{
		{0,1,1,1,1,1,0,0},
		{0,1,0,0,0,1,0,0},
		{1,1,0,0,0,1,1,1},
		{1,0,2,2,2,2,0,1},
		{1,0,3,3,3,3,0,1},
		{1,1,0,4,0,1,1,1},
		{0,1,1,1,1,1,0,0},
		{0,0,0,0,0,0,0,0},
	},
	//第三关
	{
		{0,0,1,1,1,1,0,0},
		{0,0,1,0,0,1,1,1},
		{1,1,1,0,3,0,0,1},
		{1,4,2,2,2,0,0,1},
		{1,0,0,0,1,3,0,1},
		{1,1,1,3,0,0,1,1},
		{0,0,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0},
	},

};
int MAP_static[3][8][8]
{
	//第一关
	{
		{0,0,1,1,1,0,0,0},
		{0,0,1,3,1,0,0,0},
		{0,0,1,0,1,1,1,1},
		{1,1,1,2,0,2,3,1},
		{1,3,0,2,4,1,1,1},
		{1,1,1,1,2,1,0,0},
		{0,0,0,1,3,1,0,0},
		{0,0,0,1,1,1,0,0},
	},
	//第二关
	{
		{0,1,1,1,1,1,0,0},
		{0,1,0,0,0,1,0,0},
		{1,1,0,0,0,1,1,1},
		{1,0,2,2,2,2,0,1},
		{1,0,3,3,3,3,0,1},
		{1,1,0,4,0,1,1,1},
		{0,1,1,1,1,1,0,0},
		{0,0,0,0,0,0,0,0},
	},
	//第三关
	{
		{0,0,1,1,1,1,0,0},
		{0,0,1,0,0,1,1,1},
		{1,1,1,0,3,0,0,1},
		{1,4,2,2,2,0,0,1},
		{1,0,0,0,1,3,0,1},
		{1,1,1,3,0,0,1,1},
		{0,0,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0},
	},

};
// 辅助地图
int MAP_TEMP[8][8]{};
//步数
int stepNumber1[3]{20,80,80};
int stepNumber = 20;
//小人当前的位置
int l;//行
int c;//列

//游戏结束标志
int flag = 0;//默认0为未结束

//游戏分数
int score;

/*
* ---------菜单界面-----------
	负责人：
	功能：
		1.展示选项：开始游戏 设置 退出游戏 背景介绍与操作教学 充值
		2.用户通过鼠标点击选择功能
		3.要求：点击开始游戏能进入游戏界面，点击设置能进入设置界面，
		点击退出函数返回1，其余非核心不做要求

		参数：NULL
		返回值：int ---如果点击退出游戏则返回1，其余情况返回0。
*/
int menuView();
int menuView1();
void set1();

/*
-------------------设置界面-------------------
	负责人：
	功能：
		1.展示选项：个人中心   音乐，音效    分享  返回主菜单
		2.用户通过鼠标点击音乐，音效可以实现背景音乐的开关功能，点击返回主菜单
		能够返回到主菜单界面
		其余非核心不做要求
		参数：NULL
		返回值：NULL
*/
void set();

/*
------------------绘制游戏界面-------------------
	负责人：
	功能：
		1.用不同的贴图覆盖游戏的元素（人，墙.....）
		2.显示步数
		3.点击暂停界面能进入暂停界面
		参数：NULL
		返回值：NULL

*/
void drawMap();

/*
--------------------暂停界面--------------
	负责人：
	功能：
		1.展示选项：继续游戏 重新开始 设置 返回主菜单
		2.用户点击不同的选项可以进入到选项对应的功能界面中
		参数：NULL
		返回值:NULL
*/
void drawPause();

/*
---------------获取小人位置------------
	负责人：
	功能：
		1.通过循环找到小人（1.小人在空地，2.小人在目的地），将小人的位置赋给
		全局变量line column
		参数：NULL
		返回值：NULL
*/
void getPersonXY();

/*
------------------判断小人是否移动------------
	负责人：
	功能：
		1.通过头文件conio.h获得用户键盘输入（wasd 上左下右）
		2.逻辑判断小人是否可以移动，如果可以则更改地图数据（例如：小人当前位置数字变为空地的数字，移动后的位置的数字变为小人的数字）来实现移动
		3.玩家每走一步，步数减一
		4.回退功能，玩家按回退键可实现返回上一步，相应的步数加一
		参数：NULL
		返回值：NULL
*/
void movePerson();

/*
--------------判断游戏是否结束------------
	负责人：
	功能：
		1.判断目的地是否全被箱子覆盖
		2.判断步数是否用完
		3.通过玩家表现修改score值（评分标准可自行决断）
	参数：	NULL
	返回值：int 游戏成功返回0，游戏失败返回1
*/
int gameover();

/*
-------------失败界面--------------
	负责人：
	功能:
		1.展示评分和步数 重新开始 返回主菜单
		2.用户点击重新开始可以重新打印该关的地图，点击返回主菜单可以回到主菜单界面
		参数：NULL
		返回值：NULL

*/
void failure();

/*
--------------成功界面----------
	负责人：
	功能：
	1.展示分数  下一关 返回主菜单
	2.用户点击下一关可以跳到下一关游戏界面 点击返回主菜单可以回到主菜单
*/
void success(int k);


void run_geme(int k);
void traceback();
void init(void);
void init1(void);
//充值界面
void Topup();

//游戏背景
void Background();
int main()
{
	init();

	getchar();
	closegraph();
	return 0;
}

void init(void)
{
	initgraph(640, 640, EX_SHOWCONSOLE);//初始化绘图窗口
	//menuView();
	system("cls");//清屏
	int exit = 0;//退出判断
	int k = -1;//游戏成功与否判断
	exit = menuView(); 
	if (exit == 0)
	{
		run_geme(k);
	}                                                    
}
void init1(void)
{
	initgraph(640, 640, EX_SHOWCONSOLE);//初始化绘图窗口
	//menuView();
	system("cls");//清屏
	int exit = 0;//退出判断
	int k = -1;//游戏成功与否判断
	exit = menuView1();
	if (exit == 0)
	{
		run_geme(k);
	}
}
void failure()
{
	HWND hnd = GetHWnd();
	int isok=MessageBox(hnd, "你太菜咯哥们，憋玩啦~", "警告",MB_OKCANCEL);
	if (isok == IDOK)
	{
		init();
	}
	else if (isok == IDCANCEL)
	{
		cleardevice();
		stepNumber = 20;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				MAP[num][i][j] = MAP_static[num][i][j];
			}
		}
		run_geme(-1);
	}
}
void getPersonXY()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (MAP[num][i][j] == PERSON || MAP[num][i][j] == PERSON_AIM)
			{
				l = i;
				c = j;
			}
		}
	}
}

void run_geme(int k)
{
	int x(540), y(0), width(100), heigth(60), Ratio(50);
	drawMap();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			MAP_TEMP[i][j] = MAP[num][i][j];
		}
	}
	while (1)
	{
		if (_kbhit())
		{
			movePerson();
			drawMap();
		
		}
		k = gameover();
		if (k == 1)
			failure();
			//printf("失败");
		else if (k == 0)
			success(k);
		ExMessage Msg;
		if (peekmessage(&Msg, WH_MOUSE)) {
			switch (Msg.message)
			{
			case WM_LBUTTONUP:
				if (Msg.x >= x && Msg.y <= width && Msg.y >= 0 && Msg.y <= heigth) {
					cleardevice();
					drawPause();
				}
				break;
			default:
				break;
			}
		}

	}
}

int menuView()
{
	cleardevice();
	int i = 0;//判断是否点击了退出游戏按钮
	IMAGE age;
	ExMessage msg;
	loadimage(&age, "./菜单界面背景图.jpg", 640, 640);
	putimage(0, 0, &age);
	settextcolor(BLACK);
	//settextstyle(64, 0, "宋体");
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 64;
	_tcscpy(f.lfFaceName, "黑体");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	outtextxy(160, 0, "推箱子游戏");

	rectangle(250, 100, 402, 152);
	rectangle(200, 200, 452, 252);
	rectangle(250, 300, 402, 352);
	rectangle(250, 400, 402, 452);
	rectangle(250, 500, 402, 552);

	settextcolor(BLUE);
	settextstyle(30, 0, "楷体");
	char arr1[] = "开始游戏";
	int width1 = 152 / 2 - textwidth(arr1) / 2;
	int height1 = 52 / 2 - textheight(arr1) / 2;
	outtextxy(width1 + 250, height1 + 100, arr1);

	char arr2[] = "游戏介绍及其背景";
	int width2 = 252 / 2 - textwidth(arr2) / 2;
	int height2 = 52 / 2 - textheight(arr2) / 2;
	outtextxy(width2 + 200, height2 + 200, arr2);

	char arr3[] = "设置";
	int width3 = 152 / 2 - textwidth(arr3) / 2;
	int height3 = 52 / 2 - textheight(arr3) / 2;
	outtextxy(width3 + 250, height3 + 300, arr3);

	char arr4[] = "充值";
	int width4 = 152 / 2 - textwidth(arr4) / 2;
	int height4 = 52 / 2 - textheight(arr4) / 2;
	outtextxy(width4 + 250, height4 + 400, arr4);

	char arr5[] = "退出游戏";
	int width5 = 152 / 2 - textwidth(arr5) / 2;
	int height5 = 52 / 2 - textheight(arr5) / 2;
	outtextxy(width5 + 250, height5 + 500, arr5);

	while (true)
	{
		if (peekmessage(&msg, WH_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
			{
				if (msg.x >= 250 && msg.x <= 250 + 152 && msg.y >= 100 && msg.y <= 100 + 52)
				{
					//drawMap();
					return 0;
				}
				else if (msg.x >= 200 && msg.x <= 200 + 252 && msg.y >= 200 && msg.y <= 200 + 52)
				{
					//printf("该功能暂未开放！！！\n");
					Background();
				}
				else if (msg.x >= 250 && msg.x <= 250 + 152 && msg.y >= 300 && msg.y <= 300 + 52)
				{
					set();
					return 0;
				}
				else if (msg.x >= 250 && msg.x <= 250 + 152 && msg.y >= 400 && msg.y <= 400 + 52)
				{
					//printf("该功能暂未开放！！！\n");
					Topup();
				}
				else if (msg.x >= 250 && msg.x <= 250 + 152 && msg.y >= 500 && msg.y <= 500 + 52)
				{
					i = 1;
					closegraph();
					break;

				}
			}
			}

		}
		if (i == 1) break;
	}
	if (i == 1)
	{
		return 1;
	}
	else
		return 0;
};
int menuView1()
{
	cleardevice();
	int i = 0;//判断是否点击了退出游戏按钮
	IMAGE age;
	ExMessage msg;
	loadimage(&age, "./菜单界面背景图.jpg", 640, 640);
	putimage(0, 0, &age);
	settextcolor(BLACK);
	//settextstyle(64, 0, "宋体");
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 64;
	_tcscpy(f.lfFaceName, "黑体");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	outtextxy(160, 0, "推箱子游戏");

	rectangle(250, 100, 402, 152);
	rectangle(200, 200, 452, 252);
	rectangle(250, 300, 402, 352);
	rectangle(250, 400, 402, 452);
	rectangle(250, 500, 402, 552);

	settextcolor(BLUE);
	settextstyle(30, 0, "楷体");
	char arr1[] = "继续游戏";
	int width1 = 152 / 2 - textwidth(arr1) / 2;
	int height1 = 52 / 2 - textheight(arr1) / 2;
	outtextxy(width1 + 250, height1 + 100, arr1);

	char arr2[] = "游戏介绍及其背景";
	int width2 = 252 / 2 - textwidth(arr2) / 2;
	int height2 = 52 / 2 - textheight(arr2) / 2;
	outtextxy(width2 + 200, height2 + 200, arr2);

	char arr3[] = "设置";
	int width3 = 152 / 2 - textwidth(arr3) / 2;
	int height3 = 52 / 2 - textheight(arr3) / 2;
	outtextxy(width3 + 250, height3 + 300, arr3);

	char arr4[] = "充值";
	int width4 = 152 / 2 - textwidth(arr4) / 2;
	int height4 = 52 / 2 - textheight(arr4) / 2;
	outtextxy(width4 + 250, height4 + 400, arr4);

	char arr5[] = "退出游戏";
	int width5 = 152 / 2 - textwidth(arr5) / 2;
	int height5 = 52 / 2 - textheight(arr5) / 2;
	outtextxy(width5 + 250, height5 + 500, arr5);

	while (true)
	{
		if (peekmessage(&msg, WH_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
			{
				if (msg.x >= 250 && msg.x <= 250 + 152 && msg.y >= 100 && msg.y <= 100 + 52)
				{
					//drawMap();
					return 0;
				}
				else if (msg.x >= 200 && msg.x <= 200 + 252 && msg.y >= 200 && msg.y <= 200 + 52)
				{
					//printf("该功能暂未开放！！！\n");
					Background();
				}
				else if (msg.x >= 250 && msg.x <= 250 + 152 && msg.y >= 300 && msg.y <= 300 + 52)
				{
					set();
					return 0;
				}
				else if (msg.x >= 250 && msg.x <= 250 + 152 && msg.y >= 400 && msg.y <= 400 + 52)
				{
					//printf("该功能暂未开放！！！\n");
					Topup();
				}
				else if (msg.x >= 250 && msg.x <= 250 + 152 && msg.y >= 500 && msg.y <= 500 + 52)
				{
					i = 1;
					closegraph();
					break;

				}
			}
			}

		}
		if (i == 1) break;
	}
	if (i == 1)
	{
		return 1;
	}
	else
		return 0;
};
void success(int k)
{
	
	cleardevice();
	IMAGE age;
	ExMessage msg;
	loadimage(&age, "./胜利背景图.jpg", 640, 640);
	putimage(0, 0, &age);

	rectangle(100, 400, 252, 452);
	rectangle(400, 400, 552, 452);

	char a1[] = "下一关";
	char a2[] = "返回主菜单";

	settextcolor(GREEN);
	settextstyle(30, 0, "楷体");

	int width1 = 152 / 2 - textwidth(a1) / 2;
	int height1 = 52 / 2 - textheight(a1) / 2;
	outtextxy(width1 + 100, height1 + 400, a1);

	int width2 = 152 / 2 - textwidth(a2) / 2;
	int height2 = 52 / 2 - textheight(a2) / 2;
	outtextxy(width2 + 400, height2 + 400, a2);

	settextcolor(RED);
	settextstyle(64, 0, "仿宋体");
	outtextxy(200, 0, "关卡胜利");

	settextstyle(50, 0, "仿宋体");
	if (score >= 90)
	{
		outtextxy(200, 200, "Very Good");
	}
	else
		outtextxy(270, 200, "Good");
	while (true)
	{
		if (peekmessage(&msg, WH_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
			{
				if (msg.x >= 100 && msg.x <= 252 && msg.y >= 400 && msg.y <= 452)
				{
					num++;
					stepNumber = stepNumber1[num];
					run_geme(k);
				}
				else if (msg.x >= 400 && msg.x <= 552 && msg.y >= 400 && msg.y <= 452)
				{
					menuView();
				}
			}
			}
		}
	}

}
void drawPause() {
	int x(120), y(100), width(520), heigth(150), Ratio(50), increase(80);
	int rectangle_heigth(heigth - y), text_heigth(heigth - y);
	IMAGE BKimage;
	char _num[4][20] = {
		"继续游戏",
		"设置",
		"重新开始",
		"返回主菜单",
	};
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setfillcolor(RGB(117, 250, 141));
	settextcolor(WHITE);
	loadimage(&BKimage, "./Background2.png", 640, 640, true);
	putimage(0, 0, &BKimage);
	fillrectangle(x, y, width, heigth);
	outtextxy(x + (400 - textwidth(_num[0])) / 2, y + (rectangle_heigth - textheight(_num[0])) / 2, _num[0]);
	fillrectangle(x, y + heigth - y + increase, width, y + 2 * (heigth - y) + increase);
	outtextxy(x + (400 - textwidth(_num[1])) / 2, y + text_heigth + increase + (rectangle_heigth - textheight(_num[1])) / 2, _num[1]);
	fillrectangle(x, y + 2 * (heigth - y) + 2 * increase, width, y + 3 * (heigth - y) + 2 * increase);
	outtextxy(x + (400 - textwidth(_num[2])) / 2, y + 2 * (heigth - y) + 2 * increase + (rectangle_heigth - textheight(_num[2])) / 2, _num[2]);
	fillrectangle(x, y + 3 * (heigth - y) + 3 * increase, width, y + 4 * (heigth - y) + 3 * increase);
	outtextxy(x + (400 - textwidth(_num[3])) / 2, y + 3 * (heigth - y) + 3 * increase + (rectangle_heigth - textheight(_num[1])) / 2, _num[3]);
	ExMessage MOUSE;
	while (true) {
		if (peekmessage(&MOUSE, WH_MOUSE)) {
			switch (MOUSE.message)
			{
			case WM_LBUTTONDOWN:
				if (MOUSE.x >= x && MOUSE.x <= width && MOUSE.y >= y && MOUSE.y <= heigth) {
					cleardevice();
					run_geme(-1);
					return;
				}
				if (MOUSE.x >= x && MOUSE.x <= width && MOUSE.y >= y + heigth - y + increase && MOUSE.y <= y + 2 * (heigth - y) + increase) {
					cleardevice();
					set1();
					//set();
				}
				if (MOUSE.x >= x && MOUSE.x <= width && MOUSE.y >= y + 2 * (heigth - y) + 2 * increase && MOUSE.y <= y + 3 * (heigth - y) + 2 * increase) {
					cleardevice();
					stepNumber = stepNumber1[num];
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							MAP[num][i][j] = MAP_static[num][i][j];
						}
					}
					run_geme(-1);
				}
				if (MOUSE.x >= x && MOUSE.x <= width && MOUSE.y >= y + 3 * (heigth - y) + 3 * increase && MOUSE.y <= y + 4 * (heigth - y) + 3 * increase) {
					cleardevice();
					init1();
				}
				break;
			default:
				break;
			}
		}
	}
}

void drawMap() {
	int x(540), y(0), width(100), heigth(60), Ratio(50);
	IMAGE img[8];
	char _num[10];
	int q, k;
	sprintf_s(_num, "% d", stepNumber);
	//空地-0 墙-1 箱子-2 目的地-3 小人-4 箱子在目的地-5 小人在目的地-7
	//加载游戏贴图
	loadimage(&img[0], "./SPACE.png", Ratio, Ratio);
	loadimage(&img[1], "./Wall.png", Ratio, Ratio);
	loadimage(&img[2], "./BOX.png", Ratio, Ratio);
	loadimage(&img[3], "./AIM.png", Ratio, Ratio);
	loadimage(&img[4], "./PERSON.png", Ratio, Ratio);
	loadimage(&img[5], "./BOX_AIM.png", Ratio, Ratio);
	loadimage(&img[7], "./PERSON.png", Ratio, Ratio);
	setbkcolor(RGB(0, 0, 0));
	cleardevice();
	//打印游戏贴图
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//MAP_TEMP[i][j] = MAP[num][i][j];
			k = 0; q = 0;
			k += 120 + j * Ratio, q += 120 + i * Ratio;
			//putimage(120 + i * Ratio, 120 + j * Ratio, &img[MAP[num][i][j]]);
			switch (MAP[num][i][j])
			{
			case SPACE:
				putimage(k, q, &img[0]);
				break;
			case Wall:
				putimage(k, q, &img[1]);
				break;
			case BOX:
				putimage(k, q, &img[2]);
				break;
			case PERSON:
				putimage(k, q, &img[4]);
				break;
			case AIM:
				putimage(k, q, &img[3]);
				break;
			case BOX_AIM:
				putimage(k, q, &img[5]);
				break;
			case PERSON_AIM:
				putimage(k, q, &img[7]);
				break;
			}
		}
	}
	//暂停键
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	setfillcolor(RED);
	fillroundrect(x, y, x + width, y + heigth, 15, 15);
	outtextxy(x + (width - textwidth("暂停")) / 2, y + (heigth - textheight("暂停")) / 2, "暂停");
	outtextxy(0, 10, "步数:");
	outtextxy(0 + textwidth("步数:"), 10, _num);
}

void movePerson()
{

	getPersonXY();
	char key = _getch();
	int x(540), y(0), width(100), heigth(60), Ratio(50);
	switch (key)
	{
	case 'w':
	case 'W':
	case 72:
		stepNumber--;
		if (MAP[num][l - 1][c] == SPACE || MAP[num][l - 1][c] == AIM)
		{
			f = 'w';
			MAP[num][l - 1][c] += PERSON;
			MAP[num][l][c] -= PERSON;
		}
		else if (MAP[num][l - 1][c] == BOX || MAP[num][l - 1][c] == BOX_AIM)
		{
			if (MAP[num][l - 2][c] == SPACE || MAP[num][l - 2][c] == AIM)
			{
				f = 'W';
				MAP[num][l - 2][c] += BOX;
				MAP[num][l - 1][c] += (PERSON - BOX);
				MAP[num][l][c] -= PERSON;

			}
		}
		break;    //up

	case 'a':
	case 'A':
	case 75:
		stepNumber--;
		
		if (MAP[num][l][c - 1] == SPACE || MAP[num][l][c - 1] == AIM)
		{
            f = 'a';
			MAP[num][l][c - 1] += PERSON;
			MAP[num][l][c] -= PERSON;
		}
		else if (MAP[num][l][c - 1] == BOX || MAP[num][l][c - 1] == BOX_AIM)
		{
			if (MAP[num][l][c - 2] == SPACE || MAP[num][l][c - 2] == AIM)
			{
				f = 'A';
				MAP[num][l][c - 2] += BOX;
				MAP[num][l][c - 1] += (PERSON - BOX);
				MAP[num][l][c] -= PERSON;

			}
		}
		break;    //left

	case 's':
	case 'S':
	case 80:
		
		stepNumber--;
		if (MAP[num][l + 1][c] == SPACE || MAP[num][l + 1][c] == AIM)
		{
			f = 's';
			MAP[num][l + 1][c] += PERSON;
			MAP[num][l][c] -= PERSON;
		}
		else if (MAP[num][l + 1][c] == BOX || MAP[num][l + 1][c] == BOX_AIM)
		{
			if (MAP[num][l + 2][c] == SPACE || MAP[num][l + 2][c] == AIM)
			{
				f = 'S';
				MAP[num][l + 2][c] += BOX;
				MAP[num][l + 1][c] += (PERSON - BOX);
				MAP[num][l][c] -= PERSON;

			}
		}
		break;    //down

	case 'd':
	case 'D':
	case 77:
		
		stepNumber--;
		if (MAP[num][l][c + 1] == SPACE || MAP[num][l][c + 1] == AIM)
		{
			f = 'd';
			MAP[num][l][c + 1] += PERSON;
			MAP[num][l][c] -= PERSON;
		}
		else if (MAP[num][l][c + 1] == BOX || MAP[num][l][c + 1] == BOX_AIM)
		{
			if (MAP[num][l][c + 2] == SPACE || MAP[num][l][c + 2] == AIM)
			{
				f = 'D';
				MAP[num][l][c + 2] += BOX;
				MAP[num][l][c + 1] += (PERSON - BOX);
				MAP[num][l][c] -= PERSON;

			}
		}
		break;    //right
	case 'f': traceback(); break;
	}
}


int gameover()
{
	int state = 0;                        //设置初始状态 
	score = stepNumber * 10;                     //用更少的步数结束游戏会得到更高分  一步多十分 

	for (int i = 0; i < 10; i++)                 //循环遍历场中是否还存在箱子 
	{
		for (int j = 0; j < 10; j++)
		{
			if (MAP[num][i][j] == BOX)
			{
				state = 1;
				break;
			}
		}
	}

	if (state == 0 && stepNumber != 0)              //判断步数与现场是否还有箱子没有推到目的地 
	{
		return 0;
	}
	else if (state == 1 && stepNumber ==0)
	{
		return 1;                                    //游戏失败
	}
	else
	{
		return 2;                                 //正常运行
	}
}
void BGM()
{
	// pe mpegvideo
		//mciSendString("open mus_xpart_a.wav alias BGM1 type mpegcideo",0,0,0);
	mciSendString("open mus_xpart_a.wav alias BGM1 type mpegvideo", 0, 0, 0);
	mciSendString("play BGM1 repeat", 0, 0, 0);

}
void STOP()
{
	mciSendString("open mus_xpart_a.wav alias BGM1 type mpegvideo", 0, 0, 0);
	mciSendString("close BGM1", 0, 0, 0);

}
void set1()
{
	//BGM();

	IMAGE img;
	loadimage(&img, "./11223.jpg", getwidth(), getheight());
	putimage(0, 0, &img);

	ExMessage msg;    //存放鼠标的值
	while (true)
	{
		//ExMessage msg;
		if (peekmessage(&msg, WH_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
			{
				if (msg.x >= 210 && msg.x <= 390 && msg.y >= 250 && msg.y <= 590)
				{
					BGM();
				}
				else if (msg.x >= 400 && msg.x <= 630 && msg.y >= 250 && msg.y <= 590)
				{
					STOP();
				}
				else if (msg.x >= 10 && msg.x <= 200 && msg.y >= 250 && msg.y <= 590)
				{
					cleardevice();
					run_geme(-1);
				}

				break;
			}
			}
		}
	}
}
void set()
{
	//BGM();

	IMAGE img;
	loadimage(&img, "./11223.jpg", getwidth(), getheight());
	putimage(0, 0, &img);

	ExMessage msg;    //存放鼠标的值
	while (true)
	{
		//ExMessage msg;
		if (peekmessage(&msg, WH_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
			{
				if (msg.x >= 210 && msg.x <= 390 && msg.y >= 250 && msg.y <= 590)
				{
					BGM();
				}
				else if (msg.x >= 400 && msg.x <= 630 && msg.y >= 250 && msg.y <= 590)
				{
					STOP();
				}
				else if (msg.x >= 10 && msg.x <= 200 && msg.y >= 250 && msg.y <= 590)
				{
					init();
				}

				break;
			}
			}
		}
	}
}
void Topup()
{
	setbkcolor(WHITE);
	cleardevice();
	setfillcolor(GREEN);
	IMAGE age;
	loadimage(&age, "./收款码.jpg", 480, 480);
	putimage(80, 0, &age);
	ExMessage msg;
	fillrectangle(200, 500, 400, 600);
	settextcolor(BLACK);
	settextstyle(30, 0, "楷体");
	char arr1[] = "返回";
	int width1 = 200 / 2 - textwidth(arr1) / 2;
	int height1 = 100 / 2 - textheight(arr1) / 2;
	outtextxy(width1 + 200, height1 + 500, arr1);
	while (true)
	{
		if (peekmessage(&msg, WH_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
			{
				if (msg.x >= 200 && msg.x <= 400 && msg.y >= 500 && msg.y <= 600)
				{
					init();
				}
			}
			}
		}
	}
}
void Background()
{
	cleardevice();
	setfillcolor(GREEN);
	IMAGE age;
	loadimage(&age, "./Background.jpg",640, 640);
	putimage(0, 0, &age);
	ExMessage msg;
	fillrectangle(200, 500, 400, 600);
	settextcolor(BLACK);
	settextstyle(30, 0, "楷体");
	char arr1[] = "返回";
	int width1 = 200 / 2 - textwidth(arr1) / 2;
	int height1 = 100 / 2 - textheight(arr1) / 2;
	outtextxy(width1 + 200, height1 + 500, arr1);
	settextcolor(BLACK);
	outtextxy(0, 0, "背景介绍：");
	outtextxy(0, 30, "long long ago，有一个小蓝人被困进了地牢当");
	outtextxy(0, 60, "中，并被下毒了N步逍遥散，在它的周围有一些");
	outtextxy(0, 90, "木箱子，如果不能在N步之内将箱子推到目的地");
	outtextxy(0, 120, "他将会受到怎样的处罚……而这个地牢………");
	outtextxy(0, 150, "究竟有多深呢……");
	outtextxy(0, 180, "游戏介绍：");
	outtextxy(0, 210, "在本游戏中，你将在有限的步数内将所有箱子");
	outtextxy(0, 240, "推到目的地。");
	outtextxy(0, 270, "w向上移动，s向下移动，d向右移动 ");
	outtextxy(0, 300, "a向左移动，f回退功能");
	outtextxy(0, 330, "（亦可使用↑↓←→）");
	while (true)
	{
		if (peekmessage(&msg, WH_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
			{
				if (msg.x >= 200 && msg.x <= 400 && msg.y >= 500 && msg.y <= 600)
				{
					init();
				}
			}
			}
		}
	}
}
void traceback()                                   //回退功能
{
	getPersonXY();
	switch (f)
	{
	case 'w':                  //人物向下回退
		f= '0';
		MAP[num][l + 1][c] += PERSON;
		MAP[num][l][c] -= PERSON;
		stepNumber++;
		drawMap();
		break;    //down
	case 'W':                  //带箱向下回退
		f = '0';
		MAP[num][l-1][c] -= BOX;
		MAP[num][l][c] += (BOX-PERSON);
		MAP[num][l + 1][c] += PERSON;
		stepNumber++;
		drawMap();
		break;
	case 'a':              //人物向右回退
		f = '0';
		MAP[num][l][c+1] += PERSON;
		MAP[num][l][c] -= PERSON;
		stepNumber++;
		drawMap();
		break;    //down
	case 'A':              //带箱向右回退
		f = '0';
		MAP[num][l][c-1]-= BOX;
		MAP[num][l][c] += (BOX - PERSON);
		MAP[num][l][c+1] += PERSON;
		stepNumber++;
		drawMap();
		break;

	case 's':              //人物向上回退
		f = '0';
		MAP[num][l - 1][c] += PERSON;
		MAP[num][l][c] -= PERSON;
		stepNumber++;
		drawMap();
		break;    
	case 'S':              //带箱向上回退
		f = '0';
		MAP[num][l + 1][c] -= BOX;
		MAP[num][l][c] += (BOX - PERSON);
		MAP[num][l - 1][c] += PERSON;
		stepNumber++;
		drawMap();
		break;
	case 'd':              //人物向左回退
		f = '0';
		MAP[num][l][c - 1] += PERSON;
		MAP[num][l][c] -= PERSON;
		stepNumber++;
		drawMap();
		break;    
	case 'D':              //带箱向左回退
		f = '0';
		MAP[num][l][c + 1] -= BOX;
		MAP[num][l][c] += (BOX - PERSON);
		MAP[num][l][c - 1] += PERSON;
		stepNumber++;
		drawMap();
		break;
	case '0': break;
	}
}
