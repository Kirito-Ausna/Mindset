#include <windows.h>
#include <winuser.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"
#include "Map2Tree.h"
#include <stdio.h>
#include "draw.h"

#define DEMO_MENU
#define DEMO_BUTTON
#define DEMO_MINDMAP
#define MODE_CHOICE

struct NodeClass Father={0.4,2.2,5,2,0};
PtrTreeNode root;
PtrTreeNode Children[10];
int ChildrenNum;
int idnum=0;

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

//�л�ѡ��ģʽ���ܺ��� 
void ModeChoice()
{
	double fH=GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double x = winwidth/30;
	double y = winheight/2-h;
	double w = TextStringWidth("mode 1")*2; // �ؼ����
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

void DrawMode1()//��һ��һ������������ 
{
	double x=winwidth/3.5;
	double y=winheight/1.8;
	double w=winwidth/10;
	double fH=GetFontHeight();
	double h=fH*2; 
	char*hint="Click Here To Add Text";//������ʾ 
    if(button(GenUIID(0), x, y, w*1.8, h, hint))
    {
    	isEditing=1;
    }
    if(isEditing==1)
    {
    	char memoroot[80]="Text Here";
    	int isTexting=0;
    	textbox(GenUIID(0),winwidth/1.5,winheight/15,w*3,h,memoroot,sizeof(memoroot)); 
		    
			button(GenUIID(0), x, y, w*1.8, h, memoroot);
    	    CreateTree(0,root->NodeObject);
		    EditContent(root,memoroot);
		    
    }
}
void DrawMode2()
{}
void DrawMode3()
{}

void DrawChildren(PtrTreeNode Parent,int ChildNum,PtrTreeNode Children[])
{
	double x = 2.0; //��λ��Ӣ��
	double y = 3.0; //��λ��Ӣ��
	char* hint="Click Here to Edit";
	double w = TextStringWidth(hint)+0.2; //��λ��Ӣ��
	double h = GetFontHeight() * 2; //��λ��Ӣ��
	double fH=GetFontHeight();
	
	double fatherx=Parent->NodeObject.dx+w+0.1;
	double fatherydw=Parent->NodeObject.dy+fH/2+ChildNum/2*h*1.3;
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
		for(i=0;i<ChildNum-1;i++)
		{
			DrawLine(0.1,0);
			if(button(GenUIID(0),move_x+0.1,move_y-0.5*h,TextStringWidth(Children[i]->Content)+0.2,h,Children[i]->Content));
			MovePen(fatherx,move_y);
			DrawLine(0,-h*1.3);
			move_x=GetCurrentX();
			move_y=GetCurrentY();
		}
		DrawLine(0.1,0);
		if(button(GenUIID(0),move_x+0.1,move_y-0.5*h,TextStringWidth(Children[i]->Content)+0.2,h,Children[i]->Content));
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
			if (key == VK_RETURN)//ɾ��һ������ 
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

	// �򿪿���̨��������printf/scanf���/�������Ϣ���������
	// InitConsole(); 
}


void display()
{
	// ����
	DisplayClear();
	
	//����ģʽ 
	ModeChoice();
	
#if defined(DEMO_MENU)
	// ���ƺʹ���˵�
	drawMenu();
#endif

}













