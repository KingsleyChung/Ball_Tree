#include "BallTree.h"
#include "Utility.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include<string>
#include<queue>
#include<fstream>
#include<vector>
#include <iostream> // for testing
#include<sstream>
using namespace std; // for testing

int currentSum = 0;//查询时保存当前的最大内积

string intToString(int num) {
	stringstream ss;
	ss << num;
	return ss.str();
}

string floatToString(float num) {
	stringstream ss;
	ss << num;
	return ss.str();
}

bool BallTree::buildTree(int n, int d, float** data) {
    buildSubTree(root, 1, n, d, data);
    return true;
}

void BallTree::buildSubTree(Node* &subroot, int index, int n, int d, float** &data) {
    //如果数据量小于N0，则节点为叶子节点
    if (n < N0) {
        subroot = new Node(index, n, d, nullptr, 0);
        //复制数据
        subroot->data = new float*[n];
        for (int i = 0; i < n; i++) {
            subroot->data[i] = new float[d];
            for (int j = 0; j <= d; j++) {
                subroot->data[i][j] = data[i][j];
            }
        }
        return;
    }
    //随机选取一个数据x
    srand((unsigned)time(NULL));
    int randomNum = rand() % n;
    float* x = data[randomNum];
    //cout << randomNum << endl;//for testing
    //找到距离x最远的数据A
    float* A = FindFurthestData(x, data, n, d);
    //找到距离A最远的数据B
    float* B = FindFurthestData(A, data, n, d);
    //记录节点信息
    float* center = FindCenter(data, n, d);
    //printVector(center, d);//for testing
    float* furthestDataFromCenter = FindFurthestData(center, data, n, d);
    float radius = DistanceBetween(center, furthestDataFromCenter, d);
    subroot = new Node(index, n, d, center, radius);
    //分裂树
    MakeBallTreeSplit(A, B, subroot, n, d, data);
}

void BallTree::MakeBallTreeSplit(float* &A, float* &B, Node* &subroot, int n, int d, float** &data) {
    //声明变量
    int dataCountOfLeft = 0, dataCountOfRight = 0;      //记录左右节点数据个数
    float** dataOfLeft = new float*[n];                 //记录左节点数据
    float** dataOfRight = new float*[n];                //记录右节点数据
    for (int i = 0; i < n; i++) {
        if (CloserTo(data[i], A, B, d) == 1) {          //距离A较近或距离A、B相等
            dataOfLeft[dataCountOfLeft] = data[i];
            dataCountOfLeft++;
        }
        else {
            dataOfRight[dataCountOfRight] = data[i];
            dataCountOfRight++;
        }
    }
    //建立左子树
    //for (int i = 0; i < dataCountOfLeft; i++) {//for testing
    //    printVector(dataOfLeft[i], d);
    //}
    buildSubTree(subroot->left, subroot->index * 2, dataCountOfLeft, d, dataOfLeft);
    //建立右子树
    //for (int i = 0; i < dataCountOfRight; i++) {//for testing
    //    printVector(dataOfRight[i], d);
    //}
    buildSubTree(subroot->right, subroot->index * 2 + 1, dataCountOfRight, d, dataOfRight);
}

float* BallTree::FindFurthestData(float* &x, float** &data, int n, int d) {
    float furthestDistance = 0;             //记录最远距离的平方
    int furthestDataPos = 0;                //记录最远距离数据在数组中的位置
    for (int i = 0; i < n; i++) {
        float result = 0;                   //记录此次遍历数据距离的平方
        for (int j = 1; j <= d; j++) {
            result += (x[j] - data[i][j]) * (x[j] - data[i][j]);
        }
        if (result > furthestDistance) {    //如果距离大于之前记录的最远距离
            furthestDistance = result;
            furthestDataPos = i;
        }
    }
    //cout << furthestDataPos << endl;//for testing
    return data[furthestDataPos];           //返回最远距离数据
}

int BallTree::CloserTo(float* &selectedData, float* &A, float* &B, int d) {
    float distanceToA = 0;                  //选中数据到A的距离
    float distanceToB = 0;                  //选中数据到B的距离
    for (int j = 1; j <= d; j++) {
        distanceToA += (A[j] - selectedData[j]) * (A[j] - selectedData[j]);
        distanceToB += (B[j] - selectedData[j]) * (B[j] - selectedData[j]);
    }
    return distanceToA <= distanceToB ? 1 : 2;//离A较近或到A、B距离相等返回1，其他返回2
}

float* BallTree::FindCenter(float** &data, int n, int d) {
    float* center = new float[d];           //圆心向量
    for (int j = 0; j < d; j++) {           //向量的每个维度
        float sumOfOneDimension = 0;        //一个维度的和
        for (int i = 0; i < n; i++) {       //每个向量
            sumOfOneDimension += data[i][j];
        }
        center[j] = sumOfOneDimension / (n + 0.0);//平均值
    }
    return center;
}

