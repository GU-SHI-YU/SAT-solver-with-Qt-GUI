#pragma once

#include "define.h"
#include <stdio.h>

typedef struct literal_info
{
	int is_assigned; //�Ƿ񱻸�ֵ
	int n_occur; //���Ӿ��г��ִ���
	int* lit_in_clauses; //���ֹ����Ӿ�
	int* lit_in_clause_locs; //���ֹ����Ӿ��б�Ԫ��λ��
	int is_unit; //�Ƿ�Ϊ���Ӿ�
	int antecedent_clause; //ǰ���Ӿ�
}literal_info;

static literal_info l_info[MAX_VARS + 5][2]; //�����Ԫ

typedef struct clause_info
{
	int* literals; //������Ԫ
	int current_length; //��ǰ����
	int original_length; //ԭʼ����
	int is_satisfied; //�Ƿ�����
	int binary_code; //��Ԫ�����Զ����Ʊ���
	int current_ucl; //��ǰ���־䣬���ǵ��־�ʱΪ0
}clause_info;

static clause_info* clauses; //�����Ӿ�
static int n_clauses; //ԭʼ�Ӿ�����
static int r_clauses; //��ǰ�Ӿ�����

typedef struct changes_info
{
	int clause_index; //�ϴα����ĵ��Ӿ�
	int literal_index; //�ϴα����ĵı�Ԫ
}changes_info;

static changes_info changes[50000000]; //�������
static unsigned int changes_index; //�ϴθ��ĵ�����
static unsigned int n_changes[MAX_VARS + 5][2]; //ĳһ��ȱ�����ͱ������ı�Ԫ��

typedef struct assign_info
{
	int type; //��ֵΪ����
	int depth; //���и�ֵ�����
	int decision; //����ֵ��ԭ��
}assign_info;

static assign_info assign[MAX_VARS + 5]; //���渳ֵ

static int contradictory_unit_clauses; //�Ƿ���ì�ܵ��Ӿ�
static int conflicting_literals; //����ì�ܵı�Ԫ

static int gucl_stack[MAX_VARS + 5]; //���ڵ��Ӿ䴫����ջ
static int n_gucl; //���ڵ��Ӿ䴫����ջ�Ĵ�С

static int depth; //DPLL���
static int backtrack_level; //�������

static int impl_clauses[MAX_CLAUSES + 5]; //���浥�Ӿ䴫������ì��ʱǰ���Ӿ�
static int icl_cnt; //���Ӿ䴫������ì��ʱǰ���Ӿ���

static int n_backtracks; //���ݴ���
static int n_units; //���Ӿ��Ԫ��ֵ����

static int n_vars; //��Ԫ��

static int resolvent[MAX_VARS + 5]; //��õĽ�

static int changes_occured;
static int resolvents_added; //������Ľ�
static int n_resolvents_threshold; //���Ա�����Ľ�����

static int max_clause_len; //����Ӿ䳤�ȣ����ڷ���

static FILE* fout;