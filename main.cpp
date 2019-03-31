#include <easyx.h>
#include "Myhotel.h"
#include "MyControl.h"
#pragma comment(lib,"WINMM.LIB")
const int SCREENW = 1024;					//���򴰿ڴ�С
const int SCREENH = 768;
bool gameover = false;						//�˳�����
bool admin = false;							//����Ա���
IMAGE img_back0,img_back1,img_back2,img_back3;//����ͼ��
MOUSEMSG mouse;								//���
HOTEL hotel;								//�õ�
UI UI_Now;									//��ǰ����
BUTTON UI0_ZhuSu, UI0_GuanLi, UI0_BangZhu, UI0_TuiChu;						//��ʼ����ؼ� 0
BUTTON UI1_ZhuSu, UI1_TuiFang, UI1_FanHui, UI1_Sort1, UI1_Sort2;			//�û�����ؼ� 1
LABEL UI1_Show;
BUTTON UI1_Add, UI1_Del, UI1_Edit,UI1_Look;									//����Ա����ؼ� 1
INPUTBOX UI2_ZhangHu, UI2_MiMa;												//��¼����ؼ� 2
BUTTON UI2_QueDing, UI2_FanHui;				
BUTTON UI3_FanHui;															//չʾ����ؼ� 3
LABEL UI3_Show;
INPUTBOX UI4_XingMing,UI4_ID, UI4_Phone;									//�Ǽǽ���ؼ� 4
BUTTON UI4_QueDing, UI4_FanHui;		
INPUTBOX UI5_ID, UI5_Type, UI5_Price, UI5_Num, UI5_VIPprice;				//���÷������ 5
BUTTON UI5_QueDing, UI5_FanHui;
BUTTON UI_Music;															//���ְ�ť
//****************************************��������
int UI4_RoomID,UI4_Num,UI4_CurNum;		//����ţ���Ҫ�Ǽǵ������Լ���ǰ�Ǽǵ��ڼ�����
int UI5_Operator;						//Ҫ���еĲ���		1��� 2�޸�
ostringstream UI4_oss;					//�������ֵ����
USER user; 
CUSTUMER custumer;
CUSTUMER custumers[9];
string HELP =  "���ߺ�����ʲô��û������" ;
//�����ʼ��
void Game_Init();
//������Ϣˢ��
void Game_Update();
//�����߼�����
void Game_Run();
//����滭
void Game_Draw();
//�������
void Game_End();
//������
void main()
{
	Game_Init();
	while (!gameover)
	{
		Game_Update();
		Game_Run();
		Game_Draw();
	}
	Game_End();
}

void Game_Init()
{
	initgraph(SCREENW, SCREENH);
	settextstyle(20, 0, "����");
	setbkmode(TRANSPARENT);
	//������Դ
	loadimage(&img_back0, "0.jpg", SCREENW, SCREENH, true);
	putimage(0, 0, &img_back0);
	loadimage(&img_back1, "1.jpg", SCREENW, SCREENH, true);
	loadimage(&img_back2, "2.jpg", SCREENW, SCREENH, true);
	loadimage(&img_back3, "3.jpg", SCREENW, SCREENH, true);
	mciSendString("open \"backmusic.mp3\" alias mymusic", NULL, 0, NULL);
	//��������ؼ���������
	UI_Music.SetValues(900, 100, 70, 30, "������");
	//��ʼ��������
	UI0_ZhuSu.SetValues(500, 200, 60, 30, "ס��");
	UI0_GuanLi.SetValues(500, 300, 110, 30, "����Ա��¼");
	UI0_BangZhu.SetValues(500, 400, 60, 30, "����");
	UI0_TuiChu.SetValues(500, 500, 60, 30, "�˳�");
	//�û��͹���Ա��������
	UI1_ZhuSu.SetValues(30, 100, 60, 30, "ס��");
	UI1_TuiFang.SetValues(30, 200, 60, 30, "�˷�");
	UI1_FanHui.SetValues(30, 300, 60, 30, "����");
	UI1_Add.SetValues(30, 400, 90, 30, "��ӷ���");
	UI1_Del.SetValues(30, 500, 90, 30, "ɾ������");
	UI1_Edit.SetValues(30, 600, 90, 30, "�޸ķ���");
	UI1_Look.SetValues(30, 700, 90, 30, "�鿴����");
	UI1_Sort1.SetValues(150, 150, 50, 30, "����");
	UI1_Sort2.SetValues(600, 150, 50, 30, "����");
	UI1_Show.SetValues(150, 200, 0, 0, "");
	//��¼��������
	UI2_ZhangHu.SetValues(200, 200, 180, 30, "�˻���");
	UI2_MiMa.SetValues(200, 300, 180, 30, "���룻");
	UI2_FanHui.SetValues(150, 400, 60, 30, "����");
	UI2_QueDing.SetValues(300, 400, 60, 30, "ȷ��");
	UI2_MiMa.Hide(true);
	//չʾ��������
	UI3_FanHui.SetValues(700, 700, 60, 30, "����");
	UI3_Show.SetValues(200, 200, 0, 0, "");
	//�Ǽǽ�������
	UI4_XingMing.SetValues(200, 200, 180, 30, "*������");
	UI4_ID.SetValues(200, 300, 180, 30, "*���֤�ţ�");
	UI4_Phone.SetValues(200, 400, 180, 30, "�ֻ��ţ�");
	UI4_QueDing.SetValues(300, 500, 60, 30, "ȷ��");
	UI4_FanHui.SetValues(150, 500, 60, 30, "����");
	//���÷����������
	UI5_ID.SetValues(200, 200, 180, 30, "����ţ�");
	UI5_Type.SetValues(200, 300, 180, 30, "�������ͣ�");
	UI5_Num.SetValues(200, 400, 180, 30, "����������");
	UI5_Price.SetValues(200, 500, 180, 30, "�۸�");
	UI5_VIPprice.SetValues(200, 600, 180, 30, "��Ա�ۣ�");
	UI5_QueDing.SetValues(300, 700, 60, 30, "ȷ��");
	UI5_FanHui.SetValues(150, 700, 60, 30, "����");
	//��ʼ����
	UI_Now = UI0;
	//���ù���Ա����
	Control_Enable(false, &UI1_Add, &UI1_Del, &UI1_Edit, &UI1_Look);
}

