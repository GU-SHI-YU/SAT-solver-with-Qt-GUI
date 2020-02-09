#include "define.h"
#include  "global_var.h"
#include "function.h"
#include "solver.cpp"

#include <stdlib.h>
#include <stdio.h>


int compare(const void* a, const void* b)
{
	int v = abs(*(int*)a);
	int u = abs(*(int*)b);
	return v - u;
}

int computeResolvent(int x, int a, int b, int& len, int limit)
{
	int j, k;
	int* check = (int*)calloc(n_vars + 1, sizeof(int));
	if (!check)
		exit(-1);
	int res_size = 0;
	int C[2] = { a,b };
	for(j = 0;j < 2;j++)
	{
		for(k = 0;k < clauses[C[j]].original_length;k++)
		{
			int w = abs(clauses[C[j]].literals[k]);
			if (w == x)
				continue;
			if(check[w] == clauses[C[j]].literals[k])
				continue;
			if(check[w] == -clauses[C[j]].literals[k])
			{
				free(check);
				return FALSE;
			}
			if(assign[abs(clauses[C[j]].literals[k])].decision != ASSIGN_NONE)
				continue;
			if(check[w] == 0)
			{
				check[w] = clauses[C[j]].literals[k];
				resolvent[res_size++] = clauses[C[j]].literals[k];
				if(res_size > limit)
				{
					free(check);
					return FALSE;
				} //如果解超过大小，放弃求解
			} //记录不是x也没记录过的变元
		}
	} //在两个子句中检索
	len = res_size;
	free(check);
	return TRUE;
}

int getRestrictedResolvent(int x, int limit)
{
	int i, j, k, a, b, res_length;
	int found;
	changes_occured = FALSE;
	for(i = 0;i<l_info[x][SATISFIED].n_occur;i++)
	{
		a = l_info[x][SATISFIED].lit_in_clauses[i];
		if(clauses[a].is_satisfied == NO)
		{
			for(j = 0;j < l_info[x][SHRUNK].n_occur;j++)
			{
				b = l_info[x][SHRUNK].lit_in_clauses[j];
				if(clauses[b].is_satisfied == NO)
				{
					found = computeResolvent(x, a, b, res_length, limit);
					if(found)
					{
						if (resolvents_added < n_resolvents_threshold)
						{
							resolvents_added += addClause(resolvent, res_length);
							changes_occured = TRUE;
						} //解的数量未达到上限
						else
							return 0;
					} //找到解
				}
			}
		}
	} //求化简这个变元可以获得的解
	return -1;
}

int subsumable(int j, int k)
{
	int i;
	int* check = (int*)calloc(n_vars + 1, sizeof(int));
	if (!check)
		exit(-1);
	for (i = 0;i < clauses[k].original_length;i++)
		check[abs(clauses[k].literals[i])] = clauses[k].literals[i];
	for(i = 0;i < clauses[j].original_length;i++)
		if(clauses[j].literals[i] != check[abs(clauses[j].literals[i])])
		{
			free(check);
			return NO;
		}
	free(check);
	return YES;
}

int preprocessSubsume()
{
	int n_subsumed = 0;
	int i, j, k, c1, c2, type;
	changes_occured = FALSE;
	for(i = 1;i <=n_vars;i++)
	{
		if (assign[i].decision != ASSIGN_NONE)
			continue;
		for(type=0;type<2;type++)
		{
			for(j = 0;j<l_info[i][type].n_occur;j++)
			{
				for(k = 0;k< l_info[i][type].n_occur;k++)
				{
					if(j==k)
						continue;;
					c1 = l_info[i][type].lit_in_clauses[j];
					c2 = l_info[i][type].lit_in_clauses[k];
					if (clauses[c1].is_satisfied || clauses[c2].is_satisfied) 
						continue;
					if (clauses[c1].original_length >= clauses[c2].original_length) 
						continue;
					if (subsumable(c1, c2))
					{
						clauses[c2].is_satisfied = YES;
						r_clauses--;
						n_subsumed++;
						changes_occured = TRUE;
					}
				}
			}
		}
	} //检查包含
	return n_subsumed;
}

