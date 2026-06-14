#include <stdio.h>
#include <graphics.h> //easx 图形库
#include <string.h>
#include <time.h>
#include<conio.h> //kbhit头文件

int score = 0;
int rank = 0;


#define BLOCK_COUNT 5
#define BLOCK_WIDTH 5
#define BLOCK_HIGHT 5
#define UNIT_SIZE 20

#define START_X 130
#define START_Y 30

#define KEY_UP 72
#define KEY_RIGHT 77
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_space 32//定义键盘活动对应编码

int speed = 500;
int minX = 30;
int minY = 30;




//定义枚举类型
typedef enum {
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN,
	BLOCK_LEFT,
}block_dir_t;

//定义方块是否可移动
typedef enum {
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
}move_dir_t;


int Nestindex = -1;
int BlockIndex = -1;

int color[BLOCK_COUNT]{
  GREEN,CYAN,MAGENTA,BROWN,YELLOW
};

int visit[30][15]; //访问数组
int markColor[30][15];//访问对应颜色

int block[BLOCK_COUNT * 4][BLOCK_WIDTH][BLOCK_HIGHT] = {
	//I型方块
	{
      0,0,0,0,0,
      0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,0,0,0
    },
	{
	  0,0,0,0,0,
	  0,0,0,0,0,
	  0,1,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0
	},
	{
	  0,0,0,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,0,0,0
	},
	{
	  0,0,0,0,0,
	  0,0,0,0,0,
	  0,1,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0
	},
	//L型
	{
	  0,0,0,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,1,1,0,
	  0,0,0,0,0
    },
	{
	  0,0,0,0,0,
	  0,0,0,0,0,
	  0,1,1,1,0,
	  0,1,0,0,0,
	  0,0,0,0,0

    },
	{
	  0,0,0,0,0,
	  0,1,1,0,0,
	  0,0,1,0,0,
	  0,0,1,0,0,
	  0,0,0,0,0
	},
	{
	  0,0,0,0,0,
	  0,0,0,1,0,
	  0,1,1,1,0,
	  0,0,0,0,0,
	  0,0,0,0,0

    },
	//田型
	{
	  0,0,0,0,0,
	  0,1,1,0,0,
	  0,1,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0
    },
	{
	  0,0,0,0,0,
	  0,1,1,0,0,
	  0,1,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0
	},
	{
	  0,0,0,0,0,
	  0,1,1,0,0,
	  0,1,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0
	},
	{
	  0,0,0,0,0,
	  0,1,1,0,0,
	  0,1,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0
	},
	//T型
	{
	  0,0,0,0,0,
	  0,1,1,1,0,
	  0,0,1,0,0,
	  0,0,0,0,0,
	  0,0,0,0,0
     }, 
	{
	  0,0,0,0,0,
	  0,0,0,1,0,
	  0,0,1,1,0,
	  0,0,0,1,0,
	  0,0,0,0,0
	 },
	{
	 0,0,0,0,0,
	 0,0,1,0,0,
	 0,1,1,1,0,
	 0,0,0,0,0,
	 0,0,0,0,0 
	},

	{ 
    0,0,0,0,0,
	0,1,0,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 
	},


	//z型
   { 
	0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 },

	{ 0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0 },

	{
	0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0 
	}
};//定义数组存放方块



//欢迎界面
void welcome() {
	//初始化画布
	initgraph(550,660);

	//设置窗口标题，调用widows接口
	HWND windows = GetHWnd();//得到当前窗口
	SetWindowText(windows, _T("俄罗斯方块 张政行")); //设置窗口

	//设置文本样式高度40，宽度自适应
	setfont(40, 0, _T("宋体"));
	setcolor(WHITE);
	outtextxy(205, 200, _T("俄罗斯方块"));

	setfont(22, 0, _T("楷体"));
	setcolor(WHITE);
	outtextxy(175, 300, _T("时代少年团我们喜欢你"));

	Sleep(3000); //页面停留3000ms
}

//初始化游戏场景
void initgamescreen() {
	wchar_t str[16];
	cleardevice();  //清屏

rectangle(27,27,336,635);//设置游戏主题边框
rectangle(29, 29, 334, 633);

rectangle(370, 50, 515, 195);//设置下一个方块的展示框

setfont(24, 0, _T("楷体"));
setcolor(LIGHTGRAY);
outtextxy(405, 215, _T("下一个"));

setcolor(RED);
outtextxy(405, 280, _T("分数"));
swprintf(str, 16, L"%d", score);
outtextxy(415, 310,str );  //_T只能接受常量

outtextxy(405, 375, _T("等级"));
swprintf(str, 16, L"%d", rank);
outtextxy(425, 405, str);

setcolor(LIGHTBLUE);                 //操作说明模块
outtextxy(390, 475, _T("操作说明"));
outtextxy(390, 500, _T("↑:旋转"));
outtextxy(390, 525, _T("↓:下降"));
outtextxy(390, 550, _T("←:左移"));
outtextxy(390, 575, _T("←:向移"));
outtextxy(390, 600, _T("空格:暂停"));

}