void Game_Update()
{
	if (MouseHit())				//�������Ϣ��ˢ�����
		mouse = GetMouseMsg();		
	FlushMouseMsgBuffer();	
}

void Game_Run()
{
	if (UI_Music.Click())
	{
		static bool play = false;
		if (play)
		{
			play = false;
			mciSendString("pause mymusic", NULL, 0, NULL);
			UI_Music.SetText("������");
		}
		else
		{
			play = true;
			mciSendString("play mymusic repeat", NULL, 0, NULL);
			UI_Music.SetText("������");
		}
	}
	switch (UI_Now)
	{
	case UI0:
		if (UI0_ZhuSu.Click())							//���ס�޼��������û�����
		{
			UI1_Show.SetText(hotel.Show());
			UI_Now = UI1;
		}
		if (UI0_GuanLi.Click())							//���������������¼����
		{
			if (admin)
			{
				admin = false;
				UI0_GuanLi.SetText("����Ա��¼");
				Control_Enable(false, &UI1_Add, &UI1_Del, &UI1_Edit, &UI1_Look);
			}
			else	UI_Now = UI2;
		}
		if (UI0_BangZhu.Click())						//����������������������
		{
			UI_Now = UI3;
			UI3_Show.SetText(HELP);
		}
		if (UI0_TuiChu.Click()) gameover = true;
		break;
	case UI1:
		if (UI1_ZhuSu.Click())
		{
			int ID = InputBoxS(ID, "%d", "�͹�Ҫ�ļ䷿��", "������");
			UI4_Num = hotel.Find(ID);
			if (UI4_Num > 0)						//������ڴ˷���
			if (!hotel.Full(ID))				//�Ҵ˷���û��
			{
				UI_Now = UI4;
				UI4_CurNum = 0;
				UI4_RoomID = ID;
				UI4_oss.str("");
				UI4_oss << "�����Ƶ� " << UI4_CurNum + 1 << " �˵���Ϣ,�� " << UI4_Num << " ����";
			}
			else MessageBox(GetHWnd(), "�Բ��𣬴˷�������", "sorry", 0);
			else MessageBox(GetHWnd(), "�Բ���û�ҵ��˷���...", "", 0);
			UI1_Show.SetText(hotel.Show());
		}
		if (UI1_TuiFang.Click())
		{
			int ID = InputBoxS(ID, "%d", "�͹�Ҫ���ļ䷿��", "������");
			if (hotel.Find(ID) > 0)
			if (hotel.Full(ID) > 0)
			{
				hotel.CheckOut(ID);
				MessageBox(GetHWnd(), "�˷��ɹ�����ӭ�´ι��٣�", "", 0);
			}
			else MessageBox(GetHWnd(), "�˷���û��", "", 0);
			else MessageBox(GetHWnd(), "�Բ���û�ҵ��˷���...", "", 0);
			UI1_Show.SetText(hotel.Show());
		}
		if (UI1_Add.Click())
		{
			UI_Now = UI5;
			UI5_Operator = 1;
		}
		if (UI1_Edit.Click())
		{
			UI_Now = UI5;
			UI5_Operator = 2;
		}
		if (UI1_Del.Click())
		{
			int ID = InputBoxS(ID, "%d", "��Ҫɾ���ķ���ŷ����", "������");
			if (hotel.Full(ID))
				MessageBox(GetHWnd(), "�������ˣ������˷���ɾ��", "�������ɹ�", 0);
			else
				hotel.DelRoom(ID);
			UI1_Show.SetText(hotel.Show());
		}
		if (UI1_Look.Click())
		{
			int ID = InputBoxS(ID, "%d", "��Ҫ�鿴�ķ����", "������");
			if (hotel.Find(ID) > 0)
			if (hotel.Full(ID))
			{
				UI3_Show.SetText(hotel.ShowOne(ID));
				UI_Now = UI3;
			}
			else MessageBox(GetHWnd(), "�˷���û��", "", 0);
			else MessageBox(GetHWnd(), "�Բ���û�ҵ��˷���...", "", 0);
		}
		if (UI1_FanHui.Click()) UI_Now = UI0;
		if (UI1_Sort1.Click())
		{
			hotel.SortByID();
			UI1_Show.SetText(hotel.Show());
		}
		if (UI1_Sort2.Click()) 
		{
			hotel.SortByPrice(); 
			UI1_Show.SetText(hotel.Show());
		}
		break;
	case UI2:
		if (UI2_ZhangHu.Click()) UI2_ZhangHu.Input();
		if (UI2_MiMa.Click()) UI2_MiMa.Input();
		if (UI2_QueDing.Click())
		{
			user.ID = UI2_ZhangHu.Input();
			user.PassWord = UI2_MiMa.Input();
			if (hotel.Admin(user))
			{
				Control_Enable(true, &UI1_Add, &UI1_Del, &UI1_Edit, &UI1_Look);
				MessageBox(GetHWnd(), "��¼�ɹ�", "��ӭ��¼", 0);
				UI0_GuanLi.SetText("����Ա�˳�");
				UI_Now = UI0;
				admin = true;
				UI2_ZhangHu.Clear();
				UI2_MiMa.Clear();
			}
			else MessageBox(GetHWnd(), "�˻�����������", "����", 0);
		}
		if (UI2_FanHui.Click()) UI_Now = UI0;
		break;
	case UI3:
		if (UI3_FanHui.Click()) UI_Now = UI0;
		break;
	case UI4:
		if (UI4_XingMing.Click()) UI4_XingMing.Input();
		if (UI4_ID.Click())	UI4_ID.Input();
		if (UI4_Phone.Click()) UI4_Phone.Input();

		if (UI4_FanHui.Click())
		{
			UI4_Num = UI4_CurNum = 0;
			UI_Now = UI1;
			UI1_Show.SetText(hotel.Show());
			UI4_XingMing.Clear();
			UI4_ID.Clear();
			UI4_Phone.Clear();
		}
		if (UI4_QueDing.Click())
		{
			if (UI4_XingMing.Empty() || UI4_ID.Empty())
			{
				MessageBox(GetHWnd(), "���������֤����Ϊ��", "��������Ϣ", 0);
				break;
			}
			strcpy(custumers[UI4_CurNum].Name, UI4_XingMing.Input().c_str());
			strcpy(custumers[UI4_CurNum].ID, UI4_ID.Input().c_str());
			strcpy(custumers[UI4_CurNum].Phone, UI4_Phone.Input().c_str());
			custumers[UI4_CurNum].RoomID = UI4_RoomID;
			UI4_XingMing.Clear();
			UI4_ID.Clear();
			UI4_Phone.Clear();
			if (UI4_CurNum + 1 == UI4_Num)
			{
				hotel.CheckIn(custumers, UI4_Num);
				UI4_Num = UI4_CurNum = 0;
				UI_Now = UI1;
				UI1_Show.SetText(hotel.Show());
			}
			else UI4_CurNum++;
			UI4_oss.str("");
			UI4_oss << "�����Ƶ� " << UI4_CurNum + 1 << " �˵���Ϣ,�� " << UI4_Num << " ����";
		}
		break;
	case UI5:
		if (UI5_ID.Click()) UI5_ID.Input();
		if (UI5_Type.Click()) UI5_Type.Input();
		if (UI5_Num.Click()) UI5_Num.Input();
		if (UI5_Price.Click()) UI5_Price.Input();
		if (UI5_VIPprice.Click()) UI5_VIPprice.Input();
		if (UI5_FanHui.Click())
		{ 
			UI_Now = UI1;
			UI1_Show.SetText(hotel.Show()); 
		}
		if (UI5_QueDing.Click())
		{
			room t;
			if (UI5_ID.Empty() || UI5_Type.Empty() || UI5_Num.Empty() || UI5_Price.Empty() || UI5_VIPprice.Empty())
			{
				MessageBox(GetHWnd(), "��������Ϣ", "", 0);
				break;
			}
			t.ID = stoi(UI5_ID.Input().c_str());
			if (t.ID <= 0)
			{
				MessageBox(GetHWnd(), "����������0", "����ŷǷ�", 0);
				break;
			}
			if (hotel.Find(t.ID) > 0 && UI5_Operator == 1)
			{
				MessageBox(GetHWnd(), "�˷�����Ѿ����ڣ������ظ����", "����ŷǷ�", 0);
				break;
			}
			if (hotel.Full(t.ID) > 0 && UI5_Operator == 2)
			{
				MessageBox(GetHWnd(), "�˷������ˣ����˷����޸ķ�����Ϣ", "�˷��䲻���޸�", 0);
				break;
			}
			t.num = stoi(UI5_Num.Input().c_str());
			if (t.num <= 0 || t.num > 8)
			{
				MessageBox(GetHWnd(), "����������1-8��", "���������Ƿ�", 0);
				break;
			}
			t.price = stof(UI5_Price.Input().c_str());
			t.VIPprice = stof(UI5_VIPprice.Input().c_str());
			if (t.price <= 0 || t.VIPprice <= 0)
			{
				MessageBox(GetHWnd(), "����۸������0", "����۸�Ƿ�", 0);
				break;
			}
			strcpy(t.type, UI5_Type.Input().c_str());
			if (UI5_Operator == 1) hotel.AddRoom(t);
			else if (UI5_Operator == 2) hotel.EditRoom(t);
			UI_Now = UI1;
			UI1_Show.SetText(hotel.Show());
			UI5_ID.Clear();
			UI5_Num.Clear();
			UI5_Price.Clear();
			UI5_VIPprice.Clear();
			UI5_Type.Clear();
		}
	}
}

