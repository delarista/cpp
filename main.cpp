#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

struct mahasiswa {
	int pk; //primery key
	string NIM;
	string nama;
	string jurusan;
};

int menu();
void checkdatabase(fstream &data);
void addDataMahasiswa(fstream &data);
void writeData(fstream &data, int posisi, mahasiswa &inputmahasiswa);
int getDataSize(fstream &data);
void displayDataMahasiswa (fstream &data);
mahasiswa readData(fstream &data, int posisi);
void updateRecord (fstream &data);
void deleteRecord(fstream &data);

int main(){

	fstream data;

	checkdatabase(data);
	
	char is_continue;
	int input = menu();

	enum option {CREATE = 1, READ, UPDATE, DELETE, FINISH}; //biar yang baca program ngarti

	while (input != FINISH){
		
		switch (input){
			case CREATE : 
				cout << "Menambah data mahasiswa" << endl;
				addDataMahasiswa(data); 
				break;
			case READ :
				cout << "Tampilkan data mahasiswa" << endl;
				displayDataMahasiswa(data);
				break;
			case UPDATE :
				cout << "Ubah data mahasiswa" << endl; 
				displayDataMahasiswa(data);
				updateRecord(data);
				displayDataMahasiswa(data);
				break;
			case DELETE :
				cout << "Hapus data mahasiswa" << endl; 
				displayDataMahasiswa(data);
				deleteRecord(data);
				displayDataMahasiswa(data);
				break;
			default :
				cout << "Pilihan tidak ditemukan";
				break;
		}

		label_continue:

		cout << "Lanjutkan ? [y/n] : "  ;
		cin >> is_continue;
		if ((is_continue == 'y') | (is_continue == 'Y')){
			input = menu();
		} else if ((is_continue == 'n') | (is_continue == 'N')){
			break;
		} else {
			goto label_continue;
		}		
	}
	cout << "Akhir dari program";

	return 0;
}

int menu(){
	int pilih;
	//system("cls"); //system ambil dari iostream

	cout << "\nProgram CRUD data mahasiswa" << endl;
	cout << "===========================" << endl;
	cout << "1. Tambah data mahasiswa" << endl;
	cout << "2. Tampilkan data mahasiswa" << endl;
	cout << "3. Ubah data mahasiswa" << endl;
	cout << "4. Hapus data mahasiswa" << endl;
	cout << "5. Selesai" << endl;
	cout << "===========================" << endl;
	cout << "pilih [1-5] : ";
	cin >> pilih;
	cin.ignore(numeric_limits<streamsize>::max(),'\n'); //untuk enter, karena cin diatas tidak dienter mengakibatkan getline tidak jalan
	return pilih;
}

void checkdatabase(fstream &data){
	data.open("data.bin", ios::out | ios::in | ios::binary);

	//check file ada atau tidak
	if (data.is_open()){
		cout << "database ditemukan" << endl;
	} else {
		cout << "database tidak ditemukan, buat database baru" << endl;
		data.close();
		data.open("data.bin", ios::trunc | ios::out | ios::in | ios::binary);
	}
}

void writeData(fstream &data, int posisi, mahasiswa &inputmahasiswa){
	data.seekp((posisi - 1)*sizeof(mahasiswa), ios::beg);
	data.write(reinterpret_cast<char*>(&inputmahasiswa), sizeof(mahasiswa));
}

int getDataSize(fstream &data){
	int start, end;
	data.seekg(0, ios::beg);
	start = data.tellg();
	data.seekg(0, ios::end);
	end = data.tellg();
	return (end-start)/sizeof(mahasiswa);
}

mahasiswa readData(fstream &data, int posisi){
	mahasiswa readmahasiswa;
	data.seekg((posisi - 1)*sizeof(mahasiswa),ios::beg);
	data.read(reinterpret_cast<char*>(&readmahasiswa), sizeof(mahasiswa));
	return readmahasiswa;
}

void addDataMahasiswa(fstream &data){

	mahasiswa inputmahasiswa, lastmahasiswa;

	int size = getDataSize(data);
	cout << "Ukuran data : " << size << endl;

	/*if(size == 0){
		inputmahasiswa.pk = 1;
	} else {
		lastmahasiswa = readData(data, size);
		//cout << "pk = " << lastmahasiswa.pk << endl;
		inputmahasiswa.pk = lastmahasiswa.pk + 1;
	}*/

	//inputmahasiswa.pk = 1;
	cout << "Nama 	: ";
	getline(cin, inputmahasiswa.nama);
	cout << "Jurusan : ";
	getline(cin, inputmahasiswa.jurusan);
	cout << "NIM 	: ";
	getline(cin, inputmahasiswa.NIM);

	writeData(data,size+1,inputmahasiswa);
}

void displayDataMahasiswa (fstream &data){
	int size = getDataSize(data);
	mahasiswa showmahasiswa;
	cout << "no.\t pk.\t NIM.\t Nama.\t Jurusan.\t" << endl;
	for (int i = 1; i <=  size; i++){
		showmahasiswa = readData(data, i);
		cout << i << "\t";
		cout << showmahasiswa.pk << "\t";
		cout << showmahasiswa.NIM << "\t";
		cout << showmahasiswa.nama << "\t";
		cout << showmahasiswa.jurusan <<endl;
	}
} 

void updateRecord (fstream &data){
	int nomor;
	mahasiswa updatemahasiswa;
	cout << "Pilih no : ";
	cin >> nomor;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	updatemahasiswa = readData(data, nomor);

	cout << "\nPilihan data : " << endl;
	cout << "NIM : " << updatemahasiswa.NIM << endl;
	cout << "Nama : " << updatemahasiswa.nama << endl;
	cout << "Jurusan : " << updatemahasiswa.jurusan << endl;

	cout << "\nMerubah Data : " << endl;
	cout << "NIM : ";
	getline(cin, updatemahasiswa.NIM);
	cout << "Nama : ";
	getline(cin, updatemahasiswa.nama);
	cout << "Jurusan : ";
	getline(cin, updatemahasiswa.jurusan);

	writeData(data, nomor, updatemahasiswa);
}

void deleteRecord(fstream &data){
	int nomor, size, offset;
	mahasiswa blankmahasiswa, tempmahasiswa;//temporary
	fstream datasementara;

	size = getDataSize(data);
	//1.pilih nomor
	cout << "Hapus nomor : ";
	cin >> nomor;
	//2.diposisi ini diisi data kosong
	writeData(data, nomor, blankmahasiswa);
	//3.kita cek data yang ada dari file data.bin, kalau ada data kita pindahkan ke file semestara
	datasementara.open("temp.dat", ios::trunc | ios::out | ios::in | ios::binary);//tipe data terserah
	
	for (int i = 1; i <= size; i++){
		tempmahasiswa = readData(data, i);
		if (!tempmahasiswa.nama.empty()){
			writeData(datasementara, i, tempmahasiswa);
		} else {
			offset++;
			cout << "deleted item" << endl;
		}
	}

	//4.kita pindahkan data dari file sementara ke data.bin
	size = getDataSize(datasementara);
	data.close();
	data.open("data.bin", ios::trunc | ios::out | ios::binary);
	data.close();
	data.open("data.bin", ios::out | ios::in | ios::binary);

	for (int i = 1; i <= size; i++){
		tempmahasiswa = readData(datasementara, i);
		writeData(data, i, tempmahasiswa);
	}


}

