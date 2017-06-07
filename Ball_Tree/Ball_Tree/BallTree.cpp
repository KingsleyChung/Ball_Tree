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

float currentSum = 0;//��ѯʱ���浱ǰ������ڻ�
int currentIndex = -1;//��¼��ǰ������id

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
        float* center;
        float radius;
        if (n == 1) {
            center = data[0];
            radius = 0;
        }
        else {
            center = FindCenter(data, n, d);
            float* furthestDataFromCenter = FindFurthestData(center, data, n, d);
            //printVector(center, d);
            //printVector(furthestDataFromCenter, d);
            radius = DistanceBetween(center, furthestDataFromCenter, d);
        }
        subroot = new Node(index, n, d, center, radius);
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
				//���������Ƿ������ȷ
				//cout << "page id: " << dataFileIndex << " " << "slot id: " << slot_num << " " << "data: " << num << endl;
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
			radius = qu.front().radius;
			file.write((char*)&index, sizeof(int));
			file.write((char*)&dataCount, sizeof(int));
			file.write((char*)&dimension, sizeof(int));
			for (int i = 0; i < qu.front().dimension; i++) {
				arr[i] = qu.front().center[i];
				file.write((char*)&arr[i], sizeof(float));
			}
			file.write((char*)&radius, sizeof(float));
			int page_index = this->dataFileIndex;
			file.write((char*)&page_index, sizeof(int));
			int slot_index = storeData(qu.front().data, qu.front().dataCount, qu.front().dimension + 1);
			file.write((char*)&slot_index, sizeof(int));
			cout << "index: " << index << " dataCount: " << dataCount << " dimension: " << dimension << " radius: " << radius << " page_index: " << page_index << " slot_index: " << slot_index << endl;
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
			cout << "index: " << index << " dataCount: " << dataCount << " dimension: " << dimension << " radius: " << radius << " page_index: " << page_index << " slot_index: " << slot_index << endl;
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
	////string indexFilePath(index_path);
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

	//����ҳ
	float bufferPage[16384];
	//����ҳ���ҵ�txt�ļ�
	ifstream file;
	string pagename = intToString(pageNumer);
	pagename += ".txt";
	file.open(pagename, ios::binary);
	if (!file.is_open()) {
		cout << "cannot open the file\n";
		return false;
	}
	//���뵱ǰ��ҳ��
	for (int i = 0; i <= 16384; i++) {
		file.read((char*)&bufferPage[i], sizeof bufferPage[i]);
    }
	//���ݲۺ��ҵ�����
	int len = N0 * (d + 1);
	int *arr = new int[len - 1];
	for (int i = 0; i <= len - 1; i++)
		arr[i] = bufferPage[(slot- 1)*len + i];
	file.close();
	return arr;
}


int BallTree::mipSearch(int d, float* query) {
	DFS(d, root, query);
	return currentIndex;
}

void BallTree::DFS(int d, Node* p, float* query) {
	if (p == NULL) return;
	//if (p->left == NULL) cout << "0 ";
	//if (p->right == NULL) cout << "0 ";
	//cout << endl;

	//cout << "index: " << p->index << endl;
	if (p->dataCount <= N0 && p->dataCount > 0) {
		//get 20 data 
		int pid = p->pageNumer;//ҳ��
		int sid = p->slot;//�ۺ�
		float radius = p->radius;
		float* center = p->center;
		float tmp = 0, q = 0;
		for (int i = 0; i < d; i++) {
			tmp += query[i] * center[i];
			q += query[i] * query[i];
		}
		q = sqrt(q);
		tmp = tmp + q * radius;
		if (tmp > currentSum) {
			cout << "index: " << p->index << endl;
			int* data = readData(pid, sid, d);
			int** arr;
			arr = new int*[N0];
			for (int n = 0; n < N0; n++) {
				arr[n] = new int[d + 1];
			}
			int count = 0;
			for (int i = 0; i < N0; i++) {
				float sum = 0;
				for (int j = 0; j < d + 1; j++) {
					arr[i][j] = data[count++];
					//cout << "id: " << arr[i][0] << endl;
					//cout << arr[i][j] << " ";
					if (j != 0) {
						sum += arr[i][j] * query[j - 1];
					}
				}
				if (sum > currentSum) {
					currentSum = sum;
					currentIndex = arr[i][0];
				}
			}
			for (int k = 0; k < N0; k++) {
				delete[] arr[k];
			}
		}
		else cout << "skip" << endl;
	}
	if (p->left != NULL)
		DFS(d, p->left, query);
	if (p->right != NULL)
		DFS(d, p->right, query);
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
	//cout << index_path << endl;
	ifstream ifile("index.txt", ios::binary);
	Node * currentNode;
	while (!ifile.eof()) {
		int index, datacount, dimension, pageNumber;
		float * center = new float[d];
		float radius;
		int slotNumer;
		ifile.read((char*)&index, sizeof(int));
		ifile.read((char*)&datacount, sizeof(int));
		ifile.read((char*)&dimension, sizeof(int));
		ifile.read((char*)center, sizeof(float) * d);
		ifile.read((char*)&radius, sizeof(float));
		ifile.read((char*)&pageNumber, sizeof(int));
		ifile.read((char*)&slotNumer, sizeof(int));
		currentNode = findPoint(index);
		if (currentNode == nullptr) {
			cout << index << endl;
			if (root->left != nullptr)
				cout << "ddf" << endl;
			if (root->right != nullptr)
				cout << "ffd" << endl;
			return false;
		}
		currentNode->index = index;
		currentNode->dataCount = datacount;
		currentNode->dimension = dimension;
		currentNode->center = center;
		currentNode->radius = radius;
		currentNode->pageNumer = pageNumber;
		currentNode->slot = slotNumer;
		//if (root->left != nullptr && root->right != nullptr)
			//cout << "mabi" << endl;
		//cout << index<< datacount << dimension << pageNumber << slotNumer << endl;
	}
	ifile.close();
	return true;
}

Node *  BallTree::findPoint(int index) {
	if (index == 1) {
		root = new Node();
		return root;
	}
	Node * currentNode = root;
	Node * lastNode = root;
	int layer = 0;
	int result = 1;
	while (result < index) {
		layer++;
		result += pow(2, layer);
	}
	int temp = index;
	int j = layer;
	while (currentNode != nullptr) {
		if (temp - (result - pow(2, layer)) <= pow(2, j) / 2) {
			currentNode = currentNode->left;
			j--;
		}
		else {
			currentNode = currentNode->right;
			temp = temp - pow(2, j) / 2;
			j--;
		}
		if (lastNode != nullptr && 2 * lastNode->index == index) {
			lastNode->left = new Node();
			return lastNode->left;
		}
		else if (lastNode != nullptr && 2 * lastNode->index + 1 == index) {
			lastNode->right = new Node();
			return lastNode->right;
		}
		else {
			lastNode = currentNode;
		}
	}
	//return nullptr;
	while (true)
	{
		printf("%d\n", index);
	}
}