void Game_Draw()
{
	BeginBatchDraw();
	cleardevice();

	switch (UI_Now)
	{
	case UI0:
		putimage(0, 0, &img_back0);
		Control_Draw(&UI0_ZhuSu, &UI0_GuanLi, &UI0_BangZhu, &UI0_TuiChu);
		break;
	case UI1:
		putimage(0, 0, &img_back1);
		Control_Draw(&UI1_ZhuSu, &UI1_TuiFang, &UI1_FanHui,&UI1_Show);
		Control_Draw(&UI1_Sort1, &UI1_Sort2);
		if (admin) Control_Draw(&UI1_Edit, &UI1_Look, &UI1_Add, &UI1_Del);
		break;
	case UI2:
		putimage(0, 0, &img_back2);
		Control_Draw(&UI2_ZhangHu, &UI2_MiMa, &UI2_QueDing, &UI2_FanHui);
		break;
	case UI3:
		putimage(0, 0, &img_back3);
		Control_Draw(&UI3_FanHui,&UI3_Show);
		break;
	case UI4:
		putimage(0, 0, &img_back2);
		outtextxy(300, 100, UI4_oss.str().c_str());
		Control_Draw(&UI4_XingMing, &UI4_Phone, &UI4_ID);
		Control_Draw(&UI4_QueDing, &UI4_FanHui);
		break;
	case UI5:
		putimage(0, 0, &img_back2);
		outtextxy(300, 100, "�����Ʒ�����Ϣ");
		Control_Draw(&UI5_ID, &UI5_Num, &UI5_Type, &UI5_VIPprice);
		Control_Draw(&UI5_Price, &UI5_FanHui, &UI5_QueDing);
		break;
	}
	UI_Music.Draw();

	EndBatchDraw();
	FlushBatchDraw();
}

void Game_End()
{
	mciSendString("close mymusic", NULL, 0, NULL);
	closegraph();
}
