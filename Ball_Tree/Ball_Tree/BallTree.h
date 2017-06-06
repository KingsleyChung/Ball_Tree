#ifndef __BALL_TREE_H
#define __BALL_TREE_H

#include <stdio.h>
#include <iostream>
#include <queue>
using namespace std;//for testing
#define N0 3

class Node{
public:
    int index;              //序号
    int dataCount;          //此分支的数据量
    int dimension;          //维度
    float* center;          //圆心向量
    float radius;           //半径
    Node* left;             //左子树
    Node* right;            //右子树
    float** data;           //若是叶子节点，储存数据
    int page_id;            //页号
    int slot_id;            //槽号

    Node(int _index, int _dataCount, int _dimension, float* _center, float _radius, int _page_id = -1, int _slot_id = -1) {       //构造函数
        index = _index;
        dataCount = _dataCount;
        dimension = _dimension;
        center = _center;
        radius = _radius;
        left = nullptr;
        right = nullptr;
        data = nullptr;
        page_id = _page_id;
        slot_id = _slot_id;
    }
};

class BallTree {
public:
	int dataFileIndex = 0;
    Node* root;
	queue<Node> qu;

    BallTree() {
        root = nullptr;
    }
    ~BallTree() {}

	int storeData(float ** data, int firstDimension, int secondDimension);

	bool buildTree(
		int n,
		int d,
		float** data);

    void buildSubTree(Node* &subroot, int previousIndex, int n, int d, float** &data);              //建立二叉树
    void MakeBallTreeSplit(float* &A, float* &B, Node* &subroot, int n, int d, float** &data);      //分裂
    float* FindFurthestData(float* &x, float** &data, int n, int d);                                //寻找距离最远的数据
    int CloserTo(float* &selectedData, float* &A, float* &B, int d);                                //比较选定点距离A、B哪个比较近，距离A较近或距离A、B相等返回1，其他返回2
    float* FindCenter(float** &data, int n, int d);                                                 //计算圆心
    float DistanceBetween(float* &pointA, float* &pointB, int d);                                   //计算两向量的距离
    void preorderPrint(int d) {// for testing                                                       //先序遍历并打印相关信息（测试用）
        preorderTesting(root, d);
    }
    void preorderTesting(Node* subroot, int d) { // for testing                                     //先序遍历并打印相关信息（测试用）
        if (subroot == nullptr) {
            return;
        }
        if (subroot->data != nullptr) {
            printf("index:%d dataCount:%d\n", subroot->index, subroot->dataCount);
            for (int i = 0; i < subroot->dataCount; i++) {
                printVector(subroot->data[i], d);
            }
            return;
        }
        printf("center:");
        printVector(subroot->center, d);
        printf("index:%d radius:%f dataCount:%d left:%d right:%d\n", subroot->index, subroot->radius, subroot->dataCount, subroot->left->index, subroot->right->index);
        preorderTesting(subroot->left, d);
        preorderTesting(subroot->right, d);
    }
    void printVector(float* data, int d) {//for testing                                            //将给定向量按照(v1, v2, v3)格式打印（测试用）
        cout << "(";
        for (int i = 0; i < d - 1; i++) {
            cout << data[i] << ", ";
        }
        cout << data[d - 1] << ")\n";
    }

	bool storeTree(
		const char* index_path);

	bool restoreTree(
		const char* index_path);
	
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
};

#endif
