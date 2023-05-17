#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define ISEMPTY -3
#define LIST_INIT_SIZE 1000
#define LISTINCREMENT  10
#define max(i,j) ((i)>(j)?(i):(j))

typedef int status;
typedef int ElemType; //数据元素类型定义
typedef struct{  //顺序表（顺序结构）的定义
    ElemType * elem;
    int length;
    int listsize;
} SqList;

typedef struct THELISTS{  //顺序表的管理表定义
	 struct ALIST{
		 char name[30];
		 SqList L;
	  } elem[50];
	  int length = 0;
	  int listssize = 50;
 } LISTS;




// 1:初始化顺序表，如果分配空间失败，返回OVERFLOW；否则，返回OK。
status InitList(SqList& L){
	if(L.elem!=NULL) return INFEASIBLE;
	L.elem = (ElemType *)malloc( LIST_INIT_SIZE * sizeof (ElemType));
	if(!L.elem) exit(OVERFLOW);
	L.length=0;
	L.listsize=LIST_INIT_SIZE;
	return OK;
}


// 2:如果顺序表L存在，销毁顺序表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
status DestroyList(SqList& L)
{
    if(L.elem){
        free(L.elem);
        L.elem = NULL;
        L.length = 0;
        L.listsize = 0;
        return OK;
    }else return INFEASIBLE;
}


/*
status ListTrabverse(SqList L){
	int i;
	printf("\n-----------all elements -----------------------\n");
	for(i=0;i<L.length;i++) printf("%d ",L.elem[i]);
	printf("\n------------------ end ------------------------\n");
	return L.length;
}
*/


// 3:如果顺序表L存在，删除顺序表L中的所有元素，返回OK，否则返回INFEASIBLE。
status ClearList(SqList& L)
{
    if(L.elem){
        L.length = 0;
        return OK;
    }else return INFEASIBLE;
}


// 4:如果顺序表L存在，判断顺序表L是否为空，空就返回TRUE，否则返回FALSE；
// 如果顺序表L不存在，返回INFEASIBLE。
status ListEmpty(SqList L)      {
    if(L.elem){
        if(L.length == 0) return TRUE;
        else return FALSE;
    }
    else return INFEASIBLE;
}


// 5:如果顺序表L存在，返回顺序表L的长度，否则返回INFEASIBLE。
int ListLength(SqList L)
{
    if(L.elem){
        return L.length;
    }
    else return INFEASIBLE;
}


// 6:如果顺序表L存在，获取顺序表L的第i个元素，保存在e中，返回OK；
// 如果i不合法，返回ERROR；如果顺序表L不存在，返回INFEASIBLE。
status GetElem(SqList L,int i,ElemType &e)
{
    if(L.elem){
        if(i > L.length || i<=0) return ERROR;
        e = L.elem[i-1];
        return OK;
    }
    else return INFEASIBLE;
}


// 7:如果顺序表L存在，查找元素e在顺序表L中的位置序号并返回该序号；
// 如果e不存在，返回0；当顺序表L不存在时，返回INFEASIBLE（即-1）。
int LocateElem(SqList L,ElemType e,int (*compare)(ElemType,ElemType))
{
    if(L.elem){
        for(int i=1;i<=L.length;i++)
            if(compare(L.elem[i-1],e)) return i;
        return ERROR;
    }
    else return INFEASIBLE;
}


// 8:如果顺序表L存在，获取顺序表L中元素e的前驱，保存在pre中，返回OK；
// 如果没有前驱，返回ERROR；如果顺序表L不存在，返回INFEASIBLE。
status PriorElem(SqList L,ElemType e,ElemType &pre)
{
    if(L.elem){
        if(L.elem[0]==e) return ERROR;
        for(int i=2;i<=L.length;i++)
            if(L.elem[i-1]==e){
                pre = L.elem[i-2];
                return OK;
            }
        return ERROR;

    }
    else return INFEASIBLE;
}


// 9:如果顺序表L存在，获取顺序表L元素e的后继，保存在next中，返回OK；
// 如果没有后继，返回ERROR；如果顺序表L不存在，返回INFEASIBLE。
status NextElem(SqList L,ElemType e,ElemType &next)
{
    if(L.elem){
        //if(L.elem[0]==e) return ERROR;
        for(int i=1;i<=L.length;i++)
            if(L.elem[i-1]==e){
                if(i==L.length) return ERROR;
                next = L.elem[i];
                return OK;
            }
        return ERROR;
    }
    else return INFEASIBLE;
}


