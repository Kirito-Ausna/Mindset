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

void DrawChildren1(PtrTreeNode Parent,int ChildNum,PtrTreeNode Children[])
{
    double x=winwidth/3.5;
	double y=winheight/1.8;
	double w=winwidth/10;
	double fH=GetFontHeight();
	double h=fH*2;
	static char memoroot[80]="Click To Start"; //������ʾ 
	//draw the root 
	if(Parent==NULL)
	{
        if(button(GenUIID(0), x, y, w*1.8, h, memoroot))//draw the button first
        {
    	     isEditing=1;
        }
        if(isEditing)
        {
        	static char mroot[80]="Text Here";
    	    textbox(GenUIID(0),winwidth/1.5,winheight/15,w*3,h,mroot,sizeof(mroot));//ready for content adding
//    	    root = CreateTree(0,root->NodeObject);//give it to the backend
    	    EditContent(root,mroot);//while using the textbox,the value of memoroot is supposed to be changed, so you are supposed to change it in the backend
//		    root->NodeNumber = 0;//initinalization. root is a global variable defined in draw.h
//		    root->NodeObject.height=h;
//		    root->NodeObject.width=w*1.8;
//		    root->NodeObject.dx=x;
//		    root->NodeObject.dy=y;
//		    root->NodeObject.color=0;
//		    root->FirstChild=NULL;
//		    root->NextSibling=NULL;
		    button(GenUIID(0), x, y, w*1.8, h, mroot);//Actually,there are two buttons here!Thankfully,they are the same.    
		}
	}
	else if(ChildNum==0)
	{
		;
	}
	else//draw the children
	{
	    double fatherx=Parent->NodeObject.dx+w+0.1;
	    double fatherydw=Parent->NodeObject.dy+fH/2+ChildNum/2*h*1.3;
	    MovePen(Parent->NodeObject.dx+w,Parent->NodeObject.dy+fH);
	    int i;
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
			    EditCoordinate(Children[i],move_x+0.1,move_y-0.5*h);//when inserting or deleting, every child's coordinate is supposed to be changed 
				Children[i]->NodeObject.height=h;
				Children[i]->NodeObject.width=TextStringWidth(Children[i]->Content)+0.2;
				Children[i]->NodeObject.dx=move_x+0.1;
				Children[i]->NodeObject.dy=move_y-0.5*h;
				Children[i]->NodeObject.color=0;
				MovePen(fatherx,move_y);
			    DrawLine(0,-h*1.3);
			    move_x=GetCurrentX();
			    move_y=GetCurrentY();
		    }
		    DrawLine(0.1,0);
		    if(button(GenUIID(0),move_x+0.1,move_y-0.5*h,TextStringWidth(Children[i]->Content)+0.2,h,Children[i]->Content));
		    EditCoordinate(Children[i],move_x+0.1,move_y-0.5*h);
		    Children[i]->NodeObject.height=h;
			Children[i]->NodeObject.width=TextStringWidth(Children[i]->Content)+0.2;
			Children[i]->NodeObject.dx=move_x+0.1;
			Children[i]->NodeObject.dy=move_y-0.5*h;
			Children[i]->NodeObject.color=0;
	}
}

void DrawChildren2(PtrTreeNode Parent,int ChildNum,PtrTreeNode Children[])
{
}
void DrawChildren3(PtrTreeNode Parent,int ChildNum,PtrTreeNode Children[])
{
}


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
   	switch(switch_button)
	   {
	   	case 1:LevelOrderTravelsal(root,DrawChildren1);break;
	   	//case 2:LevelOrderTravelsal(root,DrawChildren2);
	   } 
}


