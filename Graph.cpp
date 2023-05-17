//
//  main.cpp
//  图
//
//  Created by xinyi liu on 2023/5/10.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <queue>
#include <cmath>
#include <limits.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#undef OVERFLOW
#define OVERFLOW -2
#define MAX_VERTEX_NUM 200

typedef int status;
typedef int KeyType;
typedef enum {DG,DN,UDG,UDN} GraphKind;
typedef struct {
    KeyType  key;
    char others[128];
} VertexType; //顶点类型定义
typedef struct ArcNode {            //表顶点类型定义
    
    int adjvex;                     //顶点位置编号
    struct ArcNode * nextarc;       //下一个表顶点指针
} ArcNode;
typedef struct VNode {              //头顶点及其数组类型定义
    VertexType data;                //顶点信息
    ArcNode * firstarc;             //指向第一条弧
} VNode, AdjList[MAX_VERTEX_NUM];
typedef  struct Graph {             //邻接表的类型定义
    AdjList vertices;               //头顶点数组
    int vexnum=0,arcnum=0;          //顶点数、弧数
    GraphKind  kind = UDG;          //图的类型
} ALGraph;

typedef struct GRAPHS{
    struct AGRAPH{
        ALGraph G;
        char name[30];
    }elem[16];
    int length = 0;
} GRAPHS;

/*
 根据u在图G中查找顶点，查找成功返回位序，否则返回-1
 */
int LocateVex(ALGraph G, KeyType u){
    for(int i=0; i<G.vexnum; i++){
        if(G.vertices[i].data.key == u) return i;
    }
    return -1;
}

/*
根据u在图G中查找顶点，查找成功将该顶点值修改成value，返回OK；
如果查找失败或关键字不唯一，返回ERROR
*/
status PutVex(ALGraph &G,KeyType u,VertexType value)
{
    int i = LocateVex(G, u);
    if(i == -1) return ERROR;
    for(int j=0; j<G.vexnum; j++)
    {
        if(j==i) continue;
        if(G.vertices[j].data.key == value.key) return ERROR;
    }
    G.vertices[i].data = value;
    return OK;
}

/* 根据u在图G中查找顶点，查找成功返回顶点u的第一邻接顶点位序，否则返回-1 */
int FirstAdjVex(ALGraph G,KeyType u)
{
    int i = LocateVex(G, u);
    if(i==-1) return -1;
    if(G.vertices[i].firstarc==NULL) return -1;
    return G.vertices[i].firstarc->adjvex;
}

/* v对应G的一个顶点,w对应v的邻接顶点；操作结果是返回v的（相对于w）下一个邻接顶点的位序；如果w是最后一个邻接顶点，或v、w对应顶点不存在，则返回-1。 */
int NextAdjVex(ALGraph G,KeyType v,KeyType w)
{
    int i = LocateVex(G, v);
    int j = LocateVex(G, w);
    if(i<0||j<0) return -1;
    ArcNode * p = G.vertices[i].firstarc;
    while (p) {
        if(G.vertices[p->adjvex].data.key == w) break;
        p = p->nextarc;
    }
    if(!p||!p->nextarc) return -1;
    return p->nextarc->adjvex;
}

/*在图G中插入顶点v，成功返回OK,否则返回ERROR*/
status InsertVex(ALGraph &G,VertexType v)
{
    if(G.vexnum+1>MAX_VERTEX_NUM) return ERROR;
    if(LocateVex(G, v.key)>=0) return ERROR;
    G.vertices[G.vexnum].data = v;
    G.vertices[G.vexnum].firstarc = NULL;
    G.vexnum++;
    return OK;
}

/* 在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR */
status DeleteVex(ALGraph &G,KeyType v)
{
    int i = LocateVex(G, v);
    if(i==-1) return ERROR;
    if(G.vexnum==1) return ERROR;
    ArcNode * p = G.vertices[i].firstarc;
    while(p){
        ArcNode * q = G.vertices[p->adjvex].firstarc;
        if(!q) return ERROR;
        if(q->adjvex==i){
            G.vertices[p->adjvex].firstarc = G.vertices[p->adjvex].firstarc->nextarc;
            free(q);
        }else
            while (q){
                if(q->nextarc&&q->nextarc->adjvex == i){
                    ArcNode * s = q->nextarc;
                    q->nextarc = q->nextarc->nextarc;
                    free(s);
                }
                q = q->nextarc;
            }
        q = p;
        p = p->nextarc;
        free(q);
        G.arcnum--;
    }
    for(int j=i; j<G.vexnum-1; j++) G.vertices[j] = G.vertices[j+1];
    G.vexnum--;
    for(int j=0; j<G.vexnum; j++){
        ArcNode * p = G.vertices[j].firstarc;
        while (p) {
            if(p->adjvex>i) p->adjvex--;
            p = p->nextarc;
        }
    }
    return OK;
}

