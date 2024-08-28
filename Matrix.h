#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

template <class T>
class Matrix
{
private:
	int sor;
	int oszlop;
	T* adat;
public:
	Matrix(int, int, const T*);
	Matrix(const Matrix<T>&);
	Matrix();
	~Matrix();
	int getRow()const;
	int getCol()const;
	void setRow(const int);
	void setCol(const int);
	void addRow(const T*,int);			//sor hozzaadasa
	Matrix<T> operator[](int);			//sor elérése
	Matrix<T> operator()(int);			//oszlop elérése
	Matrix<T> operator<<(int);			//sor csere
	Matrix<T> operator~();				//90°-kal forgatas
	Matrix<T> operator*(Matrix<T>&);	//matrixszorzas
	T** convert2matrix();	
	Matrix<T> operator*(int);			//skalarszorzas
	Matrix<T> operator+(const Matrix<T>&);		//osszeadas
	Matrix<T> operator-(const Matrix<T>&);				//kivonas
	const Matrix<T> operator=(const Matrix<T>&);		//egyenlove tesz ket matrixot
	void print()const;		//kiiro fuggveny
};

//default konstruktor
template <class T>
Matrix<T>::Matrix() {
	sor = 0;
	oszlop = 0;
	adat = nullptr;
}

//adatsoros konstruktor
template <class T>
Matrix<T>::Matrix(int row, int col, const T* data)
{
	sor = row;
	oszlop = col;
	adat = new T [sor*oszlop];		//memória foglalás a mátrixnak 
	for (int i = 0; i < sor*oszlop; ++i) {
		adat[i] = data[i];
	}
}

//masolo konstruktor
template<class T>
 Matrix<T>::Matrix(const Matrix<T>& m)
{
	 sor = m.sor;
	 oszlop = m.oszlop;
	 adat = new T[sor * oszlop];
	 for (int i = 0; i < sor * oszlop;++i) {
		 adat[i] = m.adat[i];
	 }
}


 //destruktor
template<class T>
Matrix<T>::~Matrix()
{
	sor = 0;
	oszlop = 0;
	delete[] adat;
}

template<class T>
int Matrix<T>::getRow() const
{
	return sor;
}

template<class T>
int Matrix<T>::getCol() const
{
	return oszlop;
}

template<class T>
void Matrix<T>::setRow(const int row)
{
	sor = row;
}

template<class T>
void Matrix<T>::setCol(const int col)
{
	oszlop = col;
}

//matrixhoz sor hozzaadasa, szukseges, hogy az adatsor megfelelo meretu legyen
template<class T>
void Matrix<T>::addRow(const T* newRow,int hova)
{
	T* tarolo = new T[(sor+1) * oszlop];
	int k=0,j=0;
	for (int i = 0; i < sor * oszlop; ++i) {
		if (i == (hova - 1) * oszlop) {
			for (j = (hova - 1) * oszlop, k = 0; j < (hova - 1) * oszlop + oszlop; ++j, ++k) {
				tarolo[j] = newRow[k];
			}
		}
		
			tarolo[i + k] = adat[i];
	}
	sor++;
	delete[] adat;
	adat = tarolo;
	
}

//oszlopkivalaszto operator
template<class T>
Matrix<T> Matrix<T>::operator[](int rowIdx)
{
	if (rowIdx > sor)
		throw out_of_range("Ilyen oszlop nem letezik.");
	T* ujAdat = new T[oszlop];
	int j;
	for (int i = (rowIdx - 1) * oszlop, j=0; i < ((rowIdx - 1) * oszlop + oszlop); ++i,++j) {
		ujAdat[j] = adat[i];
	}
	return Matrix<T>(1,oszlop,ujAdat);
}

//sorkivalazsto operator
template<class T>
Matrix<T> Matrix<T>::operator()(int colIdx)
{
	if (colIdx > oszlop)
		throw out_of_range("Ilyen sor nem letezik.");
	T* ujAdat = new T[sor];
	int j=0;
	for (int i = (colIdx - 1); j < sor; i += oszlop,++j) {
		ujAdat[j] = adat[i];
	}
	return Matrix<T>(sor,1,ujAdat);
}


//kivalasztott sor felcserelese a kivalasztott oszloppal
template<class T>
Matrix<T> Matrix<T>::operator<<(int melyik)
{
	if (melyik > sor && melyik > oszlop)
		throw out_of_range("Nem letezik a megadott sor vagy oszlop. Csak negyzet alaku matrixszal mukodik.");
	
	Matrix<T> col=this->operator()(melyik);
	Matrix<T> row = this->operator[](melyik);
	//oszlop beirasa sor helyere
	int j;
	for (int i = (melyik - 1)*oszlop, j = 0; j < (melyik-1)*oszlop+oszlop; ++j, ++i) {
		this->adat[i] = col.adat[j];
	}
	//sor beirasa oszlop helyere
	for (int i = melyik - 1, j = 0; j < sor; ++j, i += oszlop) {
		this->adat[i] = row.adat[j];
	}
	return *this;
}

