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
// �û�����ʾ����
void display();
// �˵���ʾ����
void drawMenu()
{ 
	static char * menuListFile[] = {"    |�ļ�|            ",  
		"���棨�����ƣ�    |Ctrl-B", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"���棨�ı��ļ���|Ctrl-T",
		"�˳�                        |Ctrl-E"};
	static char * menuListExport[] = {"   |����|     ",
		"�ļ��������ƣ�",
		"�ļ� ���ı���", 
		};
	static char * menuListHelp[] = {"    |����|  ",
		"ʹ���ֲ�",
		"����MindSet",
		"���뱳��ͼ"};
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
	if( selection==1 )Tree2BinaryFile(root,"MapFile.dat");
	if( selection==2 ){Tree2TxtFile(root,"MapFile.txt");}
	if( selection==3 )
		exit(-1); // choose to exit
	
	// �����롱 �˵�
	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListExport,sizeof(menuListExport)/sizeof(menuListExport[0]));
	if( selection>0 ) selectedLabel = menuListExport[selection];
	if( selection==1 )
	{
	    root=BinaryFile2Tree("MapFile.dat");
		display();
	}
	if( selection==2 )
	{	
		root=TxtFile2Tree("MapFile.txt");
		display();
	}
	
	
	
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
        if(button(GenUIID(0), x, y, w*1.4, h, memoroot))//draw the button first
        {
    	     isEditing=1;
        }
        if(isEditing)
        {
		    button(GenUIID(0), x, y, w*1.4, h, root->Content);    
		}
	}
	else if(ChildNum==0)
	{
		;
	}
	else//draw the children
	{
	    double fatherx=Parent->NodeObject.dx+Parent->NodeObject.width+0.1;
	    double fatherydw=Parent->NodeObject.dy+fH+ChildNum/2*h*1.3;
	    MovePen(Parent->NodeObject.dx+Parent->NodeObject.width,Parent->NodeObject.dy+fH);
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
		    	SetPenColor("Dark Gray");
			    DrawLine(0.1,0);
			    button(GenUIID(0),move_x+0.1,move_y-0.5*h,TextStringWidth(Children[i]->Content)+0.2,h,Children[i]->Content);
			    EditCoordinate(Children[i],move_x+0.1,move_y-0.5*h);//when inserting or deleting, every child's coordinate is supposed to be changed 
				Children[i]->NodeObject.height=h;
				Children[i]->NodeObject.width=TextStringWidth(Children[i]->Content)+0.2;
				Children[i]->NodeObject.dx=move_x+0.1;
				Children[i]->NodeObject.dy=move_y-0.5*h;
				Children[i]->NodeObject.color=0;
				MovePen(fatherx,move_y);
				SetPenColor("Dark Gray");
			    DrawLine(0,-h*1.3);
			    move_x=GetCurrentX();
			    move_y=GetCurrentY();
		    }
		    SetPenColor("Dark Gray");
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
//	static char memoroot[80]="Click To Start"; //������ʾ
//	if(Parent==NULL)//draw the root
//	{
//		SetPenSize(3);
//		SetPenColor("Gray");
//		MovePen(root->NodeObject.dx,root->NodeObect.dy)
//		DrawLine(1.4,0);
//		drawLabel(root->NodeObject.dx,root->NodeObject.dy,root->Content);
//	}
//	else if(ChildNum==0);
//    else//draw the children
//	{
//		double fatherx=Parent->NodeObject.dx+Parent->NodeObject.width+0.1;
//	    double fatherydw=Parent->NodeObject.dy+fH+ChildNum/2*h*1.8;
//	    MovePen(Parent->NodeObject.dx+Parent->NodeObject.width,Parent->NodeObject.dy+Parent->NodeObject.height);
//	    SetPenSize(3);
//		SetPenColor("Gray");
//		
//		for(i=0;i<ChildNum-1;i++)
//		{
//			int r=2*GetCurrentX()
//		}
//	}
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
	   	case 1:
		   {
		   	 LevelOrderTravelsal(root,DrawChildren1);
			 break;
		   }
	   	//case 2:LevelOrderTravelsal(root,DrawChildren2);
	   } 
}


