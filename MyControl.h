#pragma once
#include <cstring>
#include <iostream>
#include <easyx.h>
#include <conio.h>
#pragma comment(lib,"Imm32.lib")
using std::string;
/////////////////////////////////////////////////
//				�ⲿ����					  //
/////////////////////////////////////////////////
extern MOUSEMSG mouse;

////////////////////////////////////////////////
//				ö�ٽ���					 //
////////////////////////////////////////////////
enum UI
{
	UI0,UI1, UI2, UI3, UI4, UI5, UI6
};

////////////////////////////////////////////////
//				������						  //
////////////////////////////////////////////////
//����ؼ���������
class CONTROL
{
protected:
	int x, y, sizex, sizey;		//�ؼ�λ�����С
	string text;				//�ؼ��ϵ��ı���Ϣ
	bool Down;					//���ڼ�ⰴ���Ƿ񱻰�ס
	bool enable;				//�Ƿ�����
public:
	CONTROL();
	virtual bool Click();		//�Ƿ���
	virtual void SetValues(int x, int y, int sizex, int sizey, string text);	//��������
	virtual void SetText(string text);											//�����ı���Ϣ
	virtual void Draw() = 0;													//�滭����
	virtual bool Enable(int work = -1) = 0;		//ʹ��Ĭ�ϲ������򷵻ص�ǰ״̬���������õ�ǰ״̬
};
//����ؼ�����ǩ
class LABEL :public CONTROL
{
public:
	void Draw();
	bool Enable(int work = -1);
};
//����ؼ�����ť
class BUTTON :public CONTROL
{
protected:
	IMAGE img_down, img_up;		//��ťͼ��
public:
	void SetValues(int x, int y, int sizex, int sizey, string text);
	void Draw();
	bool Enable(int work = -1);
};
//����ؼ��������
class INPUTBOX :public CONTROL
{
protected:
	string str;			//������
	bool choose;		//���ڱ�ѡ��״̬
	bool hide;			//��������
	void GetIMEString(HWND hWnd, string& str);	//��ȡ���뷨����
public:
	void Draw();
	bool Click();
	bool Empty();
	void Hide(bool hide);
	void Clear();
	string Input();
	bool Enable(int work = -1);
};

//////////////////////////////////////////
//				��������				//
//////////////////////////////////////////
void Control_Enable(bool Enable, CONTROL *b1, CONTROL *b2 = NULL, CONTROL *b3 = NULL, CONTROL *b4 = NULL);
void Control_Draw(CONTROL *b1, CONTROL *b2 = NULL, CONTROL *b3 = NULL, CONTROL *b4 = NULL);

template<typename T>					//���׵������
T& InputBoxS(T &date, char *format = "%d", char *pPrompt = NULL, char *pTitle = NULL, int nMaxCount = 20)
{
	char *t; t = new char[nMaxCount];
	InputBox(t, nMaxCount, pPrompt, pTitle);
	sscanf(t, format, &date);
	return date;
}
