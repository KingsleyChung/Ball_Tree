#include "BallTree.h"
#include "Utility.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include<string>
#include<queue>
#include<fstream>
#include <iostream> // for testing
#include<sstream>
using namespace std; // for testing
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

//data为数据集，n为数据集的大小
//string BallTree::storeData(float ** data, int firstDimension, int secondDimension) {
//	string result = floatToString(data[0][0]);
//	string fileName = intToString(this->dataFileIndex);
//	ofstream file((fileName + ".txt"), ios::binary|ios::app|ios::out);
//	file.seekp(0, ios::end);
//	streampos ps = file.tellp();
//	//获取当前的文件字节数
//	int size = (int)ps;
//	if (ps > 1024 * 64) {
//		file.close();
//		++dataFileIndex;
//		fileName = intToString(this->dataFileIndex);
//		file.open((fileName + ".txt"), ios::binary | ios::app | ios::out);
//	}
//	result += " " + intToString(this->dataFileIndex);
//	for (int i = 0; i < firstDimension; i++) {
//		string str;
//		for (int j = 0; j < secondDimension; j++) {
//			str += (floatToString(data[i][j]) + " ");
//			file.write(str.c_str(), sizeof(char)*(str.size()));
//			streampos ps = file.tellp();
//			if (ps > 1024 * 64) {
//				file.close();
//				++dataFileIndex;
//				fileName = intToString(this->dataFileIndex);
//				file.open((fileName + ".txt"), ios::binary | ios::app | ios::out);
//			}
//		}
//		str += "\n";
//	}
//	file.close();
//	return result;
//}
//
//bool BallTree::storeTree(const char* index_path) {
//	string indexFilePath(index_path);
//	//ofstream file(indexFilePath, ios::binary);
//	ofstream file("index.txt", ios::binary);
//	if (!file.is_open()) {
//		cout << "cannot open the file\n";
//		return false;
//	}
//	//queue<Node> qu;
//	qu.push((*(this->root)));
//	while (!qu.empty()) {
//		string st;
//		//写入二进制文件
//		string content = intToString(qu.front().index) + " " + intToString(qu.front().dataCount) + " " + intToString(qu.front().dimension) + " (" + floatToString(qu.front().center[0]) + "," + floatToString(qu.front().center[1]) + ") " + floatToString(qu.front().radius) + " ";
//		if (qu.front().dataCount <= 20) {
//			st = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
//		}
//		content += st;
//		content += "\n";
//		qu.push(*(qu.front().left));
//		qu.push(*(qu.front().right));
//		qu.pop();
//	}
//	file.close();
//	return true;
//}