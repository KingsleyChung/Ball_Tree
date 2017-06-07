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

int currentSum = 0;//��ѯʱ���浱ǰ������ڻ�

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
    printVector(center, d);
    printVector(furthestDataFromCenter, d);
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
    float* center = new float[d + 1];           //Բ������
    for (int j = 1; j <= d; j++) {          //������ÿ��ά��
        float sumOfOneDimension = 0;        //һ��ά�ȵĺ�
        for (int i = 0; i < n; i++) {       //ÿ������
            sumOfOneDimension += data[i][j];
        }
        center[j] = sumOfOneDimension / (n + 0.0);//ƽ��ֵ
    }
    center[0] = -1;
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

//storeData���ص��ǲۺ�, ����һ��data���飬���������������ж����У��������������ж��ٸ�����
int BallTree::storeData(float ** data, int firstDimension, int secondDimension) {
	string fileName = intToString(this->dataFileIndex);
	ofstream file((fileName + ".txt"), ios::binary | ios::app | ios::out);
	file.seekp(0, ios::end);

	streampos ps = file.tellp();
	//��ȡ��ǰ���ļ��ֽ���
	int size = (int)ps;
	if (size > 1024 * 64) {
		file.close();
		++dataFileIndex;
		fileName = intToString(this->dataFileIndex);
		file.open((fileName + ".txt"), ios::binary | ios::app | ios::out);
	}
	//���»�ȡ��ǰ���ֽ���
	ps = file.tellp();
	size = (int)ps;
	//��ȡ��ǰ����һ���ۺ�
	int slot_num = (size / 4) / N0 / (secondDimension);
	++slot_num;
	for (int i = 0; i < N0; i++) {
		if (i < firstDimension) {
			for (int j = 0; j < secondDimension; j++) {
				float num = data[i][j];
				cout << "page id: " << dataFileIndex << " " << "slot id: " << slot_num << " " << "data: " << num << endl;
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
		if (qu.front().dataCount < N0) {
			int index, dataCount, dimension;
			float center1, center2, radius;
			index = qu.front().index;
			dataCount = qu.front().dataCount;
			dimension = qu.front().dimension;
			int * arr = new int[qu.front().dimension];
			for (int i = 0; i < qu.front().dimension; i++) {
				arr[i] = 0;
			}
			radius = 0;
			file.write((char*)&index, sizeof(int));
			file.write((char*)&dataCount, sizeof(int));
			file.write((char*)&dimension, sizeof(int));
			for (int i = 0; i < qu.front().dimension; i++) {
				file.write((char*)&arr[i], sizeof(float));
			}
			file.write((char*)&radius, sizeof(float));
			int page_index = this->dataFileIndex;
			file.write((char*)&page_index, sizeof(int));
			int slot_index = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
			file.write((char*)&slot_index, sizeof(int));
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
			float * arr = new float[qu.front().dimension];
			for (int i = 0; i < qu.front().dimension; i++) {
				arr[i] = qu.front().center[i];
				file.write((char*)&arr[i], sizeof(float));
			}
			radius = qu.front().radius;
			file.write((char*)&radius, sizeof(float));
			int page_index = 0, slot_index = 0;
			file.write((char*)&page_index, sizeof(int));
			file.write((char*)&slot_index, sizeof(int));
			qu.push(*(qu.front().left));
			qu.push(*(qu.front().right));
		}
		qu.pop();
	}
	file.close();
	return true;
}


int *BallTree::readData(int pageNumer, int slot,int d) {
	//string indexFilePath(index_path);
	////ofstream file(indexFilePath, ios::binary);
	//ifstream infile("index.txt", ios::binary);
	//if (!infile.is_open()) {
	//	cout << "cannot open the file\n";
	//	return false;
	//}
	//infile.seekg(0, ios::end);
	////queue<Node> qu;
	//qu.push((*(this->root)));
	//int pageNumer = this->dataFileIndex;
	//int slot = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
	//data = readData(pageNumer, slot, d);
	//infile.close();

	float bufferPage[16384];
	//����ҳ
	ifstream file;
	string pagename = intToString(pageNumer);
	pagename += ".txt";
	file.open(pagename, ios::binary);
	if (!file.is_open()) {
		cout << "cannot open the file\n";
		return false;
	}
	//����ҳ���ҵ�txt�ļ�
	for (int i = 0; i <= 16384; i++) {
		file.read((char*)&bufferPage[i], sizeof bufferPage[i]);
    }
	//�������е�ҳ��
	int len = N0 * (d + 1);
	int *arr = new int[len - 1];
	for (int i = 0; i <= len - 1; i++)
		arr[i] = bufferPage[(slot- 1)*len + i];
	//���ݲۺ��ҵ�����
	file.close();
	return arr;
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
		int pid = p->getPid();//ҳ��
		int sid = p->getSid();//�ۺ�
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

int BallTree::mipSearch(int d, float* query) {
	return 0;
}

bool BallTree::insertData(int d, float* data) {
	//���ĵ��ѡȡ
	return true;
}

bool BallTree::deleteData(int d, float* data) {
	if (DistanceBetween(root->center, data, d) >= root->radius)
		return false;
	Node * current_root = root;
	//�ҵ�����֮����ÿ���ڵ�datacount--;
	while (current_root->dataCount > N0) {
		if (current_root->left != nullptr && DistanceBetween(current_root->left->center, data, d) <= current_root->left->radius)
			current_root = current_root->left;
		else if (current_root->right != nullptr && DistanceBetween(current_root->right->center, data, d) <= current_root->right->radius)
			current_root = current_root->right;
		else
			return false;
	}
	return true;
}

bool BallTree::restoreTree(const char* index_path, int d) {
	ifstream ifile(index_path, ios::in | ios::binary);
	Node * currentNode = root;
	while (!ifile.eof()) {
		int index, datacount, dimension, pageNumber;
		float * center = new float[d];
		float radius;
		long slotNumer;
		ifile.read((char*)&index, sizeof(int));
		ifile.read((char*)&datacount, sizeof(int));
		ifile.read((char*)&dimension, sizeof(int));
		ifile.read((char*)center, sizeof(float) * d);
		ifile.read((char*)&radius, sizeof(float));
		ifile.read((char*)&pageNumber, sizeof(int));
		ifile.read((char*)&slotNumer, sizeof(long));
		currentNode = findPoint(index);
		currentNode->index = index;
		currentNode->dataCount = datacount;
		currentNode->dimension = dimension;
		currentNode->center = center;
		currentNode->radius = radius;
		currentNode->pageNumer = pageNumber;
		currentNode->slot = slotNumer;
	}
	return true;
}

Node *  BallTree::findPoint(int index) {
	if (index == 1) {
		root = new Node();
		return root;
	}
	Node * currentNode = root;
	int layer = 0;
	int result = 1;
	while (result < index) {
		layer++;
		result += pow(2, layer);
	}
	int j = layer;
	while (currentNode != nullptr) {
		if (index - (result - pow(2, layer)) <= pow(2, j) / 2) {
			currentNode = currentNode->left;
			j--;
		}
		else {
			currentNode = currentNode->right;
			index = index - pow(2, j) / 2;
			j--;
		}
		if (2 * currentNode->index == index) {
			currentNode->left = new Node();
			return currentNode->left;
		}
		else if (2 * currentNode->index + 1 == index) {
			currentNode->right = new Node();
			return currentNode->right;
		}
	}
}