int preprocessUnitPropagation()
{
	int* lucl_stack = nullptr; //储存局部单子句变元
	unsigned int n_lucl = 0; //局部单子句变元数量
	changes_occured = FALSE;
	while (true)
	{
		if (contradictory_unit_clauses)
		{
			icl_cnt = 0;
			int cl = abs(conflicting_literals);
			assign[cl].decision = ASSIGN_NONE;
			while (n_lucl)
			{
				unSetVar(lucl_stack[--n_lucl]);
				int s = abs(lucl_stack[n_lucl]);
				int t = (lucl_stack[n_lucl] > 0) ? SATISFIED : SHRUNK;
				assign[s].type = UNASSINGED;
				assign[s].decision = ASSIGN_NONE;
			} //还原前驱子句
			contradictory_unit_clauses = FALSE;
			free(lucl_stack);
			n_gucl = 0;
			return UNSATISFIABLE;
		} //有矛盾子句
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
			changes_occured = TRUE;
		} //没有矛盾子句，为单子句变元赋值
		else
			break;
	} //单子句传播
	return SATISFIABLE;
}

void preprocessMonotoneLiteralFixing()
{
	int i;
	changes_occured = FALSE;
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
				assign[abs(u)].type = (u > 0) ? TRUE : FALSE;
				assign[abs(u)].depth = depth;
				assign[abs(u)].decision = ASSIGN_IMPLIED;
				setVar(u);
			} //单子句变元赋值
		}
	} //单调变元优化
}

int preprocessor()
{
	int total_changes_occured, n_s = 0;
	if (n_clauses < 500)
		n_resolvents_threshold = n_clauses * 5;
	else if (n_clauses < 1000)
		n_resolvents_threshold = n_clauses * 4;
	else if (n_clauses < 2500)
		n_resolvents_threshold = n_clauses * 2.5;
	else
		n_resolvents_threshold = n_clauses;
	while (true)
	{
		total_changes_occured = 0;
		if (preprocessUnitPropagation() == UNSATISFIABLE) //预处理单子句传播
			return UNSATISFIABLE;
		total_changes_occured += changes_occured;
		preprocessMonotoneLiteralFixing(); //预处理单调变元简化
		total_changes_occured += changes_occured;
		//if(resolvents_added < n_resolvents_threshold)
		//{
		//	int i;
		//	for (i = 1;i <= n_vars;i++)
		//		if (assign[i].decision == ASSIGN_NONE)
		//			if (getRestrictedResolvent(i, 3) == 0)
		//			{
		//				printf("over\n");
		//				break;
		//			}
		//	total_changes_occured += changes_occured;
		//} //预处理求解
		preprocessSubsume(); //预处理包含子句简化
		total_changes_occured += changes_occured;
		if (total_changes_occured == 0)
			break;
	}
	return 1;
}

int clausePresent(int C[], int n)
{
	int i, j, k, p, q;
	p = abs(C[0]);
	q = C[0] > 0 ? SATISFIED : SHRUNK;
	for(j = 0;j < l_info[p][q].n_occur;j++)
	{
		if(clauses[l_info[p][q].lit_in_clauses[j]].original_length == n)
		{
			int match_count = 0;
			for(k = 0;k < n;k++)
			{
				if (clauses[l_info[p][q].lit_in_clauses[j]].literals[k] == C[k])
					match_count++;
				else break;
			}
			if (match_count == n)
				return TRUE;
		}
	}
	return FALSE;
}