float BallTree::DistanceBetween(float* &pointA, float* &pointB, int d) {
    float totalDistanceSquare = 0;
    for (int i = 1; i <= d; i++) {
        totalDistanceSquare += (pointA[i] - pointB[i]) * (pointA[i] - pointB[i]);
    }
    //cout << sqrt(totalDistanceSquare) << endl;//for testing
    return sqrt(totalDistanceSquare);
}

int BallTree::storeData(float ** data, int firstDimension, int secondDimension) {
	int haha;
	string result = floatToString(data[0][0]);
	string fileName = intToString(this->dataFileIndex);
	ofstream file((fileName + ".txt"), ios::binary | ios::app | ios::out);
	//int num = 12;
	//file.write((char*)&num, sizeof(int));
	file.seekp(0, ios::end);

	streampos ps = file.tellp();
	//获取当前的文件字节数
	int size = (int)ps;
	if (size > 1024 * 64) {
		file.close();
		++dataFileIndex;
		fileName = intToString(this->dataFileIndex);
		file.open((fileName + ".txt"), ios::binary | ios::app | ios::out);
	}
	result += " " + intToString(this->dataFileIndex);
	haha = this->dataFileIndex;
	for (int i = 0; i < firstDimension; i++) {
		string str;
		for (int j = 0; j < secondDimension; j++) {
			float num = data[i][j];
			file.write((char*)&(num), sizeof(float));
		}
		streampos ps = file.tellp();
		size = (int)ps;
		if (size > 1024 * 64) {
			file.close();
			++dataFileIndex;
			fileName = intToString(this->dataFileIndex);
			file.open((fileName + ".txt"), ios::binary | ios::app | ios::out);
		}
	}
	file.close();
	return haha;
}

bool BallTree::storeTree(const char* index_path) {
	string indexFilePath(index_path);
	//ofstream file(indexFilePath, ios::binary);
	ofstream file("index.txt", ios::binary | ios::app | ios::out);
	if (!file.is_open()) {
		cout << "cannot open the file\n";
		return false;
	}
	file.seekp(0, ios::end);
	//queue<Node> qu;
	qu.push((*(this->root)));
	while (!qu.empty()) {
		string st;
		string content;
		if (qu.front().dataCount < 3) {
			int index, dataCount, dimension;
			float center1, center2, radius;
			index = qu.front().index;
			dataCount = qu.front().dataCount;
			dimension = qu.front().dimension;
			center1 = 0;
			center2 = 0;
			radius = 0;
			file.write((char*)&index, sizeof(int));
			file.write((char*)&dataCount, sizeof(int));
			file.write((char*)&dimension, sizeof(int));
			file.write((char*)&center1, sizeof(float));
			file.write((char*)&center2, sizeof(float));
			file.write((char*)&radius, sizeof(float));
			st = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
			int dataFirstIndex = qu.front().data[0][0];
			file.write((char*)&dataFirstIndex, sizeof(int));
			int pageid = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
			file.write((char*)&pageid, sizeof(int));
			content += st;
		}
		else {
			int index, dataCount, dimension;
			float center1, center2, radius;
			index = qu.front().index;
			file.write((char*)&index, sizeof(int));
			dataCount = qu.front().dataCount;
			file.write((char*)&dataCount, sizeof(int));
			dimension = qu.front().dimension;
			file.write((char*)&dimension, sizeof(int));
			center1 = qu.front().center[0];
			file.write((char*)&center1, sizeof(float));
			center2 = qu.front().center[1];
			file.write((char*)&center2, sizeof(float));
			radius = qu.front().radius;
			file.write((char*)&radius, sizeof(float));
			qu.push(*(qu.front().left));
			qu.push(*(qu.front().right));
		}
		qu.pop();
	}
	file.close();
	return true;
}
/*
int mipSearch(int d, float* query) {
	IndexTree* root = indexTree->getroot();
	DFS(d, root, query);
}

int DFS(int d, IndexTree* p, float* query) {
	if (p == NULL) return;
	if (p->getDataCount() <= 20) {
		//get 20 data 
		int pid = p->getPid();//页号
		int sid = p->getSid();//槽号
		if (currentSum > 0) {
			int sum = 0;
			float radius = p->getRadius();
			float* center = p->getCenter();
			for (int i = 0; i < d; i++) {
				sum += radius * center[i];
			}
			if (sum <= currentSum) return;
		}
		vector<Data> vec = getData(pid, sid);
		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < d; j++) {
				
			}
		}
	}
	if (p->left != NULL)
		DFS(d, p->left, query);
	if (p->right != NULL)
		DFS(d, p->right, query);
}
*/