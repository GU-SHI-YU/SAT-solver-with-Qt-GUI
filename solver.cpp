#include "define.h"
#include "global_var.h"
#include "branching_rule.h"
#include "function.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void setVar(int v)
{
	int i;
	int p = abs(v);
	int q = (v > 0) ? SATISFIED : SHRUNK;
	for (i = 0; i < l_info[p][q].n_occur; i++)
	{
		int j = l_info[p][q].lit_in_clauses[i];
		if (clauses[j].is_satisfied)
			continue;
		clauses[j].is_satisfied = YES;
		r_clauses--;
		changes[changes_index++].clause_index = j;
		n_changes[depth][SATISFIED]++; //��¼����
	} //ʹÿ������v���Ӿ���Ϊ����
	q = !q;
	for (i = 0; i < l_info[p][q].n_occur; i++) //
	{
		int j = l_info[p][q].lit_in_clauses[i];
		if (clauses[j].is_satisfied)
			continue;
		int k = l_info[p][q].lit_in_clause_locs[i];
		clauses[j].current_length--;
		clauses[j].binary_code -= 1 << k; //���±�Ԫ������
		changes[changes_index].clause_index = j;
		changes[changes_index++].literal_index = k;
		n_changes[depth][SHRUNK]++; //��¼����
		if (clauses[j].current_length == 1)
		{
			int loc = int(log2(clauses[j].binary_code)); //��ȡ���Ӿ��Ԫ
			int w = clauses[j].literals[loc];
			int s = abs(w);
			int t = (w > 0) ? SATISFIED : SHRUNK;
			l_info[s][t].antecedent_clause = j; //���µ��Ӿ�ǰ���Ӿ�
			if (l_info[s][(!t)].is_unit == YES)
			{
				contradictory_unit_clauses = TRUE;
				conflicting_literals = w; //��¼ì��
			} //�����wҲ�ǵ��Ӿ��Ԫ
			else if (l_info[s][t].is_unit == NO)
			{
				gucl_stack[n_gucl] = clauses[j].current_ucl = w;
				l_info[s][t].is_unit = YES;
				n_gucl++; //���Ӿ��Ԫ��ջ
			}
		} //����Ӿ��ɵ��Ӿ�
	} //��ÿ��������v���Ӿ���ɾȥ��v
	if (depth && backtrack_level == depth - 1)
		backtrack_level++;
	depth++;
	l_info[p][SATISFIED].is_assigned = YES;
	l_info[p][SHRUNK].is_assigned = YES; //������Ⱥͱ�Ԫ��ֵ���
}

void unSetVar(int v)
{
	int p = abs(v);
	depth--;
	if (depth && backtrack_level == depth)
		backtrack_level--; //�������
	while (n_changes[depth][SHRUNK])
	{
		n_changes[depth][SHRUNK]--;
		int j = changes[--changes_index].clause_index;
		int k = changes[changes_index].literal_index;
		clauses[j].current_length++;
		if (clauses[j].current_length == 2)
		{
			int s = abs(clauses[j].current_ucl);
			int t = (clauses[j].current_ucl > 0) ? SATISFIED : SHRUNK;
			l_info[s][t].is_unit = NO;
			clauses[j].current_ucl = 0;
		} //�������֮ǰ�ǵ��Ӿ�
		clauses[j].binary_code += (1 << k); //�����Ӿ��Ԫ������
	} //�������з�v��Ԫ�ı�
	while (n_changes[depth][SATISFIED])
	{
		n_changes[depth][SATISFIED]--;
		int j = changes[--changes_index].clause_index;
		clauses[j].is_satisfied = NO;
		r_clauses++;
	} //��������v��Ԫ�ı�
	l_info[p][SATISFIED].is_assigned = NO;
	l_info[p][SHRUNK].is_assigned = NO; //���±�Ԫ��ֵ״̬
}

