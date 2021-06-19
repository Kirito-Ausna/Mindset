// 全局变量
double winwidth=15, winheight=10;// 窗口尺寸
int switch_button=0;//响应mode 切换
int isEditing=0;// 判断有没有在编辑文本 
double mouse_x=0;
double mouse_y=0;
struct NodeClass Father={10/3.5,7/1.8,1};
struct TreeNode rroot;
PtrTreeNode root = &rroot;
PtrTreeNode Children[10];
int ChildrenNum;
int idnum=0;
int isEdit=0;
char memo[100]="Text Here";
PtrTreeNode TargNode=NULL; 
