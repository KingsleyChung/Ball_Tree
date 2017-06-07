#ifndef __BALL_TREE_H
#define __BALL_TREE_H

#include <stdio.h>
#include <iostream>
#include <queue>
using namespace std;//for testing
#define N0 3

class Node{
public:
    int index;              //���
    int dataCount;          //�˷�֧��������
    int dimension;          //ά��
    float* center;          //Բ������
    float radius;           //�뾶
    Node* left;             //������
    Node* right;            //������
    float** data;           //����Ҷ�ӽڵ㣬��������
	int pageNumer;			//��Ӧ���ݵ�ҳ��	
	int slot;				//��Ӧ���ݵĲۺ�	

    Node(int _index = -1, int _dataCount = -1, int _dimension = -1, float* _center = nullptr, float _radius = 0, int PageNumer = -1, int Slot = -1) {       //���캯��
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
	int dataFileIndex = 0;
    Node* root;
	queue<Node> qu;
	int *data;
	//float bufferPage[16384];//����ҳ
	//long flag;//���д��±�

    BallTree() {
        root = nullptr;
    }
    ~BallTree() {}

	int storeData(float ** data, int firstDimension, int secondDimension);
	int *readData(int pageNumer, int slot,int d);

	bool buildTree(
		int n,
		int d,
		float** data);

    void buildSubTree(Node* &subroot, int previousIndex, int n, int d, float** &data);              //����������
    void MakeBallTreeSplit(float* &A, float* &B, Node* &subroot, int n, int d, float** &data);      //����
    float* FindFurthestData(float* &x, float** &data, int n, int d);                                //Ѱ�Ҿ�����Զ������
    int CloserTo(float* &selectedData, float* &A, float* &B, int d);                                //�Ƚ�ѡ�������A��B�ĸ��ȽϽ�������A�Ͻ������A��B��ȷ���1����������2
    float* FindCenter(float** &data, int n, int d);                                                 //����Բ��
    float DistanceBetween(float* &pointA, float* &pointB, int d);                                   //�����������ľ���
    void preorderPrint(int d) {// for testing                                                       //�����������ӡ�����Ϣ�������ã�
        preorderTesting(root, d);
    }
    void preorderTesting(Node* subroot, int d) { // for testing                                     //�����������ӡ�����Ϣ�������ã�
        if (subroot == nullptr) {
            return;
        }
        if (subroot->data != nullptr) {
            //printf("index:%d dataCount:%d\n", subroot->index, subroot->dataCount);
            //for (int i = 0; i < subroot->dataCount; i++) {
            //    printVector(subroot->data[i], d);
            //}
            return;
        }
		if (subroot->dataCount < N0)
			return;
        //printf("center:");
        //printVector(subroot->center, d);
		float a = subroot->radius;
        printf("index:%d radius:%f dataCount:%d left:%d right:%d\n", subroot->index, subroot->radius, subroot->dataCount, subroot->left->index, subroot->right->index);
		//printf("index:%d radius:%f dataCount:%d left:%d right:%d\n", subroot->index, subroot->radius, subroot->dataCount, subroot->left->index, subroot->right->index);
		preorderTesting(subroot->left, d);
        preorderTesting(subroot->right, d);
    }

    void printVector(float* data, int d) {//for testing                                            //��������������(v1, v2, v3)��ʽ��ӡ�������ã�
        cout << "[" << data[0] << "]";
        cout << "(";
        for (int i = 1; i < d; i++) {
            cout << data[i] << ", ";
        }
        cout << data[d] << ")\n";
    }

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

	//void readPage(int PageNumber);
	Node *  findPoint(int index);

	void DFS(int d, Node* p, float* query);
};

#endif
