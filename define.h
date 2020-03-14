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
#define ASSIGN_NONE 0 //未被赋值
#define ASSIGN_BRANCHED 1 //基于分裂策略被赋值
#define ASSIGN_IMPLIED 2 //基于单子句被赋值
#define ASSIGN_REMOVED 3 //被忽略的变元

#define MAX_CLAUSES 100000 //最大子句数
#define MAX_VARS 10000 //最大变元数