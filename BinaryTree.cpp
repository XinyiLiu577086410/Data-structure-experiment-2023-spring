#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>
#include <iostream>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#undef OVERFLOW
#define OVERFLOW -2
#define max(x,y) ((x)>(y)?(x):(y))
typedef int status;
typedef int KeyType;
typedef struct {
    KeyType  key;
    char others[20];
} TElemType; //二叉树结点类型定义

typedef struct BiTNode{  //二叉链表结点的定义
    TElemType  data;
    struct BiTNode *lchild,*rchild;
} BiTNode, *BiTree;

typedef struct THETREES{
    struct ATREE{
        BiTree T;
        char name[30];
    }elem[16];
    int length = 0;
} TREES;

BiTree RecurvalCreateBiTree(TElemType definition[], int & start) {
    BiTree p = (BiTNode * )malloc(sizeof(BiTNode));
    if(definition[start].key<=0) {
        start++;
        return NULL;
    };
    p->data = definition[start];
    start++;
    p->lchild = RecurvalCreateBiTree(definition, start);
    p->rchild = RecurvalCreateBiTree(definition, start);
    return p;
}

/*
根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，
将根节点指针赋值给T并返回OK，
如果有相同的关键字，返回ERROR
 */
status CreateBiTree(BiTree &T,TElemType definition[])
{
    if(T) return INFEASIBLE;
    for(int i=0; ; i++) {
        if(definition[i].key == -1) break;
        if(definition[i].key > 0)
        for(int j=i+1; ;j++){
            if(definition[j].key==-1) break;
            if(definition[i].key == definition[j].key)
                return ERROR; // 检查是否有重复key
        }
    }
    int start = 0;
    T = RecurvalCreateBiTree(definition,start);
    return OK;
}

/*
 删除所有结点，释放结点空间
 */
status DestoryBiTree(BiTree &T)
{
    if(T==NULL) return INFEASIBLE;
    if(T->lchild) DestoryBiTree(T->lchild);
    if(T->rchild) DestoryBiTree(T->rchild);
    free(T);
    T=NULL;
    return OK;
}

/*
将二叉树设置成空
 */
status ClearBiTree(BiTree &T)
{
    if(T==NULL) return INFEASIBLE;
    if(T->lchild) ClearBiTree(T->lchild);
    if(T->rchild) ClearBiTree(T->rchild);
    T->data.key=0;
    for(int i=0;i<30;i++) T->data.others[i]='\0';
    return OK;
}

int BiTreeEmpty(BiTree &T){
    if(T==NULL) return 1;
    else return 0;
}

/*
求二叉树T的深度
 */
int BiTreeDepth(BiTree T)
{
    if(T==NULL) return 0;
    return 1 + fmax(BiTreeDepth(T->lchild),BiTreeDepth(T->rchild));
}

/*
查找结点
 */
BiTNode * LocateNode(BiTree T, KeyType e)
{
    if(T==NULL) return NULL;//应该先检查。
    if(T->data.key==e) return T;
    BiTNode * p;
    if((p=LocateNode(T->lchild, e))!=NULL) return p;
    if((p=LocateNode(T->rchild, e))!=NULL) return p;
    else return NULL;
}

/*
 实现结点赋值
 */
status Assign(BiTree &T,KeyType e,TElemType value)
{
    if(T==NULL) return INFEASIBLE;
    BiTNode * p;
    if((p = LocateNode(T, e))!=NULL){
        if(e!=value.key) if(LocateNode(T, value.key)) return ERROR;
        p->data = value;
        return OK;
    }
    else return ERROR;
}

/*
查找双亲结点
*/
BiTNode * LocateParents(BiTree T, KeyType e)
{
    if(T==NULL) return NULL;//应该先检查。
    BiTNode * p;
    if(T->lchild) {
        if(T->lchild->data.key==e) return T;
        if((p=LocateParents(T->lchild, e))!=NULL) return p;
    }
    if(T->rchild) {
        if(T->rchild->data.key==e) return T;
        if((p=LocateParents(T->rchild, e))!=NULL) return p;
    }
    return NULL;
}

/*
实现获得兄弟结点
*/
BiTNode * GetSibling(BiTree T,KeyType e)
{
    BiTNode * p = LocateParents(T, e);
    if(p==NULL) return NULL;
    if(p->lchild) if(p->lchild->data.key==e) return p->rchild;
    if(p->rchild) if(p->rchild->data.key==e) return p->lchild;
    return NULL;
}

/*
插入结点
 */
