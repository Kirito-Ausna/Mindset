#include <windows.h>
#include <winuser.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"

#define DEMO_MENU
#define DEMO_BUTTON
#define DEMO_MINDMAP
#define MODE_CHOICE

// 全局变量
static double winwidth, winheight;// 窗口尺寸
static int switch_button=0;//响应mode 切换
static int show_textbox=0;
static int text_edit = 0; //判断是否已经编辑了文本 

void Drawmyson(void)//画一个子主题 
{
	DrawLine(1,1);
}
void Drawmybrother(void)
{
	DrawLine(1,2);
} 




// 清屏函数，provided in libgraphics
void DisplayClear(void); 
// 计时器启动函数，provided in libgraphics
void startTimer(int id,int timeinterval);

// 用户的显示函数
void display(void); 

// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	display(); //刷新显示
}

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
	switch (event)
	{
	case KEY_UP:
		if (key == VK_TAB)//添加子主题 
			Drawmyson();
		display();
	    if (key == VK_RETURN)//添加同级主题 
			Drawmybrother();
			display();
			break;
	default:
		break;
	}
	display(); // 刷新显示
}

// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event); //GUI获取鼠标
	display(); // 刷新显示
}

// 旋转计时器
#define MY_ROTATE_TIMER  1


// 用户主程序入口
// 仅初始化执行一次
void Main() 
{
	// 初始化窗口和图形系统
	SetWindowTitle("MindSet-welcome");
	//SetWindowSize(10, 10); // 单位 - 英寸
	//SetWindowSize(15, 10);
	//SetWindowSize(10, 20);  // 如果屏幕尺寸不够，则按比例缩小
    InitGraphics();

	// 获得窗口尺寸
    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();

	// 注册时间响应函数
	registerCharEvent(CharEventProcess);        // 字符
	registerKeyboardEvent(KeyboardEventProcess);// 键盘1 
	registerMouseEvent(MouseEventProcess);      // 鼠标

	// 开启定时器
	startTimer(MY_ROTATE_TIMER, 50);            

	// 打开控制台，方便用printf/scanf输出/入变量信息，方便调试
	// InitConsole(); 
}

#if defined(DEMO_MENU)
// 菜单演示程序
void drawMenu()
{ 
	static char * menuListFile[] = {"    |文件|            ",  
		"保存（二进制）    |Ctrl-B", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"保存（文本文件）|Ctrl-T",
		"退出                        |Ctrl-E"};
	static char * menuListExport[] = {"   |导出|     ",
		"图片",
		"大纲",
		};
	static char * menuListHelp[] = {"    |帮助|  ",
		"使用手册",
		"关于MindSet"};
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH*1.5; // 控件高度
	double w = TextStringWidth(menuListHelp[0])*1.4; // 控件宽度
	double wlist = TextStringWidth(menuListExport[3])*1.7;
	double xindent = winheight/20; // 缩进
	int    selection;
	
	// menu bar
	drawMenuBar(0,y-h,winwidth,h);
	// “文件” 菜单
	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if( selection>0 ) selectedLabel = menuListFile[selection];
	if( selection==1 )
	if( selection==2 )
	if( selection==3 )
		exit(-1); // choose to exit
	
	// “导出” 菜单
	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListExport,sizeof(menuListExport)/sizeof(menuListExport[0]));
	if( selection>0 ) selectedLabel = menuListExport[selection];
	
	// “帮助” 菜单
	selection = menuList(GenUIID(0),x+2*w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) selectedLabel = menuListHelp[selection];
	if( selection==1 ); 
 
} 
#endif // #if defined(DEMO_MENU)

