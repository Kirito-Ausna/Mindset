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

// ȫ�ֱ���
static double winwidth, winheight;// ���ڳߴ�
static int switch_button=0;//��Ӧmode �л�
static int isEditing=0;// �ж���û���ڱ༭�ı� 
static int mouse_x=0;
static int mouse_y=0;
struct NodeClass Root{
	height=h;
    width=w;
    dx=x, dy=y;
    color=0;/************�����color�����е�����**************/ 
};
 
void Drawmyson()//��һ�������� 
{
	MovePen(mouse_x,mouse_y);
	DrawLine(1,1);
}
void Drawmybrother(void)
{
	DrawLine(1,2);
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
	if(button(GenUIID(0),1,1,1,1,"test"))
	switch (event)
	{
	case KEY_UP:
		if (key == VK_TAB)//��������� 
			Drawmyson();
		display();
	    if (key == VK_RETURN)//���ͬ������ 
			Drawmybrother();
			display();
			break;
	default:
		break;
	}
	display(); // ˢ����ʾ
}

// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event);
	mouse_x = ScaleXInches(x);
	mouse_y = ScaleYInches(y);
	display(); // ˢ����ʾ
}

// ��ת��ʱ��
#define MY_ROTATE_TIMER  1


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
	registerKeyboardEvent(KeyboardEventProcess);// ����1 
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
	double fH = GetFontHeight();//����߶� 
	double h = fH*2;  // �ؼ��߶�
	double x = winwidth/30;  
	double y = winheight/2-h; 
	double w = winwidth/10; // �ؼ����
	char*hint="Click Here To Add Text";//������ʾ 
	
    if (button(GenUIID(0), x, y, w, h, "mode 1"))
   	    switch_button=1;
    if( button(GenUIID(0), x, y-1.5*h, w, h, "mode 2") )
   	    switch_button=2;
    if( button(GenUIID(0), x, y-3*h, w, h, "mode 3") )
   	    switch_button=3;
}
#endif

void DrawMode1()//��һ��һ������������ 
{
    double fH = GetFontHeight();
	double h = fH*2;  // �ؼ��߶�
	double x = winwidth/3.5;  
	double y = winheight/1.8; 
	double w = winwidth*0.18; // �ؼ����
	char*hint="Click Here To Add Text";//������ʾ 
	//�Ƚ��ø��ڵ� 6��7�ų��Խ���ȥ 

    if(button(GenUIID(0), x, y, w, h, hint)&&textbox(GenUIID(0),winwidth/1.2,winheight/3.5,w*1.8,h,"text here",Root.content))
    {
    	isEditing = 1;
    }
    if(isEditing)
    {
    	CreateTree(0,NodeObject); 
		button(GenUIID(0),winwidth/3.5, winheight/1.8, w*1.8, h, Root.content);   
    }
}
void DrawMode2()
{}
void DrawMode3()
{}
#if defined(DEMO_BUTTON)
// ��ť�л�ģʽ Mode 
void DrawModeorigin()
{
	double fH = GetFontHeight();
	double h = fH*2;  // �ؼ��߶�
	double x = winwidth/30;  
	double y = winheight/2-h; 
	double w = winwidth/10; // �ؼ����
	char*hint="Click Here To Add Text";//������ʾ 
    
    ModeChoice();//ѡ������
	switch(switch_button){
		case 1:DrawMode1();break;
		case 2:DrawMode2();break;
		case 3:DrawMode3();break;
	} 

}
#endif // #if defined(DEMO_BUTTON)









#if defined(DEMO_EDITBOX)
// �ı��༭��ʾ����
void drawEditText()
{
	static char firstName[80] = "Xinguo";
	static char lastName[80] = "Liu";
	static char results[200] = "";
	static char memo[80]="Hello World! Edit me!";
	static char numstr[80] = "3.14159265";
	static char newnum[80] = "";
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double w = winwidth/4; // �ؼ����
	double x = winwidth/2.5;
	double y = winheight/2-h;

	textbox(GenUIID(0), winwidth / 10, winheight-h-h, w, h, memo, sizeof(memo));
	textbox(GenUIID(0), x, y, w, h, memo, sizeof(memo));

	SetPenColor("Brown"); 
	drawLabel(x-fH/2-TextStringWidth("��"), (y-=h*1.2)+fH*0.7, "��");
	if( textbox(GenUIID(0), x, y, w, h, firstName, sizeof(firstName)) )
		sprintf(results,"Text edit result is: %s+%s", firstName,lastName);

	SetPenColor("Brown"); 
	drawLabel(x-fH/2-TextStringWidth("��"), (y-=h*1.2)+fH*0.7, "��");
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
	// ����
	DisplayClear();
			
#if defined(DEMO_BUTTON)
	// ��ť
	DrawModeorigin();
#endif

#if defined(DEMO_EDITBOX)
	// �༭�ı��ַ���
	drawEditText();
#endif

#if defined(DEMO_MENU)
	// ���ƺʹ���˵�
	drawMenu();
#endif
}













