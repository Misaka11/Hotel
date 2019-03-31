#pragma once
using namespace std;
#include<easyx.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
struct room
{
	char type[20];		//房间类型
	int ID;				//房间号
	int num;			//人数
	float price;		//价格
	float VIPprice;		//会员价
	bool full;			//是否住人
	room()
	{
		strcpy(type, "");
		ID = num = price = VIPprice = full = 0;
	}
	room operator=(const room &t)
	{
		strcpy(type, t.type);
		ID = t.ID;
		num = t.num;
		price = t.price;
		VIPprice = t.VIPprice;
		full = t.full;
		return *this;
	}
	bool operator==(const room &t)
	{
		return ID == t.ID && num == t.num && price == t.price && VIPprice == t.VIPprice && full == t.full && !strcmp(type, t.type);
	}
};
struct USER
{
	string ID, PassWord;
};
struct CUSTUMER
{
	char Name[20];
	char ID[20];
	char Phone[20];
	int RoomID;
};
class HOTEL
{
private:
	vector<room> rooms;	//房间
	string INFO;		//宾馆所有房间信息
public:
	HOTEL();
	~HOTEL();					
	void EditRoom(room &Room);		//编辑房间
	void AddRoom(room &Room);		//添加房间
	void DelRoom(int ID);			//删除房间
	bool Admin(USER &user);			//验证是否为管理员
	string Show();					//宾馆所有信息
	string ShowOne(int ID);			//宾馆一间信息
	int Find(int ID);				//查询房间号（返回房间人数，不存在返回-1
	void CheckIn(CUSTUMER custumer[], int num);	//入住
	void CheckOut(int ID);						//退房
	bool Full(int ID);							//判断是否客满
	void SortByID();
	void SortByPrice();
};
inline bool SortByID1(const room &t1, const room &t2)
{
	return t1.ID < t2.ID;
}
inline bool SortByID2(const room &t1, const room &t2)
{
	return t1.ID > t2.ID;
}
inline bool SortByPrice1(const room &t1, const room &t2)
{
	return t1.price < t2.price;
}
inline bool SortByPrice2(const room &t1, const room &t2)
{
	return t1.price > t2.price;
}