/*在图G中增加弧<v,w>，成功返回OK,否则返回ERROR*/
status InsertArc(ALGraph &G,KeyType v,KeyType w)
{
    int i = LocateVex(G, v), j = LocateVex(G, w);
    if(i<0||j<0) return ERROR;
    
    ArcNode * p = G.vertices[i].firstarc;
    while (p) {
        if(p->adjvex == j) return ERROR;
        p = p->nextarc;
    }
    
    p = (ArcNode * )malloc(sizeof(ArcNode));
    p->adjvex = j;
    p->nextarc = G.vertices[i].firstarc;
    G.vertices[i].firstarc = p;
    
    p = (ArcNode * )malloc(sizeof(ArcNode));
    p->adjvex = i;
    p->nextarc = G.vertices[j].firstarc;
    G.vertices[j].firstarc = p;
    
    G.arcnum++;
    return OK;
}

/*在图G中删除弧<v,w>，成功返回OK,否则返回ERROR*/
status DeleteArc(ALGraph &G,KeyType v,KeyType w)
{
    int i = LocateVex(G, v), j = LocateVex(G, w);
    if(i<0||j<0) return ERROR;
    int flag1 = 0, flag2 = 0;
    ArcNode * p = G.vertices[i].firstarc;
    if(G.vertices[i].firstarc->adjvex == j){
        ArcNode * s = G.vertices[i].firstarc;
        G.vertices[i].firstarc = G.vertices[i].firstarc->nextarc;
        free(s);
        flag1 = 1;
    }
    else while(p) {
        if(p->nextarc && p->nextarc->adjvex == j) {
            ArcNode * s = p->nextarc;
            p->nextarc = p->nextarc->nextarc;
            free(s);
            flag1 = 1;
        }
        p = p->nextarc;
    }
    
    p = G.vertices[j].firstarc;
    if(G.vertices[j].firstarc->adjvex == i) {
        ArcNode * s = G.vertices[j].firstarc;
        G.vertices[j].firstarc = G.vertices[j].firstarc->nextarc;
        free(s);
        flag2 = 1;
    }else while(p){
        if(p->nextarc && p->nextarc->adjvex == i) {
            ArcNode * s = p->nextarc;
            p->nextarc = p->nextarc->nextarc;
            free(s);
            flag2 = 1;
        }
        p = p->nextarc;
    }
    
    G.arcnum--;
    if(flag1&&flag2) return OK;
    else return ERROR;
}

void visit(VertexType v) {
    printf("(%d, %s)", v.key, v.others);
}

void VoidVisit(VertexType v) {
    return;
}


void dfs(ALGraph &G, int record[], int i, void (*visit)(VertexType)){
    visit(G.vertices[i].data);
    record[i] = 1;
    ArcNode * p = G.vertices[i].firstarc;
    while(p){
        if(record[p->adjvex]==0){
            dfs(G,record,p->adjvex,visit);
        }
        p = p->nextarc;
    }
}

/*对图G进行深度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次*/
status DFSTraverse(ALGraph &G, void (*visit)(VertexType))
{
    int record[MAX_VERTEX_NUM];
    memset(record, 0, sizeof(record));
    int i=0;
    while(i<G.vexnum){
        if(record[i]==0) {dfs(G, record, i, visit);printf("\n");}
        i++;
    }
    return OK;
}

int Degree(VNode v){
    ArcNode * p = v.firstarc;
    int k = 0;
    while (p) {
        k++;
        p = p->nextarc;
    }
    return k;
}

