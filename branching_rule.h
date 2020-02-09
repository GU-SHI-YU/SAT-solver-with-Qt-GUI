#pragma once
#include "define.h"
#include "global_var.h"
#include "function.h"

#include <math.h>
#include <stdlib.h>

inline int getLiteralDLCS()
{
	int i, j, C;
	int max = 0, r, s, t;
	int u;
	for(i = 1;i<=n_vars;i++)
	{
		if(assign[i].decision == ASSIGN_NONE)
		{
			s = t = 0;
			for(j=0;j<l_info[i][SATISFIED].n_occur;j++)
			{
				C = l_info[i][SATISFIED].lit_in_clauses[j];
				s += 1 - clauses[C].is_satisfied;
			}
			for(j=0;j<l_info[i][SHRUNK].n_occur;j++)
			{
				C = l_info[i][SHRUNK].lit_in_clauses[j];
				t += 1 - clauses[C].is_satisfied;
			}
			r = s + t;
			if(r > max)
			{
				max = r;
				if (s >= t)
					u = i;
				else
					u = -i;
			}
		}
	}
	return u;
}

inline int getShortestLen()
{
	int i, j, C, type, min = max_clause_len;
	if (min == 2)
		return min;
	for(i = 1;i < n_vars;i++)
	{
		if(assign[i].decision == ASSIGN_NONE)
		{
			for(type=0;type<2;type++)
			{
				for(j = 0;j < l_info[i][type].n_occur;j++)
				{
					C = l_info[i][type].lit_in_clauses[j];
					if(!clauses[C].is_satisfied && clauses[C].current_length < min)
					{
						min = clauses[C].current_length;
						if (min == 2)
							return 2;
					}
				}
			}
		}
	}
	return min;
}

void getMOMS(int x, int k, unsigned int& s,unsigned int& t)
{
	int j, c;
	s = t = 0;
	for(j = 0;j < l_info[x][SATISFIED].n_occur;j++)
	{
		c = l_info[x][SATISFIED].lit_in_clauses[j];
		if (clauses[c].current_length == k)
			s += 1 - clauses[c].is_satisfied;
	}
	for (j = 0;j < l_info[x][SHRUNK].n_occur;j++)
	{
		c = l_info[x][SHRUNK].lit_in_clauses[j];
		if (clauses[c].current_length == k)
			t += 1 - clauses[c].is_satisfied;
	}
}

inline int getLiteralMinLen()
{
	int i, k;
	unsigned int max = 0, r, s, t;
	int u = 0;
	for(i = 1;i <= n_vars;i++)
	{
		if(assign[i].decision == ASSIGN_NONE)
		{
			k = getShortestLen();
			int j, C;
			s = t = 0;
			for (j = 0;j < l_info[i][SATISFIED].n_occur;j++)
			{
				C = l_info[i][SATISFIED].lit_in_clauses[j];
				if (clauses[C].current_length == k)
					s += 1 - clauses[C].is_satisfied;
			}
			for (j = 0;j < l_info[i][SHRUNK].n_occur;j++)
			{
				C = l_info[i][SHRUNK].lit_in_clauses[j];
				if (clauses[C].current_length == k)
					t += 1 - clauses[C].is_satisfied;
			}
			r = (s + 1) * (t + 1);
			if(r > max)
			{
				max = r;
				if (s >= t)
					u = i;
				else
					u = -i;
			}
		}
	}
	return u;
}

inline int getLiteral2SJW()
{
	int i, j, C;
	double max = 0, r, s, t, mlen = max_clause_len;
	int u = 0;
	for(i=1;i<=n_vars;i++)
	{
		if(assign[i].decision == ASSIGN_NONE)
		{
			s = t = 0;
			for(j = 0;j < l_info[i][SATISFIED].n_occur;j++)
			{
				C = l_info[i][SATISFIED].lit_in_clauses[j];
				if(clauses[C].is_satisfied == NO)
					s += pow(2, -clauses[C].current_length);
			}
			for (j = 0;j < l_info[i][SHRUNK].n_occur;j++)
			{
				C = l_info[i][SHRUNK].lit_in_clauses[j];
				if (clauses[C].is_satisfied == NO)
					t += pow(2, -clauses[C].current_length);
			}
			r = s + t;
			if(r > max)
			{
				max = r;
				if (s >= t)
					u = i;
				else
					u = -i;
			}
		}
	}
	return u;
}

inline int getFirst()
{
	int i;
	for (i = 1;i <= n_vars;i++)
		if (assign[i].type == UNASSINGED)
			return i;
}