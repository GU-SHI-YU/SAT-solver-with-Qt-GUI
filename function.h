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
inline int getShortestLen(); //获得最短子句长
static void getMOMS(int x, int k,unsigned int& s, unsigned int& t); //获得MOMS权值
inline int getLiteralMinLen(); //按最短子句选择
inline int getLiteral2SJW(); //按2-Sided-J-W规则选择
inline int getFirst(); //选择第一个未赋值的变元

static int readPuzzle(char* path); //读取数独
static void combine(int* res, int i, int n, int k, int size, int type); //求组合数
static int lasVegas(int n,int m); //随机生成数独种子
static int puzzleGen(int difficulty,int m); //生成唯一解数独