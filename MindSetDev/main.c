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
	static char memoroot[80]="Text Here"; 
	static char*hint="Click Here To Add Text";//������ʾ 
    if(button(GenUIID(0), x, y, w*1.8, h, hint))
    {
    	isEditing=1;
    }
    if(isEditing==1)
    {
    	textbox(GenUIID(0),winwidth/1.5,winheight/15,w*3,h,memoroot,sizeof(memoroot));
		root->NodeNumber=0;
		root->NodeObject.height=h;
		root->NodeObject.width=w;
		root->NodeObject.dx=x;
		root->NodeObject.dy=y;
		root->NodeObject.color=0;
		button(GenUIID(0), x, y, w*1.8, h, memoroot);
    	CreateTree(0,root->NodeObject);
		EditContent(root,memoroot);
    }
}
void DrawMode2()
{}
void DrawMode3()
{}

void DrawChildren1(PtrTreeNode Parent,int ChildNum,PtrTreeNode Children[])
{
    double x=winwidth/3.5;
	double y=winheight/1.8;
	double w=winwidth/10;
	double fH=GetFontHeight();
	double h=fH*2;
	static char memoroot[80]="Text Here"; 
	static char*hint="Click Here To Add Text";//������ʾ 
	
	double fatherx=Parent->NodeObject.dx+w+0.1;
	double fatherydw=Parent->NodeObject.dy+fH/2+ChildNum/2*h*1.3;
	MovePen(Parent->NodeObject.dx+w,Parent->NodeObject.dy+fH);
	int i=1;
	SetPenColor("Dark Gray");
	if(Parent==NULL)
	{
        if(button(GenUIID(0), x, y, w*1.8, h, hint))
        {
    	     isEditing=1;
        }
        if(isEditing==1)
        {
    	    textbox(GenUIID(0),winwidth/1.5,winheight/15,w*3,h,memoroot,sizeof(memoroot));
		    root->NodeNumber=0;
		    root->NodeObject.height=h;
		    root->NodeObject.width=w;
		    root->NodeObject.dx=x;
		    root->NodeObject.dy=y;
		    root->NodeObject.color=0;
		    button(GenUIID(0), x, y, w*1.8, h, memoroot);
    	    CreateTree(0,root->NodeObject);
		    EditContent(root,memoroot);
        }
	}
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
			EditCoordinate(Children[i],move_x+0.1,move_y-0.5*h);
			MovePen(fatherx,move_y);
			DrawLine(0,-h*1.3);
			move_x=GetCurrentX();
			move_y=GetCurrentY();
		}
		DrawLine(0.1,0);
		if(button(GenUIID(0),move_x+0.1,move_y-0.5*h,TextStringWidth(Children[i]->Content)+0.2,h,Children[i]->Content));
		EditCoordinate(Children[i],move_x+0.1,move_y-0.5*h);
		display();
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
	uiGetKeyboard(key,event);
	double move_x=GetCurrentX();
	double move_y=GetCurrentY();
	PtrTreeNode yf; 
	switch (event)
	{
    	case KEY_UP:
	    	if (key == VK_F1)//��������� 
	    	    {
	    	        PtrTreeNode TargetNode=LocateNode(move_x,move_y,root);
	    	        if(TargetNode!=NULL)
					{
					    ChildrenNum=FindChildren(TargetNode,Children);
	    	            yf = InsertTreeNode(TargetNode,0,ChildrenNum,TargetNode->NodeObject);
	    	            static char memochild[30]="Text Here";
	    	            textbox(GenUIID(0),winwidth/1.5,winheight/15,winwidth/10*3,winwidth/15,memochild,sizeof(memochild));
	    	            EditContent(yf,memochild);
	    	            display();
	    	            switch(switch_button=1) 
		    	        {
		    	            case 1:LevelOrderTravelsal(root, DrawChildren1);break;
		    	            //case 2:LevelOrderTravelsal(root, DrawChildren2);break;
		    	            //case 3:LevelOrderTravelsal(root, DrawChildren3);break;
		    	        }
		    	        idnum++;
		    	    }
		        }
	        if (key == VK_F2)//���ͬ������ 
			    {
			        PtrTreeNode TargetNode=LocateNode(move_x,move_y,root);
			        if(TargetNode!=NULL)
					{
					    ChildrenNum=FindChildren(TargetNode,Children);
			            yf = InsertTreeNode(TargetNode,1,idnum,TargetNode->NodeObject);
			            static char memochild[30]="Text Here";
			            textbox(GenUIID(0),winwidth/1.5,winheight/15,winwidth/10*3,winwidth/15,memochild,sizeof(memochild)); 
			            EditContent(yf,memochild);
			            display();
	    	            switch(switch_button) 
		    	        {
		    	            case 1:LevelOrderTravelsal(root, DrawChildren1);break;
							//case 2:LevelOrderTravelsal(root, DrawChildren2);break;
		    	            //case 3:LevelOrderTravelsal(root, DrawChildren3);break;
		    	        }
			            idnum++;
			        }
			    }
			if (key == VK_F3)//ɾ��һ������ 
			    {
			    	PtrTreeNode TargetNode=LocateNode(move_x,move_y,root);
			    	if(TargetNode!=NULL)
			    	{
			    		DeleteTree(TargetNode);
			    		display();//ˢ����ʾ 
			    		switch(switch_button) 
		    	        {
		    	            case 1:LevelOrderTravelsal(root, DrawChildren1);break;
		    	            //case 2:LevelOrderTravelsal(root, DrawChildren2);break;
		    	            //case 3:LevelOrderTravelsal(root, DrawChildren3);break;
		    	        }
			    	}
			    }
			break;
	    default:
		break;
	}
	display(); 
}

// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event); // needed for using simpleGUI
	double move_x=GetCurrentX();
	double move_y=GetCurrentY();
	PtrTreeNode TargetNode=root;
	if(LocateNode(move_x,move_y,TargetNode)!=NULL&&event==DOUBLE_CLICK)
	{
		static char memochild[30]="Text Here";
		textbox(GenUIID(0),winwidth/1.5,winheight/15,winwidth/10*3,winwidth/15,memochild,sizeof(memochild));
		EditContent(TargetNode,memochild);
	}
	display(); //ˢ����ʾ 
}

// �û����������
// ����ʼ��ִ��һ��
void Main() 
{
	// ��ʼ�����ں�ͼ��ϵͳ
	SetWindowTitle("MindSet-Welcome");
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