status InsertNode(BiTree &T,KeyType e,int LR,TElemType c)
{
    if(T == NULL && LR != -1) return INFEASIBLE;
    BiTNode * p = LocateNode(T, e);
    if(p == NULL && LR != -1) return ERROR;
    if(LocateNode(T, c.key)) return ERROR;
    switch (LR) {
        case -1:
        {
            BiTNode * tmp = (BiTNode * )malloc(sizeof(BiTNode));
            tmp->data = c;
            tmp->lchild = NULL;
            tmp->rchild = T;
            T = tmp;
            break;
        }
        case 0:
        {
            BiTNode * tmp = (BiTNode * )malloc(sizeof(BiTNode));
            tmp->data = c;
            tmp->lchild = NULL;
            tmp->rchild = p->lchild;
            p->lchild = tmp;
            break;
        }
        case 1:
        {
            BiTNode * tmp = (BiTNode * )malloc(sizeof(BiTNode));
            tmp->data = c;
            tmp->lchild = NULL;
            tmp->rchild = p->rchild;
            p->rchild = tmp;
            break;
        }
        default:
            break;
    }
    return OK;
}

/*
计算结点度
*/
int Degree(BiTNode *p){
    int degree=0;
    if(p==NULL) return 0;
    if(p->lchild) degree++;
    if(p->rchild) degree++;
    return degree;
}

/*
删除结点
*/
status DeleteNode(BiTree &T,KeyType e)
{
    if(T==NULL) return INFEASIBLE;
    BiTNode * p = LocateNode(T, e);
    if(p==NULL) return ERROR;
    BiTNode * parents = LocateParents(T, e);
    if(parents==NULL) {
        switch (Degree(p)) {
            case 0:
            {
                free(T);
                T = NULL;
                break;
            }
            case 1:
            {
                p = p->lchild?p->lchild:p->rchild;
                free(T);
                T = p;
                break;
            }
            case 2:
            {
                BiTNode * q = p->rchild;
                p = p->lchild;
                free(T);
                T = p;
                while(p->rchild) p = p->rchild;
                p->rchild = q;
                break;
            }
            default:
                break;
        }
        return OK;
    }//if(parents==NULL)
    switch (Degree(p)) {
        case 0:
        {
            if(parents->lchild==p) {
                parents->lchild=NULL;
                free(p);
            }
            if(parents->rchild==p)
            {
                parents->rchild=NULL;
                free(p);
            }
            break;
        }
        case 1:
        {
            int lr;
            if(p->lchild) lr = 0; else lr = 1;
            if(parents->lchild==p) {
                parents->lchild = lr ? p->rchild : p->lchild;
                free(p);
            }
            if(parents->rchild==p)
            {
                parents->rchild = lr ? p->rchild : p->lchild;
                free(p);
            }
            break;
        }
        case 2:
        {
            BiTNode * extright = p->lchild;
            while (extright->rchild)
                extright = extright->rchild;
            extright->rchild = p->rchild;
            if(parents->lchild==p) {
                parents->lchild=p->lchild;
                free(p);
            }
            if(parents->rchild==p)
            {
                parents->rchild=p->lchild;
                free(p);
            }
            break;
        }
        default:
            break;
    }//switch(Degree(p))
    return OK;
}

/*
先序遍历二叉树T
 */
status PreOrderTraverse(BiTree T,void (*visit)(BiTree))
{
    if(T==NULL) return ERROR;
    visit(T);
    PreOrderTraverse(T->lchild, visit);
    PreOrderTraverse(T->rchild, visit);
    return OK;
}

/*
中序遍历二叉树T
 */
status InOrderTraverse(BiTree T,void (*visit)(BiTree))
{
    if(T==NULL) return ERROR;
    std::stack<BiTNode*> s;
    BiTNode * p = T;
    while(p||!s.empty()){
        if(p){
            s.push(p);
            p = p->lchild;
        }else{
            p = s.top(); s.pop();
            visit(p);
            p = p->rchild;
        }
    }
    return OK;
}

/*
后序遍历二叉树T
 */
status PostOrderTraverse(BiTree T,void (*visit)(BiTree))
{
    if(T==NULL) return ERROR;
    PostOrderTraverse(T->lchild, visit);
    PostOrderTraverse(T->rchild, visit);
    visit(T);
    return OK;
}

/*
按层遍历二叉树T
 */
status LevelOrderTraverse(BiTree T,void (*visit)(BiTree))
{
    if(T==NULL) return ERROR;
    std::queue<BiTNode*> qu;
    qu.push(T);
    while(!qu.empty()){
        BiTNode * p = qu.front();
        visit(p);
        qu.pop();
        if(p->lchild) qu.push(p->lchild);
        if(p->rchild) qu.push(p->rchild);
    }
    return OK;
}