int dpll()
{
	int* lucl_stack = nullptr; //����ֲ����Ӿ��Ԫ
	unsigned int n_lucl = 0; //�ֲ����Ӿ��Ԫ����
	int* ml_stack = nullptr; //���浥����Ԫ
	int n_ml = 0; //������Ԫ����
	while (true)
	{
		if (contradictory_unit_clauses)
		{
			icl_cnt = 0;
			int cl = abs(conflicting_literals);
			impl_clauses[icl_cnt++] = l_info[cl][SATISFIED].antecedent_clause;
			impl_clauses[icl_cnt++] = l_info[cl][SHRUNK].antecedent_clause;
			assign[cl].decision = ASSIGN_NONE;
			while (n_lucl)
			{
				unSetVar(lucl_stack[--n_lucl]);
				int s = abs(lucl_stack[n_lucl]);
				int t = (lucl_stack[n_lucl] > 0) ? TRUE : FALSE;
				impl_clauses[icl_cnt++] = l_info[s][t].antecedent_clause;
				assign[s].type = UNASSINGED;
				assign[s].decision = ASSIGN_NONE;
			} //��ԭǰ���Ӿ�
			contradictory_unit_clauses = FALSE;
			free(lucl_stack);
			n_gucl = 0;
			return UNSATISFIABLE;
		} //��ì���Ӿ�
		if (n_gucl)
		{
			lucl_stack = (int*)realloc(lucl_stack, (n_lucl + 1) * sizeof(int));
			if (!lucl_stack)
				exit(-1);
			int implied_lit = gucl_stack[--n_gucl];
			lucl_stack[n_lucl++] = implied_lit;
			assign[abs(implied_lit)].type = implied_lit > 0 ? TRUE : FALSE;
			assign[abs(implied_lit)].depth = depth;
			assign[abs(implied_lit)].decision = ASSIGN_IMPLIED;
			setVar(implied_lit);
			n_units++;
		} //û��ì���Ӿ䣬Ϊ���Ӿ��Ԫ��ֵ
		else
			break;
	} //���Ӿ䴫��
	if (!r_clauses)
		return SATISFIABLE;
	int i;
	for (i = 1; i <= n_vars; i++)
	{
		int x, y, u, C;
		x = y = 0;
		if (assign[i].decision == ASSIGN_NONE)
		{
			u = 0;
			int j;
			for (j = 0;j < l_info[i][SATISFIED].n_occur;j++)
			{
				C = l_info[i][SATISFIED].lit_in_clauses[j];
				x += 1 - clauses[C].is_satisfied;
			}
			for (j = 0; j < l_info[i][SHRUNK].n_occur; j++)
			{
				C = l_info[i][SHRUNK].lit_in_clauses[j];
				y += 1 - clauses[C].is_satisfied;
			}
			if (x && !y)
				u = i;
			if (y && !x)
				u = -i;
			if (u)
			{
				ml_stack = (int*)realloc(ml_stack, (n_ml + 1) * sizeof(int));
				if (!ml_stack)
					exit(-1);
				ml_stack[n_ml++] = u;
				assign[abs(u)].type = (u > 0) ? TRUE : FALSE;
				assign[abs(u)].depth = depth;
				assign[abs(u)].decision = ASSIGN_IMPLIED;
				setVar(u);
			} //���Ӿ��Ԫ��ֵ
		}
	} //������Ԫ�Ż�
	int v = getLiteral2SJW();
	//printf("%d ", v);
	//for (i = 0;i < n_clauses;i++)
	//	if (clauses[i].is_satisfied == NO)
	//		printf("%d ", i);
	assign[abs(v)].type = (v > 0) ? TRUE : FALSE;
	assign[abs(v)].depth = depth;
	assign[abs(v)].decision = ASSIGN_BRANCHED;
	setVar(v); //���շ��ѹ���Ϊ��Ԫ��ֵ
	if (dpll())
		return SATISFIABLE;
	unSetVar(v);
	assign[abs(v)].type = UNASSINGED;
	assign[abs(v)].decision = ASSIGN_NONE;
	int max_depth = 0, j, k, m, left = FALSE;
	if (icl_cnt)
	{
		while (icl_cnt)
		{
			i = impl_clauses[--icl_cnt];
			k = clauses[i].original_length;
			for (j = 0; j < k; j++)
			{
				m = abs(clauses[i].literals[j]);
				if (assign[m].decision == ASSIGN_BRANCHED && assign[m].depth > max_depth)
					max_depth = assign[m].depth;
			}
		} //����max_depth
		left = TRUE;
	}
	n_backtracks++;
	//if (backtrack_level >= depth - 1)
	{
		assign[abs(v)].type = !assign[abs(v)].type;
		assign[abs(v)].depth = depth;
		assign[abs(v)].decision = ASSIGN_IMPLIED;
		setVar(-v);
		if (dpll())
			return SATISFIABLE;
		unSetVar(-v);
		assign[abs(v)].type = UNASSINGED;
		assign[abs(v)].decision = ASSIGN_NONE;
		if (left && icl_cnt)
		{
			while (icl_cnt)
			{
				i = impl_clauses[--icl_cnt];
				k = clauses[i].original_length;
				for (j = 0; j < k; j++)
				{
					m = abs(clauses[i].literals[j]);
					if (assign[m].decision == ASSIGN_BRANCHED && assign[m].depth > max_depth)
						max_depth = assign[m].depth;
				}
			} //����max_depth
			if (max_depth < backtrack_level)
				backtrack_level = max_depth; //���»������
		}
	} //���������
	while (n_ml)
	{
		int u = ml_stack[--n_ml];
		unSetVar(u);
		assign[abs(u)].type = UNASSINGED;
		assign[abs(u)].decision = ASSIGN_NONE;
	} //���ص�����Ԫ�Ķ�
	icl_cnt = 0;
	while (n_lucl)
	{
		int z = lucl_stack[--n_lucl];
		unSetVar(z);
		assign[abs(z)].type = UNASSINGED;
		assign[abs(z)].decision = ASSIGN_NONE;
	} //���ظ���
	free(lucl_stack);
	contradictory_unit_clauses = FALSE;
	return UNSATISFIABLE;
}

void init()
{
	for (int i = 1;i <= n_vars;i++)
	{
		free(l_info[i][SATISFIED].lit_in_clause_locs);
		free(l_info[i][SATISFIED].lit_in_clauses);
		free(l_info[i][SHRUNK].lit_in_clause_locs);
		free(l_info[i][SHRUNK].lit_in_clauses);
		l_info[i][SATISFIED] = l_info[i][SHRUNK] = { 0,0,NULL,NULL,0,0 };
		n_changes[i][SATISFIED] = n_changes[i][SHRUNK] = 0;
		n_changes[0][SATISFIED] = n_changes[0][SHRUNK] = 0;
		assign[i].type = UNASSINGED;
		assign[i].depth = 0;
		assign[i].decision = ASSIGN_NONE;
	}
	fout = fopen("D:\\sudoku.out", "w");
	free(clauses);
	clauses = NULL;
	n_clauses = 0;
	r_clauses = 0;
	n_vars = 0;
	changes_index = 0;
	n_gucl = 0;
	icl_cnt = 0;
	depth = backtrack_level = 0;
	n_units = 0;
	n_backtracks = 0;
	resolvents_added = 0;
	max_clause_len = 0;
	contradictory_unit_clauses = FALSE;
	conflicting_literals = 0;
}
