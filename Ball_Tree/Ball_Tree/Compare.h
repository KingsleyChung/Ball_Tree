#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void compareAnswer(string path) {
    ifstream ifile_output, ifile_answer;
    ifile_output.open(path + "/dst/answer.txt");
    ifile_answer.open("Answer/" + path + ".txt");
    if (!ifile_output || !ifile_answer) {
        printf("Cannot open file.\n");
    }
    int count = 0;
    string output_data;
    string answer_data;
    while (getline(ifile_output, output_data) && getline(ifile_answer, answer_data)) {
        count++;
        if (output_data != answer_data) {
            printf("[%d] %s %s\n",count, answer_data, output_data);
        }
    }
    ifile_output.close();
    ifile_answer.close();
    printf("Finish comparison.\n");
}

void compareTree() {
    ifstream ifile_output, ifile_answer;
    ifile_output.open("Temp/buildTree.txt");
    ifile_answer.open("Temp/restoreTree.txt");
    if (!ifile_output || !ifile_answer) {
        printf("Cannot open file.\n");
    }
    int count = 0;
    string output_data;
    string answer_data;
    while (getline(ifile_output, output_data) && getline(ifile_answer, answer_data)) {
        count++;
        if (output_data != answer_data) {
            printf("[%d] %s %s\n", count, answer_data, output_data);
        }
    }
    ifile_output.close();
    ifile_answer.close();
    printf("Finish comparison.\n");
}