#ifndef __BALL_TREE_H
#define __BALL_TREE_H

#define N0 20

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