// ����������provided in libgraphics
void DisplayClear(void); 
// ��ʱ������������provided in libgraphics
//void startTimer(int id,int timeinterval);
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
//    display();
	uiGetKeyboard(key,event);
	PtrTreeNode yf=NULL; 
	static char memochild[30]="Text Here";
	PtrTreeNode TargetNode=NULL;
	switch (event)
	{
    	case KEY_UP:	
		if (key == VK_F1)//���������(why not TAB? -has been used) 
	    	    {
//	    	    	drawLabel(winwidth/1.5,winheight/22,"Hello World!");
	    	    	double move_x=GetCurrentX();
	                double move_y=GetCurrentY();
	    	        TargetNode=LocateNode(move_x,move_y,root);//temporary varible
	    	        
	    	        if(TargetNode!=NULL)
					{
						ChildrenNum=FindChildren(TargetNode,Children);
	    	            yf = InsertTreeNode(TargetNode,0,idnum,TargetNode->NodeObject);
	    	            LevelOrderTravelsal(root,DrawChildren1);
						/*switch(switch_button) 
		    	        {
		    	            case 1:LevelOrderTravelsal(root, DrawChildren1);break;
		    	            //case 2:LevelOrderTravelsal(root, DrawChildren2);break;
		    	            //case 3:LevelOrderTravelsal(root, DrawChildren3);break;
		    	        }
		    	        idnum++;*/
		    	        drawLabel(winwidth/1.5,winheight/22,"Hello World!");
		    	    }else{
//		    	    	drawLabel(winwidth/1.5,winheight/22,"Here You are");
		    	    	char toolman[10];
						sprintf(toolman,"%lf %lf",move_x,move_y);
						drawLabel(winwidth/1.5,winheight/22,toolman);
						
		    	    }
		        }
	        if (key == VK_F2)//���ͬ������ 
			    {
			    	double move_x=GetCurrentX();
	                double move_y=GetCurrentY();
			        TargetNode=LocateNode(move_x,move_y,root);
			        if(TargetNode!=NULL)
					{
						DisplayClear();
	                    //����
					    ChildrenNum=FindChildren(TargetNode,Children);
			            yf = InsertTreeNode(TargetNode,1,idnum,TargetNode->NodeObject);
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
			    	double move_x=GetCurrentX();
	                double move_y=GetCurrentY();
			    	TargetNode=LocateNode(move_x,move_y,root);
			    	if(TargetNode!=NULL)
			    	{
			    		DisplayClear();
	                    //����
			    		DeleteTree(TargetNode); 
			    		switch(switch_button) 
		    	        {
		    	            case 1:LevelOrderTravelsal(root, DrawChildren1);break;
		    	            //case 2:LevelOrderTravelsal(root, DrawChildren2);break;
		    	            //case 3:LevelOrderTravelsal(root, DrawChildren3);break;
		    	        }
			    	}
			    }
			if (key == VK_F4)//edit part (for test)
			{
				double move_x=GetCurrentX();
	            double move_y=GetCurrentY();
//	            drawLabel(winwidth/1.5,winheight/22,"Here You are");
				TargetNode=LocateNode(move_x,move_y,root);//temporary varible
				if(TargetNode == NULL){
					char toolman[10];
					sprintf(toolman,"%lf %lf",move_x,move_y);
					drawLabel(winwidth/1.5,winheight/22,toolman);
				}else{
				EditContent(TargetNode,memochild);
				drawLabel(winwidth/1.5,winheight/22,"Success!Go On~");
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
		TargetNode->NodeObject.width=TextStringWidth(memochild);
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
//	InitConsole();
	double x=winwidth/3.5;
	double y=winheight/1.8;
	double w=winwidth/10;
	double fH=GetFontHeight();
	double h=fH*2;
	static char memoroot[80]="Click To Start"; //������ʾ
	root = CreateTree(0,Father);//give it to the backend
	static char mroot[80]="Text Here";
    EditContent(root,mroot);//while using the textbox,the value of memoroot is supposed to be changed, so you are supposed to change it in the backend
    root->NodeNumber = 0;//initinalization. root is a global variable defined in draw.h
    root->NodeObject.height=h;
    root->NodeObject.width=w*1.8;
    root->NodeObject.dx=x;
    root->NodeObject.dy=y;
    root->NodeObject.color=0;
    root->FirstChild=NULL;
    root->NextSibling=NULL;
//    printf("%d",switch_button);
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