//90°-os forgatas
template<class T>
Matrix<T> Matrix<T>::operator~()
{
	T* ujAdat = new T[sor * oszlop];
	int j, k;
	for (int i = 0, j = 0, k = sor - 1; i < sor * oszlop; ++i, --k) {
		if (i % sor == 0)
			++j;
		if (k < 0)
			k = sor - 1;
		ujAdat[i] = this->operator()(j).adat[k];
	}
	Matrix<T> uj(oszlop, sor, ujAdat);
	*this = uj;
	return *this;
}


//matrixszorzas
template<class T>
Matrix<T> Matrix<T>::operator*(Matrix<T>& m)
{
	if (getCol() != m.getRow())
		throw invalid_argument("Ez a szorzas nem elvegezheto, valassz masik matrixot.");
	//atkonvertalas 2 dimenzios tombbe ezzel egyzserubb az algoritmus
	T** ez = convert2matrix();
	T** masik = m.convert2matrix();
	T** eredmeny = new T * [getRow()];
	for (int i = 0; i < getRow(); ++i) {
		eredmeny[i] = new T[m.getCol()];
	}
	for (int i = 0; i <getRow(); i++) {
		for (int j = 0; j < m.getCol(); j++) {
			T sum = 0;
			for (int k = 0; k < m.getRow(); k++) 
				sum += (ez[i][k] * masik[k][j]);
			eredmeny[i][j] = sum;
		}
	}

	//visszakonvertalas 1 dimnezios tombbe
	int k = 0;
	T* tomb = new T[getRow() * m.getCol()];
	for (int i = 0; i < getRow(); ++i) {
		for (int j = 0; j < m.getCol(); ++j) {
			tomb[k] = eredmeny[i][j];
			++k;
		}
	}
	//eredmeny tomb torlese
	for (int i = 0; i < m.getCol(); ++i) 
		delete[] eredmeny[i];
	delete[] eredmeny;
	return Matrix(getRow(), m.getCol(), tomb);
}

//1d-bol 2d tomb
template<class T>
T** Matrix<T>::convert2matrix()
{
	T** tarolo = new T * [this->getRow()];
	for (int i = 0; i < this->getRow(); ++i) {
		tarolo[i] = new T[this->getCol()];
	}
	int k = 0;
	for (int i = 0; i < this->getRow(); ++i)
		for (int j = 0; j < this->getCol(); ++j, ++k) 
			tarolo[i][j] = this->adat[k];
	return tarolo;
}

//skalarszorzas
template<class T>
Matrix<T> Matrix<T>::operator*(int a)
{
	T* ujAdat = new T[sor * oszlop];
	for (int i = 0; i < sor * oszlop; ++i) {
		ujAdat[i]=adat[i] * a;
	}
	return Matrix(sor,oszlop,ujAdat);
}

//matrix osszeadas
template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& m)
{
	if (sor != m.getRow() && oszlop != m.getCol()) {
		throw invalid_argument("Csak ugyanakkora matrixok adhatok ossze.");
	}
	T* ujAdat = new T[sor * oszlop];
	for (int i = 0; i < sor * oszlop; ++i) {
		ujAdat[i]=adat[i] + m.adat[i];
	}
	return Matrix(sor,oszlop,ujAdat);
}

//matrix kivonas
template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& m)
{
	if (sor != m.getRow() && oszlop != m.getCol()) {
		throw invalid_argument("Csak ugyanakkora matrixok adhatok ossze.");
	}
	T* ujAdat = new T[sor * oszlop];
	for (int i = 0; i < sor * oszlop; ++i) {
		ujAdat[i] = adat[i] - m.adat[i];
	}
	return Matrix(sor, oszlop, ujAdat);
}

//ket matrix egynelove tetele
template<class T>
const Matrix<T> Matrix<T>::operator=(const Matrix<T>& m)
{
	sor = m.getRow();
	oszlop = m.getCol();
	for (int i = 0; i < sor * oszlop; ++i) {
		adat[i] = m.adat[i];
	}
	
	return *this;
}

//kiiro fuggveny
template <class T>
void Matrix<T>::print()const {
	for (int i = 0; i < sor*oszlop; ++i) {
		if(i%oszlop==0)
			cout << "| ";
		cout << adat[i]<< " ";
		if(i%oszlop==(oszlop-1))
			cout <<"|"<< endl;
	}
	cout << endl;
}





