#ifndef __BALL_TREE_H
#define __BALL_TREE_H

#define N0 20

class Node{
public:
    int index;
    double radius;
    Node* left;
    Node* right;
    float** data = nullptr;
};

class BallTree {
public:
    Node* root;

	BallTree();
	~BallTree();

	bool buildTree(
		int n,
		int d,
		float** data);

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
