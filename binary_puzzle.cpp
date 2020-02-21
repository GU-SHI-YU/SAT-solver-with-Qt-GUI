#include "function.h"
#include "define.h"
#include "global_var.h"
#include "reader.cpp"
#include <time.h>

int readPuzzle(char* path)
{
	init();
	int* literal = nullptr; //子句变元
	int res_size = 0; //子句变元数
	int m; //数独阶数
	int n; //已知空数
	FILE* fp = fopen(path, "r");
	fscanf(fp, "%d%d", &m, &n);
    puzzle_size = m;
	n_vars = m * m;
	literal = (int*)malloc(sizeof(int));
	if (!literal)
		exit(-1);
	int i, j;
	for (i = 0;i < m;i++)
	{
		for (j = 0;j < m;j++)
		{
			char c;
			fscanf(fp, "%c", &c);
			if (c == '\n')
				fscanf(fp, "%c", &c);
			if (c == '0')
			{
				literal[0] = -(i * m + j + 1);
				addClause(literal, 1);
			}
			if (c == '1')
			{
				literal[0] = (i * m + j + 1);
				addClause(literal, 1);
			}
		}
	} //读取已知格子
	res_size = 3;
	for (i = 0;i < m;i++)
	{
		for (j = 1;j < m - 1;j++)
		{
			literal = (int*)malloc(sizeof(int) * 3);
			if (!literal)
				exit(-1);
			literal[0] = i * m + j + 1;
			literal[1] = i * m + j;
			literal[2] = i * m + j + 2;
			addClause(literal, res_size);
			literal[0] = -literal[0];
			literal[1] = -literal[1];
			literal[2] = -literal[2];
			addClause(literal, res_size);
			literal = nullptr;
		}
	} //横向三个
	for (i = 1;i < m - 1;i++)
	{
		for (j = 0;j < m;j++)
		{
			literal = (int*)malloc(sizeof(int) * 3);
			if (!literal)
				exit(-1);
			literal[0] = i * m + j + 1;
			literal[1] = (i - 1) * m + j + 1;
			literal[2] = (i + 1) * m + j + 1;
			addClause(literal, res_size);
			literal[0] = -literal[0];
			literal[1] = -literal[1];
			literal[2] = -literal[2];
			addClause(literal, res_size);
			literal = nullptr;
		}
	} //纵向三个
	res_size = m / 2 + 1;
	literal = (int*)malloc(sizeof(int) * res_size);
	if (!literal)
		exit(-1);
	for(i = 0;i < m;i++)
	{
		combine(literal, i, m, res_size, 0, 1);
		combine(literal, i, m, res_size, 0, 2);
	} //每行每列0和1个数相同
	int* new_vars = (int*)malloc(sizeof(int) * (3 * m + 1));
	if (!new_vars)
		exit(-1);
	for(i = 0;i < m;i++)
	{
		for(j = i + 1;j < m;j++)
		{
			new_vars[0] = ++n_vars;
			int k;
			for(k = 0;k < m;k++)
			{
				new_vars[k * 3 + 1] = ++n_vars;
				new_vars[k * 3 + 2] = ++n_vars;
				new_vars[k * 3 + 3] = ++n_vars;
				literal[0] = i * m + k + 1;
				literal[1] = -new_vars[k * 3 + 1];
				literal[2] = j * m + k + 1;
				addClause(literal, 2);
				addClause(literal + 1, 2);
				literal[0] = -literal[0];
				literal[1] = -literal[1];
				literal[2] = -literal[2];
				addClause(literal, 3);
				literal[0] = -(i * m + k + 1);
				literal[1] = -new_vars[k * 3 + 2];
				literal[2] = -(j * m + k + 1);
				addClause(literal, 2);
				addClause(literal + 1, 2);
				literal[0] = -literal[0];
				literal[1] = -literal[1];
				literal[2] = -literal[2];
				addClause(literal, 3);
				literal[0] = -new_vars[k * 3 + 1];
				literal[1] = new_vars[k * 3 + 3];
				literal[2] = -new_vars[k * 3 + 2];
				addClause(literal, 2);
				addClause(literal + 1, 2);
				literal[0] = -literal[0];
				literal[1] = -literal[1];
				literal[2] = -literal[2];
				addClause(literal, 3);
				literal[0] = new_vars[0];
				literal[1] = new_vars[k * 3 + 3];
				addClause(literal, 2);
			}
			literal = (int*)realloc(literal, sizeof(int) * (m + 1));
			if (!literal)
				exit(-1);
			for (k = 0;k < m;k++)
			{
				literal[k] = -new_vars[k * 3 + 3];
			}
			literal[m] = -new_vars[0];
			addClause(literal, m + 1);
			literal[0] = new_vars[0];
			addClause(literal, 1);
		}
	} //不能有相同的行
	for (j = 0;j < m;j++)
	{
		for (i = j + 1;i < m;i++)
		{
			new_vars[0] = ++n_vars;
			int k;
			for (k = 0;k < m;k++)
			{
				new_vars[k * 3 + 1] = ++n_vars;
				new_vars[k * 3 + 2] = ++n_vars;
				new_vars[k * 3 + 3] = ++n_vars;
				literal[0] = k * m + i + 1;
				literal[1] = -new_vars[k * 3 + 1];
				literal[2] = k * m + j + 1;
				addClause(literal, 2);
				addClause(literal + 1, 2);
				literal[0] = -literal[0];
				literal[1] = -literal[1];
				literal[2] = -literal[2];
				addClause(literal, 3);
				literal[0] = -(k * m + i + 1);
				literal[1] = -new_vars[k * 3 + 2];
				literal[2] = -(k * m + j + 1);
				addClause(literal, 2);
				addClause(literal + 1, 2);
				literal[0] = -literal[0];
				literal[1] = -literal[1];
				literal[2] = -literal[2];
				addClause(literal, 3);
				literal[0] = -new_vars[k * 3 + 1];
				literal[1] = new_vars[k * 3 + 3];
				literal[2] = -new_vars[k * 3 + 2];
				addClause(literal, 2);
				addClause(literal + 1, 2);
				literal[0] = -literal[0];
				literal[1] = -literal[1];
				literal[2] = -literal[2];
				addClause(literal, 3);
				literal[0] = new_vars[0];
				literal[1] = new_vars[k * 3 + 3];
				addClause(literal, 2);
			}
			literal = (int*)realloc(literal, sizeof(int) * (m + 1));
			if (!literal)
				exit(-1);
			for (k = 0;k < m;k++)
			{
				literal[k] = -new_vars[k * 3 + 3];
			}
			literal[m] = -new_vars[0];
			addClause(literal, m + 1);
			literal[0] = new_vars[0];
			addClause(literal, 1);
		}
	} //不能有相同的列
	free(literal);
	fclose(fp);
	if (!preprocessor())
	{
		fclose(fout);
		return UNSATISFIABLE;
	}
	fseek(fout, 0, SEEK_SET);
	fprintf(fout,"p cnf %d %d\n", n_vars, n_clauses);
	fclose(fout);
	return 1;
}

