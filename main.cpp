#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "commonDefine.h"

using namespace std;

/* 注意：
 * 位置从 1 开始
 * 下标从 0 开始（一般默认从0下标开始存储）
 */

/* ----------- 顺序表初始化 ---------------*/
Status InitList_Sq(SqList &L) {
    //分配存储空间
    L.elem = (ElemType *) malloc(sizeof(ElemType) * LIST_INIT_SIZE);
    if (!L.elem) {   //分配失败，退出
        exit(OVERFLOW);
    }
    L.length = 0;
    L.listsize = LIST_INIT_SIZE;

    return OK;
}

/* ----------- 顺序表插入 ---------------*/
//在第i个位置之前插入，插入后成为第i个元素。i∈[1,L.length+1]
//插入前需要先后移元素（[i-1,L.length-1]==>[i,L.length]），空出插入位置
//注意：插入时需要检查存储空间是否满了
Status ListInsert_Sq(SqList &L, int i, ElemType e) {
    //插入位置合法性检查
    if (i < 1 || i > L.length + 1) {
        return ERROR;
    }
    //存储空间检查（是否满了需要扩容）
    if (L.length >= L.listsize) {
        //扩容，realloc的大小是当前的加上要扩容的最终总长度
        ElemType *newbase = (ElemType *) realloc(L.elem, sizeof(ElemType) * (L.listsize + LISTINCREMENT));
        if (!newbase) {
            exit(OVERFLOW);
        }
        L.elem = newbase;
        L.listsize += LISTINCREMENT;
    }
    //新元素插入位置
    ElemType *p = &(L.elem[i - 1]);
    //后移元素：[i-1,L.length-1]==>[i,L.length]
    for (ElemType *q = &(L.elem[L.length - 1]); q >= p; --q) {
        *(q + 1) = *q;
    }
    //插入新元素
    *p = e;
    //长度增加
    L.length++;

    return OK;
}

/* ----------- 顺序表插入 ---------------*/
//删除第i个位置元素。i∈[1,L.length]
//将元素前移覆盖即可实现删除（[i,L.length-1]==>[i-1,L.length-2]）
Status ListDelete_Sq(SqList &L, int i, ElemType &e) {
    //删除位置合法性检查
    if (i < 1 || i > L.length) {
        return ERROR;
    }
    //返回删除位置的值
    e = L.elem[i - 1];
    //当前最后一个元素的位置
    ElemType *p = &(L.elem[L.length - 1]);
    //前移元素：[i,L.length-1]==>[i-1,L.length-2]
    for (ElemType *q = &(L.elem[i]); q <= p; ++q) {
        *(q - 1) = *q;
    }
    //长度减少
    L.length--;

    return OK;
}

/* ----------- 顺序表查找 ---------------*/
//查找关系函数
Status cmp(ElemType a, ElemType b) {
    return a > b ? TRUE : FALSE;    //例如判断a>b
}

//查找成功，则返回满足指定关系的第一个值的位置；否则返回0
int LocateElem_Sq(SqList L, ElemType e, Status (*cmp)(ElemType, ElemType)) {
    int i = 1;            //起始位置1
    ElemType *p = L.elem; //首元素存储位置
    //顺序表未查找完 且 未查找到满足条件的元素，则循环向后查找
    while (i <= L.length && !cmp(*p, e)) {
        i++;
        p++;
    }
    //查找结束，可能查找到（i<=L.length）或者没查找到(i>L.length)
    if (i <= L.length) {
        return i;
    } else {
        return 0;
    }
}

