// ArchiveMaker.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../Common/EntityDataSerializer.h"
#include <iostream>
#include <fstream>
#include "../Common/Property.h"
using namespace JSB;
using namespace std;

#define READ_AND_CHECK(x) { i >> (x); if (i.fail()) throw exception("reading "#x##" failed."); else cout << (x) << endl;  }

void makeProp(ifstream& i, DataStream& data)
{
	Property prop;

	READ_AND_CHECK(prop.name);
	READ_AND_CHECK(prop.hp);
	READ_AND_CHECK(prop.str);
	READ_AND_CHECK(prop.dex);
	READ_AND_CHECK(prop.inte);
	READ_AND_CHECK(prop.speed);
	READ_AND_CHECK(prop.level);
	READ_AND_CHECK(prop.exp);

	prop.progress = 0;

	String end;
	i >> end;
	
	if (end != "[/property]" )
		throw exception("property need end id");

	data << EntityDataSerializer::getKeyword(EntityDataSerializer::ID_PROP)->val;
	data << prop.name;
	data << prop.hp;
	data << prop.str;
	data << prop.dex;
	data << prop.inte;
	data << prop.speed;
	data << prop.level;
	data << prop.exp;
	data << prop.progress;
	data << EntityDataSerializer::getKeyword(EntityDataSerializer::ID_END)->val;

}

void makeSkill(ifstream& i, DataStream& data)
{
	String skill;
	while (true)
	{
		i >> skill;

		if (i.fail())
			throw exception("skill format error");
		if (skill == "[/skill]")
			break;

		cout << "skill: " << skill << endl;
		if (!skill.empty())
		{
			data << EntityDataSerializer::getKeyword(EntityDataSerializer::ID_SKILL)->val;
			data << skill;
			data << EntityDataSerializer::getKeyword(EntityDataSerializer::ID_END)->val;
		}
	} 
}

void make(const String& input ,const String& output )
{
	ifstream i(input);
	if (!i)
	{
		stringstream ss;
		ss << "cant find file \"" << input << "\""<< endl;

		throw exception(ss.str().c_str());
	}
	DataStream data;

	while (true)
	{
		String id;
		i >> id;
		if (i.eof())
			break;
		if (id == "[property]")
		{
			makeProp(i, data);
		}
		else if (id == "[skill]")
		{
			makeSkill(i, data);
		}
		else
		{
			stringstream ss;
			ss << "unknown key word \"" << id << "\"" << endl;
			throw exception(ss.str().c_str());

		}
	}

	ofstream o(output, ios::binary);
	o.write(data.data(), data.size());
	if (!o)
		throw exception("��ȷ���ļ�������������ռ��.");
	cout << "finish!" << endl << endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//system("pause");
	try
	{
		if (argc == 5)
		{
			_TCHAR** input = argv + 1;
			if (String(*input) == STR("-s"))
			{
				String src(*++input);
				if (String(*++input) == STR("-o"))
				{
					make(src, *++input);
				}
				else
				{
					cout << "command format error";
				}
			}
		}
		else if (argc == 2)
		{
			_TCHAR** input = argv + 1;
			String src(*input);
			size_t pos = src.rfind('.');
			if (pos == ~0)
				pos = src.size();
			String dst(src.c_str(), pos);
			dst = dst + ".ent";
			make(src, dst);
		}
		else if (argc == 1)
		{
			cout
				<< "׼��һ���ı��ļ�����ʽ���£�" << endl
				<< "[property]" << endl
				<< "��" << endl
				<< "Ѫ" << endl
				<< "��" << endl
				<< "��" << endl
				<< "��" << endl
				<< "��" << endl
				<< "��" << endl
				<< "��" << endl

				<< "[/property]" << endl
				<< "[skill]" << endl
				<< "��1" << endl
				<< "��2" << endl
				<< "��3" << endl
				<< "[/skill]"<<endl;

			while (true)
			{
				String cmd;
				cout << "\n1.���ļ�����\n2.����\n";
				cin >> cmd;

				if (cmd == "1")
				{
					while (true)
					{
						cout << "\n\n�ı��ļ�·��: ";
						String file;
						cin >> file;
						size_t pos = file.rfind('.');
						if (pos == ~0)
							pos = file.size();
						String out(file.c_str(), pos);
						out = out + ".ent";

						make(file, out);

						cout << "\n���ɵ��ļ�����" << out << endl;

					}
				}
				else if (cmd == "2")
				{
					Property prop;
					DataStream data;

#define MK_PROP(n, p) { cout << (n) << ": "; cin >> (p); data << (p);}

					data << EntityDataSerializer::getKeyword(EntityDataSerializer::ID_PROP)->val;
					cout << "[����]" << endl;
					MK_PROP("��", prop.name);
					MK_PROP("Ѫ", prop.hp);
					MK_PROP("��", prop.str);
					MK_PROP("��", prop.dex);
					MK_PROP("��", prop.inte);
					MK_PROP("��", prop.speed);
					MK_PROP("��", prop.level);
					data << (prop.exp = 0);
					data << (prop.progress = 0);

					data << EntityDataSerializer::getKeyword(EntityDataSerializer::ID_END)->val;

					cout << "[����]" << endl;
					String skill;
					while (true)
					{
						cout << "��Ӽ���(����\"q\"�˳����): ";
						cin >> skill;
						if (skill[0] == 'q') break;

						{
							data << EntityDataSerializer::getKeyword(EntityDataSerializer::ID_SKILL)->val;
							data << skill;
							data << EntityDataSerializer::getKeyword(EntityDataSerializer::ID_END)->val;
							cout << "����Ӽ��� \"" << skill << "\"" << endl;
						}
					} 



					while (true)
					{
						String filename;
						cout << "�����ļ���: ";
						cin >> filename;
						ofstream of(filename, ios::binary);
						if (!of)
						{
							cout << "��ǰ�ļ���ռ�ã�������ļ���������" << endl;
							continue;
						}
						of.write(data.data(), data.size());
						cout << "finish!" << endl << endl;
						break;
					}
					

				}



			}
		}
		else
		{
			cout << "command format error";
		}
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}

	system("pause");
	return 0;
}