void combine(int* res, int i, int n, int k, int size, int type)
{
	int m;
	if (size == k)
	{
		int* res1 = (int*)malloc(sizeof(int) * size);
		for (m = 0;m < size;m++)
			res1[m] = res[m];
		addClause(res1, k);
		int* res2 = (int*)malloc(sizeof(int) * size);
		for (m = 0;m < size;m++)
			res2[m] = -res[m];
		addClause(res2, k);
		free(res1);
		free(res2);
		return;
	}
	for(m = n - 1;m >= 0;m--)
	{
		if (type == 1)
			res[size] = i * (k - 1) * 2 + m + 1;
		else
			res[size] = m * (k - 1) * 2 + i + 1;
		combine(res, i, m, k, size + 1, type);
	}
}

int lasVegas(int n, int m)
{
    int k = n;
    int** board = (int**)malloc(m *  sizeof(int*));
    if (!board)
        exit(-1);
    for(int i = 0;i < m;i++)
    {
        board[i] = (int*)malloc(sizeof(int) * m);
        if (!board[i])
            exit(-1);
    }
    for (int i = 0;i < m;i++)
        for (int j = 0;j < m;j++)
            board[i][j] = '*';
    srand((unsigned)time(NULL));
    while(n)
    {
        int i = rand() % m;
        int j = rand() % m;
        if(board[i][j] == '*')
        {
            int v = rand() % 2 + 48;
            if (i == 0)
                if (board[i + 1][j] == v && board[i + 2][j] == v)
                    break;
            if (i == m - 1)
                if (board[i - 1][j] == v && board[i - 2][j] == v)
                    break;
            if (i == 1)
                if (board[i - 1][j] == v && board[i + 1][j] == v || board[i + 1][j] == v && board[i + 2][j] == v)
                    break;
            if (i == m - 2)
                if (board[i - 1][j] == v && board[i + 1][j] == v || board[i - 1][j] == v && board[i - 2][j] == v)
                    break;
			
            if (j == 0)
                if (board[i][j + 1] == v && board[i][j + 2] == v)
                    break;
            if (j == m - 1)
                if (board[i][j - 1] == v && board[i][j - 2] == v)
                    break;
            if (j == 1)
                if (board[i][j - 1] == v && board[i][j + 1] == v || board[i][j + 1] == v && board[i][j + 2] == v)
                    break;
            if (j == m - 2)
                if (board[i][j - 1] == v && board[i][j + 1] == v || board[i][j - 1] == v && board[i][j - 2] == v)
                    break;
            if (i > 1 && i < m - 2 && j > 1 && j < m - 2)
                if (board[i - 1][j] == v && board[i + 1][j] == v || board[i + 1][j] == v && board[i + 2][j] == v || board[i - 1][j] == v && board[i - 2][j] == v || board[i][j - 1] == v && board[i][j + 1] == v || board[i][j + 1] == v && board[i][j + 2] == v || board[i][j - 1] == v && board[i][j - 2] == v)
                    break;
            board[i][j] = v; //不违反规则时填入一个随机数
            n--;
        } //格子为空
    }
    FILE* temp = fopen(".\\prototype.sud", "w");
    char path[20] = ".\\prototype.sud";
    fprintf(temp, "%d %d\n", m, k);
    for (int i = 0;i < m;i++)
    {
        for (int j = 0;j < m;j++)
            fprintf(temp, "%c", board[i][j]);
        fprintf(temp,"\n");
    } //记录数独种子
    fclose(temp);
	if(!readPuzzle(path))
		return NO;
	if (dpll())
    {
        FILE* fans = fopen(".\\ans.sud","w");
        fprintf(fans,"%d %d\n",m,m*m);
        for(int i = 1;i <= m*m;i++)
        {
            if(assign[i].type == TRUE)
                fprintf(fans,"%c",'1');
            else
                fprintf(fans,"%c",'0');
            if(i % m == 0)
                fprintf(fans,"\n");
        }
        fclose(fans);
		return YES;
    } //查看种子是否能得到一个解
	return NO;
}

