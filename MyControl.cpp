#include "MyControl.h"

CONTROL::CONTROL()
{
	enable = true;
	Down = false;
}
void CONTROL::SetValues(int tx, int ty, int tsizex, int tsizey, string ttext)
{
	x = tx; y = ty; sizex = tsizex; sizey = tsizey;text = ttext;
}
void CONTROL::SetText(string ttext)
{
	text = ttext;
}
bool CONTROL::Click()
{
	if (!enable) return false;
	if ((mouse.uMsg == WM_LBUTTONDOWN) && mouse.x >= x && mouse.y >= y && mouse.x <= x + sizex && mouse.y <= y + sizey) Down = true;
	if (Down && mouse.uMsg == WM_LBUTTONUP)
	{
		Down = false;
		if (mouse.x >= x && mouse.y >= y && mouse.x <= x + sizex && mouse.y <= y + sizey)
			return true;
	}
	return false;
}
void LABEL::Draw()
{
	settextcolor(LIGHTBLUE);
	RECT r = { x, y };
	drawtext(text.c_str(), &r, DT_CALCRECT);
	drawtext(text.c_str(), &r, DT_LEFT);
}
bool LABEL::Enable(int work)
{
	if (work == 0) enable = 0;
	else if (work == 1) enable = 1;
	return enable;
}
void BUTTON::SetValues(int tx, int ty, int tsizex, int tsizey, string ttext)
{
	x = tx; y = ty; sizex = tsizex; sizey = tsizey; text = ttext;
	loadimage(&img_up, "buttonUp.jpg", sizex, sizey, true);
	loadimage(&img_down, "buttonDown.jpg", sizex, sizey, true);
}
void BUTTON::Draw()
{
	settextcolor(BLACK);
	if(Down) putimage(x, y, &img_down);
	else     putimage(x, y, &img_up);
	outtextxy(x + 5, y + 5, text.c_str());
}
bool BUTTON::Enable(int work)
{
	if (work == 0) enable = 0;
	else if (work == 1) enable = 1;
	return enable;
}
void INPUTBOX::GetIMEString(HWND hWnd, string &str)
{
	HIMC hIMC = ImmGetContext(hWnd);//获取HIMC	
	if (hIMC)
	{
		//这里先说明一下，以输入“中国”为例	
		//切换到中文输入法后，输入“zhongguo”，这个字符串称作IME组成字符串	
		//而在输入法列表中选择的字符串“中国”则称作IME结果字符串	
		static bool flag = false;//输入完成标记：在输入中时，IME组成字符串不为空，置true；输入完成后，IME组成字符串为空，置false	
		DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0);	//获取IME组成输入的字符串的长度	
		if (dwSize>0)//如果IME组成字符串不为空，且没有错误（此时dwSize为负值），则置输入完成标记为true	
		{
			if (flag == false)
			{
				flag = true;
			}
		}
		else if (dwSize == 0 && flag)	//如果IME组成字符串为空，并且标记为true，则获取IME结果字符串	
		{
			int iSize;	//IME结果字符串的大小	
			LPSTR pszMultiByte = NULL;//IME结果字符串指针	
			int ChineseSimpleAcp = 936;//宽字节转换时中文的编码	
			WCHAR *lpWideStr = NULL;//宽字节字符数组	
			dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);//获取IME结果字符串的大小	
			if (dwSize>0)	//如果IME结果字符串不为空，且没有错误	
			{
				dwSize += sizeof(WCHAR);//大小要加上NULL结束符	
				//为获取IME结果字符串分配空间	
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				lpWideStr = new WCHAR[dwSize];
				memset(lpWideStr, 0, dwSize);	//清空结果空间	
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpWideStr, dwSize);//获取IME结果字符串，这里获取的是宽字节	
				iSize = WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, NULL, 0, NULL, NULL);//计算将IME结果字符串转换为ASCII标准字节后的大小	
				//为转换分配空间	
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
				pszMultiByte = new char[iSize + 1];
				WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, pszMultiByte, iSize, NULL, NULL);//宽字节转换	
				pszMultiByte[iSize] = '\0';
				str += pszMultiByte;//添加到string中	
				//释放空间	
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
			}
			flag = false;
		}
		ImmReleaseContext(hWnd, hIMC);//释放HIMC	
	}
}

