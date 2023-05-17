#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "limits.h"
#include "stack"
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#undef OVERFLOW
#define OVERFLOW -2
#define max(i,j) ((i)>(j)?(i):(j))

typedef int status;
typedef int ElemType; //数据元素类型定义
typedef int ElemType;
typedef struct LNode{  //单链表（链式结构）结点的定义
    ElemType data;
    struct LNode *next;
}LNode,*LinkList;

typedef struct THELISTS{
    struct ALIST{
        char name[30] = {0,};
        LinkList L;
    }elem[50];
    int length = 0, listssize=50;
} LISTS;



// 1:初始化单链表，成功返回OK，失败返回OVERFLOW。
status InitList(LinkList &L)
{
    if(!L){
        L = (LNode*) malloc(sizeof(LNode));
        L->next = NULL;
        return OK;
    }
    else return INFEASIBLE;
}


// 2:如果单链表L存在，销毁单链表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
status DestroyList(LinkList &L)
{
    if(L){
        LNode *p = L, *q;
        while(p){
            q = p->next;
            free(p);
            p = q;
        }
        L = NULL;
        return OK;
    }
    else return INFEASIBLE;
}


//status ListTrabverse(SqList L){
//    int i;
//    printf("\n-----------all elements -----------------------\n");
//    for(i=0;i<L.length;i++) printf("%d ",L.elem[i]);
//    printf("\n------------------ end ------------------------\n");
//    return L.length;
//}


// 3:如果单链表L存在，删除单链表L中的所有元素，返回OK，否则返回INFEASIBLE。
status ClearList(LinkList &L)
{
    if(L){
        LNode *p = L->next,*q;
        while(p){
            q = p->next;
            free(p);
            p = q;
        }
        L->next = NULL;
        return OK;
    }
    else return INFEASIBLE;
}


// 4:如果单链表L存在，判断单链表L是否为空，空就返回TRUE，否则返回FALSE；如果单链表L不存在，返回INFEASIBLE。
status ListEmpty(LinkList L)
{
    if(L) {
        if(L->next) return FALSE;
        else return TRUE;
    }
    return INFEASIBLE;
}


// 5:如果单链表L存在，返回单链表L的长度，否则返回INFEASIBLE。
int ListLength(LinkList L)
{
    if(L) {
        LNode *p = L->next;
        int i=0;
        while(p){
            i++;
            p = p->next;
        }
        return i;
    }
    else return INFEASIBLE;
}