#if defined(MODE_CHOICE)//切换选择模式的总函数 
void ModeChoice()
{
	double fH = GetFontHeight();
	double h = fH*2;  // 控件高度
	double x = winwidth/30;  
	double y = winheight/2-h; 
	double w = winwidth/10; // 控件宽度
	char*hint="Click Here To Add Text";//输入提示 
	
    if (button(GenUIID(0), x, y, w, h, "mode 1"))
   	    switch_button=1;
    if( button(GenUIID(0), x, y-1.5*h, w, h, "mode 2") )
   	    switch_button=2;
    if( button(GenUIID(0), x, y-3*h, w, h, "mode 3") )
   	    switch_button=3;
}
#endif
void DrawMode1()//画一个一级（根）主题 
{
    double fH = GetFontHeight();
	double h = fH*2;  // 控件高度
	double x = winwidth/30;  
	double y = winheight/2-h; 
	double w = winwidth/10; // 控件宽度
	char*hint="Click Here To Add Text";//输入提示 
	
    if(button(GenUIID(0), winwidth/3.5, winheight/1.8, w*1.8, h, hint))
    {
    	show_textbox=1;
    }
    if(show_textbox)
    {
        static char memoroot[80]="Text here";
        if(textbox(GenUIID(0), winwidth/1.5, winheight/15, w*3, h, memoroot, sizeof(memoroot)))
        button(GenUIID(0),winwidth/3.5, winheight/1.8, w*1.8, h, memoroot);
    }
}
void DrawMode2()
{}
void DrawMode3()
{}
#if defined(DEMO_BUTTON)
// 按钮切换模式 Mode 
void DrawModeorigin()
{
	double fH = GetFontHeight();
	double h = fH*2;  // 控件高度
	double x = winwidth/30;  
	double y = winheight/2-h; 
	double w = winwidth/10; // 控件宽度
	char*hint="Click Here To Add Text";//输入提示 
    
    ModeChoice();//选择主题
	switch(switch_button){
		case 1:DrawMode1();break;
		case 2:DrawMode2();break;
		case 3:DrawMode3();break;
	} 

}
#endif // #if defined(DEMO_BUTTON)









#if defined(DEMO_EDITBOX)
// 文本编辑演示程序
void drawEditText()
{
	static char firstName[80] = "Xinguo";
	static char lastName[80] = "Liu";
	static char results[200] = "";
	static char memo[80]="Hello World! Edit me!";
	static char numstr[80] = "3.14159265";
	static char newnum[80] = "";
	double fH = GetFontHeight();
	double h = fH*2; // 控件高度
	double w = winwidth/4; // 控件宽度
	double x = winwidth/2.5;
	double y = winheight/2-h;

	textbox(GenUIID(0), winwidth / 10, winheight-h-h, w, h, memo, sizeof(memo));
	textbox(GenUIID(0), x, y, w, h, memo, sizeof(memo));

	SetPenColor("Brown"); 
	drawLabel(x-fH/2-TextStringWidth("名"), (y-=h*1.2)+fH*0.7, "名");
	if( textbox(GenUIID(0), x, y, w, h, firstName, sizeof(firstName)) )
		sprintf(results,"Text edit result is: %s+%s", firstName,lastName);

	SetPenColor("Brown"); 
	drawLabel(x-fH/2-TextStringWidth("姓"), (y-=h*1.2)+fH*0.7, "姓");
	if( textbox(GenUIID(0), x, y, w, h, lastName, sizeof(lastName)) )
		sprintf(results,"Text edit result is: %s+%s", firstName,lastName);

	SetPenColor("Gray"); 
	drawLabel(x, y-=fH*1.2, results);

	//y = winheight/2;
	if( textbox(GenUIID(0),x,y-=h*1.8,w,h,numstr,sizeof(numstr)) )
	{
		double value; sscanf(numstr,"%lf",&value);
		sprintf(newnum,"Represented value is: %.20f", value);
	}
	SetPenColor("Gray"); 
	drawLabel(x,y-=fH*1.2, newnum);
}
#endif // #if defined(DEMO_EDITBOX)

void display()
{
	// 清屏
	DisplayClear();
			
#if defined(DEMO_BUTTON)
	// 按钮
	DrawModeorigin();
#endif

#if defined(DEMO_EDITBOX)
	// 编辑文本字符串
	drawEditText();
#endif

#if defined(DEMO_MENU)
	// 绘制和处理菜单
	drawMenu();
#endif
}













