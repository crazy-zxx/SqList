//
// Created by xylx on 2020/10/14.
//

#ifndef SQLIST_COMMONDEFINE_H
#define SQLIST_COMMONDEFINE_H


/* ----------- 常用返回值预定义 --------------*/
typedef int Status;
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
/*-----------------------------------------*/


/* ----------- 顺序表结构定义 ---------------*/
#define LIST_INIT_SIZE 100      //初始化长度
#define LISTINCREMENT 10        //每次扩容长度

//元素类型
typedef int ElemType;

typedef struct sqlNode{
    ElemType *elem;     //基址（动态分配数组内存）
    int length;         //长度（有效数据个数）
    int listsize;       //容量
}SqList;    //SqList为线性表类型
/*-----------------------------------------*/



#endif //SQLIST_COMMONDEFINE_H
