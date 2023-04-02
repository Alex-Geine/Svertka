#pragma once
#include <vector>
#include <math.h>

#define Pi 3.1415926535
#define  NUMBER_IS_2_POW_K(x)   ((!((x)&((x)-1)))&&((x)>1))  // x is pow(2, k), k=1,2, ...
#define  FT_DIRECT        -1    // Direct transform.
#define  FT_INVERSE        1    // Inverse transform.
using namespace std;





//класс самого сигнала
class Sign_Analizer {
public:
		vector <double>
			data,
			Ndata,//вектора со значениями сигнала и ключами
			Sdata,
			Skeys,
			keys;
		
		//минимумы и максимумы векторов
		double
			NdataMin,
			NdataMax,
			dataMin,
			dataMax,
			SdataMin,
			SdataMax;



		double
			fd;		//частота дисретизации
		int	N;		//количество отсчетов

		double
			* Re, 
			* Im;	//Указатели на массивы со спектральным представлением сигнала
		
	

	Sign_Analizer(double N, double fd) {
		this->fd = fd;
		this->N = N;
		Re = new double[N];
		Im = new double[N];
	}

	//Создает сигнал
	void Get_Signal(double A1, double A2, double A3, double F1, double F2, double F3, double F01, double F02, double F03) {
		

		for (int i = 0; i < N; i++) {
			data.push_back(
				A1 * sin(2 * Pi * F1 * (i / fd) + F01) +
				A2 * sin(2 * Pi * F2 * (i / fd) + F02) +
				A3 * sin(2 * Pi * F3 * (i / fd) + F03)
			);
			keys.push_back(i / fd);
				
		}

		Find_Min_Max(data, dataMin, dataMax);

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

	//Зашумляет сигнал
	void Noise(double alpha) {
		vector<double>
			n0, //вектор с некоррелированным шумом
			n;	//коррелированный шум
		double
			Es, //энергия сигнала
			En,	//энергия шума
			B,
			Al = alpha / 100;	//коэффициенты пропорциональности

		//получаем некоррелированный белый шум
		for (int i = 0; i < N; i++)
			n0.push_back(Random());

		Es = Sum(data);
		En = Sum(n0);

		B = sqrt(Al * Es / En);

		//получаем зашумленную шляпу
		for (int i = 0; i < N; i++) {
			Ndata.push_back(data[i] + B * n0[i]);
			Re[i] = Ndata[i];
			Im[i] = 0.;
		}

		Find_Min_Max(Ndata, NdataMin, NdataMax);
	}

	//Находит экстремумы функции
	void Find_Min_Max(vector<double> mas, double & min, double & max) {
		min = mas[0];
		max = min;

		for (int i = 0; i < N; i++) {
			if (min >= mas[i])
				min = mas[i];
			if (max <= mas[i])
				max = mas[i];
		}
	}

	//Быстрое Фурье
	bool  FFT(int Ft_Flag)
	{
		int LogN = log2(N);
		double* Rdat = this->Re;
		double* Idat = this->Im;

		register int  i, j, n, k, io, ie, in, nn;
		float         ru, iu, rtp, itp, rtq, itq, rw, iw, sr;

		static const float Rcoef[14] =
		{ -1.0000000000000000F,  0.0000000000000000F,  0.7071067811865475F,
			0.9238795325112867F,  0.9807852804032304F,  0.9951847266721969F,
			0.9987954562051724F,  0.9996988186962042F,  0.9999247018391445F,
			0.9999811752826011F,  0.9999952938095761F,  0.9999988234517018F,
			0.9999997058628822F,  0.9999999264657178F
		};
		static const float Icoef[14] =
		{ 0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
		   -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
		   -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
		   -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
		   -0.0007669903187427F, -0.0003834951875714F
		};

		nn = N >> 1;
		ie = N;
		for (n = 1; n <= LogN; n++)
		{
			rw = Rcoef[LogN - n];
			iw = Icoef[LogN - n];
			if (Ft_Flag == FT_INVERSE) iw = -iw;
			in = ie >> 1;
			ru = 1.0F;
			iu = 0.0F;
			for (j = 0; j < in; j++)
			{
				for (i = j; i < N; i += ie)
				{
					io = i + in;
					rtp = Rdat[i] + Rdat[io];
					itp = Idat[i] + Idat[io];
					rtq = Rdat[i] - Rdat[io];
					itq = Idat[i] - Idat[io];
					Rdat[io] = rtq * ru - itq * iu;
					Idat[io] = itq * ru + rtq * iu;
					Rdat[i] = rtp;
					Idat[i] = itp;
				}

				sr = ru;
				ru = ru * rw - iu * iw;
				iu = iu * rw + sr * iw;
			}

			ie >>= 1;
		}

		for (j = i = 1; i < N; i++)
		{
			if (i < j)
			{
				io = i - 1;
				in = j - 1;
				rtp = Rdat[in];
				itp = Idat[in];
				Rdat[in] = Rdat[io];
				Idat[in] = Idat[io];
				Rdat[io] = rtp;
				Idat[io] = itp;
			}

			k = nn;

			while (k < j)
			{
				j = j - k;
				k >>= 1;
			}

			j = j + k;
		}

		if (Ft_Flag == FT_DIRECT) return true;

		rw = 1.0F / N;

		for (i = 0; i < N; i++)
		{
			Rdat[i] *= rw;
			Idat[i] *= rw;
		}

		return true;
	}

	//Спектр
	void Get_Spectrum(vector<double>& data, vector<double> &keys) {
		for (int i = 0; i < N; i++) {

			data.push_back(sqrt( (Im[i] * Im[i]) + (Re[i] * Re[i]) )); 
			keys.push_back(fd * i / N);
				
		}
		Find_Min_Max(data, SdataMin, SdataMax);

	}
	
};

//класс фильтров
class Filter : public Sign_Analizer
{
public:
	double Width;


