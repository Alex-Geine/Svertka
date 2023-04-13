#include <vector>
#include <time.h>
#include <algorithm>
#include "Drawer.h"
#define Pi 3.1415926535
using namespace std; 

class Signal {
public:
	vector<double>
		X,
		XR,
		Y,
		H,
		keys;

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
	
	float Ex, Exr = 0; // ������� ��������
	float* liambda = new float[N];// ������ � �������	
private:

	//�������� �������
	void Create();

	//������� �
	float* functionX(float* x);	

	//���������� ��� �����������
	float function(float* x);	

	//������� �������
	double Random();

	//������� ����� ���������
	double Sum(vector<double> mas);

	//������� ����������
	void Noise();

public:

	//����������
	~Signal();

	//���������� �������
	void FillSignal(double n, double Fd, double Ah, double Gh, double noise, double A1, double A2, double A3,
		double G1, double G2, double G3, double niu1, double niu2, double niu3, Drawer* dr);

	//�������� �������
	void ClearSignal();	
	
	//������� ����������
	float GetE();
	
	//����� ����-������
	float MHJ(int kk, float* x, MSG msg, float TAU, float& h, float& e);	
};
