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
	char type[20];		//��������
	int ID;				//�����
	int num;			//����
	float price;		//�۸�
	float VIPprice;		//��Ա��
	bool full;			//�Ƿ�ס��
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
	vector<room> rooms;	//����
	string INFO;		//�������з�����Ϣ
public:
	HOTEL();
	~HOTEL();					
	void EditRoom(room &Room);		//�༭����
	void AddRoom(room &Room);		//��ӷ���
	void DelRoom(int ID);			//ɾ������
	bool Admin(USER &user);			//��֤�Ƿ�Ϊ����Ա
	string Show();					//����������Ϣ
	string ShowOne(int ID);			//����һ����Ϣ
	int Find(int ID);				//��ѯ����ţ����ط��������������ڷ���-1
	void CheckIn(CUSTUMER custumer[], int num);	//��ס
	void CheckOut(int ID);						//�˷�
	bool Full(int ID);							//�ж��Ƿ����
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