// 10:如果顺序表L存在，将元素e插入到顺序表L的第i个元素之前，返回OK；
// 当插入位置不正确时，返回ERROR；如果顺序表L不存在，返回INFEASIBLE。
status ListInsert(SqList &L,int i,ElemType e)
{
    if(L.elem){
        if(i <= 0 || i > 1 + L.length) return ERROR;
        if(L.length + 1 > L.listsize){
            L.elem = (ElemType*)realloc(L.elem, sizeof(int)*2*L.listsize);
            L.listsize <<= 1;
        }
        int j;
        for(j = L.length; j >= i; j--)
            L.elem[j] = L.elem[j-1];
        L.elem[j] = e;
        L.length++;
        return OK;

    }
    else return INFEASIBLE;
}


// 11:如果顺序表L存在，删除顺序表L的第i个元素，并保存在e中，返回OK；
// 当删除位置不正确时，返回ERROR；如果顺序表L不存在，返回INFEASIBLE。
status ListDelete(SqList &L,int i,ElemType &e)
{
	if (L.length==0) {
		return ISEMPTY;
	}
	if(L.elem){
		if(i <= 0 || i > L.length) return ERROR;
		int j;
		e = L.elem[i-1];
		for(j = i - 1; j < L.length - 1; j++)
			L.elem[j] = L.elem[j+1];
		L.length--;
		return OK;
	}
	else return INFEASIBLE;
}


