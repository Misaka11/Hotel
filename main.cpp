#include <easyx.h>
#include "Myhotel.h"
#include "MyControl.h"
#pragma comment(lib,"WINMM.LIB")
const int SCREENW = 1024;					//程序窗口大小
const int SCREENH = 768;
bool gameover = false;						//退出变量
bool admin = false;							//管理员身份
IMAGE img_back0,img_back1,img_back2,img_back3;//背景图案
MOUSEMSG mouse;								//鼠标
HOTEL hotel;								//旅店
UI UI_Now;									//当前界面
BUTTON UI0_ZhuSu, UI0_GuanLi, UI0_BangZhu, UI0_TuiChu;						//初始界面控件 0
BUTTON UI1_ZhuSu, UI1_TuiFang, UI1_FanHui, UI1_Sort1, UI1_Sort2;			//用户界面控件 1
LABEL UI1_Show;
BUTTON UI1_Add, UI1_Del, UI1_Edit,UI1_Look;									//管理员界面控件 1
INPUTBOX UI2_ZhangHu, UI2_MiMa;												//登录界面控件 2
BUTTON UI2_QueDing, UI2_FanHui;				
BUTTON UI3_FanHui;															//展示界面控件 3
LABEL UI3_Show;
INPUTBOX UI4_XingMing,UI4_ID, UI4_Phone;									//登记界面控件 4
BUTTON UI4_QueDing, UI4_FanHui;		
INPUTBOX UI5_ID, UI5_Type, UI5_Price, UI5_Num, UI5_VIPprice;				//设置房间界面 5
BUTTON UI5_QueDing, UI5_FanHui;
BUTTON UI_Music;															//音乐按钮
//****************************************辅助变量
int UI4_RoomID,UI4_Num,UI4_CurNum;		//房间号，需要登记的人数以及当前登记到第几个人
int UI5_Operator;						//要进行的操作		1添加 2修改
ostringstream UI4_oss;					//方便文字的输出
USER user; 
CUSTUMER custumer;
CUSTUMER custumers[9];
string HELP =  "作者很懒，什么都没有留下" ;
//程序初始化
void Game_Init();
//程序信息刷新
void Game_Update();
//程序逻辑处理
void Game_Run();
//程序绘画
void Game_Draw();
//程序结束
void Game_End();
//主函数
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
	settextstyle(20, 0, "楷体");
	setbkmode(TRANSPARENT);
	//加载资源
	loadimage(&img_back0, "0.jpg", SCREENW, SCREENH, true);
	putimage(0, 0, &img_back0);
	loadimage(&img_back1, "1.jpg", SCREENW, SCREENH, true);
	loadimage(&img_back2, "2.jpg", SCREENW, SCREENH, true);
	loadimage(&img_back3, "3.jpg", SCREENW, SCREENH, true);
	mciSendString("open \"backmusic.mp3\" alias mymusic", NULL, 0, NULL);
	//各个界面控件属性设置
	UI_Music.SetValues(900, 100, 70, 30, "开音乐");
	//初始界面设置
	UI0_ZhuSu.SetValues(500, 200, 60, 30, "住宿");
	UI0_GuanLi.SetValues(500, 300, 110, 30, "管理员登录");
	UI0_BangZhu.SetValues(500, 400, 60, 30, "帮助");
	UI0_TuiChu.SetValues(500, 500, 60, 30, "退出");
	//用户和管理员界面设置
	UI1_ZhuSu.SetValues(30, 100, 60, 30, "住宿");
	UI1_TuiFang.SetValues(30, 200, 60, 30, "退房");
	UI1_FanHui.SetValues(30, 300, 60, 30, "返回");
	UI1_Add.SetValues(30, 400, 90, 30, "添加房间");
	UI1_Del.SetValues(30, 500, 90, 30, "删除房间");
	UI1_Edit.SetValues(30, 600, 90, 30, "修改房间");
	UI1_Look.SetValues(30, 700, 90, 30, "查看房间");
	UI1_Sort1.SetValues(150, 150, 50, 30, "↑↓");
	UI1_Sort2.SetValues(600, 150, 50, 30, "↑↓");
	UI1_Show.SetValues(150, 200, 0, 0, "");
	//登录界面设置
	UI2_ZhangHu.SetValues(200, 200, 180, 30, "账户：");
	UI2_MiMa.SetValues(200, 300, 180, 30, "密码；");
	UI2_FanHui.SetValues(150, 400, 60, 30, "返回");
	UI2_QueDing.SetValues(300, 400, 60, 30, "确定");
	UI2_MiMa.Hide(true);
	//展示界面设置
	UI3_FanHui.SetValues(700, 700, 60, 30, "返回");
	UI3_Show.SetValues(200, 200, 0, 0, "");
	//登记界面设置
	UI4_XingMing.SetValues(200, 200, 180, 30, "*姓名：");
	UI4_ID.SetValues(200, 300, 180, 30, "*身份证号：");
	UI4_Phone.SetValues(200, 400, 180, 30, "手机号：");
	UI4_QueDing.SetValues(300, 500, 60, 30, "确定");
	UI4_FanHui.SetValues(150, 500, 60, 30, "返回");
	//设置房间界面设置
	UI5_ID.SetValues(200, 200, 180, 30, "房间号：");
	UI5_Type.SetValues(200, 300, 180, 30, "房间类型：");
	UI5_Num.SetValues(200, 400, 180, 30, "房间人数：");
	UI5_Price.SetValues(200, 500, 180, 30, "价格：");
	UI5_VIPprice.SetValues(200, 600, 180, 30, "会员价：");
	UI5_QueDing.SetValues(300, 700, 60, 30, "确定");
	UI5_FanHui.SetValues(150, 700, 60, 30, "返回");
	//初始界面
	UI_Now = UI0;
	//禁用管理员功能
	Control_Enable(false, &UI1_Add, &UI1_Del, &UI1_Edit, &UI1_Look);
}

