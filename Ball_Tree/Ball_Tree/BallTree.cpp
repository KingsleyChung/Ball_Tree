#include "BallTree.h"
#include "Utility.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>

bool BallTree::buildTree(int n, int d, float** data) {
    buildSubTree(root, -1, n, d, data);
    return true;
}
void BallTree::buildSubTree(Node* subroot, int previousIndex, int n, int d, float** data) {
    //���������С��N0����ڵ�ΪҶ�ӽڵ�
    if (n < N0) {
        subroot = new Node(previousIndex + 1, n, d, nullptr, 0);
        //��������
        subroot->data = new float*[n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < d; j++) {
                subroot->data[i][j] = data[i][j];
            }
        }
        return;
    }
    //���ѡȡһ������x
    srand((unsigned)time(NULL));
    int randomNum = rand() % n;
    float* x = data[randomNum];
    //�ҵ�����x��Զ������A
    float* A = FindFurthestData(x, data, n, d);
    //�ҵ�����A��Զ������B
    float* B = FindFurthestData(A, data, n, d);
    //��¼�ڵ���Ϣ
    subroot = new Node(previousIndex + 1, n, d, FindCenter(data, n, d), DistanceBetween(subroot->center, A, d));
    //������
    MakeBallTreeSplit(A, B, subroot, n, d, data);
}
void BallTree::MakeBallTreeSplit(float* A, float* B, Node* subroot, int n, int d, float** data) {
    //��������
    int dataCountOfLeft = 0, dataCountOfRight = 0;      //��¼���ҽڵ����ݸ���
    float** dataOfLeft = new float*[n];                 //��¼��ڵ�����
    float** dataOfRight = new float*[n];                //��¼�ҽڵ�����
    for (int i = 0; i < n; i++) {
        if (CloserTo(data[i], A, B, d) == 1) {
            dataOfLeft[dataCountOfLeft] = data[i];
            dataCountOfLeft++;
        }
        else {
            dataOfRight[dataCountOfRight] = data[i];
            dataCountOfRight++;
        }
    }
    //����������
    buildSubTree(subroot->left, subroot->index, dataCountOfLeft, d, dataOfLeft);
    //����������
    buildSubTree(subroot->right, subroot->index, dataCountOfRight, d, dataOfRight);
}
float* BallTree::FindFurthestData(float* x, float** &data, int n, int d) {
    float furthestDistance = 0;             //��¼��Զ�����ƽ��
    int furthestDataPos = 0;                //��¼��Զ���������������е�λ��
    for (int i = 0; i < n; i++) {
        float result = 0;                   //��¼�˴α������ݾ����ƽ��
        for (int j = 0; j < d; j++) {
            result += (x[j] - data[i][j]) * (x[j] - data[i][j]);
        }
        if (result > furthestDistance) {    //����������֮ǰ��¼����Զ����
            furthestDistance = result;
            furthestDataPos = i;
        }
    }
    return data[furthestDataPos];           //������Զ��������
}

int BallTree::CloserTo(float* selectedData, float* A, float* B, int d) {
    float distanceToA = 0;                  //ѡ�����ݵ�A�ľ���
    float distanceToB = 0;                  //ѡ�����ݵ�B�ľ���
    for (int j = 0; j < d; j++) {
        distanceToA += (A[j] - selectedData[j]) * (A[j] - selectedData[j]);
        distanceToB += (B[j] - selectedData[j]) * (B[j] - selectedData[j]);
    }
    return distanceToA <= distanceToB ? 1 : 2;//��A�Ͻ���A��B������ȷ���1����������2
}

float* BallTree::FindCenter(float** data, int n, int d) {
    float* center = new float[d];           //Բ������
    for (int j = 0; j < d; j++) {           //������ÿ��ά��
        float sumOfOneDimension = 0;        //һ��ά�ȵĺ�
        for (int i = 0; i < n; i++) {       //ÿ������
            sumOfOneDimension += data[i][j];
        }
        center[j] = sumOfOneDimension / (d + 0.0);//ƽ��ֵ
    }
    return center;
}

float BallTree::DistanceBetween(float* center, float* point, int d) {
    float totalDistanceSquare = 0;
    for (int i = 0; i < d; i++) {
        totalDistanceSquare += (center[i] - point[i]) * (center[i] - point[i]);
    }
    return sqrt(totalDistanceSquare);
}