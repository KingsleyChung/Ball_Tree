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
//storeData返回的是槽号, 参数一：data数组，参数二：数组中有多少列，参数三：列中有多少个属性
int BallTree::storeData(float ** data, int firstDimension, int secondDimension) {
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
	int slot_num = (size / 4) / N0;
	++slot_num;
	result += " " + intToString(this->dataFileIndex);
	for (int i = 0; i < N0; i++) {
		if (i < firstDimension) {
			for (int j = 0; j < secondDimension; j++) {
				float num = data[i][j];
				file.write((char*)&(num), sizeof(float));
			}
		}
		else {
			for (int j = 0; j < secondDimension; j++) {
				float num = 0;
				file.write((char*)&(num), sizeof(float));
			}
		}
	}
	file.close();
	return slot_num;
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
			int * arr = new int[qu.front().dimension];
			for (int i = 0; i < qu.front().dimension; i++) {
				arr[i] = 0;
			}
			//center1 = 0;
			//center2 = 0;
			radius = 0;
			file.write((char*)&index, sizeof(int));
			file.write((char*)&dataCount, sizeof(int));
			file.write((char*)&dimension, sizeof(int));
			//file.write((char*)&center1, sizeof(float));
			for (int i = 0; i < qu.front().dimension; i++) {
				file.write((char*)&arr[i], sizeof(float));
			}
			file.write((char*)&radius, sizeof(float));
			st = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
			int pageid = this->dataFileIndex;
			file.write((char*)&pageid, sizeof(int));
			int slotid = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
			file.write((char*)&slotid, sizeof(int));
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
			//center1 = qu.front().center[0];
			//file.write((char*)&center1, sizeof(float));
			//center2 = qu.front().center[1];
			//file.write((char*)&center2, sizeof(float));
			float * arr = new float[qu.front().dimension];
			for (int i = 0; i < qu.front().dimension; i++) {
				arr[i] = qu.front().center[i];
				file.write((char*)&arr[i], sizeof(float));
			}
			radius = qu.front().radius;
			file.write((char*)&radius, sizeof(float));
			float num = 0, num2 = 0;
			file.write((char*)&num, sizeof(float));
			file.write((char*)&num2, sizeof(float));
			qu.push(*(qu.front().left));
			qu.push(*(qu.front().right));
		}
		qu.pop();
	}
	file.close();
	return true;
}

int *BallTree::readData(int pageNumer, int slot,int d) {
	float bufferPage[16384];
	//缓冲页
	ifstream file;
	string pagename = intToString(pageNumer);
	pagename += ".txt";
	file.open(pagename, ios::binary);
	if (!file.is_open()) {
		cout << "cannot open the file\n";
		return false;
	}
	//根据页号找到txt文件
	for (int i = 0; i <= 16384; i++) {
	file.read((char*)&bufferPage[i],sizeof bufferPage[i])
    }
	//读入所有的页面
	int len = N0*(d + 1);
	int *arr=new int[len - 1];
	for (int i = 0; i <= len - 1; i++)
		arr[i] = bufferPage[(slot- 1)*len + i];
	//根据槽号找到数据
	file.close();
	return arr;
}

bool BallTree::restoreTree(const char* index_path, int d) {
	string indexFilePath(index_path);
	//ofstream file(indexFilePath, ios::binary);
	ifstream infile("index.txt", ios::binary);
	if (!infile.is_open()) {
		cout << "cannot open the file\n";
		return false;
	}
	infile.(0, ios::end);
	//queue<Node> qu;
	qu.push((*(this->root)));
	int pageNumer = this->dataFileIndex;
	int slot = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
	data[] = readData(pageNumer, slot, d);
	infile.close();
	return true;
}

int BallTree::mipSearch(int d, float* query) {

}

bool BallTree::insertData(int d, float* data) {
	//中心点的选取
}

bool BallTree::deleteData(int d, float* data) {
	if (DistanceBetween(root->center, data, d) >= root->radius)
		return false;
	Node * current_root = root;
	//找到数据之后再每个节点datacount--;
	while (current_root->dataCount > N0) {
		if (current_root->left != nullptr && DistanceBetween(current_root->left->center, data, d) <= current_root->left->radius)
			current_root = current_root->left;
		else if (current_root->right != nullptr && DistanceBetween(current_root->right->center, data, d) <= current_root->right->radius)
			current_root = current_root->right;
		else
			return false;
	}
		
}
