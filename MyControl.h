#pragma once
#include <cstring>
#include <iostream>
#include <easyx.h>
#include <conio.h>
#pragma comment(lib,"Imm32.lib")
using std::string;
/////////////////////////////////////////////////
//				外部变量					  //
/////////////////////////////////////////////////
extern MOUSEMSG mouse;

////////////////////////////////////////////////
//				枚举界面					 //
////////////////////////////////////////////////
enum UI
{
	UI0,UI1, UI2, UI3, UI4, UI5, UI6
};

////////////////////////////////////////////////
//				窗体类						  //
////////////////////////////////////////////////
//窗体控件：抽象类
class CONTROL
{
protected:
	int x, y, sizex, sizey;		//控件位置与大小
	string text;				//控件上的文本信息
	bool Down;					//用于监测按键是否被按住
	bool enable;				//是否启用
public:
	CONTROL();
	virtual bool Click();		//是否点击
	virtual void SetValues(int x, int y, int sizex, int sizey, string text);	//设置属性
	virtual void SetText(string text);											//设置文本信息
	virtual void Draw() = 0;													//绘画函数
	virtual bool Enable(int work = -1) = 0;		//使用默认参数，则返回当前状态，否则设置当前状态
};
//窗体控件：标签
class LABEL :public CONTROL
{
public:
	void Draw();
	bool Enable(int work = -1);
};
//窗体控件；按钮
class BUTTON :public CONTROL
{
protected:
	IMAGE img_down, img_up;		//按钮图案
public:
	void SetValues(int x, int y, int sizex, int sizey, string text);
	void Draw();
	bool Enable(int work = -1);
};
//窗体控件：输入框
class INPUTBOX :public CONTROL
{
protected:
	string str;			//缓存区
	bool choose;		//处于被选中状态
	bool hide;			//密文输入
	void GetIMEString(HWND hWnd, string& str);	//获取输入法内容
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
//				辅助函数				//
//////////////////////////////////////////
void Control_Enable(bool Enable, CONTROL *b1, CONTROL *b2 = NULL, CONTROL *b3 = NULL, CONTROL *b4 = NULL);
void Control_Draw(CONTROL *b1, CONTROL *b2 = NULL, CONTROL *b3 = NULL, CONTROL *b4 = NULL);

template<typename T>					//简易的输入框
T& InputBoxS(T &date, char *format = "%d", char *pPrompt = NULL, char *pTitle = NULL, int nMaxCount = 20)
{
	char *t; t = new char[nMaxCount];
	InputBox(t, nMaxCount, pPrompt, pTitle);
	sscanf(t, format, &date);
	return date;
}
