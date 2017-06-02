#include <cstdio>
#include <cmath>
#include <algorithm>

#include "Utility.h"

using namespace std;

bool read_data(int n, int d, float** &data, const char* file_name)
{
	FILE* fin = fopen(file_name, "r");
	if (!fin) {
		printf("%s doesn't exist!\n", file_name);
		return false;
	}

	int id;
	data = new float*[n];
	for (int i = 0; i < n; i++) {
		data[i] = new float[d + 1];
		fscanf(fin, "%f", &data[i][0]);
		for (int j = 1; j <= d; j++) {
			fscanf(fin, "%f", &data[i][j]);
		}
	}

	printf("Finish reading %s\n", file_name);
	fclose(fin);

	return true;
}

