#include<stdio.h>
#include<stdlib.h>
#include<Map2Tree.h>
#include<string.h>

PtrTreeNode CreateTree(int NodeNum, struct NodeClass NodeObject){

    PtrTreeNode root = (PtrTreeNode)malloc(sizeof(struct TreeNode));
    strcpy(root->Content, "Text Here!");// Initially set to be empty
    root->NodeObject = NodeObject;
    root->NodeNumber = NodeNum;
    root->FirstChild = NULL;
    root->NextSibling = NULL;
	return root;
}

PtrTreeNode LocateNode(double x, double y, PtrTreeNode root){
    if(root == NULL){
        return NULL;
    }

    struct NodeClass obj = root->NodeObject;
//    printf("%lf %lf %lf %lf",obj.dx,obj.dx+obj.width,obj.dy,obj.dy+obj.height);
    if((x>obj.dx)&&(x<obj.dx+obj.width)&&(y>obj.dy)&&(y<obj.dy+obj.height))
        return root;

    PtrTreeNode Node1 = LocateNode(x, y, root->FirstChild);
    PtrTreeNode Node2 = LocateNode(x, y, root->NextSibling);

    if(Node1 != NULL)
        return Node1;
    else if(Node2 != NULL)
        return Node2;
    else
        return NULL;
}

void EditContent(PtrTreeNode node, char value[]){
    strcpy(node->Content, value);
}

void EditCoordinate(PtrTreeNode node, double x, double y){
    node->NodeObject.dx = x;
    node->NodeObject.dx = y;
}

void FreeNode(PtrTreeNode node){
    free(node);
}

void DeleteTree(PtrTreeNode subtree){
    if(subtree == NULL){
        return;
    }
    DeleteTree(subtree->FirstChild);
    DeleteTree(subtree->NextSibling);
    if(subtree != Root){
        FreeNode(subtree);
    }else{
        EditContent(subtree,"");
    }
}

PtrTreeNode InsertTreeNode(PtrTreeNode ChosedNode, int relation, int NodeNum, struct NodeClass NodeObject)
{
    if(relation == 0)// define inserting a child or next level node
    {// insert a child
        PtrTreeNode fisrtchild = ChosedNode->FirstChild;
        ChosedNode->FirstChild = CreateTree(NodeNum,NodeObject);
        ChosedNode->FirstChild->NextSibling = fisrtchild;
        return ChosedNode->FirstChild;
    }else{
        while (ChosedNode->NextSibling != NULL)
        {
            ChosedNode = ChosedNode->NextSibling;
        }
        ChosedNode->NextSibling = CreateTree(NodeNum,NodeObject);
        return ChosedNode->NextSibling;
    }
}

int FindChildren(PtrTreeNode Parent, PtrTreeNode Children[]){
    int ChildNum = 0;
    PtrTreeNode Child = Parent->FirstChild;
    while (Child)
    {
        Children[ChildNum] = Child;
        ChildNum++;
        Child = Child->NextSibling;
    }
    Child = Parent->FirstChild;
    return ChildNum;
}

void InitalQueue(){
    queue.head = 0;
    queue.tail = -1;
    queue.max_size = 20;
    queue.cur_size = 0;
}

int is_full(){
    if(queue.cur_size == queue.max_size){
        return 1;
    }else
        return 0;
}

int is_empty(){
    if(queue.cur_size == 0)
    {
        return 1;
    }else{
        return 0;
    }
}

void q_push(PtrTreeNode node){
    if(!is_full())
    {
        queue.tail = (queue.tail + 1)%queue.max_size;
        queue.Q[queue.tail] = node;
        queue.cur_size++;
    }else{
        printf("The space of Queue is run out!\n");
    }
}

PtrTreeNode q_pop(){
    if(!is_empty())
    {
        PtrTreeNode front = queue.Q[queue.head];
        queue.head = (queue.head + 1)%queue.max_size;
        queue.cur_size--;
        return front;
    }else{
        return NULL;
    }
}


void LevelOrderTravelsal(PtrTreeNode Root, void(*DrawChildren)(
    PtrTreeNode Parent, int ChildNum, PtrTreeNode Children[])){
    // Draw the root
    DrawChildren(NULL, 1, &Root);
    PtrTreeNode Children[20];
    while (Root)
    {
        int num_child = 0;
        PtrTreeNode Child = Root->FirstChild;
        while (Child)
        {
            Children[num_child] = Child;
            num_child++;
            q_push(Child);
            Child = Child->NextSibling;
        }
        if(num_child != 0)
            DrawChildren(Root, num_child, Children);
        Root = q_pop();
    }
}


