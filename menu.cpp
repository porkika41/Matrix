#include "menu.h"
//int matrix beolvasasa filebol
int* beolvas(int row,int col)
{
	ifstream ifs;
	int* data = new int[row * col];
	ifs.open("matrix.txt");
	if (ifs.is_open()) {
		for (int i = 0; i < row * col; ++i) {
			ifs >> data[i];
		}
		ifs.close();
	}
	else
		throw runtime_error("Nem sikerult megnyitni a filet.");
	return data;
}

//inicializalas
Matrix<int> start()
{
	int row, col;
	cout << "Add meg a hasznalni kivant matrixot!" << endl;
	cout << "Matrix sorainak szama..." << endl;
	cin >> row;
	cout << "Add meg az olszopok szamat!" << endl;
	cout << "Matrix oszlopainak szama..." << endl;
	cin >> col;
	Matrix<int> m(row, col, beolvas(row,col));
	cout << "A beolvasott matrix: " << endl;
	m.print();
	return m;
}
