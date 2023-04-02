#include <vector>
#include <time.h>
#include <algorithm>
#include "Drawer.h"
#define Pi 3.1415926535
using namespace std; 

class Signal {
public:
	Drawer* dr;
	double
		N,
		fd,
		Ah,
		Gh,
		A1, 
		A2,
		A3,
		G1,
		G2,
		G3,
		niu1,
		niu2,
		niu3,
		noise;
	vector<double>
		X,
		XR,
		Y,
		H,
		keys;
	float Ex, Exr = 0; // энергии сигналов
	float* liambda = new float[N];// массив с лямбдами
	
	//конструктор по умолчанию
	Signal() {};

	//Деструктор
	~Signal() {
		X.~vector();
		Y.~vector();
		H.~vector();
		XR.~vector();
		keys.~vector();
		delete liambda;
	}

	//заполнение сигнала
	void FillSignal(double n, double Fd, double Ah, double Gh,double noise, double A1, double A2, double A3,
		double G1, double G2, double G3, double niu1, double niu2, double niu3, Drawer *dr) {
		this->N = n;
		this->fd = Fd;
		this->Ah = Ah;
		this->Gh = Gh;
		this->A1 = A1;
		this->A2 = A2;
		this->A3 = A3;
		this->G1 = G1;
		this->G2 = G2;
		this->G3 = G3;
		this->niu1 = niu1;
		this->niu2 = niu2;
		this->niu3 = niu3;
		this->dr = dr;
		this->noise = noise;
		Create();
	}

	//отчистка сигнала
	void ClearSignal() {
		X.clear();
		Y.clear();
		H.clear();
		XR.clear();
		keys.clear();
	}

	//Создание сигнала
	void Create() {
		srand(time(0));		
		Ex = 0;
		Exr = 0;
		for (int i = 0; i < N; i++) {
			liambda[i] = (float)rand() / RAND_MAX; // Задается начальное приближение
			keys.push_back(i / fd);
			X.push_back(				
			A1 * exp(-(keys[i] - niu1/fd)* (keys[i] - niu1/fd) / ( G1 * G1 )) +
			A2 * exp(-(keys[i] - niu2/fd)* (keys[i] - niu2/fd) / ( G2 * G2)) +
			A3 * exp(-(keys[i] - niu3/fd)* (keys[i] - niu3/fd) / ( G3 * G3 ))
			);
			Ex += X[i] * X[i];
			
			if (i < N / 2)
				H.push_back(exp(-keys[i] * keys[i] / (Gh * Gh)));
			else {
				H.push_back(H[N - 1 - i]);
				
			}				
		}

		//свертка
		for (int i = 0; i < N; i++) {
			Y.push_back(0);
			for (int k = 0; k < N; k++) {
				if (i - k < 0)
					Y[i] += X[k] * H[k - i];									
				else
					Y[i] += X[k] * H[i - k];								
			}
		}

		//наложили шум на свертку
		Noise();
	}
	
	//находит отклонение
	float GetE() {
		float* buf = new float[N];
		buf = functionX(liambda);		
		
		for (int i = 0; i < N; i++) 			
			Exr += buf[i] * buf[i];
		
		delete buf;
		float res = abs((Ex - Exr) / Ex * 100);
		Exr = 0;
		return  res;		
	}
	
	//метод Хука-Дживса
	float MHJ(int kk, float* x, MSG msg, float TAU, float& h, float &e)
	{		
		int i, j, bs, ps, calc = 0;
		float z, k, fi, fb;
		float* b = new float[kk];
		float* y = new float[kk];
		float* p = new float[kk];		
		
		k = h;
		for (i = 0; i < kk; i++)	y[i] = p[i] = b[i] = x[i];
		fi = function(x);
		ps = 0;   bs = 1;  fb = fi;

		j = 0;
		while (1)
		{
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.lParam == (LPARAM("PAUSE")))
				{
					h = k;
					return 0;
				}
			}
			calc++; // Счетчик итераций.

			x[j] = y[j] + k;
			z = function(x);
			if (z >= fi) {
				x[j] = y[j] - k;
				z = function(x);
				if (z < fi)   y[j] = x[j];
				else  x[j] = y[j];
			}
			else  y[j] = x[j];
			fi = function(x);