//使用特殊字符来清下一个区域的屏
void clearblock() {
	setcolor(BLACK);
	setfont(23, 0, _T("楷体"));
	for (int i = 0; i < BLOCK_HIGHT; i++) {
		for (int j = 0; j < BLOCK_WIDTH; j++) {
			
			int x = 391 + j * UNIT_SIZE;
			int y = 71 + i * UNIT_SIZE;
			outtextxy(x, y, _T("■"));
		}
	}
}

//清除指定位置的屏 x表示绝对距离，y是偏移量
void clearblock(int x, int y, block_dir_t dir) {
	setcolor(BLACK);
	int id = BlockIndex * 4 + dir;
	y += START_Y;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[id][i][j] == 1) {
				outtextxy(x + j * 20, y + i * 20, _T("■"));
			}
		}
	}


}

//绘制下一个方块
void drawBlock(int x,int y) {
	setcolor(color[Nestindex]);
	setfont(23, 0, _T("楷体"));
	for (int i = 0; i < BLOCK_HIGHT; i++) {
		for (int j = 0; j < BLOCK_WIDTH; j++) {
			if(block[4* Nestindex][i][j]){
				int x2 = x + j * UNIT_SIZE;
				int y2 = y + i * UNIT_SIZE;
				outtextxy(x2, y2, _T("■"));
			}
		}
	}


};


//绘制当前方块
void drawBlock(int x,int y,int blockindex, block_dir_t dir){
	setcolor(color[blockindex]);
	setfont(23, 0, _T("楷体"));
	int id = blockindex * 4 + dir;
	for (int i = 0; i < BLOCK_HIGHT; i++) {
		for (int j = 0; j < BLOCK_WIDTH; j++) {
			if (block[id][i][j]) {
				int x2 = x + j * UNIT_SIZE;
				int y2 = y + i * UNIT_SIZE;
				outtextxy(x2, y2, _T("■"));
			}
		}
	}
}

void nextblock() {
	clearblock();

	srand(time(NULL)); //使用时间随机产生随机种子
	 Nestindex = rand() % 5;
	 drawBlock(391,71);
}

//如果在指定位置可以向指定方向移动 可以 1 否则 0
int moveable(int x0, int y0, move_dir_t moveDir, block_dir_t blockDir) {
	int id = BlockIndex * 4 + blockDir;
	int ret = 1;
	//计算当前方块在30*15在第几行第几列
	int x = (y0 - minY) / UNIT_SIZE;
	int y = (x0 - minX) / UNIT_SIZE;

	if (moveDir == MOVE_DOWN) {
		for (int i = 0; i < BLOCK_WIDTH; i++) {
			for (int j = 0; j < BLOCK_HIGHT; j++) {
				if (block[id][i][j] == 1 && (x + i + 1 >= 30 || visit[x + i + 1][y + j] == 1)) {
					ret = 0;
				} //判段向下是否到达边界或下方有方块。
			}
		}
	}
	else if (moveDir == MOVE_LEFT) {
		for (int i = 0; i < BLOCK_WIDTH; i++) {
			for (int j = 0; j < BLOCK_HIGHT; j++) {
				if (block[id][i][j] == 1 && (y+j==0 || visit[x + i][y + j-1] == 1)) {
					ret = 0;
				} //判段向左是否到达边界或左方有方块。
			}
		}
	
	}
	else if (moveDir == MOVE_RIGHT) {
		for (int i = 0; i < BLOCK_WIDTH; i++) {
			for (int j = 0; j < BLOCK_HIGHT; j++) {
				if (block[id][i][j] == 1 && (y + j +1>=15 || visit[x + i][y + j +1] == 1)) {
					ret = 0;
				} //判段向右是否到达边界或右方有方块。
			}
		}
	
	}

	return ret;
}

void failcheck(){
	if(!moveable(START_X,START_Y,MOVE_DOWN,BLOCK_UP)){
		setcolor(WHITE);
		setfont(45, 0, _T("隶书"));
		outtextxy(75, 300, _T("GAME OVER!"));
		Sleep(1000);
		system("pause");
		closegraph();
		exit(0);
	
	}




}


void wait(int interval){
	int cout = interval / 10;
	for (int i = 0; i < cout; i++) {
		Sleep(10);
		if (_kbhit()) {
			return;
		}
	}

}

