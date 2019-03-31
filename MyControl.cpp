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
	HIMC hIMC = ImmGetContext(hWnd);//��ȡHIMC	
	if (hIMC)
	{
		//������˵��һ�£������롰�й���Ϊ��	
		//�л����������뷨�����롰zhongguo��������ַ�������IME����ַ���	
		//�������뷨�б���ѡ����ַ������й��������IME����ַ���	
		static bool flag = false;//������ɱ�ǣ���������ʱ��IME����ַ�����Ϊ�գ���true��������ɺ�IME����ַ���Ϊ�գ���false	
		DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0);	//��ȡIME���������ַ����ĳ���	
		if (dwSize>0)//���IME����ַ�����Ϊ�գ���û�д��󣨴�ʱdwSizeΪ��ֵ��������������ɱ��Ϊtrue	
		{
			if (flag == false)
			{
				flag = true;
			}
		}
		else if (dwSize == 0 && flag)	//���IME����ַ���Ϊ�գ����ұ��Ϊtrue�����ȡIME����ַ���	
		{
			int iSize;	//IME����ַ����Ĵ�С	
			LPSTR pszMultiByte = NULL;//IME����ַ���ָ��	
			int ChineseSimpleAcp = 936;//���ֽ�ת��ʱ���ĵı���	
			WCHAR *lpWideStr = NULL;//���ֽ��ַ�����	
			dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);//��ȡIME����ַ����Ĵ�С	
			if (dwSize>0)	//���IME����ַ�����Ϊ�գ���û�д���	
			{
				dwSize += sizeof(WCHAR);//��СҪ����NULL������	
				//Ϊ��ȡIME����ַ�������ռ�	
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				lpWideStr = new WCHAR[dwSize];
				memset(lpWideStr, 0, dwSize);	//��ս���ռ�	
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpWideStr, dwSize);//��ȡIME����ַ����������ȡ���ǿ��ֽ�	
				iSize = WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, NULL, 0, NULL, NULL);//���㽫IME����ַ���ת��ΪASCII��׼�ֽں�Ĵ�С	
				//Ϊת������ռ�	
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
				pszMultiByte = new char[iSize + 1];
				WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, pszMultiByte, iSize, NULL, NULL);//���ֽ�ת��	
				pszMultiByte[iSize] = '\0';
				str += pszMultiByte;//��ӵ�string��	
				//�ͷſռ�	
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
		ImmReleaseContext(hWnd, hIMC);//�ͷ�HIMC	
	}
}

void INPUTBOX::Draw()
{
	if (choose) setlinecolor(BLUE);
	else setlinecolor(RED);
	setfillcolor(WHITE);
	settextcolor(BLACK);
	outtextxy(x - textwidth(text.c_str()), y, text.c_str());	//��ʾ��Ϣ
	fillrectangle(x, y, x + sizex, y + sizey);					//�������
	rectangle(x - 1, y - 1, x + sizex + 1, y + sizey + 1);		//ʹ�߿������
	static bool cursor = false;									//���
	int str_w = textwidth(str.c_str()) + 3;						//��ȡ�ַ������س���
	int str_h = textheight(text.c_str());						//��ȡ�ַ������ظ߶ȣ���Ϊ�߶�ʼ�ղ��䣬����ֱ������text�ĸ߶ȣ�
	if (str == "")												//�ַ���Ϊ��ʱ�����ù��λ��
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
			outtextxy(x + 10 * i, y, '*');										//���������Ϣ
	else drawtext(str.c_str(), &r, DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE);	//���������Ϣ
}

string INPUTBOX::Input()
{
	if (kbhit())	//�����ASCII����	
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
		case 47: break;				//б��
		case 13: break;				//�س�
		case 9:	 break;				//Tab
		case -32:
			switch (getch())		//�������(û�����ݲ�����)
			{
			case 72: break;			//�ϼ�
			case 77: break;			//�¼�
			case 75: break;			//���
			case 80: break;			//�Ҽ�
			}
			break;
		default:
			str += c;
		}
	}
	else	 //����֮�⣬����Ƿ���IME���룬����У�����������ӵ�string��	
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
