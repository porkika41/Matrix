#include "Matrix.h"
#include "menu.h"

int main() {
	try {
		Matrix<int> m=start();
		Matrix<int> n = m;
		int b = 7;
		Matrix<int> o = m * b;
		cout <<"matrix szorzasa " <<b<<" skalarral = "<<endl;
		o.print();
		cout << " Ebbol kivonva  " << endl;
		m.print();
		o = o-m;
		cout << " = " << endl;
		o.print();
		cout << "kivalasztas ebbol a matrixbol:" << endl;
		m.print();
		cout << " 1.oszlopa: " << endl;
		m(1).print();
		cout << " 1. sora:" << endl;
		m[1].print();
		cout << "elso sor 2.eleme: ";
		m[1](2).print();
		cout << "90 fokkal valo forgatasa: " << endl << "forgatas elott: " << endl;;
		m.print();
		~m;
		cout << "forgatas utan: " << endl;;
		m.print();
		cout << " 1.soranak felcserelese az 1. oszloppal" << endl;
		m << 1;
		m.print();
		cout << "matrix szorzas: " << endl;
		o = n * m;
		n.print();
		cout << " * " << endl;
		m.print();
		cout << "=" << endl;
		o.print();
		m.print();
		cout << " + " << endl;
		n.print();
		cout << "=" << endl;
		o = m + n;
		o.print();
		cout << "ugyanezeket kivonva egymasbol: " << endl;
		o = m - n;
		o.print();
		int a[] = { 9,6 };
		cout <<"("<< a[0] << " " << a[1] << ") sor hozzaadasa a ket sor koze: " << endl;
		n.addRow(a, 2);
		n.print();
		cout << "mas tipussal par muvelet:" << endl << "DOUBLE tipussal szorzas: " << endl;
		double d[] = { 1.5,0.4,2.7,3.11 }, e[] = { 0.42,6.66,1.1,2.0 };
		Matrix<double> d1(2, 2, d), d2(2, 2, e);
		d1.print();
		cout << "*" << endl;
		d2.print();
		cout << "=" << endl;
		Matrix<double> d3 = d1 * d2;
		d3.print();
		cout << "elforgatva az elso matrix:" << endl;
		~d1;
		d1.print();
	}
	catch(invalid_argument& ia){
		cerr <<"HIBA invalid_argument: "<< ia.what() << endl;
	}
	catch (out_of_range& oor) {
		cerr <<"HIBA out_of_range: " <<oor.what() << endl;
	}
	catch (runtime_error& re) {
		cerr <<"HIBA runtime_error: "<< re.what() << endl;
	}
	return 0;
} 