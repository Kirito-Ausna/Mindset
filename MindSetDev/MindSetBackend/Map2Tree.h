#ifndef __Map2Tree__
#define __Map2Tree__
typedef struct TreeNode* PtrTreeNode;

struct NodeClass{
    double height;
    double width;
    double dx, dy;
    int color;
};

struct TreeNode
{
    int NodeNumber;
    struct NodeClass NodeObject;
    char Content[50];
    PtrTreeNode FirstChild, NextSibling;
};

// Global Variable Tree Root
static PtrTreeNode Root;
// A queue for level order travelsal, it's global
struct Queue{
    PtrTreeNode Q[20];
    int head;
    int tail;
    int max_size;
    int cur_size;
} queue;


// Create and initialize a tree root node or a subtree root node
PtrTreeNode CreateTree(int NodeNum, struct NodeClass NodeObject);
// To see whether the user move mouse on our node or not, (x,y) is the location of mouse
PtrTreeNode LocateNode(double x, double y, PtrTreeNode root);
// Edit the Content
void EditContent(PtrTreeNode node, char value[]);
// Update the node coordinate
void EditCoordinate(PtrTreeNode node, double x, double y);
// Delete a node on cascade
void FreeNode(PtrTreeNode node);//node points to the TreeNode which is empty
void DeleteTree(PtrTreeNode subtree);//Root can't be deleted, only set content to null

// Insert a Tree Node and get its pointer for the sake of updating simplicity
PtrTreeNode InsertTreeNode(PtrTreeNode ChosedNode, int relation, int NodeNum, struct NodeClass NodeObject);

// Find Children and return the children array
int FindChildren(PtrTreeNode Parent, PtrTreeNode Children[]);

void LevelOrderTravelsal(PtrTreeNode Root, void(*DrawChildren)(
    PtrTreeNode Parent, int ChildNum, PtrTreeNode Children[]
));// Need to combine with frontend function.DrawChildren is function pointer

//Save and read, need to input name
void Tree2BinaryFile(PtrTreeNode Root, char name[]);
PtrTreeNode BinaryFile2Tree(char name[]);
void Tree2TxtFile(PtrTreeNode Root, char name[]);
PtrTreeNode TxtFile2Tree(char name[]);


#endif // !__TreeStructure__