	Filter(double N, double fd, double Width) :Sign_Analizer(N, fd) {
		this->Width = Width;
	}
	//задаеn фильтр прямоугольного окна
	void CreateFilterOne(double D) {
		
		/*for (int i = 0; i < N; i++) {
			keys.push_back(i/fd );
			if (i < Width)
				data.push_back(1);
			else
				data.push_back(0);
			Re[i] = data[i];
			Im[i] = 0.;
		}*/
		for (int i = 0; i < N; i++) {
			keys.push_back(i / fd);
			if ((i < Width/2)||(i >= N - Width/2))
				data.push_back(D);
			else
				data.push_back(0);
			Re[i] = data[i];
			Im[i] = 0.;
		}
		Find_Min_Max(data, dataMin, dataMax);
	} 

	//задает фильтр треугольного окна
	void CreateFilterTwo(double D) {
		double k = atan(2/Width); // угол наклона прямой
		int j = Width/2;
		for (int i = 0; i < N; i++) {
			keys.push_back(i / fd);
			
			if (i <= Width / 2)
				data.push_back(D*(1 - k *i)); // y = x*k
			else if ((i >= N -  Width / 2)) {
				data.push_back(data[j]);
				j--;
			}
				
			else
				data.push_back(0);
			Re[i] = data[i];
			Im[i] = 0.;
		}
		Find_Min_Max(data, dataMin, dataMax);
	}
	//задает фильтр окна косинуса
	void CreateFilterThree(double D) {
		int j = Width / 2;
		for (int i = 0; i < N; i++) {
			keys.push_back(i / fd);

			if (i <= Width/2)
				data.push_back(D * cos(Pi*i / (Width) )); 	
			else if ((i >= N - Width / 2)) {
				data.push_back(data[j]);
				j--;
			}
			else
				data.push_back(0);
			Re[i] = data[i];
			Im[i] = 0.;
		}
		Find_Min_Max(data, dataMin, dataMax);
	}
	//задает фильтр окна Гаусса
	//D - дисперсия
	void CreateFilterFour(double D) {
		double
			b = 0, 
			a = D,
			c = Width/8;
		int j = Width / 2;
		for (int i = 0; i < N; i++) {
			keys.push_back(i / fd);

			if (i < Width / 2)
				data.push_back(
					a * exp(-((i - b) * (i - b)) / (2 * c * c))
				);
			else if ((i >= N - Width / 2)) {
				data.push_back(data[j]);
				j--;
			}
			else
				data.push_back(0);
			Re[i] = data[i];
			Im[i] = 0.;
		}
		Find_Min_Max(data, dataMin, dataMax);
	}
	//задает фильтр окна Ханна
	void CreateFilterFive(double D) {
		int j = Width / 2;
		for (int i = 0; i < N; i++) {
			keys.push_back(i / fd);

			if (i <= Width/2)
				data.push_back(D * 0.5*(1 - cos(2 * Pi * i / (Width) + Pi)));
			else if ((i >= N - Width / 2)) {
				data.push_back(data[j]);
				j--;
			}
			else
				data.push_back(0);
			Re[i] = data[i];
			Im[i] = 0.;
		}
		Find_Min_Max(data, dataMin, dataMax);
	}

	//Фильтрует сигнал
	void Filtration(double * ReS, double * ImS) {
		for (int i = 0; i < N; i++) {
			double buf = Re[i];
			Re[i] = (ReS[i] * Re[i] - Im[i] * ImS[i]);
			Im[i] = buf * ImS[i] + ReS[i] * Im[i];
		}
		FFT(1);
		for (int i = 0; i < N; i++) {
			Ndata.push_back(Re[i]);
		}
		Find_Min_Max(Ndata, NdataMin, NdataMax);
	}

	//дает энергию ошибки
	double Get_Error(vector<double> sign) {
		double Err = 0;
		for (int i = 0; i < N; i++) 
			Err += (Ndata[i] - sign[i]) * (Ndata[i] - sign[i]);
		return Err;
	}
};

