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
    //���������С��N0����ڵ�ΪҶ�ӽڵ�
    if (n < N0) {
        subroot = new Node(index, n, d, nullptr, 0);
        //��������
        subroot->data = new float*[n];
        for (int i = 0; i < n; i++) {
            subroot->data[i] = new float[d];
            for (int j = 0; j <= d; j++) {
                subroot->data[i][j] = data[i][j];
            }
        }
        return;
    }
    //���ѡȡһ������x
    srand((unsigned)time(NULL));
    int randomNum = rand() % n;
    float* x = data[randomNum];
    //cout << randomNum << endl;//for testing
    //�ҵ�����x��Զ������A
    float* A = FindFurthestData(x, data, n, d);
    //�ҵ�����A��Զ������B
    float* B = FindFurthestData(A, data, n, d);
    //��¼�ڵ���Ϣ
    float* center = FindCenter(data, n, d);
    //printVector(center, d);//for testing
    float* furthestDataFromCenter = FindFurthestData(center, data, n, d);
    float radius = DistanceBetween(center, furthestDataFromCenter, d);
    subroot = new Node(index, n, d, center, radius);
    //������
    MakeBallTreeSplit(A, B, subroot, n, d, data);
}
void BallTree::MakeBallTreeSplit(float* &A, float* &B, Node* &subroot, int n, int d, float** &data) {
    //��������
    int dataCountOfLeft = 0, dataCountOfRight = 0;      //��¼���ҽڵ����ݸ���
    float** dataOfLeft = new float*[n];                 //��¼��ڵ�����
    float** dataOfRight = new float*[n];                //��¼�ҽڵ�����
    for (int i = 0; i < n; i++) {
        if (CloserTo(data[i], A, B, d) == 1) {          //����A�Ͻ������A��B���
            dataOfLeft[dataCountOfLeft] = data[i];
            dataCountOfLeft++;
        }
        else {
            dataOfRight[dataCountOfRight] = data[i];
            dataCountOfRight++;
        }
    }
    //����������
    //for (int i = 0; i < dataCountOfLeft; i++) {//for testing
    //    printVector(dataOfLeft[i], d);
    //}
    buildSubTree(subroot->left, subroot->index * 2, dataCountOfLeft, d, dataOfLeft);
    //����������
    //for (int i = 0; i < dataCountOfRight; i++) {//for testing
    //    printVector(dataOfRight[i], d);
    //}
    buildSubTree(subroot->right, subroot->index * 2 + 1, dataCountOfRight, d, dataOfRight);
}
float* BallTree::FindFurthestData(float* &x, float** &data, int n, int d) {
    float furthestDistance = 0;             //��¼��Զ�����ƽ��
    int furthestDataPos = 0;                //��¼��Զ���������������е�λ��
    for (int i = 0; i < n; i++) {
        float result = 0;                   //��¼�˴α������ݾ����ƽ��
        for (int j = 1; j <= d; j++) {
            result += (x[j] - data[i][j]) * (x[j] - data[i][j]);
        }
        if (result > furthestDistance) {    //����������֮ǰ��¼����Զ����
            furthestDistance = result;
            furthestDataPos = i;
        }
    }
    //cout << furthestDataPos << endl;//for testing
    return data[furthestDataPos];           //������Զ��������
}

int BallTree::CloserTo(float* &selectedData, float* &A, float* &B, int d) {
    float distanceToA = 0;                  //ѡ�����ݵ�A�ľ���
    float distanceToB = 0;                  //ѡ�����ݵ�B�ľ���
    for (int j = 1; j <= d; j++) {
        distanceToA += (A[j] - selectedData[j]) * (A[j] - selectedData[j]);
        distanceToB += (B[j] - selectedData[j]) * (B[j] - selectedData[j]);
    }
    return distanceToA <= distanceToB ? 1 : 2;//��A�Ͻ���A��B������ȷ���1����������2
}

float* BallTree::FindCenter(float** &data, int n, int d) {
    float* center = new float[d];           //Բ������
    for (int j = 0; j < d; j++) {           //������ÿ��ά��
        float sumOfOneDimension = 0;        //һ��ά�ȵĺ�
        for (int i = 0; i < n; i++) {       //ÿ������
            sumOfOneDimension += data[i][j];
        }
        center[j] = sumOfOneDimension / (n + 0.0);//ƽ��ֵ
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

//dataΪ���ݼ���nΪ���ݼ��Ĵ�С
//string BallTree::storeData(float ** data, int firstDimension, int secondDimension) {
//	string result = floatToString(data[0][0]);
//	string fileName = intToString(this->dataFileIndex);
//	ofstream file((fileName + ".txt"), ios::binary|ios::app|ios::out);
//	file.seekp(0, ios::end);
//	streampos ps = file.tellp();
//	//��ȡ��ǰ���ļ��ֽ���
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
//		//д��������ļ�
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