#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define UNSATISFIABLE 0
#define SATISFIABLE 1
#define SHRUNK 0
#define SATISFIED 1
#define NO 0
#define YES 1
#define FALSE 0
#define TRUE 1
#define UNASSINGED 2
#define ASSIGN_NONE 0 //δ����ֵ
#define ASSIGN_BRANCHED 1 //���ڷ��Ѳ��Ա���ֵ
#define ASSIGN_IMPLIED 2 //���ڵ��Ӿ䱻��ֵ
#define ASSIGN_REMOVED 3 //�����Եı�Ԫ

#define MAX_CLAUSES 10000
#define MAX_VARS 1000