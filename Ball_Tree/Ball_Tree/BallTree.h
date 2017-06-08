#ifndef __BALL_TREE_H
#define __BALL_TREE_H

#include <stdio.h>
#include <iostream>
#include <queue>
#include <fstream>
using namespace std;//for testing
#define N0 20

class Node{
public:
    int* index;              //序号
    int dataCount;          //此分支的数据量
    int dimension;          //维度
    float* center;          //圆心向量
    float radius;           //半径
    Node* left;             //左子树
    Node* right;            //右子树
    float** data;           //若是叶子节点，储存数据
	int pageNumer;			//对应数据的页号	
	int slot;				//对应数据的槽号	

    Node(int* _index = nullptr, int _dataCount = -1, int _dimension = -1, float* _center = nullptr, float _radius = 0, int PageNumer = -1, int Slot = -1) {       //构造函数
        index = _index;
        dataCount = _dataCount;
        dimension = _dimension;
        center = _center;
        radius = _radius;
        left = nullptr;
        right = nullptr;
        data = nullptr;
		pageNumer = PageNumer;
		slot = Slot;
    }
};

class BallTree {
public:
    int indexCount;
	int dataFileIndex = 0;
    Node* root;
	queue<Node> qu;
	float *data;
	//float bufferPage[16384];//缓冲页
	//long flag;//空闲处下标

    BallTree() {
        root = nullptr;
    }
    ~BallTree() {}

	int storeData(float ** data, int firstDimension, int secondDimension);
	float *readData(int pageNumer, int slot,int d);

	bool buildTree(
		int n,
		int d,
		float** data);

    void buildSubTree(Node* &subroot, int* index, int n, int d, float** &data);                     //建立二叉树
    void MakeBallTreeSplit(float* &A, float* &B, Node* &subroot, int n, int d, float** &data);      //分裂
    float* FindFurthestData(float* &x, float** &data, int n, int d);                                //寻找距离最远的数据
    int CloserTo(float* &selectedData, float* &A, float* &B, int d);                                //比较选定点距离A、B哪个比较近，距离A较近或距离A、B相等返回1，其他返回2
    float* FindCenter(float** &data, int n, int d);                                                 //计算圆心
    float DistanceBetween(float* &pointA, float* &pointB, int d);                                   //计算两向量的距离

	bool storeTree(
		const char* index_path);

	bool restoreTree(
		const char* index_path,
		int d);
	
	int mipSearch(
		int d,
		float* query);

	// optional
	bool insertData(
		int d,
		float* data);

	// optional
	bool deleteData(
		int d,
		float* data);

	// optional
	bool buildQuadTree(
		int n,
		int d,
		float** data);

	Node *  findPoint(int * index);

	void DFS(int d, Node* p, float* query);
};

#endif