// 12:如果顺序表L存在，依次显示顺序表中的元素，每个元素间空一格，返回OK；
// 如果顺序表L不存在，返回INFEASIBLE。
status ListTraverse(SqList L, int (*visit)(ElemType))
{
	if(L.length==0){
		printf("顺序表是空表！");
		return OK;
	}
	if(L.elem){
		printf("\n-----------all elements -----------------------\n");
		for(int j = 0; j < L.length; j++) {
			// printf("%d",L.elem[j]);
			//if(j < L.length - 1) putchar(' ');
			visit(L.elem[j]);
		}
		printf("\n----------------end ---------------------------\n");
		return OK;
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


// 判断顺序表是否存在，存在返回0，不存在返回1
int NonExist(SqList &L){
	return L.elem != NULL ? 0 : 1;
}

// 13:最大连续子数组和
long long int MaxSubArray(SqList &L){
	
	ElemType *arr = L.elem;
	long long maxSum = LLONG_MIN;
	for (int i = 0; i < L.length; i++) { 	//枚举左端点
		long long sum = 0;
		for (int j = i; j<L.length; j++) {	//枚举右端点
			sum += arr[j];
			maxSum = max(maxSum,sum);
		}
	}
	return maxSum;
}


// 14:求和为K的子数组个数。
long long int SubArrayNum(SqList &L, long long k){
	ElemType *arr = L.elem;
	long long cnt = 0;
	for (int i = 0; i < L.length; i++) { 	//枚举左端点
		long long sum = 0;
		for (int j = i; j<L.length; j++) {	//枚举右端点
			sum += arr[j];
			if(sum == k) cnt++;
		}
	}
	return cnt;
}


// 15:顺序表排序。
status SortList(SqList &L){
	if(L.elem){
		if(L.length==0) return OK;
		for(int i = 0; i<L.length; i++)
			for (int j = 0; j<L.length-i-1; j++) {
				if(L.elem[j]>L.elem[j+1])
				{
					ElemType tmp = L.elem[j];
					L.elem[j] = L.elem[j+1];
					L.elem[j+1] = tmp;
				}
			}
		return OK;
	}
	else return INFEASIBLE;
}


// 以下的功能函数未完成。
// 如果顺序表L存在，将顺序表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
status  SaveList(SqList L,char FileName[])
{
	if(L.elem) {
		FILE* fp;
		if((fp = fopen(FileName, "w"))== NULL){
			printf("打开文件失败");
			return ERROR;
		}
		fprintf(fp,"%d ", L.length);
		fprintf(fp,"%d ", L.listsize);
		for(int i=0; i<L.length; i++) {
			fprintf(fp, " %d", L.elem[i]);
		}
		fclose(fp);
		return OK;
	} else return INFEASIBLE;
}


// 如果顺序表L不存在，将FileName文件中的数据读入到顺序表L中，返回OK，否则返回INFEASIBLE。
status  LoadList(SqList &L,char FileName[])
{
	if(!L.elem) {
			L.elem = (ElemType*)malloc(LIST_INIT_SIZE*sizeof(ElemType));
			L.length = 0;
			L.listsize = LIST_INIT_SIZE;
			FILE* fp = fopen(FileName, "r");
			if(fp == NULL){
				printf("打开文件失败");
				return ERROR;
			}
			int i = 0;
			fscanf(fp,"%d%*d",&L.length);
			while(i < L.length && fscanf(fp, "%d", &L.elem[i++])==1) {
				continue;
			}
			fclose(fp);
			return OK;
		} else return INFEASIBLE;
}


// Lists中删除一个名称为ListName的顺序表
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


// Lists中添加一个名称为ListName的顺序表
status AddList(LISTS &Lists,char ListName[])
{
	if(Lists.length == Lists.listssize) {
		printf("顺序表数目达到最大值!\n");
		return ERROR;
	}
	THELISTS::ALIST & list = Lists.elem[Lists.length];
	strcpy(list.name, ListName);
	list.L.elem = (ElemType*) malloc(LIST_INIT_SIZE*sizeof(ElemType));
	if(list.L.elem==NULL) return ERROR;
	list.L.length = 0;
	list.L.listsize = LIST_INIT_SIZE;
	Lists.length++;
	return OK;
}


// Lists中查找一个名称为ListName的顺序表
int LocateList(LISTS Lists,char ListName[])
{
	for(int i=0; i<Lists.length; i++) {
		if(strcmp(Lists.elem[i].name, ListName)==0) return i + 1;
	}
	return 0;
}




int main(void){
//    SqList L = {NULL,};
	int op=20;
	LISTS Lists;
	int idt = 1;
	for(int i = 0;i<Lists.length;i++){
		Lists.elem[i].L.elem = NULL;
		Lists.elem[i].L.length = 0;
	}
	strcpy(Lists.elem[0].name, "default = [0]");
	Lists.elem[0].L.elem = NULL;
	SqList *PtrL = &Lists.elem[0].L;
    while(op){
        // system("clear");
		SqList & L = *PtrL;
        printf("\n\n");
		printf("	  Now working on List %s \n", Lists.elem[idt-1].name);
        printf("      Menu for Linear Table On Sequence Structure \n");
        printf("-------------------------------------------------\n");
        printf("          1. InitList       7. LocateElem\n");
        printf("          2. DestroyList    8. PriorElem\n");
        printf("          3. ClearList      9. NextElem \n");
        printf("          4. ListEmpty      10. ListInsert\n");
        printf("          5. ListLength     11. ListDelete\n");
        printf("          6. GetElem        12. ListTraverse\n");
		printf("          13. MaxSubArray   14. SubArrayNum \n");
		printf("          15. SortList      16. SaveList\n");
		printf("          17. LoadList      18. AddList\n");
		printf("          19. RemoveList    20. SwitchList\n");
		printf("          21. ShowLists     0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~21]:");
        scanf("%d",&op);
        switch(op){
           case 1:
                 if(InitList(L)==OK) printf("顺序表创建成功！\n");
                 else printf("顺序表创建失败！顺序表已经存在。\n");
                 getchar();getchar();
             break;
           case 2:
                if(DestroyList(L)==OK) printf("顺序表删除成功！\n");
                else printf("顺序表删除失败！顺序表不存在。\n");
                getchar();getchar();
             break;
           case 3:
                if(ClearList(L)==OK) printf("顺序表清空成功！\n");
                else printf("顺序表清空失败！顺序表不存在。\n");
                getchar();getchar();
             break;
           case 4:
                int empty;
                if((empty = ListEmpty(L))!=INFEASIBLE)
                    if(empty) printf("顺序表为空！\n");
                    else printf("顺序表不为空！\n");
                else printf("顺序表判空失败！顺序表不存在。\n");
                getchar();getchar();
             break;
           case 5:
                int len;
                if((len = ListLength(L))!=INFEASIBLE)
                    printf("顺序表长度为: %d 。 \n", len);
                else printf("顺序表求长度失败，顺序表不存在！\n");
                getchar();getchar();
             break;
           case 6:
            {
                int i;
                ElemType e;
                printf("请输入查找的元素的逻辑序号（从1开始）：");
                scanf("%d",&i);
                if(GetElem(L, i, e)!=INFEASIBLE) printf("顺序表的第 %d 个元素为 %d 。\n", i, e);
                else printf("顺序表查找失败！顺序表不存在。\n");
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
						printf("查找元素失败，顺序表不存在！ \n");
						break;
                    case ERROR:
						printf("查找元素失败，顺序表中不存在该元素! \n");
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
						printf("查找前驱元素失败，该元素不存在，或无前驱！ \n");
						break;
					case INFEASIBLE:
						printf("查找前驱元素失败，顺序表不存在！ \n");
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
						printf("查找后继元素失败，该元素不存在，或无后继！ \n");
						break;
					case INFEASIBLE:
						printf("查找后继元素失败，顺序表不存在！ \n");
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
						printf("顺序表插入成功！\n");
						break;
					case ERROR:
						printf("顺序表插入失败，插入位置非法！\n");
						break;
					case INFEASIBLE:
						printf("顺序表插入失败，顺序表不存在！");
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
					   printf("删除元素失败，顺序表不存在！ \n");
					   break;
				   case ISEMPTY:
					   printf("删除元素失败，顺序表是空表！ \n");
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
					printf("遍历失败，顺序表不存在！\n");
				getchar();getchar();
				break;
			}
			case 13:
			{
				if(ListEmpty(L)==TRUE)
				{
					printf("操作失败，顺序表为空！\n");
					getchar();getchar();
					break;
				}
				if(NonExist(L)){
					printf("操作失败，线性表不存在！\n");
					getchar();getchar();
					break;
				}
				printf("最大子数组和为 %lld 。", MaxSubArray(L));
				getchar();getchar();
				break;
			}
			case 14:
			{
				if(ListEmpty(L)==TRUE)
				{
					printf("操作失败，顺序表为空！\n");
					getchar();getchar();
					break;
				}
				if(NonExist(L)){
					printf("操作失败，线性表不存在！\n");
					getchar();getchar();
					break;
				}
				long long int k;
				printf("请输入子数组和K：");
				scanf("%lld",&k);
				printf("子数组和为 %lld 的个数为 %lld 。", k, SubArrayNum(L,k));
				getchar();getchar();
				break;
			}
		  case 15:
			{
				if (SortList(L)!=INFEASIBLE) {
					if(ListEmpty(L)) printf("顺序表是空表。\n");
					printf("排序操作成功！\n");
				}
				else printf("操作失败，顺序表不存在！\n");
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
						printf("保存顺序表到文件操作成功！\n");
						break;
					case ERROR:
						printf("操作失败，请检查文件名。\n");
						break;
					case INFEASIBLE:
						printf("操作失败，顺序表不存在！\n");
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
				printf("请输入你要读取到的顺序表名：\n");
				scanf("%s",ListName);
				int i = LocateList(Lists, ListName);
				if(i==0){
					printf("不存在顺序表%s!\n",ListName);
				}
				else
					Lists.elem[i-1].L.elem = NULL;
					switch(LoadList(Lists.elem[i-1].L, FileName))
					{
						case OK:
							ListTraverse(Lists.elem[i-1].L,&TraversePrint);
							printf("从文件读取到顺序表操作成功！\n");
							break;
						case ERROR:
							printf("操作失败，请检查文件名。\n");
							break;
						case INFEASIBLE:
							printf("操作失败，顺序表已存在！\n");
							break;
					}
				getchar();getchar();
				break;
			}
		  case 18:
			{
				char SqListName[30];
				printf("请输入你要添加的顺序表名：\n");
				scanf("%s",SqListName);
				if(LocateList(Lists, SqListName)){
					printf("添加顺序表失败,名称重复！\n");
					break;
				}
				switch (AddList(Lists, SqListName)) {
					case OK:
						printf("添加顺序表成功！\n");
						break;
					case ERROR:
						printf("添加顺序表失败！\n");
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
				  printf("请输入你要删除的顺序表名：\n");
				  scanf("%s",SqListName);
				  switch (RemoveList(Lists, SqListName)) {
					  case OK:
						  printf("删除顺序表成功！\n");
						  break;
					  case ERROR:
						  printf("删除顺序表失败！\n");
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
				  printf("请输入你要切换到的顺序表名：\n");
				  scanf("%s",SqListName);
				  switch ((idt=LocateList(Lists, SqListName))) {
					  case 0:
						  printf("切换顺序表失败！\n");
						  break;
					  default:
						  PtrL = &Lists.elem[idt-1].L;
						  printf("切换到顺序表 %s 成功！\n", SqListName);
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
		  case 0:
				break;
		  default:
				printf("操作编号不正确！\n");
				getchar();getchar();
				break;
        }//end of switch
      }//end of while
    printf("欢迎下次再使用本系统！\n");
}//end of main()
