#pragma once

#include "define.h"
#include <stdio.h>

typedef struct literal_info
{
	int is_assigned; //是否被赋值
	int n_occur; //在子句中出现次数
	int* lit_in_clauses; //出现过的子句
	int* lit_in_clause_locs; //出现过的子句中变元的位置
	int is_unit; //是否为单子句
	int antecedent_clause; //前驱子句
}literal_info;

static literal_info l_info[MAX_VARS + 5][2]; //储存变元

typedef struct clause_info
{
	int* literals; //包含变元
	int current_length; //当前长度
	int original_length; //原始长度
	int is_satisfied; //是否满足
	int binary_code; //变元满足性二进制编码
	int current_ucl; //当前单字句，不是单字句时为0
}clause_info;

static clause_info* clauses; //储存子句
static int n_clauses; //原始子句数量
static int r_clauses; //当前子句数量

typedef struct changes_info
{
	int clause_index; //上次被更改的子句
	int literal_index; //上次被更改的变元
}changes_info;

static changes_info changes[50000000]; //储存更改
static unsigned int changes_index; //上次更改的索引
static unsigned int n_changes[MAX_VARS + 5][2]; //某一深度被满足和被消除的变元数

typedef struct assign_info
{
	int type; //赋值为真或假
	int depth; //进行赋值的深度
	int decision; //被赋值的原因
}assign_info;

static assign_info assign[MAX_VARS + 5]; //储存赋值

static int contradictory_unit_clauses; //是否有矛盾单子句
static int conflicting_literals; //产生矛盾的变元

static int gucl_stack[MAX_VARS + 5]; //用于单子句传播的栈
static int n_gucl; //用于单子句传播的栈的大小

static int depth; //DPLL深度
static int backtrack_level; //回溯深度

static int impl_clauses[MAX_CLAUSES + 5]; //储存单子句传播发生矛盾时前驱子句
static int icl_cnt; //单子句传播发生矛盾时前驱子句数

static int n_backtracks; //回溯次数
static int n_units; //单子句变元赋值次数

static int n_vars; //变元数

static int resolvent[MAX_VARS + 5]; //求得的解

static int changes_occured;
static int resolvents_added; //被加入的解
static int n_resolvents_threshold; //可以被加入的解上限

static int max_clause_len; //最大子句长度，用于分裂

static FILE* fout;

static int ans[8][8];
static int editable[8][8];

static int puzzle_size;