/*
先序遍历二叉树T来构建保存定义数组
 */
void SaveTraverse(BiTree T, TElemType def[], int &start)
{
    if(T==NULL) {def[start].key = 0; start++; return;}
    else def[start] = T->data;
    start++;
    SaveTraverse(T->lchild, def, start);
    SaveTraverse(T->rchild, def, start);
    return;
}

/*
 将二叉树的结点数据写入到文件FileName中
 */
status SaveBiTree(BiTree T, char FileName[])
{
    if(T==NULL) return INFEASIBLE;
    TElemType def[128];
    memset((void*)def, 0, sizeof(def));
    int start=0;
    SaveTraverse(T,def,start);
    def[127].key = -1;
    FILE *fp = fopen(FileName, "wb");
    if(fp == NULL) return ERROR;
    fwrite(def, sizeof(TElemType), 128, fp);
    fclose(fp);
    return OK;
}

/*
 读入文件FileName的结点数据，创建二叉树
 */
status LoadBiTree(BiTree &T,  char FileName[])
{
    if(T) return INFEASIBLE;
    TElemType def[128];
    FILE *fp = fopen(FileName, "rb");
    if(fp == NULL) return ERROR;
    fread(def, sizeof(TElemType), 128, fp);
    CreateBiTree(T, def);
    return OK;
}

int LocateTree(TREES &Trees,char name[]){
    for(int i=0;i<Trees.length;i++)
    {
        if(strcmp(name, Trees.elem[i].name)==0) return i;
    }
    return -1;
}

void visit(BiTree T){
    printf("%d %s ", T->data.key, T->data.others);
}

int MaxPathSum(BiTree T){
    if(T==NULL) return 0;
    return T->data.key + max(MaxPathSum(T->lchild), MaxPathSum(T->rchild));
}

BiTNode * LowestCommonAncestor(BiTree T, KeyType e1, KeyType e2) {
    if(T->data.key==e1||T->data.key==e2) return T;
    if(!LocateNode(T, e1) || !LocateNode(T, e2)) {
        return NULL;
    }
    BiTNode *t[2][2];
    memset(t, 0, sizeof(t));
    t[0][0] = LocateNode(T->lchild, e1);
    t[0][1] = LocateNode(T->lchild, e2);
    t[1][0] = LocateNode(T->rchild, e1);
    t[1][1] = LocateNode(T->rchild, e2);
    if(t[0][0] && t[0][1]) return LowestCommonAncestor(T->lchild, e1, e2);
    if(t[1][0] && t[1][1]) return LowestCommonAncestor(T->rchild, e1, e2);
    return T;
}

void InvertTree(BiTree T) {
    if(T==NULL) return;
    BiTNode * tmp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = tmp;
    InvertTree(T->lchild);
    InvertTree(T->rchild);
}