// ����������provided in libgraphics
void DisplayClear(void); 
// ��ʱ������������provided in libgraphics
//void startTimer(int id,int timeinterval);

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
	PtrTreeNode yf=NULL; 
	static char memochild[30]="Text Here";
	PtrTreeNode TargetNode=NULL;
	switch (event)
	{
    	case KEY_UP:	
		if (key == VK_F1)//���������(why not TAB? -has been used) 
	    	    {
	    	        TargetNode=LocateNode(mouse_x,mouse_y,root);//temporary varible
	    	        if(TargetNode!=NULL)
					{
						ChildrenNum=FindChildren(TargetNode,Children);
	    	            yf = InsertTreeNode(TargetNode,0,idnum,TargetNode->NodeObject);
//	    	            LevelOrderTravelsal(root,DrawChildren1);
						switch(switch_button) 
		    	        {
		    	            case 1:LevelOrderTravelsal(root, DrawChildren1);break;
		    	            case 2:LevelOrderTravelsal(root, DrawChildren2);break;
		    	            case 3:LevelOrderTravelsal(root, DrawChildren3);break;
		    	        }
		    	        idnum++;
		    	        drawLabel(winwidth/1.5,winheight/22,"Hello World!");
		    	    }else{
		    	    	char toolman[10];
						sprintf(toolman,"Sorry.Do Not Find the Target.(Mouse in %lf %lf)",mouse_x,mouse_y);
						drawLabel(winwidth/1.5,winheight/22,toolman);
		    	    }
		        }
	        if (key == VK_F2)//���ͬ������ 
			    {
//			    	double move_x=GetCurrentX();
//	                double move_y=GetCurrentY();
			        TargetNode=LocateNode(mouse_x,mouse_y,root);
			        if(TargetNode!=NULL)
					{
					    ChildrenNum=FindChildren(TargetNode,Children);
			            yf = InsertTreeNode(TargetNode,1,idnum,TargetNode->NodeObject);
	    	            switch(switch_button) 
		    	        {
		    	            case 1:LevelOrderTravelsal(root, DrawChildren1);break;
							case 2:LevelOrderTravelsal(root, DrawChildren2);break;
		    	            case 3:LevelOrderTravelsal(root, DrawChildren3);break;
		    	        }
			            idnum++;
			        }
			    }
			if (key == VK_F3)//ɾ��һ������ 
			    {
//			    	double move_x=GetCurrentX();
//	                double move_y=GetCurrentY();
			    	TargetNode=LocateNode(mouse_x,mouse_y,root);
//			    	printf("TargetNode->NodeObject.dx=%lf\ncurrentX=%lf\n",TargetNode->NodeObject.dx,mouse_x);
			    	if(TargetNode!=NULL)
			    	{
//			    		printf("Here you enter delete targ name = %s\n", TargetNode->Content);
			    		DeleteTree(TargetNode);
//						printf("After delete");
			    		switch(switch_button) 
		    	        {
		    	            case 1:LevelOrderTravelsal(root, DrawChildren1);break;
		    	            case 2:LevelOrderTravelsal(root, DrawChildren2);break;
		    	            case 3:LevelOrderTravelsal(root, DrawChildren3);break;
		    	        }
			    	}
			    }
			if (key == VK_F4)//edit part (for test)
			{
//				double move_x=GetCurrentX();
//	            double move_y=GetCurrentY();
//	            drawLabel(winwidth/1.5,winheight/22,"Here You are");
//				TargetNode=LocateNode(mouse_x,mouse_y,root);//temporary varible
//				if(TargetNode == NULL){
//					char toolman[10];
//					sprintf(toolman,"%lf %lf",mouse_x,mouse_y);
//					drawLabel(winwidth/1.5,winheight/22,toolman);
//					printf("It's a test");
//				}else{
//				EditContent(TargetNode,memochild);
//				drawLabel(winwidth/1.5,winheight/22,"Success!Go On~");
//			    }
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
//	display(); //ˢ����ʾ 
	uiGetMouse(x, y, button, event); // needed for using simpleGUI
	mouse_x=ScaleXInches(x); 
	mouse_y=ScaleYInches(y);
//	PtrTreeNode TargetNode=NULL;
//	display();
	if(event==DOUBLE_CLICK)
	{
		PtrTreeNode TempNode = LocateNode(mouse_x,mouse_y,root);
		if( TempNode != NULL){
			TargNode = TempNode;
			strcpy(memo, TargNode->Content);
			isEdit=1;	
		}
	}
	display(); //ˢ����ʾ
//	if(isEdit==1&&TargetNode!=NULL)
//	{
//		static char memochild[30]="Text Here";
//		textbox(GenUIID(0),winwidth/1.5,winheight/15,winwidth/10*3,GetFontHeight()*2,memochild,sizeof(memochild));
//		EditContent(TargetNode,memo);
//		LevelOrderTravelsal(root,DrawChildren1);
//		TargetNode->NodeObject.width=TextStringWidth(memochild);//!!!!!!!!!!!!!
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
	InitConsole();
	double x=winwidth/3.5;//2.857
	double y=winheight/1.8;//3.889
	double w=winwidth/10;//1
	double fH=GetFontHeight();//0.1667
//	printf("%lf",fH);
	double h=fH*2;//0.3334
	static char memoroot[80]="Click To Start"; //������ʾ
	root = CreateTree(0,Father);//give it to the backend
	static char mroot[80]="Text Here";
    EditContent(root,mroot);//while using the textbox,the value of memoroot is supposed to be changed, so you are supposed to change it in the backend
    root->NodeNumber = 0;//initinalization. root is a global variable defined in draw.h
    root->NodeObject.height=h;
    root->NodeObject.width=w*1.4;//1.4
    root->NodeObject.dx=x;
    root->NodeObject.dy=y;
    root->NodeObject.color=0;
    root->FirstChild=NULL;
    root->NextSibling=NULL;
//    printf("root->NodeObject.width = %lf\n", root->NodeObject.width); 
    InitalQueue();
//    TxtFile2Tree("�ļ�1.txt");
	display();
}

void display()
{
//	printf("in display root->content = %s\n", root->Content);
//	PtrTreeNode TargetNode=NULL;
	// ����
	DisplayClear();
	if(isEdit){
		textbox(GenUIID(0),winwidth/1.5,winheight/15,winwidth/10*3,GetFontHeight()*2,memo,sizeof(memo));
		if(TargNode != NULL){
//			printf("In display, Targnode->Context = %s\n", TargNode->Content);
			EditContent(TargNode,memo);	
		}
		
	}
	
	//����ģʽ 
	ModeChoice();
#if defined(DEMO_MENU)
	// ���ƺʹ���˵�
	drawMenu();
#endif
}