//判断当前方块是否能转到目的方块,方块还没有变化
int rotatable(int x,int y, block_dir_t dir){
	int id = BlockIndex * 4 + dir;
	int xindex = (y - minY)/20;
	int yindex = (x - minX) / 20;
	if (!moveable(x, y, MOVE_DOWN, dir)) {
		return 0;
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[id][i][j] == 1 && (yindex + i < 0 || yindex + j >= 15 || visit[xindex + i][yindex + j] == 1)) {
				return 0;
			}

		}
	}

	return 1;
}     //判断是否可以旋转


//实现方块固化
void mark(int x, int y, int blockIndex, block_dir_t dir) {
	int id = blockIndex * 4 + dir;
	int x2 = (y - minY)/20;
	int y2 = (x - minX) / 20;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (block[id][i][j] == 1) {
				visit[x2 + i][y2 + j] = 1;
				 markColor[x2+i][y2+j]=color[BlockIndex];
			}
		}
	}


}


void move() {
	int x = START_X;
	int y = START_Y;
	int k = 0;
	block_dir_t blockDir = BLOCK_UP;//方块朝向
	int curSpeed = speed; //

	failcheck(); //检测游戏是否结束

	while (1) { //持续向下降落
		if (_kbhit()) {
			int key = _getch();//取得按键的ascil码值
			if (key == KEY_space) {
				_getch();
			}
		}
		clearblock(x, k, blockDir);
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_UP) {
				block_dir_t nextDir = (block_dir_t)((blockDir + 1) % 4);     //实现旋转功能
				if (rotatable(x, y + k, nextDir)) {
					blockDir = nextDir;
				}
			}
			else if (key == KEY_DOWN) {
				curSpeed = (10);
			}
			else if (key == KEY_RIGHT) {
				if (moveable(x, y + k + 20, MOVE_RIGHT, blockDir)){ x += 20; }
				
			}
			else if (key == KEY_LEFT) {
				if (moveable(x, y + k + 20, MOVE_LEFT, blockDir)) { x -= 20; }
				
			}
		}
		k += 20;
		//绘制当前方块
		drawBlock(x, y + k, BlockIndex, blockDir);


		wait(curSpeed);


		//方块的固化处理
		if (!moveable(x, y + k, MOVE_DOWN, blockDir)) {
			mark(x, y + k, BlockIndex, blockDir);
			break;
		}
	}
}



//产生新方块下降
void newblock() {

	BlockIndex = Nestindex;



	//在游戏区顶部产生方块
 drawBlock(START_X, START_Y);

	//让新方块的出现停住
	Sleep(100);


	//右上角更新
	nextblock();


	//方块降落
	move();
}

void down(int x) {
	for (int i = x; i>0; i--) {
		for (int j = 0; j < 15; j++) {
			if (visit[i - 1][j]) {
				visit[i][j] = 1;
				markColor[i][j] = markColor[i - 1][j];
				setcolor(markColor[i][j]);
				outtextxy(minX + j * 20,minY + i * 20, _T("■"));
			}
			else {
				visit[i][j] = 0;
				setcolor(BLACK);
				outtextxy(minX + j * 20, minY + i * 20, _T("■"));
			}
		}
	}
	//清除第0行;
	setcolor(BLACK);
	for (int j = 0; j < 15; j++) {
		visit[0][j] = 0;
		outtextxy(minX + j * 20, minY, _T("■"));
	}
}

//更新分数
void addScore(int lines) {
	wchar_t str[32];

	setcolor(RED);
	score += lines * 10;
	swprintf(str, 32, L"%d", score);
	outtextxy(415, 310, str);            
};

//更新等级
void updateGrade() {
	rank = score / 10;
	wchar_t str[16];
	swprintf(str, 16, L"%d", rank);
	outtextxy(425, 405, str);

	speed = 500 - rank * 100;
	if (speed <= 0) {
		speed = 100;
	}
}





void cheak() {
	int i = 29;
	int clearLines = 0;
	for (i; i >= 0; i--) {
		int j = 0;
		for (j; j < 15 && visit[i][j]; j++);    //执行到此有两种情况1：i不满；2：j=15,i满了

		if (j >= 15) {
			//此时第i行满了
			down(i);  //把第i行清除
			i++; //避免连续两行满时消不了的情况
			clearLines++;
		}
		addScore(clearLines);  //更新分数

		updateGrade(); //更新等级
	}
}

int main() {
	welcome();
	initgamescreen();
	
	//产生新方块
	nextblock();
	Sleep(500);

	//初始化访问数组
	memset(visit, 0, sizeof(visit));

	while (1) {
		newblock();

		//消除满行 
		cheak();
	} 

	system("pause");
	closegraph();
	return 0;
}
