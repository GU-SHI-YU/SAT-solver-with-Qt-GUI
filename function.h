#pragma once
static void setVar(int v); //更新变元v的值
static void unSetVar(int v); //撤回变元v的值
static int dpll(); //DPLL算法
static void init(); //初始化求解器

static int compare(const void* a, const void* b); //按变元编号比较大小
static int computeResolvent(int x, int a, int b, int& len, int limit); //计算解
static int getRestrictedResolvent(int x, int limit); //获得解
static int subsumable(int j, int k); //判断子句是否包含于另一个子句
static int preprocessSubsume(); //处理包含子句
static int preprocessUnitPropagation(); //预处理单子句传播
static void preprocessMonotoneLiteralFixing(); //预处理单调变元
static int preprocessor(); //预处理
static int clausePresent(int C[], int n); //检测子句是否重复
static int addClause(int C[], int n); //将子句保存到公式中
static int reader(char* path); //读取文件

inline int getLiteralDLCS(); //按变元出现次数选择
inline int getShortestLen();
static void getMOMS(int x, int k,unsigned int& s, unsigned int& t);
inline int getLiteralMinLen();
inline int getLiteral2SJW();
inline int getFirst();

static int readPuzzle(char* path);
static void combine(int* res, int i, int n, int k, int size, int type);
static int lasVegas(int n,int m);
static int puzzleGen(int difficulty,int m);