int main(void){
    for(int i=0;i<5;i++) std::cout<<std::endl;
    std::cout<<"****************二****叉****树****实****验****************"<<std::endl;
    for(int i=0;i<5;i++) std::cout<<std::endl;
    int op=1, idt=0;
    TREES Trees;
    while(op){
        if(Trees.length==0) {
            printf("二叉树管理表为空，请先建立二叉树！\n");
            op = 18;
        }
        else
        {
            printf("\n\n");
            printf("            Menu for Binary Tree \n");
            printf("-------------------------------------------------\n");
            printf("          1. CreatBiTree        2. DestroyBiTree\n");
            printf("          3. ClearBiTree        4. BiTreeEmpty\n");
            printf("          5. BiTreeDeepth       6. LocateNode \n");
            printf("          7. Assign             8. GetSibling\n");
            printf("          9. InsertNode         10. DeleteNode\n");
            printf("          11. PreOrderTraverse  12. InOrderTraverse\n");
            printf("          13. PostOrderTraverse 14. LevelOrderTraverse\n");
            printf("          15. MaxPathSum        16. LowestCommonAncestor\n");
            printf("          17. InvertTree        18. AddTree\n");
            printf("          19. RemoveTree        20. SwitchTree\n");
            printf("          21. SaveTree          22. LoadTree(DestroyBiTree will be called)\n");
            printf("          23. ShowTrees         \n");
            printf("          0. Exit\n");
            printf("-------------------------------------------------\n");
            printf("    活动二叉树：%s\n", Trees.elem[idt].name);
            printf("    请选择你的操作[0~22]:\n");
            scanf("%d",&op);
        }
        BiTree & T = Trees.elem[idt].T;
        switch(op){
            case 1:
            {
                if(T) {printf("活动二叉树已存在！\n");getchar();getchar();break;}
                printf("请输入定义（带空子树的前序遍历）：\n");
                TElemType def[128];
                int i=0;
                while(1){
//                    scanf("%d %s\n", &def[i].key, def[i].others);
                    std::cin>>def[i].key>>def[i].others;
                    if(def[i].key == -1) break;
                    i++;
                }
                CreateBiTree(T, def);
                printf("创建二叉树成功！\n");
                getchar();getchar();
                break;
            }
            case 2:
            {
                switch(DestoryBiTree(T))
                {
                    case OK:
                        printf("销毁二叉树成功！\n");
                        break;
                    case INFEASIBLE:
                        printf("销毁二叉树失败，二叉树已经不存在！\n");
                        break;
                }
                getchar();getchar();
                break;
            }
            case 3:
            {
                
                switch(ClearBiTree(T))
                {
                    case OK:
                        printf("清空二叉树成功！\n");
                        break;
                    case INFEASIBLE:
                        printf("清空二叉树失败，二叉树已经不存在！\n");
                        break;
                }
                getchar();getchar();
                break;
                
            }
            case 4:
            {
                switch(BiTreeEmpty(T)) {
                    case 0:
                        printf("树不为空！\n");
                        break;
                    case 1:
                        printf("树为空！\n");
                        break;
                    default:
                        break;
                }
                getchar();getchar();
                break;
                
            }
            case 5:
            {
                printf("树的深度是 %d 。\n", BiTreeDepth(T));
                getchar();getchar();
                break;
            }
            case 6:
            {
                printf("请输入要查找的关键字：\n");
                KeyType key;
                scanf("%d", &key);
                BiTree p = LocateNode(T, key);
                if(p) printf("结点内容：key: %d , others : %s ." , p->data.key, p->data.others);
                else
                    printf("查找该关键字失败！");
                getchar();getchar();
                break;
                
            }
            case 7:
            {
                TElemType value;
                KeyType key;
                printf("请输入要赋值的结点的关键字：\n");
                scanf("%d", &key);
                printf("请输入要赋的关键字的值：\n");
                scanf("%d", &value.key);
                printf("请输入要赋的others值：\n");
                scanf("%s", value.others);
                switch (Assign(T, key, value)) {
                    case INFEASIBLE:
                        printf("赋值失败，二叉树为空！\n");
                        break;
                    case OK:
                        printf("赋值成功！\n");
                        break;
                    case ERROR:
                        printf("赋值失败！目标关键字不存在或赋值后有关键字相同。\n");
                        break;
                    default:
                        break;
                }
                getchar();getchar();
                break;
            }
            case 8:
            {
                printf("请输入目标关键字：\n");
                KeyType key;
                scanf("%d", &key);
                BiTree sibling = GetSibling(T, key);
                if(sibling)
                    printf("兄弟结点：key : %d ; others：%s .\n", sibling->data.key, sibling->data.others);
                else
                    printf("查找兄弟结点失败，目标结点无兄弟结点，或者目标结点是根结点。\n");
                getchar();getchar();
                break;
                
            }
            case 9:
            {
                printf("请输入目标关键字：\n");
                KeyType key;
                scanf("%d", &key);
                TElemType newdata;
                printf("0表示作为左孩子插入，1表示作为右孩子插入，-1代表作为根结点插入，请输入(0 or 1）：\n");
                int lr;
                scanf("%d", &lr);
                printf("请输入插入的结点的关键字、数据内容:\n");
                scanf("%d %s", &newdata.key, newdata.others);
                switch(InsertNode(T, key, lr, newdata)){
                    case INFEASIBLE:
                    {
                        printf("插入失败，树是空树！\n");
                        break;
                    }
                    case OK:
                    {
                        char namelr[5];
                        if(lr) strcpy(namelr, "右");
                        else  strcpy(namelr, "左");
                        printf("在关键字为 %d 的结点插入关键字为 %d 数据内容为 %s 的 \
                               结点作为%s孩子成功！",
                               key, newdata.key, newdata.others, namelr);
                        break;
                    }
                    case ERROR:
                    {
                        printf("插入结点失败！，可能找不到目标结点，或者出现名称冲突。");
                        break;
                    }
                }
                getchar();getchar();
                break;
                
            }
            case 10:
            {
                printf("请输入目标关键字：\n");
                KeyType key;
                scanf("%d", &key);
                switch (DeleteNode(T, key)) {
                    case OK:
                        printf("删除成功！\n");
                        break;
                    case INFEASIBLE:
                        printf("删除失败！树是空树！");
                        break;
                    case ERROR:
                        printf("删除失败！树上没有目标结点。");
                        break;
                    
                    default:
                        break;
                }
                getchar();getchar();
                break;
                
            }
            case 11:
            {
                PreOrderTraverse(T, &visit);
                printf("\n");
                getchar();getchar();
                break;
            }
            case 12:
            {
                InOrderTraverse(T, &visit);
                printf("\n");
                getchar();getchar();
                break;
            }
            case 13:
            {
                PostOrderTraverse(T, &visit);
                printf("\n");
                getchar();getchar();
                break;
            }
            case 14:
            {
                LevelOrderTraverse(T, &visit);
                printf("\n");
                getchar();getchar();
                break;
            }
            case 15:
            {
                int theMaxPathSum;
                if((theMaxPathSum = MaxPathSum(T))){
                    printf("最大路径和为 %d 。\n", theMaxPathSum);
                }
                else printf("树是空树！无法求最大路径和。\n");
                getchar();getchar();
                break;
            }
            case 16:
            {
                KeyType e1, e2;
                printf("请输入要查找公共祖先的两个结点关键字：\n");
                scanf("%d %d", &e1, &e2);
                BiTNode * LCA = LowestCommonAncestor(T, e1, e2);
                if(LCA) printf("公共祖先是:\n key : %d ;\n others : %s \n",LCA->data.key, LCA->data.others);
                else printf("查找公共祖先失败！\n");
                getchar();getchar();
                break;
            }
            case 17:
            {
                if(T==NULL) printf("树为空树！\n");
                InvertTree(T);
                printf("翻转二叉树成功！\n");
                getchar();getchar();
                break;
            }
            case 18:
            {
                printf("请输入要添加的二叉树名称：\n");
                char name[30];
                scanf("%s", name);
                if(LocateTree(Trees, name) >= 0){
                    printf("添加二叉树失败，名称不可以重复！\n");
                    getchar();getchar();
                    break;
                }
                else{
                    strcpy(Trees.elem[Trees.length].name,name);
                    Trees.elem[Trees.length++].T = NULL;
                    printf("添加二叉树 %s 成功！\n", name);
                    getchar();getchar();
                    break;
                }
            }
            case 19:
            {
                printf("请输入要移除的二叉树名称：\n");
                char name[30];
                scanf("%s", name);
                int RmNum = LocateTree(Trees, name);
                char nowName[30];
                strcpy(nowName, Trees.elem[idt].name);
                for (int i = RmNum; i < Trees.length; i++) {
                    Trees.elem[i] = Trees.elem[i+1];
                }
                Trees.length--;
                int newidt;
                if((newidt = LocateTree(Trees, nowName)) >= 0)
                    idt = newidt;
                else
                    idt++;
                getchar();getchar();
                break;
            }
            case 20:
            {
                char name[30];
                int j;
                printf("请输入要切换到的二叉树名称：\n");
                scanf("%s",name);
                if((j=LocateTree(Trees,name))>=0) idt = j;
                getchar();getchar();
                break;
            }
            case 21:
            {
                char filename[30];
                printf("请输入要保存到的文件的名称：\n");
                scanf("%s", filename);
                switch(SaveBiTree(T, filename)){
                    case INFEASIBLE:
                    {
                        printf("保存失败，树是空树！\n");
                        break;
                    }
                    case OK:
                    {
                        printf("保存成功！\n");
                        break;
                    }
                    case ERROR:
                    {
                        printf("保存失败，打开文件失败！\n");
                        break;
                    }
                }
                getchar();getchar();
                break;
            }
            case 22:
            {
                char filename[30];
                DestoryBiTree(T);
                printf("读取内容到当前二叉树！树已经被销毁！\n");
                printf("请输入要读取内容的文件的名称：\n");
                scanf("%s", filename);
                switch(LoadBiTree(T, filename)){
                    case INFEASIBLE:
                    {
                        printf("读取失败，树非空！\n");
                        break;
                    }
                    case OK:
                    {
                        printf("读取成功！\n");
                        break;
                    }
                    case ERROR:
                    {
                        printf("读取失败，打开文件失败！\n");
                        break;
                    }
                }
                getchar();getchar();
                break;
            }
            case 23:
            {
                printf("管理表中的树：");
                for(int i=0; i<Trees.length; i++){
                    printf("\tTree%d : %s",i,Trees.elem[i].name);
                }
                printf("\n");
                getchar();getchar();
                break;
            }
            default:
                break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n"); // op==0
}//end of main()
