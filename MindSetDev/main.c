#include <windows.h>
#include <winuser.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"
#include "Map2Tree.h"
#include <stdio.h>

#define DEMO_MENU
#define DEMO_BUTTON
#define DEMO_MINDMAP
#define MODE_CHOICE

// 全局变量
static double winwidth=15, winheight=10;// 窗口尺寸
static int switch_button=0;//响应mode 切换
static int isEditing=0;// 判断有没有在编辑文本 
double mouse_x=0;
double mouse_y=0;
char*hint="Text here";
double fH = 0.2;
double h = 0.4;  // 控件高度
double x = 5;  
double y = 2; 
double w = 2.2; // 控件宽度
struct NodeClass Father={0.4,2.2,5,2,0};
PtrTreeNode root;
PtrTreeNode Children[10];
int ChildrenNum;
int idnum=0;





DrawChildren(PtrTreeNode Parent,int ChildNum,PtrTreeNode Children[])
{
	double fatherx=Parent->NodeObject.dx+w+0.1;
	double fatherydw=Parent->NodeObject.dy+fH/2+ChildrenNum/2*h*1.3;
	MovePen(Parent->NodeObject.dx+w,Parent->NodeObject.dy+fH);
	int i=1;
	SetPenColor("Dark Gray");
	if(ChildNum%2!=0)
	{
		DrawLine(0.2,0);
		MovePen(fatherx,fatherydw);
	}
	else
	{
		DrawLine(0.1,0);
		MovePen(fatherx,fatherydw-fH);
	}
		double move_x=GetCurrentX();
		double move_y=GetCurrentY();
		for(i=0;i<ChildrenNum-1;i++)
		{
			DrawLine(0.1,0);
			if(button(GenUIID(0),move_x+0.1,move_y-0.5*h,TextStringWidth(Children[i].content)+0.2,h,Children[i].content));
			MovePen(fatherx,move_y);
			DrawLine(0,-h*1.3);
			move_x=GetCurrentX;
			move_y=GetCurrentY;
		}
		DrawLine(0.1,0);
		if(button(GenUIID(0),move_x+0.1,move_y-0.5*h,TextStringWidth(Children[i].content)+0.2,h,Children[i].content));
	}
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
	double move_x=GetCurrentX();
	double move_y=GetCurrentY();
	switch (event)
	{
    	case KEY_UP:
	    	if (key == VK_TAB)//添加子主题 
	    	    {
	    	        PtrTreeNode TargetNode=LocateNode(move_x,move_y,root);
	    	        if(TargetNode!=NULL)
					{
					    ChildrenNum=FindChildren(TargetNode,Children);
	    	            InsertTreeNode(TargetNode,0,ChildrenNum,TargetNode->NodeObject);
		    	        LevelOrderTravelsal(root, DrawChildren);
		    	        idnum++;
		    	    }
		            display();
		        }
	        if (key == VK_RETURN)//添加同级主题 
			    {
			        PtrTreeNode TargetNode=LocateNode(move_x,move_y,root);
			        if(TargetNode!=NULL)
					{
					    ChildrenNum=FindChildren(TargetNode,Children);
			            InsertTreeNode(TargetNode,1,idnum,TargetNode->NodeObject);
			            LevelOrderTravelsal(root, DrawChildren);
			            idnum++;
			        }
			        display();
			    }
			if (key == VK_RETURN)
			    {
			    	PtrTreeNode TargetNode=LocateNode(move_x,move_y,root);
			    	if(TargetNode!=NULL)
			    	{
			    		DeleteTree(TargetNode);
			    	}
			    }
			break;
	    default:
		break;
	}
	display(); // 刷新显示
}

// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event); // needed for using simpleGUI
	display(); // 刷新显示
}

// 用户主程序入口
// 仅初始化执行一次
void Main() 
{
	// 初始化窗口和图形系统
	SetWindowTitle("MindSet-Welcome");
	//SetWindowSize(10, 10); // 单位 - 英寸
	//SetWindowSize(15, 10);
	//SetWindowSize(10, 20);  // 如果屏幕尺寸不够，则按比例缩小
    InitGraphics();

	// 获得窗口尺寸
    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();

	// 注册时间响应函数
	registerCharEvent(CharEventProcess);        // 字符
	registerKeyboardEvent(KeyboardEventProcess);// 键盘 
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

	double fH = GetFontHeight();//fH字体高度 
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
    if (button(GenUIID(0), x, y, w, h, "mode 1"))
   	    switch_button=1;
    if( button(GenUIID(0), x, y-1.5*h, w, h, "mode 2") )
   	    switch_button=2;
    if( button(GenUIID(0), x, y-3*h, w, h, "mode 3") )
   	    switch_button=3;
   	    
   	switch(switch_button){
		case 1:DrawMode1();break;
		case 2:DrawMode2();break;
		case 3:DrawMode3();break;
	} 
}
#endif

void DrawMode1()//画一个一级（根）主题 
{
	double tbx=winwidth/1.2;
	double tby=winheight/3.5;
	double tbw=w*1.8;
	double tbh=h; 
	char*hint="Click Here To Add Text";//输入提示 
    if(button(GenUIID(0), x, y, w, h, hint)&&textbox(GenUIID(0),tbx,tby,tbw,tbh,"text here",hint))
    {
    	isEditing = 1;
    }
    if(isEditing)
    {
    	CreateTree(0,Father);
		EditContent(root,hint);   
    }
}
void DrawMode2()
{

}
void DrawMode3()
{}






void display()
{
	// 清屏
	DisplayClear();
	ModeChoice();
#if defined(DEMO_MENU)
	// 绘制和处理菜单
	drawMenu();
#endif
}