/*对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次*/
status BFSTraverse(ALGraph &G,void (*visit)(VertexType))
{
    int record[MAX_VERTEX_NUM];
    memset(record, 0, sizeof(record));
    std::queue<int> q;
    for(int i=0; i<G.vexnum; i++){
        if(record[i]==0){
            q.push(i); // start
            record[i] = 1;
            while(!q.empty()) {
                visit(G.vertices[q.front()].data);
                ArcNode * p = G.vertices[q.front()].firstarc;
                q.pop();
                while (p) {
                    if(!record[p->adjvex]) q.push(p->adjvex);
                    record[p->adjvex] = 1;
                    p = p->nextarc;
                }
            }
        }
        else;
    }
    return 0;
}



/*将图的数据写入到文件FileName中*/
status SaveGraph(ALGraph G, char FileName[])
{
    FILE * fp = fopen(FileName, "w");
    if(!fp) return ERROR;
    fprintf(fp, "%d %d\n", G.vexnum, G.arcnum);
    for(int i=0; i<G.vexnum; i++){
        fprintf(fp, "%d %s\n", G.vertices[i].data.key, G.vertices[i].data.others);
        fprintf(fp, "%d ", Degree(G.vertices[i]));
        ArcNode * p = G.vertices[i].firstarc;
        while (p) {
            fprintf(fp, "%d ", p->adjvex);
            p = p->nextarc;
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return OK;
}



/*读入文件FileName的图数据，创建图的邻接表*/
status LoadGraph(ALGraph &G, char FileName[])
{
    FILE * fp = fopen(FileName, "r");
    if(!fp) return ERROR;
    fscanf(fp, "%d %d", &G.vexnum, &G.arcnum);
    for(int i=0; i<G.vexnum; i++){
        fscanf(fp, "%d %s", &G.vertices[i].data.key, G.vertices[i].data.others);
        int d;
        fscanf(fp, "%d", &d);
        if(d==0) {G.vertices[i].firstarc = NULL; continue;}
        else{
            G.vertices[i].firstarc = (ArcNode*) malloc(sizeof(ArcNode));
            fscanf(fp, "%d", &G.vertices[i].firstarc->adjvex);
            ArcNode * p = G.vertices[i].firstarc;
            for(int j=1; j<d; j++){
                p->nextarc = (ArcNode*) malloc(sizeof(ArcNode));
                fscanf(fp, "%d", &p->nextarc->adjvex);
                p = p->nextarc;
            }
            p->nextarc = NULL;
        }
    }
    fclose(fp);
    return OK;
}





/*
 根据V和VR构造图T并返回OK，如果V和VR不正确，返回ERROR
 如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务
 */
status CreateGraph(ALGraph &G, VertexType V[], KeyType VR[][2]) {
    if(V[0].key==-1) return ERROR;
    for(int i=0; V[i].key != -1; i++)
        for (int j=i+1; V[j].key != -1; j++)
            if(V[i].key == V[j].key) return ERROR;
    memset(&G.vertices, 0, sizeof(G.vertices));
    G.kind = UDG;
    G.arcnum = 0;
    G.vexnum = 0;
    int i=0;
    while(V[i].key != -1) {
        G.vertices[i].data = V[i]; G.vexnum++; i++; //don't use "G.vertices[i].data = V[i++]; G.vexnum++; "
    }
    if(G.vexnum>MAX_VERTEX_NUM) return ERROR;
    i=0;
    while(VR[i][0] != -1) {
        G.arcnum++;
        int k=0, j=0;
        while(G.vertices[j].data.key != VR[i][0]){
            j++;
            if(j >= G.vexnum){
                return ERROR;
            }
        }
        while(G.vertices[k].data.key != VR[i][1]){
            k++;
            if(k >= G.vexnum){
                return ERROR;
            }
        }
        if(G.vertices[j].firstarc != NULL){
            ArcNode * p;
            p = (ArcNode *) malloc(sizeof(ArcNode));
            p->adjvex = k;
            p->nextarc = G.vertices[j].firstarc;
            G.vertices[j].firstarc = p;
        }
        else {
            G.vertices[j].firstarc = (ArcNode * ) malloc(sizeof(ArcNode));
            G.vertices[j].firstarc->adjvex = k;
            G.vertices[j].firstarc->nextarc = NULL;
        }
        
        if(G.vertices[k].firstarc != NULL){
            ArcNode * p;
            p = (ArcNode *) malloc(sizeof(ArcNode));
            p->adjvex = j;
            p->nextarc = G.vertices[k].firstarc;
            G.vertices[k].firstarc = p;
        }
        else {
            G.vertices[k].firstarc = (ArcNode * ) malloc(sizeof(ArcNode));
            G.vertices[k].firstarc->adjvex = j;
            G.vertices[k].firstarc->nextarc = NULL;
        }
        i++;
    }
    return OK;
}

/*销毁无向图G,删除G的全部顶点和边*/
status DestroyGraph(ALGraph &G) {
    for(int i=0; i<G.vexnum; i++) {
        ArcNode * p = G.vertices[i].firstarc;
        while(p){
            ArcNode * q = p;
            p = p->nextarc;
            free(q);
        }
    }
    memset(&G, 0, sizeof(G));
    return OK;
}

int LocateGraph(GRAPHS Graphs, char name[]) {
    int k = 0;
    while(k<Graphs.length){
        if(strcmp(Graphs.elem[k].name, name)==0) return k;
        k++;
    }
    return -1;
}


unsigned int Targetdfs(ALGraph &G, int record[], int i, int target){
    record[i] = 1;
    ArcNode * p = G.vertices[i].firstarc;
    unsigned int min = UINT_MAX;
    while(p){
        if(record[p->adjvex]==0){
            if(p->adjvex == target) return 1;
            else min = fmin(Targetdfs(G,record,p->adjvex,target), min);
        }
        p = p->nextarc;
    }
    record[i] = 0;
    return min==UINT_MAX? min : min + 1;
}


unsigned int ShortestPathLength(ALGraph G, KeyType u,KeyType v){
    int i = LocateVex(G, u);
    int j = LocateVex(G, v);
    int record[MAX_VERTEX_NUM];
    memset(record, 0, sizeof(record));
    
    if(u==v) return 0;
    return Targetdfs(G, record, i, j);
}

int ConnectedComponetsNums(ALGraph G){
    int record[MAX_VERTEX_NUM];
    int k = 0;
    memset(record, 0, sizeof(record));
    int i = 0;
    while(i<G.vexnum){
        if(record[i]==0) {dfs(G, record, i, VoidVisit); k++;}
        i++;
    }
    return k;
}

std::vector<VertexType> VerticesSetLessThanK(ALGraph G, KeyType u, unsigned int k)
{
    std::vector<VertexType> vec;
    for(int i=0; i<G.vexnum; i++){
        if(G.vertices[i].data.key==u) continue;
        if(ShortestPathLength(G,u,G.vertices[i].data.key) < k) {//操作结果是返回与顶点v距离**小于**k的顶点集合
            vec.push_back(G.vertices[i].data);
        }
    }
    return vec;
}

int main(void)
{
    for(int i=0;i<5;i++) std::cout<<std::endl;
    std::cout<<"****************图****实****验****************"<<std::endl;
    for(int i=0;i<5;i++) std::cout<<std::endl;
    int op=1, identy=0;
    GRAPHS Graphs;
    while(op){
        if(Graphs.length==0) {
            printf("图管理表为空，请先添加无向图！\n");
            op = 17;
        }
        else
        {
            printf("\n\n");
            printf("            Menu for Graph                       \n");
            printf("-------------------------------------------------\n");
            printf("基础功能：\n");
            printf("1.  CreatGraph               2.  DestroyGraph\n");
            printf("3.  LocateVex                4.  PutVex\n");
            printf("5.  FirstAdjVex              6.  NextAdjVex \n");
            printf("7.  InsertVex                8.  DeleteVex\n");
            printf("9.  InsertArc                10. DeleteArc\n");
            printf("11. DFSTraverse              12. BFSTraverse\n");
            printf("附加功能：\n");
            printf("13. VerticesSetLessThanK     14. ShortestPathLength\n");
            printf("15. ConnectedComponentsNums  \n");
            printf("多无向图管理：\n");
            printf("16. RemoveGraph              17. AddGraph\n");
            printf("18. SwitchGraph              19. LocateGraph\n");
            printf("20. SaveGraphToFile          21. LoadGraphFromFile(DestroyGraph will be called)\n");
            printf("22. ShowGraphs               \n退出：\n0.  Exit\n");
            printf("-------------------------------------------------\n");
            printf("    活动无向图：%s\n", Graphs.elem[identy].name);
            printf("    请选择你的操作[0~22]:\n");
            scanf("%d",&op);
        }
        ALGraph & G = Graphs.elem[identy].G;
        switch (op) {
            case 1:{
                int vexnum=0, arcnum=0;
                VertexType V[MAX_VERTEX_NUM];
                KeyType Arcs[MAX_VERTEX_NUM][2];
                if(G.vexnum) {printf("活动无向图已有顶点！\n");getchar();getchar();break;} // quit switch
                /* VERTICES */
                printf("请输入顶点定义：\n");
                while(1){
                    scanf("%d %s", &V[vexnum].key, V[vexnum].others);
                    if(V[vexnum].key == -1) break;
                    vexnum++;
                }
                /* ARCS */
                printf("请输入边定义：\n");
                while(1){
                    scanf("%d %d", &Arcs[arcnum][0], &Arcs[arcnum][1]);
                    if(Arcs[arcnum][0] == -1) break;
                    arcnum++;
                }
                if(vexnum==0) printf("图中无顶点！\n");
                /* END */
                switch(CreateGraph(G, V, Arcs)){
                    case ERROR:
                    {
                        printf("创建无向图失败，请检查数据。\n");
                        break;
                    }
                    case OK:
                    {
                        printf("创建无向图成功\n");
                        break;
                    }
                }
                getchar();getchar();
                break;
            }
            case 2:{
                DestroyGraph(G);
                printf("图销毁成功！\n");
                getchar();getchar();
                break;
            }
            case 3:{
                KeyType u;
                printf("请输入要查找的关键字：\n");
                scanf("%d", &u);
                int i = LocateVex(G, u);
                if(i<0){
                    printf("未能查找到顶点！\n");
                }else{
                    printf("关键字%d的位序是%d，内容是%s\n",u,i,G.vertices[i].data.others);
                }
                getchar();getchar();
                break;
            }
            case 4:{
                KeyType u;
                VertexType value;
                printf("请输入要赋值的顶点关键字：\n");
                scanf("%d", &u);
                printf("请输入要赋值关键字：\n");
                scanf("%d", &value.key);
                printf("请输入要赋值内容：\n");
                scanf("%s", value.others);
                if(PutVex(G, u, value)==ERROR){
                    printf("赋值失败！\n");
                }else{
                    printf("赋值成功！\n");
                }
                getchar();getchar();
                break;
            }
            case 5:{
                KeyType u;
                printf("请输入目标关键字：\n");
                scanf("%d", &u);
                int i = FirstAdjVex(G, u);
                if(i==-1) printf("查找失败！\n");
                else printf("关键字%d的第一邻接顶点是key:%d,others:%s\n",u,G.vertices[i].data.key,G.vertices[i].data.others);
                getchar();getchar();
                break;
            }
            case 6:{
                KeyType v,w;
                printf("查找v的邻接顶点中w的下一邻接点，请输入v，w:\n");
                scanf("%d %d",&v,&w);
                int i = NextAdjVex(G, v, w);
                if(i==-1) printf("查找失败！\n");
                else printf("关键字%d的下一邻接顶点是key:%d,others:%s\n",w,G.vertices[i].data.key,G.vertices[i].data.others);
                getchar();getchar();
                break;
            }
            case 7:{
                VertexType v;
                printf("请输入要插入的顶点关键字：\n");
                scanf("%d",&v.key);
                printf("请输入要插入的顶点内容：\n");
                scanf("%s",v.others);
                if(InsertVex(G, v) == OK){
                    printf("插入顶点成功！\n");
                }
                else
                {
                    printf("插入顶点失败！\n");
                }
                getchar();getchar();
                break;
            }
            case 8:{
                int key;
                printf("请输入要删除的顶点关键字：\n");
                scanf("%d",&key);
                if(DeleteVex(G, key) == OK){
                    printf("删除顶点成功！\n");
                }
                else
                {
                    printf("删除顶点失败！\n");
                }
                getchar();getchar();
                break;
            }
            case 9:{
                KeyType v,w;
                printf("插入边<v,w>，请输入v，w:\n");
                scanf("%d %d",&v,&w);
                if(InsertArc(G, v, w)==OK){
                    printf("插入边成功！\n");
                }else{
                    printf("插入边失败！\n");
                }
                getchar();getchar();
                break;
            }
            case 10:{
                KeyType v,w;
                printf("删除边<v,w>，请输入v，w:\n");
                scanf("%d %d",&v,&w);
                if(DeleteArc(G, v, w)==OK){
                    printf("删除边成功！\n");
                }else{
                    printf("删除边失败！\n");
                }
                getchar();getchar();
                break;
            }
            case 11:{
                DFSTraverse(G, &visit);
                getchar();getchar();
                break;
            }
            case 12:{
                BFSTraverse(G, &visit);
                getchar();getchar();
                break;
            }
            case 13:{
                int u,k;
                printf("请输入关键字和距离：\n");
                scanf("%d %d", &u, &k);
                if(LocateVex(G, u)==-1) {printf("操作失败！\n"); getchar();getchar();break;}
                std::vector<VertexType> set = VerticesSetLessThanK(G, u, k);
                for(auto p : set){
                    visit(p);
                }
                getchar();getchar();
                break;
            }
            case 14:{
                int u,v;
                printf("请输入关键字u,v：\n");
                scanf("%d %d", &u, &v);
                if(LocateVex(G, u)==-1 || LocateVex(G, v)==-1){
                    printf("求距离失败！\n");
                    getchar();
                    getchar();
                    break;
                }
                unsigned int spl = ShortestPathLength(G, u, v);
                if(spl == UINT_MAX) printf("INFINITE\n");
                else printf("\n%d\n",spl);
                getchar();getchar();
                break;
            }
            case 15:{
                int ccn = ConnectedComponetsNums(G);
                printf("\n%d\n", ccn);
                getchar();getchar();
                break;
            }
        
            case 16:{
                printf("请输入要移除的图名称：\n");
                char name[30];
                scanf("%s", name);
                int RmNum = LocateGraph(Graphs, name);
                if(RmNum<0) {
                    printf("找不到图%s",name);
                    getchar();getchar();
                    break;
                }
                char nowName[30];
                strcpy(nowName, Graphs.elem[identy].name);
                for (int i = RmNum; i < Graphs.length; i++) {
                    Graphs.elem[i] = Graphs.elem[i+1];
                }
                Graphs.length--;
                int newidenty;
                if((newidenty = LocateGraph(Graphs, nowName)) >= 0)
                    identy = newidenty;
                else
                    /*Do Nothing.*/;
                getchar();getchar();
                break;
            }
            case 17:{
                char name[20];
                printf("请输入要添加的图名称：\n");
                scanf("%s", name);
                if(LocateGraph(Graphs, name)!=-1) {
                    printf("无向图的名称不能重复！添加失败。\n");
                    getchar();getchar();
                    break;
                }else{
                    strcpy(Graphs.elem[Graphs.length].name, name);
                    Graphs.length++;
                    printf("添加图 %s 成功！\n", Graphs.elem[Graphs.length-1].name);
                    getchar();getchar();
                    break;
                }
            }
            case 18:{
                char name[30];
                int j;
                printf("请输入要切换的图名称：\n");
                scanf("%s",name);
                if((j=LocateGraph(Graphs,name))>=0) {
                    identy = j;
                    printf("切换成功！\n");
                }
                else{
                    printf("切换失败！\n");
                }
                getchar();getchar();
                break;
            }
            case 19:{
                char name[30];
                int j;
                printf("请输入要查找的图名称：\n");
                scanf("%s",name);
                if((j=LocateGraph(Graphs,name))>=0){
                    printf("图%s在第%d位，有%d个顶点，%d条边\n",name,j,Graphs.elem[j].G.vexnum,Graphs.elem[j].G.arcnum);
                }else{
                    printf("查找失败！\n");
                }
                getchar();getchar();
                break;
            }
            case 20:{
                char name[128];
                printf("保存到文件，请输入文件名：\n");
                scanf("%s",name);
                if(SaveGraph(G, name)==OK)
                {
                    printf("\n保存成功！\n");
                }
                else
                {
                    printf("\n保存失败！\n");
                }
                getchar();getchar();
                break;
            }
            case 21:{
                char name[128];
                printf("从文件加载，请输入文件名：\n");
                scanf("%s",name);
                if(LoadGraph(G, name)==OK)
                {
                    printf("\n加载成功！\n");
                }
                else
                {
                    printf("\n加载失败！\n");
                }
                getchar();getchar();
                break;
            }
            case 22:
            {
                printf("管理表中的图：");
                for(int i=0; i<Graphs.length; i++){
                    printf("\tGraph%d : %s",i,Graphs.elem[i].name);
                }
                printf("\n");
                getchar();getchar();
                break;
            }
            default:
                break;
        } // end switch
    } // end while
    printf("欢迎下次再使用本系统！\n"); // op==0
}