void Game_Update()
{
	if (MouseHit())				//鼠标有信息，刷新鼠标
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
			UI_Music.SetText("开音乐");
		}
		else
		{
			play = true;
			mciSendString("play mymusic repeat", NULL, 0, NULL);
			UI_Music.SetText("关音乐");
		}
	}
	switch (UI_Now)
	{
	case UI0:
		if (UI0_ZhuSu.Click())							//点击住宿键，进入用户界面
		{
			UI1_Show.SetText(hotel.Show());
			UI_Now = UI1;
		}
		if (UI0_GuanLi.Click())							//点击管理键，进入登录界面
		{
			if (admin)
			{
				admin = false;
				UI0_GuanLi.SetText("管理员登录");
				Control_Enable(false, &UI1_Add, &UI1_Del, &UI1_Edit, &UI1_Look);
			}
			else	UI_Now = UI2;
		}
		if (UI0_BangZhu.Click())						//点击帮助键，进入帮助界面
		{
			UI_Now = UI3;
			UI3_Show.SetText(HELP);
		}
		if (UI0_TuiChu.Click()) gameover = true;
		break;
	case UI1:
		if (UI1_ZhuSu.Click())
		{
			int ID = InputBoxS(ID, "%d", "客官要哪间房？", "请输入");
			UI4_Num = hotel.Find(ID);
			if (UI4_Num > 0)						//如果存在此房间
			if (!hotel.Full(ID))				//且此房间没人
			{
				UI_Now = UI4;
				UI4_CurNum = 0;
				UI4_RoomID = ID;
				UI4_oss.str("");
				UI4_oss << "请完善第 " << UI4_CurNum + 1 << " 人的信息,共 " << UI4_Num << " 个人";
			}
			else MessageBox(GetHWnd(), "对不起，此房间已满", "sorry", 0);
			else MessageBox(GetHWnd(), "对不起，没找到此房间...", "", 0);
			UI1_Show.SetText(hotel.Show());
		}
		if (UI1_TuiFang.Click())
		{
			int ID = InputBoxS(ID, "%d", "客官要退哪间房？", "请输入");
			if (hotel.Find(ID) > 0)
			if (hotel.Full(ID) > 0)
			{
				hotel.CheckOut(ID);
				MessageBox(GetHWnd(), "退房成功，欢迎下次光临！", "", 0);
			}
			else MessageBox(GetHWnd(), "此房间没人", "", 0);
			else MessageBox(GetHWnd(), "对不起，没找到此房间...", "", 0);
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
			int ID = InputBoxS(ID, "%d", "需要删除的房间号房间号", "请输入");
			if (hotel.Full(ID))
				MessageBox(GetHWnd(), "房间有人，请先退房在删除", "操作不成功", 0);
			else
				hotel.DelRoom(ID);
			UI1_Show.SetText(hotel.Show());
		}
		if (UI1_Look.Click())
		{
			int ID = InputBoxS(ID, "%d", "需要查看的房间号", "请输入");
			if (hotel.Find(ID) > 0)
			if (hotel.Full(ID))
			{
				UI3_Show.SetText(hotel.ShowOne(ID));
				UI_Now = UI3;
			}
			else MessageBox(GetHWnd(), "此房间没人", "", 0);
			else MessageBox(GetHWnd(), "对不起，没找到此房间...", "", 0);
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
				MessageBox(GetHWnd(), "登录成功", "欢迎登录", 0);
				UI0_GuanLi.SetText("管理员退出");
				UI_Now = UI0;
				admin = true;
				UI2_ZhangHu.Clear();
				UI2_MiMa.Clear();
			}
			else MessageBox(GetHWnd(), "账户或秘密有误", "错误", 0);
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
				MessageBox(GetHWnd(), "姓名和身份证不能为空", "请完善信息", 0);
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
			UI4_oss << "请完善第 " << UI4_CurNum + 1 << " 人的信息,共 " << UI4_Num << " 个人";
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
				MessageBox(GetHWnd(), "请完善信息", "", 0);
				break;
			}
			t.ID = stoi(UI5_ID.Input().c_str());
			if (t.ID <= 0)
			{
				MessageBox(GetHWnd(), "房间号需大于0", "房间号非法", 0);
				break;
			}
			if (hotel.Find(t.ID) > 0 && UI5_Operator == 1)
			{
				MessageBox(GetHWnd(), "此房间号已经存在，请勿重复添加", "房间号非法", 0);
				break;
			}
			if (hotel.Full(t.ID) > 0 && UI5_Operator == 2)
			{
				MessageBox(GetHWnd(), "此房间有人，请退房后修改房间信息", "此房间不能修改", 0);
				break;
			}
			t.num = stoi(UI5_Num.Input().c_str());
			if (t.num <= 0 || t.num > 8)
			{
				MessageBox(GetHWnd(), "房间人数需1-8人", "房间人数非法", 0);
				break;
			}
			t.price = stof(UI5_Price.Input().c_str());
			t.VIPprice = stof(UI5_VIPprice.Input().c_str());
			if (t.price <= 0 || t.VIPprice <= 0)
			{
				MessageBox(GetHWnd(), "房间价格需大于0", "房间价格非法", 0);
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
		outtextxy(300, 100, "请完善房间信息");
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
