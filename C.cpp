#include <iostream>
#include <time.h>
#include <cstring>
#include <fstream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <thread>

using namespace std;
clock_t StartTime, EndTime; // Время начала и завершения обработки
HANDLE hThread[10]; // дескрипторы потоков
DWORD IDThread[10]; // идентификаторы потоков
DWORD dwExitCode[10]; // коды завершения потоков


struct str {
	int n; // порядковый номер операции
	clock_t tn; // время начала операции
	clock_t tk; // время завершения операции
	string fname; //имя файла
} str[10];

//Функция обработки файла

LPTHREAD_START_ROUTINE FileProc(struct str* f)
{
	(*f).tn = clock(); //время начала выполнения функции
	char ch;
	int max = 0;
	int now = 0;
	string maxi;
	string txt = "";
	ifstream fin;
	ofstream fout; //создаём файл для записи
	fin.open("C:\\Users\\hozi1\\OneDrive\\Рабочий стол\\Универ\\6 семестр\\ОС\\Курсовая работа\\Text\\" + (*f).fname);
	if (!fin) {
		cout << "Файл " << (*f).fname << " не открыт\n" << endl; //вывод сообщения, если файл не был открыт и выход из программы
		exit(0);
	}
	int n = 0;
	fout.open("C:\\Users\\hozi1\\OneDrive\\Рабочий стол\\Универ\\6 семестр\\ОС\\Курсовая работа\\Text\\out_" + (*f).fname);
	while (fin.get(ch)) {//считываем посимвольно файл и записываем в переменную
		txt += ch;
		if (ch == ',' || ch == '-' || ch == ':' || ch == ';') { //если знак препинания прибавляем 1 к счётчику знаков
			now++;
		}
		if (ch == '.' || ch == '?' || ch == '!') {//если конец предложения
			now++;									// прибавляем 1 к счётчику так как знак препинания
			if (now > max) {						// если нынешнее значение больше предыдущего максимума
				max = now;							// перезаписываем максимум
				maxi = txt;							// запоминаем предложение
			}
			now = 0;								// обнуляем счётчик
			txt = "";								// обнуляем переменную предложения
		}
	}
	fout << maxi;									// запись в файл
	fout.close(); //закрываем файл
	(*f).tk = clock(); //время конца функции
	return 0;

}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	int i;
	string f[10];
	int N = argc - 1;
	int k = 0;
	StartTime = clock(); // Запомнили время начала обработки
	if (argc < 2) //не заданы параметры - выход из программы
	{
		cout << "Параметры не были заданы!" << endl << "Завершение программы!" << endl;
		exit(0);
	}
	for (int i = 0; i < N; i++) { //запись в структуру имена файлов и их номер
		str[i].fname = argv[i + 1];
		str[i].n = i + 1;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (str[i].fname == str[j].fname) {	//проверка на неповторяющиеся имена файлов
				k++;
			}
		}
		if (k > 1) {
			cout << "Введены повторяющиеся имена файлов!" << endl;
			return 0;
		}
		k = 0;
	}

	for (int i = 0; i <= N - 1; i++)
	{
		str[i].n = i + 1;
		// Создание потока для выполнения функции обработки файла
		hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FileProc, &str[i], CREATE_SUSPENDED, &IDThread[i]);
		SetThreadPriority(hThread[i], THREAD_PRIORITY_TIME_CRITICAL);
		ResumeThread(hThread[i]);

	}
	// Блокирование главного потока, до тех пор, пока не завершатся все порожденные им дочерние потоки
	WaitForMultipleObjects(N, hThread, true, INFINITE);



	EndTime = clock(); // Запомнили время окончания обработки
	//Вывод продолжительности обработки и завершение программы
	cout << "Время работы " << (EndTime - StartTime) << " мс" << endl;
	cout << "№ Потока | Время начала работы потока | Время конца работы потока |" << endl;
	cout << "----------------------------------------------------------------------------------------------" << endl;

	for (i = 0; i < N; i++)
	{
		cout << str[i].n << "	  " << str[i].tn << "				" << str[i].tk << endl;
	}

	_getch();
	return 0;
}

