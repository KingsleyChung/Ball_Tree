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
    int* index;              //���
    int dataCount;          //�˷�֧��������
    int dimension;          //ά��
    float* center;          //Բ������
    float radius;           //�뾶
    Node* left;             //������
    Node* right;            //������
    float** data;           //����Ҷ�ӽڵ㣬��������
	int pageNumer;			//��Ӧ���ݵ�ҳ��	
	int slot;				//��Ӧ���ݵĲۺ�	

    Node(int* _index = nullptr, int _dataCount = -1, int _dimension = -1, float* _center = nullptr, float _radius = 0, int PageNumer = -1, int Slot = -1) {       //���캯��
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
	//float bufferPage[16384];//����ҳ
	//long flag;//���д��±�

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

    void buildSubTree(Node* &subroot, int* index, int n, int d, float** &data);                     //����������
    void MakeBallTreeSplit(float* &A, float* &B, Node* &subroot, int n, int d, float** &data);      //����
    float* FindFurthestData(float* &x, float** &data, int n, int d);                                //Ѱ�Ҿ�����Զ������
    int CloserTo(float* &selectedData, float* &A, float* &B, int d);                                //�Ƚ�ѡ�������A��B�ĸ��ȽϽ�������A�Ͻ������A��B��ȷ���1����������2
    float* FindCenter(float** &data, int n, int d);                                                 //����Բ��
    float DistanceBetween(float* &pointA, float* &pointB, int d);                                   //�����������ľ���

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