/* ----------- 顺序表合并（非递减有序） ---------------*/
//将La、Lb合并到Lc
void MergeList_Sq(SqList La, SqList Lb, SqList &Lc) {
    ElemType *pa = La.elem, *pb = Lb.elem, *pc;           //La、Lb、Lc首元素存储位置
    ElemType *la = pa + La.length - 1, *lb = pb + Lb.length - 1; //La、Lb尾元素存储位置

    Lc.listsize = Lc.length = La.length + Lb.length;      //Lc属性设置
    pc = Lc.elem = (ElemType *) malloc(sizeof(ElemType) * Lc.listsize);    //分配存储空间

    //遍历两个顺序表，每次取二者其中小者加入Lc中
    while (pa <= la && pb <= lb) {
        if (*pa <= *pb) {
            if (*pa==*pb){    //去除重复值(此处只能去除两个链表相互间的重复值，单个链表内的此处无法去除)
                pb++;
                Lc.length--;
            }
            *pc++ = *pa++;
        } else {
            *pc++ = *pb++;
        }
    }
    //插入有剩余的表的剩余部分（有且只有一个表会剩余，即以下只有某一个循环会执行）
    while (pa <= la) {
        *pc++ = *pa++;
    }
    while (pb <= lb) {
        *pc++ = *pb++;
    }
}

/* ----------- 顺序表输出 ---------------*/
void PrintList_Sq(SqList L){
    for (int i = 0; i < L.length; ++i) {
        printf("%d ",L.elem[i]);
    }
    printf("\n");
}


/* ----------- 顺序表去重 ---------------*/
//从前向后遍历，依次检查每个元素与其之前的元素序列是否重复，
//重复则删去当前元素（即前移[i+1,L.length-1]==>[i,L.length-2]）,并修改顺序表长度
void TrimList_Sq(SqList &L){
    //思路①
    //从第一个元素向后遍历，依次检查每个元素 与其之后 的元素序列是否重复，
    //重复则删去 其后元素（即前移[j+1,L.lengthj-1]==>[j,L.length-2]）,并修改顺序表长度
    for(int i=0;i<L.length;i++){
        for(int j=i+1; j<L.length; j++){
            if(L.elem[j] == L.elem[i]){
                for(int k=j; k<L.length-1;k++){
                    L.elem[k] = L.elem[k+1];
                }
                L.length--;
                j--;    //删除该位置元素后，当前循环中的比较位置也要相应前移
            }
        }
    }

    //思路②
    //从第二个元素向后遍历，依次检查每个元素 与其之前 的元素序列是否重复，
    //重复则删去 当前元素（即前移[i+1,L.length-1]==>[i,L.length-2]）,并修改顺序表长度
//    for(int i=1;i<L.length;i++){
//        for(int j=0; j<i; j++){
//            if(L.elem[j] == L.elem[i]){
//                for(int k=i; k<L.length-1;k++){
//                    L.elem[k] = L.elem[k+1];
//                }
//                L.length--;
//                break;  //与其前序列只可能存在一个重复
//            }
//        }
//    }

}


int main() {

    //以下为测试具体函数的代码
    SqList La,Lb,Lc;
    InitList_Sq(La);
    InitList_Sq(Lb);
    InitList_Sq(Lc);

    for (int i = 1; i <= 10; ++i) {
        ListInsert_Sq(La,i,rand()%100+1);
    }
    PrintList_Sq(La);
    printf("\n");

    ListInsert_Sq(La,1,11);
    ListInsert_Sq(La,12,99);
    PrintList_Sq(La);
    printf("\n");

    printf("locate first >50:%d \n",LocateElem_Sq(La,50,cmp));
    printf("\n");

    ElemType e;
    ListDelete_Sq(La,1,e);
    printf("e:%d\n",e);
    ListDelete_Sq(La,11,e);
    printf("e:%d\n",e);
    PrintList_Sq(La);
    printf("\n");

    for (int i = 1; i <= 10; ++i) {
        ListInsert_Sq(Lb,i,rand()%100+1);
    }
    PrintList_Sq(La);
    PrintList_Sq(Lb);
    printf("\n");
    sort(La.elem,La.elem+La.length);
    sort(Lb.elem,Lb.elem+Lb.length);
    PrintList_Sq(La);
    PrintList_Sq(Lb);
    printf("\n");
    TrimList_Sq(La);
    TrimList_Sq(Lb);
    PrintList_Sq(La);
    PrintList_Sq(Lb);
    printf("\n");
    MergeList_Sq(La,Lb,Lc);
    PrintList_Sq(Lc);
    printf("\n");

    return 0;
}
