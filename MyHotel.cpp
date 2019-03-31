#include "MyHotel.h"

//define HOTEL
HOTEL::HOTEL()		//读取房间数据
{
	ifstream fin;
	fin.open("Hotel.dat", ios::in | ios::binary);
	if (fin.fail())
	{
		MessageBox(NULL, "找不到Hotel文件", "Error", 0);
		exit(0);
	}
	room t;
	while (fin.read((char*)&t, sizeof(t)))
	{
		if (fin.fail()) break;
		rooms.push_back(t);
	}
	fin.close();
}
HOTEL::~HOTEL()		//记录房间数据
{
	ofstream fout;
	fout.open("Hotel.dat", ios::out | ios::binary);
	for (vector<room>::iterator roomit = rooms.begin(); roomit != rooms.end(); roomit++)
		fout.write((char*)&(*roomit), sizeof(*roomit));
	fout.close();
}

void HOTEL::AddRoom(room &Room)
{
	rooms.push_back(Room);
}
void HOTEL::DelRoom(int ID)
{
	for (vector<room>::iterator roomit = rooms.begin(); roomit != rooms.end();)
		if (roomit->ID == ID)
			roomit = rooms.erase(roomit);
		else roomit++;	
}
void HOTEL::EditRoom(room &Room)
{
	for (vector<room>::iterator roomit = rooms.begin(); roomit != rooms.end();roomit++)
	if (roomit->ID == Room.ID)
	{
		*roomit = Room;
	}
}
string HOTEL::Show()
{
	ostringstream oss;
	oss.str("");
	oss.setf(ios::left);
	oss << setw(15) << "房间号" << setw(15) << "房间类型" << setw(15) << "人数" <<setw(15) << "价格" << setw(15)<<"会员价" << "是否空房" << endl << endl;
	for (vector<room>::iterator roomit = rooms.begin(); roomit != rooms.end(); roomit++)
	{
		oss << setw(15) << roomit->ID
			<< setw(15) << roomit->type
			<< setw(15) << roomit->num
			<< setw(15) << roomit->price
			<< setw(15) << roomit->VIPprice;
		if (roomit->full) oss << setw(15) << "已满" << endl;
		else oss << setw(15) << "空房" << endl;
	}
	INFO = oss.str();
	return INFO;
}
string HOTEL::ShowOne(int ID)
{
	ostringstream oss;
	oss.str("");

	ifstream fin;
	fin.open("Custumer.dat", ios::in);
	CUSTUMER t; int n = 0;
	while (fin.read((char*)&t, sizeof(t)))
	{
		if (t.RoomID == ID)
			oss << "此房间第" << ++n << "个人的信息：" << endl
			<< "	姓名：" << t.Name << endl
			<< "	身份证：" << t.ID << endl
			<< "	电话：" << t.Phone << endl << endl;
	}
	return oss.str();
	fin.close();
}

bool HOTEL::Admin(USER &tuser)
{
	ifstream fin;
	fin.open("Administrator.txt", ios::in);
	if (fin.fail())
	{
		MessageBox(NULL, "找不到Administrator.dat文件", "Error", 0);
		exit(0);
	}
	string Z, M;
	while (!fin.eof())
	{
		fin >> Z >> M;
		if (fin.fail()) break;
		if (Z == tuser.ID && M == tuser.PassWord) return true;
	}
	return false;
}

int HOTEL::Find(int ID)
{
	for (vector<room>::iterator roomit = rooms.begin(); roomit != rooms.end(); roomit++)
	if (roomit->ID == ID)
		return roomit->num;				//返回房间人数
	return -1;							//没有此房间则返回-1
}
void HOTEL::CheckIn(CUSTUMER custumer[],int num)
{
	ofstream fout;
	fout.open("Custumer.dat", ios::app | ios::binary);
	if (fout.fail())
	{
		MessageBox(NULL, "找不到Custumer.dat文件", "Error", 0);
		exit(0);
	}
	for (int i = 0; i < num;i++)
		fout.write((char *)&custumer[i] ,sizeof(custumer[i]));
	for (vector<room>::iterator roomit = rooms.begin(); roomit != rooms.end(); roomit++)
	if (roomit->ID == custumer[0].RoomID)
	{
		roomit->full = true;
		break;
	}
	fout.close();
}
void HOTEL::CheckOut(int ID)
{
	ifstream fin;
	fin.open("Custumer.dat", ios::in | ios::binary);
	CUSTUMER temp; int n = 0;
	while (fin.read((char*)&temp, sizeof(temp))) n++;
	CUSTUMER *t = new CUSTUMER[n];
	n = 0;
	fin.close(); 
	fin.open("Custumer.dat", ios::in | ios::binary);
	while (fin.read((char*)&t[n], sizeof(t[n]))) n++;
	fin.close();
	ofstream fout;
	fout.open("Custumer.dat", ios::out | ios::binary);
	for (int i = 0; i < n; i++)
	if (t[i].RoomID != ID)
		fout.write((char*)&t[i], sizeof(t[i]));
	fout.close();
	for (vector<room>::iterator roomit = rooms.begin(); roomit != rooms.end(); roomit++)
	if (roomit->ID == ID)
	{
		roomit->full = false;
		break;
	}
	delete[] t;
}
bool HOTEL::Full(int ID)
{
	for (vector<room>::iterator roomit = rooms.begin(); roomit != rooms.end(); roomit++)
	if (roomit->ID == ID)
		return roomit->full;
	return false;
}
void HOTEL::SortByID()
{
	static bool UP = true;
		if (UP) sort(rooms.begin(), rooms.end(), SortByID1);
		else	sort(rooms.begin(), rooms.end(), SortByID2);
	UP = !UP;
}
void HOTEL::SortByPrice()
{
	static bool UP = true;
	if (UP)  sort(rooms.begin(), rooms.end(), SortByPrice1);
	else	 sort(rooms.begin(), rooms.end(), SortByPrice2);
	UP = !UP;
}