int puzzleGen(int difficulty,int m)
{
	int k = m * m;
	while (lasVegas(5,m) != YES); //得到一个有解的数独种子
	int** board = (int**)malloc(sizeof(int*) * m);
	if (!board)
		exit(-1);
	for(int i = 0;i < m;i++)
	{
		board[i] = (int*)malloc(sizeof(int) * m);
		if (!board[i])
			exit(-1);
	}
	for(int i = 0;i < m;i++)
	{
		for(int j = 0;j < m;j++)
		{
			if (assign[i * m + j + 1].type == TRUE)
				board[i][j] = 49;
			else
				board[i][j] = 48;
		}
	}
	int** marked = (int**)malloc(sizeof(int*) * m);
	if (!marked)
		exit(-1);
	for (int i = 0;i < m;i++)
	{
		marked[i] = (int*)calloc(m, sizeof(int));
		if (!marked[i])
			exit(-1);
	}
	srand((unsigned)time(NULL));
	while (true)
	{
		int i = rand() % m;
		int j = rand() % m;
        if (marked[i][j] == 1)
            continue;
        marked[i][j] = 1; //随机选择一个没尝试过的格子
        if (k < m * m * 0.4)
			break; //给定一个空数上限
		if (board[i][j] == '1')
		{
			board[i][j] = '0';
            FILE* temp = fopen(".\\question.sud", "w");
            char path[80] = ".\\question.sud";
			fprintf(temp, "%d %d\n", m, k - 1);
			for (int i = 0;i < m;i++)
			{
				for (int j = 0;j < m;j++)
					fprintf(temp, "%c", board[i][j]);
				fprintf(temp, "\n");
			} //将这个位置替换成另一个赋值
			fclose(temp);
			if (!readPuzzle(path) || !dpll())
			{
				board[i][j] = '*';
                FILE* temp = fopen(".\\question.sud", "w");
				fprintf(temp, "%d %d\n", m, k - 1);
				for (int i = 0;i < m;i++)
				{
					for (int j = 0;j < m;j++)
						fprintf(temp, "%c", board[i][j]);
					fprintf(temp, "\n");
				}
				fclose(temp);
				k--;
			} //挖空后有唯一解，挖空
			else
				board[i][j] = '1'; //没有唯一解，不能挖空
		} //这个空是1
		else if (board[i][j] == '0')
		{
			board[i][j] = '1';
            FILE* temp = fopen(".\\question.sud", "w");
            char path[20] = ".\\question.sud";
			fprintf(temp, "%d %d\n", m, k - 1);
			for (int i = 0;i < m;i++)
			{
				for (int j = 0;j < m;j++)
					fprintf(temp, "%c", board[i][j]);
				fprintf(temp, "\n");
			} //替换为另一个赋值
			fclose(temp);
			if (!readPuzzle(path) || !dpll())
			{
				board[i][j] = '*';
                FILE* temp = fopen(".\\question.sud", "w");
				fprintf(temp, "%d %d\n", m, k - 1);
				for (int i = 0;i < m;i++)
				{
					for (int j = 0;j < m;j++)
						fprintf(temp, "%c", board[i][j]);
					fprintf(temp, "\n");
				}
				fclose(temp);
				k--;
			} //有唯一解，可以挖空
			else
				board[i][j] = '0'; //没有唯一解，不能挖空
		} //是0
	}
}