void Tree2BinaryFile(PtrTreeNode Root, char name[]){
    FILE* fin = fopen(name, "wb+");
    struct TreeNode Children[20];
    fwrite(Root, sizeof(struct TreeNode), 1, fin);
    while (Root)
    {
        int num_child = 0;
        PtrTreeNode Child = Root->FirstChild;
        while (Child)
        {
            Children[num_child] = *Child;
            num_child++;
            q_push(Child);
            Child = Child->NextSibling;
        }
        fwrite(&num_child, sizeof(int), 1, fin);
        if(num_child != 0){
            fwrite(Children, sizeof(struct TreeNode), num_child, fin);
        }
        Root = q_pop();
    }
}

PtrTreeNode BinaryFile2Tree(char name[]){
    FILE* fout = fopen(name, "rb");
    PtrTreeNode Root = (PtrTreeNode)malloc(sizeof(struct TreeNode));
    struct TreeNode Children[20];
    fread(Root, sizeof(struct TreeNode), 1, fout);
    Root->FirstChild = NULL;
    Root->NextSibling = NULL;
    PtrTreeNode R = Root;
    while (Root)
    {
        int num_children;
        fread(&num_children, sizeof(int), 1, fout);
        if(num_children != 0){
            fread(Children, sizeof(struct TreeNode), num_children, fout);
            Root->FirstChild = (PtrTreeNode)malloc(sizeof(struct TreeNode));
            *(Root->FirstChild) = Children[0];
            q_push(Root->FirstChild);
            PtrTreeNode Child = Root->FirstChild;
            int i=1;
            for(i = 1;i<num_children;i++){
                Child->NextSibling = (PtrTreeNode)malloc(sizeof(struct TreeNode));
                q_push(Root->NextSibling);
                *(Child->NextSibling) = Children[i];
                Child = Child->NextSibling;
            }
            Child->NextSibling = NULL;
        }else{
            Root->FirstChild = NULL;
        }
        Root = q_pop();
    }
    return R;
}

void Tree2TxtFile(PtrTreeNode Root, char name[]){
    FILE* fin = fopen(name, "w+");
    struct TreeNode Children[20];
//    fprintf(fin, "Here is test save\n");
    fprintf(fin, "%d %lf %lf %lf %lf %d %s ",Root->NodeNumber,
    Root->NodeObject.height, Root->NodeObject.width, Root->NodeObject.dx, Root->NodeObject.dy,
    Root->NodeObject.color,Root->Content);
    while (Root)
    {
        int num_child = 0;
        PtrTreeNode Child = Root->FirstChild;
        while (Child)
        {
            Children[num_child] = *Child;
            num_child++;
            q_push(Child);
            Child = Child->NextSibling;
        }
        fprintf(fin, "%d\n", num_child);
        int i=0;
        for(i=0;i<num_child;i++){
            fprintf(fin, "%d %lf %lf %lf %lf %d %s\n",Children[i].NodeNumber,
            Children[i].NodeObject.height, Children[i].NodeObject.width, Children[i].NodeObject.dx, Children[i].NodeObject.dy,
            Children[i].NodeObject.color,Children[i].Content);
        }
        Root = q_pop();
    }
//    add by shengnv 2021.6.19 
//	update reason: forget fclose, can not save successfully
    fclose(fin);
}

PtrTreeNode TxtFile2Tree(char name[]){
    FILE* fout = fopen(name, "r");
    PtrTreeNode Root = (PtrTreeNode)malloc(sizeof(struct TreeNode));
    struct TreeNode Children[20];
    fscanf(fout, "%d %lf %lf %lf %lf %d %s", &Root->NodeNumber,
    &Root->NodeObject.height, &Root->NodeObject.width, &Root->NodeObject.dx, &Root->NodeObject.dy,
    &Root->NodeObject.color,Root->Content);
    Root->FirstChild = NULL;
    Root->NextSibling = NULL;
    PtrTreeNode R = Root;
    while (Root)
    {
        int num_children;
        fscanf(fout, "%d", &num_children);
        if(num_children != 0){
        	int i=0;
           for(i=0;i<num_children;i++){
            fscanf(fout, "%d %lf %lf %lf %lf %d %s",&Children[i].NodeNumber,
            &Children[i].NodeObject.height, &Children[i].NodeObject.width, &Children[i].NodeObject.dx, &Children[i].NodeObject.dy,
            &Children[i].NodeObject.color,Children[i].Content);
        }
            Root->FirstChild = (PtrTreeNode)malloc(sizeof(struct TreeNode));
            *(Root->FirstChild) = Children[0];
            q_push(Root->FirstChild);
            PtrTreeNode Child = Root->FirstChild;
            for(i = 1;i<num_children;i++){
                Child->NextSibling = (PtrTreeNode)malloc(sizeof(struct TreeNode));
                q_push(Root->NextSibling);
                *(Child->NextSibling) = Children[i];
                Child = Child->NextSibling;
            }
            Child->NextSibling = NULL;
        }else{
            Root->FirstChild = NULL;
        }
        Root = q_pop();
    }
    printf("TxtFile2Tree End here\n");
    fclose(fout);
    return R;
}