int addClause(int C[], int n)
{
	int i;
	qsort(C, n, sizeof(int),compare);
	if (clausePresent(C, n))
		return FALSE; //检查是否是重复子句
	clauses = (clause_info*)realloc(clauses, (n_clauses + 1) * sizeof(clause_info));
	if (!clauses)
		exit(-1);
	clauses[n_clauses].is_satisfied = NO;
	clauses[n_clauses].current_length = n;
	clauses[n_clauses].original_length = n;
	clauses[n_clauses].binary_code = (((1 << (n - 1)) - 1) << 1) + 1;
	clauses[n_clauses].current_ucl = 0;
	clauses[n_clauses].literals = (int*)malloc((n + 1) * sizeof(int)); //子句初始化
	if (n > max_clause_len)
		max_clause_len = n;
	for(i = 0;i < n;i++)
	{
		int p = abs(C[i]);
		int q = C[i] > 0 ? SATISFIED : SHRUNK;
		l_info[p][q].lit_in_clauses = (int*)realloc(l_info[p][q].lit_in_clauses, (l_info[p][q].n_occur + 1) * sizeof(int));
		if (!l_info[p][q].lit_in_clauses)
			exit(-1);
		l_info[p][q].lit_in_clause_locs = (int*)realloc(l_info[p][q].lit_in_clause_locs, (l_info[p][q].n_occur + 1) * sizeof(int));
		if (!l_info[p][q].lit_in_clause_locs)
			exit(-1);
		l_info[p][q].lit_in_clauses[l_info[p][q].n_occur] = n_clauses;
		l_info[p][q].lit_in_clause_locs[l_info[p][q].n_occur] = i;
		l_info[p][q].n_occur++;
		l_info[p][q].is_assigned = NO;
		clauses[n_clauses].literals[i] = C[i];
		//fprintf(fout, "%d ", C[i]);
		assign[p].decision = ASSIGN_NONE;
		assign[p].type = UNASSINGED;
	} //子句赋值
	//fprintf(fout, "0\n");
	if(n == 1)
	{
		int s = abs(clauses[n_clauses].literals[0]);
		int t = clauses[n_clauses].literals[0] > 0 ? SATISFIED : SHRUNK;
		l_info[s][t].antecedent_clause = n_clauses;
		if(l_info[s][(!t)].is_unit == YES)
		{
			contradictory_unit_clauses = TRUE;
			conflicting_literals = clauses[n_clauses].literals[0];
		}
		else if(l_info[s][t].is_unit == NO)
		{
			gucl_stack[n_gucl] = clauses[n_clauses].literals[0];
			clauses[n_clauses].current_ucl = clauses[n_clauses].literals[0];
			l_info[s][t].is_unit = YES;
			n_gucl++; //单子句变元入栈
		}
	} //是单子句
	n_clauses++;
	r_clauses++;
	return TRUE;
}

int reader(char* path)
{
	init();
	int* literals = nullptr; //子句变元
	int res_size = 0; //子句变元数
	FILE* fp = fopen(path, "r");
	int n_c;
	char bin[80];
	while(true)
	{
		char c = fgetc(fp);
		if (c == EOF)
			break;
		if(c == 'c')
		{
			fgets(bin, 80, fp);
		}//跳过注释
		else if(c == 'p')
		{
			fscanf(fp,"%s", bin);
			fscanf(fp,"%d%d", &n_vars, &n_c); //获取子句数，变元数
			fgetc(fp);
		}
		else
		{
			ungetc(c,fp);
			int i;
			fscanf(fp,"%d", &i);
			while (i != 0)
			{
				literals = (int*)realloc(literals, (res_size + 1) * sizeof(int));
				if (!literals)
					exit(-1);
				literals[res_size++] = i;
				fscanf(fp,"%d", &i);
			} //读取子句变元
			if (res_size >= 32)
			{
				int j;
				int temp[31];
				for (j = 0;j < 30;j++)
				{
					temp[j] = literals[j];
				}
				n_vars++;
				temp[30] = n_vars;
				addClause(temp, 31);
				res_size -= 30; //过长子句头部处理
				while (res_size > 30)
				{
					temp[0] = n_vars;
					int k;
					for (k = 1;k < 30;k++)
					{
						temp[k] = literals[j++];
					}
					n_vars++;
					temp[30] = n_vars;
					addClause(temp, 31);
					res_size -= 29;
				} //过长子句中间部分处理
				temp[0] = n_vars;
				int k;
				for (k = 1;k <= res_size;k++)
				{
					temp[k] = literals[j++];
				}
				addClause(temp, res_size + 1); //过长子句尾部处理
			} //处理过长子句
			else
				addClause(literals, res_size);
			literals = nullptr;
			res_size = 0;
			fgetc(fp);
		}
	}
	fclose(fp);
	free(literals);
	if (!preprocessor())
	{
		fclose(fout);
		return UNSATISFIABLE;
	}
	fclose(fout);
	return 1;
}