void INPUTBOX::Draw()
{
	if (choose) setlinecolor(BLUE);
	else setlinecolor(RED);
	setfillcolor(WHITE);
	settextcolor(BLACK);
	outtextxy(x - textwidth(text.c_str()), y, text.c_str());	//提示信息
	fillrectangle(x, y, x + sizex, y + sizey);					//画输入框
	rectangle(x - 1, y - 1, x + sizex + 1, y + sizey + 1);		//使边框更清晰
	static bool cursor = false;									//光标
	int str_w = textwidth(str.c_str()) + 3;						//获取字符串像素长度
	int str_h = textheight(text.c_str());						//获取字符串像素高度（因为高度始终不变，所以直接用了text的高度）
	if (str == "")												//字符串为空时，设置光标位置
		str_w = 3;
	if (choose && cursor)
		line(x + str_w, y, x + str_w, y + str_h);
	static DWORD starttime;
	if (GetTickCount() - starttime > 500)
	{
		starttime = GetTickCount();
		cursor = !cursor;
	}
	
	RECT r = { x + 3, y + 3, x + sizex, y + sizey };
	if (hide) 
		for (int i = 0; i < str.length(); i++)
			outtextxy(x + 10 * i, y, '*');										//输出密文信息
	else drawtext(str.c_str(), &r, DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE);	//输出明文信息
}

string INPUTBOX::Input()
{
	if (kbhit())	//如果是ASCII输入	
	{
		char c = getch(); 
		switch (c)
		{
		case '\b':
			if (str.length()>0)
			{
				if (str.at(str.length() - 1) & 0x8000)
					str.erase(str.end() - 1);
				str.erase(str.end() - 1);
			}
			break;
		case 27: break;				//esc
		case 47: break;				//斜杠
		case 13: break;				//回车
		case 9:	 break;				//Tab
		case -32:
			switch (getch())		//处理方向键(没能力暂不处理)
			{
			case 72: break;			//上键
			case 77: break;			//下键
			case 75: break;			//左键
			case 80: break;			//右键
			}
			break;
		default:
			str += c;
		}
	}
	else	 //除此之外，检测是否有IME输入，如果有，则将输入结果添加到string中	
	{
		GetIMEString(GetHWnd(), str);
	}
	if (str.length()>100)
		str = "";
	return str;
}
void INPUTBOX::Clear()
{
	str = "";
}
bool INPUTBOX::Enable(int work)
{
	if (work == 0) enable = 0;
	else if (work == 1)
	{
		enable = 1;
		str = "";
	}
	return enable;
}
void INPUTBOX::Hide(bool thide)
{
	hide = thide;
}
bool INPUTBOX::Empty()
{
	return str == "";
}
bool INPUTBOX::Click()
{
	if (!enable) return false;
	if (mouse.uMsg == WM_LBUTTONDOWN) 
		if (mouse.x >= x && mouse.y >= y && mouse.x <= x + sizex && mouse.y <= y + sizey)
			choose = true;
		else choose = false;
	return choose;
}
void Control_Enable(bool Enable, CONTROL *b1, CONTROL *b2, CONTROL *b3, CONTROL *b4)
{
	if (b1) b1->Enable(Enable);
	if (b2) b2->Enable(Enable);
	if (b3) b3->Enable(Enable);
	if (b4) b4->Enable(Enable);
}
void Control_Draw(CONTROL *b1, CONTROL *b2, CONTROL *b3, CONTROL *b4)
{
	if (b1 && b1->Enable()) b1->Draw();
	if (b2 && b2->Enable()) b2->Draw();
	if (b3 && b3->Enable()) b3->Draw();
	if (b4 && b4->Enable()) b4->Draw();
}