			if (j < kk - 1) { j++;  continue; }
			if (fi + 1e-8 >= fb) {
				if (ps == 1 && bs == 0) {
					for (i = 0; i < kk; i++) {
						p[i] = y[i] = x[i] = b[i];
					}
					z = function(x);
					bs = 1;   ps = 0;   fi = z;   fb = z;   j = 0;
					continue;
				}
				k /= 10.;
				if (k < TAU) break;
				j = 0;
				continue;
			}

			for (i = 0; i < kk; i++) {
				p[i] = 2 * y[i] - b[i];
				b[i] = y[i];
				x[i] = p[i];

				y[i] = x[i];
			}
			z = function(x);
			fb = fi;   ps = 1;   bs = 0;   fi = z;   j = 0;

			//отрисовка
			if (!(calc % 100)) {
				float* buf = new float[kk];
				buf = functionX(x);
				for (i = 0; i < kk; i++) {
					XR.push_back(buf[i]);					
				}
								
				delete buf;

				dr->DrawTwo(XR, X, 
					*min_element(keys.begin(), keys.end()), 						
					*max_element(keys.begin(), keys.end()),
					min( * min_element(XR.begin(), XR.end()), *min_element(X.begin(), X.end())),
					max( * max_element(XR.begin(), XR.end()), *max_element(X.begin(), X.end())),
					't', 'A',  keys, keys);	
				XR.clear();
			}

		} //  end of while(1)

		XR.clear();
		for (i = 0; i < kk; i++)  x[i] = p[i];
		
		float* buf = new float[kk];
		buf = functionX(x);
		for (i = 0; i < kk; i++) {
			XR.push_back(buf[i]);			
		}
		
		delete buf;
				
		dr->DrawTwo(XR, X,
			*min_element(keys.begin(), keys.end()),
			*max_element(keys.begin(), keys.end()),
			min(*min_element(XR.begin(), XR.end()), *min_element(X.begin(), X.end())),
			max(*max_element(XR.begin(), XR.end()), *max_element(X.begin(), X.end())),
			't', 'A', keys, keys);
		
		delete b;
		delete y;
		delete p;
		e = GetE();
		ClearSignal();
		
		PostThreadMessageA(GetCurrentThreadId(), 12343, WPARAM("COMPLETE"), LPARAM("COMPLETE"));

		return fb;		
	}

	//Функция Х
	float* functionX(float* x)
	{
		float *xi = new float[N];
		for (int i = 0; i < N; i++) {
			
			float sum = 0;
			
			//свертка
			for (int j = 0; j < N; j++) {
				if (i - j < 0)
					sum += x[j] * H[j - i];
				else
					sum += x[j] * H[i - j];
			}
			xi[i] = exp(-1 - sum);
		}
		return xi;
	}

	//Функционал для минимизации
	float function(float* x)
	{
		float z = 0; //функционал
		float
			* xi = functionX(x),//иксы
			* yi = new float[N];//y
		

		for (int i = 0; i < N; i++) {
			yi[i] = 0;
			//свертка
			for (int j = 0; j < N; j++) {
				if (i - j < 0)
					yi[i] += xi[j] * H[j - i];
				else
					yi[i] += xi[j] * H[i - j];
			}

			z += (Y[i] - yi[i]) * (Y[i] - yi[i]);
		}

		delete xi;
		delete yi;
		return z;
	}

	//функция рандома
	double Random() {

		double a = 0;
		for (int i = 0; i < 12; i++)
			a += -1. + 2 * (double)rand() / RAND_MAX;
		return a;
	}
	//функция суммы квадратов
	double Sum(vector<double> mas) {
		double a = 0;
		for (int i = 0; i < mas.size(); i++)
			a += mas[i] * mas[i];
		return a;
	}

	//функция зашумления
	void Noise() {
		vector<double>
			n0, //вектор с некоррелированным шумом
			n,
			buf;	//коррелированный шум
		double
			Es, //энергия сигнала
			En,	//энергия шума
			B,
			Al = noise / 100;	//коэффициенты пропорциональности

		buf = Y;
		Y.clear();
		//получаем некоррелированный белый шум
		for (int i = 0; i < N; i++)
			n0.push_back(Random());

		Es = Sum(buf);
		En = Sum(n0);

		B = sqrt(Al * Es / En);

		//получаем зашумленную шляпу
		for (int i = 0; i < N; i++) {
			Y.push_back(buf[i] + B * n0[i]);
		}
		n0.~vector();
		n.~vector();
		buf.~vector();
	}
};
