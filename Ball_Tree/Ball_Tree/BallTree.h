#ifndef __BALL_TREE_H
#define __BALL_TREE_H

#define N0 20

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

    Node(int _index, int _dataCount, int _dimension, float* _center, float _radius) {
        index = _index;
        dataCount = _dataCount;
        dimension = _dimension;
        center = _center;
        radius = _radius;
        left = nullptr;
        right = nullptr;
        data = nullptr;
    }
};

class BallTree {
public:
    Node* root;

    BallTree() {
        root = nullptr;
    }
    ~BallTree() {}

	bool buildTree(
		int n,
		int d,
		float** data);

    void buildSubTree(Node* subroot, int previousIndex, int n, int d, float** data);
    void MakeBallTreeSplit(float* A, float* B, Node* subroot, int n, int d, float** data);
    float* FindFurthestData(float* x, float** &data, int n, int d);
    int CloserTo(float* selectedData, float* A, float* B, int d);
    float* FindCenter(float** data, int n, int d);
    float DistanceBetween(float* center, float* point, int d);

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
