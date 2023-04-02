
// Svertka.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CSvertkaApp:
// Сведения о реализации этого класса: Svertka.cpp
//

class CSvertkaApp : public CWinApp
{
public:
	CSvertkaApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CSvertkaApp theApp;
