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

// ȫ�ֱ���
static double winwidth=15, winheight=10;// ���ڳߴ�
static int switch_button=0;//��Ӧmode �л�
static int isEditing=0;// �ж���û���ڱ༭�ı� 
double mouse_x=0;
double mouse_y=0;
char*hint="Text here";
double fH = 0.2;
double h = 0.4;  // �ؼ��߶�
double x = 5;  
double y = 2; 
double w = 2.2; // �ؼ����
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
// ����������provided in libgraphics
void DisplayClear(void); 
// ��ʱ������������provided in libgraphics
void startTimer(int id,int timeinterval);
// �û�����ʾ����
void display(void); 
// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	display(); //ˢ����ʾ
}

// �û��ļ����¼���Ӧ����
void KeyboardEventProcess(int key, int event)
{
	double move_x=GetCurrentX();
	double move_y=GetCurrentY();
	switch (event)
	{
    	case KEY_UP:
	    	if (key == VK_TAB)//��������� 
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
	        if (key == VK_RETURN)//���ͬ������ 
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
	display(); // ˢ����ʾ
}

// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event); // needed for using simpleGUI
	display(); // ˢ����ʾ
}

// �û����������
// ����ʼ��ִ��һ��
void Main() 
{
	// ��ʼ�����ں�ͼ��ϵͳ
	SetWindowTitle("MindSet-Welcome");
	//SetWindowSize(10, 10); // ��λ - Ӣ��
	//SetWindowSize(15, 10);
	//SetWindowSize(10, 20);  // �����Ļ�ߴ粻�����򰴱�����С
    InitGraphics();

	// ��ô��ڳߴ�
    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();

	// ע��ʱ����Ӧ����
	registerCharEvent(CharEventProcess);        // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);// ���� 
	registerMouseEvent(MouseEventProcess);      // ���

	// ������ʱ��
	startTimer(MY_ROTATE_TIMER, 50);            

	// �򿪿���̨��������printf/scanf���/�������Ϣ���������
	// InitConsole(); 
}

#if defined(DEMO_MENU)
// �˵���ʾ����
void drawMenu()
{ 
	static char * menuListFile[] = {"    |�ļ�|            ",  
		"���棨�����ƣ�    |Ctrl-B", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"���棨�ı��ļ���|Ctrl-T",
		"�˳�                        |Ctrl-E"};
	static char * menuListExport[] = {"   |����|     ",
		"ͼƬ",
		"���",
		};
	static char * menuListHelp[] = {"    |����|  ",
		"ʹ���ֲ�",
		"����MindSet"};
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();//fH����߶� 
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH*1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListHelp[0])*1.4; // �ؼ����
	double wlist = TextStringWidth(menuListExport[3])*1.7;
	double xindent = winheight/20; // ����
	int    selection;
	
	// menu bar
	drawMenuBar(0,y-h,winwidth,h);
	// ���ļ��� �˵�
	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if( selection>0 ) selectedLabel = menuListFile[selection];
	if( selection==1 )
	if( selection==2 )
	if( selection==3 )
		exit(-1); // choose to exit
	
	// �������� �˵�
	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListExport,sizeof(menuListExport)/sizeof(menuListExport[0]));
	if( selection>0 ) selectedLabel = menuListExport[selection];
	
	// �������� �˵�
	selection = menuList(GenUIID(0),x+2*w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) selectedLabel = menuListHelp[selection];
	if( selection==1 ); 
 
} 
#endif // #if defined(DEMO_MENU)

#if defined(MODE_CHOICE)//�л�ѡ��ģʽ���ܺ��� 
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

void DrawMode1()//��һ��һ������������ 
{
	double tbx=winwidth/1.2;
	double tby=winheight/3.5;
	double tbw=w*1.8;
	double tbh=h; 
	char*hint="Click Here To Add Text";//������ʾ 
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
	// ����
	DisplayClear();
	ModeChoice();
#if defined(DEMO_MENU)
	// ���ƺʹ���˵�
	drawMenu();
#endif
}