// 6:如果单链表L存在，获取单链表L的第i个元素，保存在e中，返回OK；
// 如果i不合法，返回ERROR；如果单链表L不存在，返回INFEASIBLE。
status GetElem(LinkList &L,int i,ElemType &e)
{
    if(L) {
        if(i<1) return ERROR;
        LNode *p = L->next;
        int j=0;
        while(p){
            j++;
            if(j==i) {e = p->data; return OK;}
            p = p->next;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}


//LocateElem()使用的compare()函数
int ElemEqual(ElemType i, ElemType j){
    return i==j;
}


//ListTraverse()使用的visit()函数
int TraversePrint(ElemType e){
    printf("%d ", e);
    return 0;
}


int NonExist(LinkList L){
    return L->next==NULL?1:0;
}


// 7:如果单链表L存在，查找元素e在单链表L中的位置序号；如果e不存在，返回ERROR；
// 当单链表L不存在时，返回INFEASIBLE。
status LocateElem(LinkList L,ElemType e, int (*compare)(ElemType,ElemType))
{
    if(L) {
        LNode *p = L->next;
        int j=0;
        while(p){
            j++;
            if(compare(p->data,e)) return j;
            p = p->next;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}


// 8:如果单链表L存在，获取单链表L中元素e的前驱，保存在pre中，返回OK；
// 如果没有前驱，返回ERROR；如果单链表L不存在，返回INFEASIBLE。
status PriorElem(LinkList L,ElemType e,ElemType &pre)
{
    if(L) {
        LNode *p = L->next;
        while(p&&p->next){
            if(p->next->data==e) {pre = p->data; return OK;}
            p = p->next;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}


// 9:如果单链表L存在，获取单链表L元素e的后继，保存在next中，返回OK；
// 如果没有后继，返回ERROR；如果单链表L不存在，返回INFEASIBLE。
status NextElem(LinkList L,ElemType e,ElemType &next)
{
    if(L) {
        LNode *p = L->next;
        while(p&&p->next){
            if(p->data==e) {next = p->next->data; return OK;}
            p = p->next;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}


// 10:如果单链表L存在，将元素e插入到单链表L的第i个元素之前，返回OK；
// 当插入位置不正确时，返回ERROR；如果单链表L不存在，返回INFEASIBLE。
status ListInsert(LinkList &L,int i,ElemType e)
{
    if(L) {
        if(i<1) return ERROR;
        LNode *p = L;
        int j=-1;
        while(p->next){
            j++;
            if(j==i-1) {
                LNode *q = (LNode*) malloc(sizeof(LNode));
                q->next = p->next;
                p->next = q;
                q->data = e;
                return OK;
            }
            p = p->next;
        }
        if(i==j+2){
            LNode *q = (LNode*) malloc(sizeof(LNode));
            q->next = NULL;
            p->next = q;
            q->data = e;
            return OK;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}

// 11:如果单链表L存在，删除单链表L的第i个元素，并保存在e中，返回OK；
// 当删除位置不正确时，返回ERROR；如果单链表L不存在，返回INFEASIBLE。
status ListDelete(LinkList &L,int i,ElemType &e)
{
    if(L) {
        if(i<1) return ERROR;
        LNode *p = L;
        int j=-1;
        while(p->next){
            j++;
            if(j==i-1) {
                e = p->next->data;
                LNode *q = p->next;
                p->next = q->next;
                free(q);
                return OK;
            }
            p = p->next;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}


// 12:如果单链表L存在，依次显示单链表中的元素，每个元素间空一格，返回OK；
// 如果单链表L不存在，返回INFEASIBLE。
status ListTraverse(LinkList L, int (*visit)(ElemType))
{
    if(L){
        LNode *p = L->next;
        printf("\n------------ all elements--------------\n");
        if(ListEmpty(L)) printf("\t\t\t单链表是空表。");
        while(p){
            visit(p->data);
            p = p->next;
        }
        printf("\n-----------end all elements------------\n");
        return OK;
    }
    else return INFEASIBLE;
}


// 13.链表翻转
status ReverseList(LinkList & L){
    if(L){
        if(ListLength(L)==0){
            printf("单链表是空表！\n");
            return OK;
        }
        std::stack<int> St;
        LNode * p = L->next;
        while (p) {
            St.push(p->data);
            p = p->next;
        }
        p = L->next;
        while (p) {
            p->data = St.top();
            St.pop();
            p = p->next;
        }
        return OK;
    }
    else return INFEASIBLE;
}

//
//void swap(auto &i,auto &j){
//    auto tmp = i;
//    i = j;
//    j = tmp;
//    return;
//}

// 15.链表排序
status SortList(LinkList & L){
    if(L){
        LNode * i = L->next, * j;
        while(i&&i->next){
            int iNextData = i->next->data;
            j = L;
            int flag = 1;
            while(j != i){
                if((j==L || j->data < iNextData) && j->next->data > iNextData){
                    LNode * k = (LNode*)malloc(sizeof(LNode));
                    k->data = iNextData;
                    k->next = j->next;
                    j->next = k;
                    k = i->next;
                    i->next = i->next->next;
                    flag = 0;
                    free(k);
                    break;
                }
                j = j->next;
            }
            if(flag) i = i->next;
        }
        return OK;
    }
    else return INFEASIBLE;
}

// 14.删除链表的倒数第k个结点
status RemoveNthFromEnd(LinkList & L, int n){
    if(L){
        int l, i=1, k;
        if(n > (l=ListLength(L)) || ListEmpty(L))return ERROR;
        k = l-n+1;
        LNode * p = L, * q;
        while(k != i){
            p = p->next;
            i++;
        }
        q = p->next;
        p->next = p->next->next;
        free(q);
        return OK;
    }
    else return INFEASIBLE;
}



// 16:如果单链表L存在，将单链表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
status SaveList(LinkList & L,const char FileName[])
{
    if(L){
        FILE *fp = fopen(FileName,"wb");
        if(fp == NULL){
            printf("打开文件失败\n");
            return ERROR;
        }
        LNode *p = L->next;
        while(p){
            fprintf(fp, " %d", p->data);
            p = p->next;
        }
        fclose(fp);
        return OK;
    }else return INFEASIBLE;
}


// 17:如果单链表L不存在，将FileName文件中的数据读入到单链表L中，返回OK，否则返回INFEASIBLE。
status LoadList(LinkList &L, const char FileName[])
{
    if(!L){
        FILE *fp = fopen(FileName,"rb");
        if(fp == NULL){
            printf("打开文件失败!\n");
            return ERROR;
        }
        L = (LinkList) malloc(sizeof(LNode));//empty head node.
        L->next = NULL;
        LNode *p = L;
        int k;
        while(!feof(fp)&&fscanf(fp,"%d",&k)){
            p->next = (LNode*)malloc(sizeof(LNode));
            if(!p->next) return ERROR;
            p = p->next;
            p->data = k;
        }
        p->next = NULL;
        fclose(fp);
        return OK;
    }
    else return INFEASIBLE;
}


// 22:Lists中查找一个名称为ListName的单链表
int LocateList(LISTS Lists, char ListName[])
{
    for(int i=0; i<Lists.length; i++) {
        if(strcmp(Lists.elem[i].name, ListName)==0) return i + 1;
    }
    return 0;
}



// 19:Lists中删除一个名称为ListName的单链表
status RemoveList(LISTS &Lists,char ListName[])
{
    int flag = 0;
    for(int i=0; i<Lists.length; i++){
        if(strcmp(ListName, Lists.elem[i].name) == 0){
            flag = 1;
            for(int j=i; j<Lists.length-1; j++){
                Lists.elem[j] = Lists.elem[j+1];
            }
            Lists.length--;
        }
    }
    if(flag == 0) return ERROR;
    else return OK;
}


// 18:Lists中添加一个名称为ListName的单链表
status AddList(LISTS &Lists,char ListName[])
{
    if(Lists.length == Lists.listssize) {
        printf("单链表数目达到最大值!\n");
        return ERROR;
    }
    if(LocateList(Lists, ListName)){
        printf("不允许表名重复！\n");
        return ERROR;
    }
    THELISTS::ALIST & list = Lists.elem[Lists.length];
    strcpy(list.name, ListName);
    list.L = NULL;
    Lists.length++;
    return OK;
}

// 22:DisplayList
status DisplayList(LISTS&Lists, char filename[]){
    int i;
    if(i=LocateList(Lists, filename)){
        printf("ListName:%s is the NO.%d list\n",Lists.elem[i-1].name,i);
        if(ListTraverse(Lists.elem[i-1].L, &TraversePrint)==OK);
        else printf("单链表未初始化！\n");
        return OK;
    }else return ERROR;
}

int main(void){
    int op=1, idt=1;
    LISTS Lists;
    for(int i = 0;i < 50;i++) Lists.elem[i].L = NULL;
//    LinkList *PtrL = &Lists.elem[idt-1].L;
    strcpy(Lists.elem[0].name, "Default: Lists.elem[0]");
    while(op){
        // system("clear");
        LinkList &L = Lists.elem[idt-1].L;//原来是[idt]，这也导致了打印不出名字
        printf("\n\n");
        printf("Menu for Linear Table On Single Linked Structure \n");
        printf("-------------------------------------------------\n");
        printf("          1. InitList       2. DestroyList\n");
        printf("          3. ClearList      4. ListEmpty\n");
        printf("          5. ListLength     6. GetElem \n");
        printf("          7. LocateElem     8. PriorElem\n");
        printf("          9. NextElem       10. ListInsert\n");
        printf("          11. ListDelete    12. ListTraverse\n");
        printf("          13. ReverseList   14. RemoveNthFromEnd\n");
        printf("          15. SortList      16. SaveList\n");
        printf("          17. LoadList      18. AddList\n");
        printf("          19. RemoveList    20. SwitchList\n");
        printf("          21. ShowLists     22. DisplayList\n");
        printf("          0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("\t活动单链表：%s\n", Lists.elem[idt-1].name);//原来是[idt]，这也导致了打印不出名字
        printf("    请选择你的操作[0~22]:\n");
        scanf("%d",&op);
        switch(op){
            case 1:
                if(InitList(L)==OK) printf("单链表创建成功！\n");
                else printf("单链表创建失败！单链表已存在\n");
                getchar();getchar();
                break;
            case 2:
                if(DestroyList(L)==OK) printf("单链表删除成功！\n");
                else printf("单链表删除失败！单链表不存在。\n");
                getchar();getchar();
                break;
            case 3:
                if(ClearList(L)==OK) printf("单链表清空成功！\n");
                else printf("单链表清空失败！单链表不存在。\n");
                getchar();getchar();
                break;
            case 4:
                int empty;
                if((empty = ListEmpty(L))!=INFEASIBLE)
                    if(empty) printf("单链表为空！\n");
                    else printf("单链表不为空！\n");
                    else printf("单链表判空失败！单链表不存在。\n");
                getchar();getchar();
                break;
            case 5:
                int len;
                if((len = ListLength(L))!=INFEASIBLE)
                    printf("单链表长度为: %d 。 \n", len);
                else printf("单链表求长度失败，单链表不存在！\n");
                getchar();getchar();
                break;
            case 6:
            {
                int i;
                ElemType e;
                printf("请输入查找的元素的逻辑序号（从1开始）：");
                scanf("%d",&i);
                if(GetElem(L, i, e)!=INFEASIBLE) printf("单链表的第 %d 个元素为 %d 。\n", i, e);
                else printf("单链表查找失败！\n");
                getchar();getchar();
            }
                break;
            case 7:
            {
                ElemType e;
                int i;
                printf("请输入查找的目标元素：");
                scanf("%d",&e);
                switch(i=LocateElem(L, e, &ElemEqual)){
                    case INFEASIBLE:
                        printf("查找元素失败，或单链表不存在！ \n");
                        break;
                    case ERROR:
                        printf("查找元素失败，单链表中不存在该元素! \n");
                        break;
                    default:
                        printf("元素 %d 第一次出现在第 %d 个元素上。\n", e, i);
                        break;
                }
                getchar();getchar();
                break;
            }
            case 8:
            {
                ElemType e,pre;
                printf("请输入要查找前驱的元素：");
                scanf("%d",&e);
                switch(PriorElem(L, e, pre)){
                    case OK:
                        printf("元素 %d 的前驱是 %d 。\n",e,pre);
                        break;
                    case ERROR:
                        printf("查找前驱元素失败，该元素不存在或不存在前驱！ \n");
                        break;
                    case INFEASIBLE:
                        printf("查找前驱元素失败，单链表不存在！ \n");
                        break;
                    default:
                        break;
                }
                getchar();getchar();
                break;
            }
            case 9:
            {
                ElemType e,ne;
                printf("请输入要查找后继的元素：");
                scanf("%d",&e);
                switch(NextElem(L, e, ne)){
                    case OK:
                        printf("元素 %d 的后继是 %d 。\n",e,ne);
                        break;
                    case ERROR:
                        printf("查找后继元素失败，该元素不存在或不存在后继！ \n");
                        break;
                    case INFEASIBLE:
                        printf("查找后继元素失败，单链表不存在！ \n");
                        break;
                    default:
                        break;
                }
                getchar();getchar();
                break;
            }
            case 10:
            {
                ElemType e;
                int j;
                printf("请输入插入的元素的逻辑序号（从1开始）：");
                scanf("%d",&j);
                printf("请输入插入的元素的值 ：");
                scanf("%d",&e);
                switch(ListInsert(L, j, e)){
                    case OK:
                        printf("单链表插入成功！\n");
                        break;
                    case ERROR:
                        printf("单链表插入失败，插入位置非法！\n");
                        break;
                    case INFEASIBLE:
                        printf("单链表插入失败，单链表不存在！");
                        break;
                    default:
                        break;
                }
                getchar();getchar();
                break;
            }
            case 11:{
                int i;
                ElemType e;
                printf("请输入要删除的元素的逻辑序号：");
                scanf("%d",&i);
                switch(ListDelete(L, i, e)){
                    case OK:
                        printf("被删除的元素是 %d 。\n", e);
                        break;
                    case ERROR:
                        printf("删除元素失败，指定的逻辑序号不合法！ \n");
                        break;
                    case INFEASIBLE:
                        printf("删除元素失败，单链表不存在！ \n");
                        break;
                    default:
                        break;
                }
                getchar();getchar();
                break;
            }
            case 12:
            {
                if(ListTraverse(L,&TraversePrint)==OK)
                    printf("\n遍历成功！\n");
                else
                    printf("遍历失败，单链表不存在！\n");
                getchar();getchar();
                break;
            }
            case 13:
            {
                if(ReverseList(L)==OK){
                    printf("链表翻转成功！\n");
                }
                else printf("链表翻转失败！单链表不存在。\n");
                getchar();getchar();
                break;
            }
            case 14:
            {
                int n;
                printf("删除倒数第n个元素！请输入n：");
                scanf("%d", &n);
                switch (RemoveNthFromEnd(L, n)) {
                    case OK:
                        printf("删除倒数第n个元素成功！\n");
                        break;
                        
                    case INFEASIBLE:
                        printf("删除倒数第n个元素失败！单链表不存在\n");
                        break;
                        
                    case ERROR:
                        printf("删除倒数第n个元素失败！单链表是空表，或者n不合法！\n");
                        break;
                        
                    default:
                        break;
                }
                getchar();getchar();
                break;
            }
            case 15:
            {
                if (SortList(L)!=INFEASIBLE) {
                    if(ListEmpty(L)) printf("单链表是空表。\n");
                    printf("排序操作成功！\n");
                }
                else printf("操作失败，单链表不存在！\n");
                getchar();getchar();
                break;
            }
            case 16:
            {
                char FileName[128];
                printf("请输入你要保存的文件名：\n");
                scanf("%s",FileName);
                switch(SaveList(L, FileName))
                {
                    case OK:
                        printf("保存单链表到文件操作成功！\n");
                        break;
                    case ERROR:
                        printf("操作失败，请检查文件名。\n");
                        break;
                    case INFEASIBLE:
                        printf("操作失败，单链表不存在！\n");
                        break;
                }
                getchar();getchar();
                break;
            }
            case 17:
            {
                char FileName[128];
                char ListName[30];
                //SqList Demo = {NULL,};
                printf("请输入你要读取的文件名：\n");
                scanf("%s",FileName);
                printf("请输入你要读取到的单链表名：\n");
                scanf("%s",ListName);
                int i = LocateList(Lists, ListName);
                if(i==0){
                    printf("不存在单链表%s!\n",ListName);
                    getchar();getchar();
                    break;
                }
                else
                    L = NULL;
                switch(LoadList(L, FileName))
                {
                    case OK:
                        ListTraverse(Lists.elem[i-1].L,&TraversePrint);
                        printf("从文件读取到单链表操作成功！\n");
                        break;
                    case ERROR:
                        printf("操作失败，请检查文件名。\n");
                        break;
                    case INFEASIBLE:
                        printf("操作失败，单链表已存在！\n");
                        break;
                }
                getchar();getchar();
                break;
            }
            case 18:
            {
                char SqListName[30];
                printf("请输入你要添加的单链表名：\n");
                scanf("%s",SqListName);
                switch (AddList(Lists, SqListName)) {
                    case OK:
                        printf("添加单链表成功！\n");
                        break;
                    case ERROR:
                        printf("添加单链表失败！\n");
                        break;
                    default:
                        break;
                }
                getchar();getchar();
                break;
            }
            case 19:
            {
                char SqListName[30];
                printf("请输入你要删除的单链表名：\n");
                scanf("%s",SqListName);
                int i;
                if(i=LocateList(Lists, SqListName)){
                    if(i<idt){
                        idt--;
                    }
                    else;
                }
                switch (RemoveList(Lists, SqListName)) {
                    case OK:
                        printf("删除单链表成功！\n");
                        break;
                    case ERROR:
                        printf("删除单链表失败！\n");
                        break;
                    default:
                        break;
                }
                getchar();getchar();
                break;
            }
            case 20:
            {
                char SqListName[30];
                printf("请输入你要切换到的单链表名：\n");
                scanf("%s",SqListName);
                switch ((idt=LocateList(Lists, SqListName))) {
                    case 0:
                        printf("切换单链表失败！\n");
                        break;
                    default:
//                        PtrL = &Lists.elem[idt-1].L;
                        printf("切换到单链表 %s 成功！\n", SqListName);
                        break;
                }
                getchar();getchar();
                break;
            }
            case 21:
            {
                printf("\n--------------Lists-------------\n");
                for (int i = 0; i<Lists.length; i++) {
                    printf("%s\n",Lists.elem[i].name);
                }
                printf("-------------EndLists------------\n");
                getchar();getchar();
                break;
            }
            case 22:
            {
                char name[30];
                printf("请输入要显示的表名：\n");
                scanf("%s",name);
                if(DisplayList(Lists, name)==OK){
                    printf("显示链表成功！\n");
                }else printf("显示链表失败，没有找到该链表。\n");
                getchar();getchar();
                break;
            }
            case 0:
                break;
            default:
                printf("输入的操作编号不合法！\n");
                break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
}//end of main()
