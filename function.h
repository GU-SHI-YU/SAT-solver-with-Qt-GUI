#pragma once
static void setVar(int v); //���±�Ԫv��ֵ
static void unSetVar(int v); //���ر�Ԫv��ֵ
static int dpll(); //DPLL�㷨
static void init(); //��ʼ�������

static int compare(const void* a, const void* b); //����Ԫ��űȽϴ�С
static int computeResolvent(int x, int a, int b, int& len, int limit); //�����
static int getRestrictedResolvent(int x, int limit); //��ý�
static int subsumable(int j, int k); //�ж��Ӿ��Ƿ��������һ���Ӿ�
static int preprocessSubsume(); //��������Ӿ�
static int preprocessUnitPropagation(); //Ԥ�����Ӿ䴫��
static void preprocessMonotoneLiteralFixing(); //Ԥ��������Ԫ
static int preprocessor(); //Ԥ����
static int clausePresent(int C[], int n); //����Ӿ��Ƿ��ظ�
static int addClause(int C[], int n); //���Ӿ䱣�浽��ʽ��
static int reader(char* path); //��ȡ�ļ�

inline int getLiteralDLCS(); //����Ԫ���ִ���ѡ��
inline int getShortestLen();
static void getMOMS(int x, int k,unsigned int& s, unsigned int& t);
inline int getLiteralMinLen();
inline int getLiteral2SJW();
inline int getFirst();

static int readPuzzle(char* path);
static void combine(int* res, int i, int n, int k, int size, int type);
static int lasVegas(int n,int m);
static int puzzleGen(int difficulty,int m);