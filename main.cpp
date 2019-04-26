#include "gen_net.h"
#include <ctime>

int main()
{
	setlocale(LC_ALL, "rus");

	/*Node n10 = { 0,{ 1 }, 10 }; //(0)�������� ������� � ����
	Node n11 = { 1,{ 2 }, 20 };
	Node n12 = { 2,{ 3 }, 30 };
	Node n13 = { 3,{}, 40 };

	Node n20 = { 0,{ 1 }, 10 };
	Node n21 = { 1,{ 2 }, 20 };
	Node n22 = { 2,{ 3 }, 30 };
	Node n23 = { 3,{ 4 }, 40 };
	Node n24 = { 4,{ 5 }, 50 };
	Node n25 = { 5,{}, 60 };*/
	
	/*Node n10 = { 0,{ 1 }, 10 }; //(1)�������� ������� 
	Node n11 = { 1,{ 2 }, 20 };
	Node n12 = { 2,{ 3 }, 30 };
	Node n13 = { 3,{}, 40 };

	Node n20 = { 0,{ 1,2,3 }, 30 };
	Node n21 = { 1,{ 4 }, 20 };
	Node n22 = { 2,{ 4 }, 30 };
	Node n23 = { 3,{ 1,2 }, 0 };
	Node n24 = { 4,{ 5 }, 45 };
	Node n25 = { 5,{}, 59 };*/

	/*Node n10 = { 0,{ 1, 2 }, 0 }; //(2)
	Node n11 = { 1,{ 3 }, 10 };
	Node n12 = { 2,{ }, 20 };
	Node n13 = { 3,{}, 50 };

	Node n20 = { 0,{ 1,2,3 }, 20 };
	Node n21 = { 1,{ 4 }, 40 };
	Node n22 = { 2,{ }, 15 };
	Node n23 = { 3,{ 2,4 }, 10 };
	Node n24 = { 4,{ 1}, 15 };*/

	/*Node n10 = { 0,{ 1 }, 0 }; //(3) ������� -- �� ����� �.�., � ���� -- �� ����
	Node n11 = { 1,{ 2,3,4 }, 20 };
	Node n12 = { 2,{ }, 40 };
	Node n13 = { 3,{ }, 60 };
	Node n14 = { 4,{ }, 80 };

	Node n20 = { 0,{ 1 }, 0 };
	Node n21 = { 1,{ 2, 3 }, 15 };
	Node n22 = { 2,{ }, 30 };
	Node n23 = { 3,{}, 40 };

	Node n24 = { 4,{ 5 }, 5 };
	Node n25 = { 5,{ 6,7,8 }, 20 };
	Node n26 = { 6,{ }, 40 };
	Node n27 = { 7,{ }, 50 };
	Node n28 = { 8,{}, 90 };*/

	/*Node n10 = { 0,{ 1}, 0 }; //(4) � � ��������, � � ���� ���� �����
	Node n11 = { 1,{ 2 }, 30 };
	Node n12 = { 2,{0}, 40 };

	Node n20 = { 0,{ 1 }, 30 };
	Node n21 = { 1,{ 4, 2 }, 10 };
	Node n22 = { 2,{3}, 20 };
	Node n23 = { 3,{ }, 30 };
	Node n24 = { 4,{ 0,3 }, 40 };*/

	/*Node n10 = { 0,{ 2 }, 50 }; //(5) � �������� ���� ������� �������
	Node n11 = { 1,{ 2 }, 20 };
	Node n12 = { 2,{ 4 }, 40 };
	Node n13 = { 3,{ 4 }, 10 };
	Node n14 = { 4,{ }, 100 };

	Node n20 = { 0,{ 1,2 }, 40 };
	Node n21 = { 1,{ 4 }, 20 };
	Node n22 = { 2,{5}, 40 };
	Node n23 = { 3,{ 4 }, 50 };
	Node n24 = { 4,{ 6 }, 30 };
	Node n25 = { 5,{ 6 }, 15 };
	Node n26 = { 6,{ }, 90 };*/

	Net pattern, net;
	int mode1;

	try
	{
		try
		{
			cout << "�������� ����� ������" << endl << "0, ���� ������ ���������� �� ������������ ��������� �����" << endl;

			cout << "1, ���� ������ ������ ���� ����" << endl;

			cin >> mode1;

			while (mode1 < 0 || mode1 > 1)
			{
				cout << "����������, ������� ���������� �����: ";
				cin >> mode1;
				cout << endl;
			}

			if (mode1 == 0)
			{
				int mode2;
				cout << "�������� ����� ���� [0,5]: ";
				cin >> mode2;
				while (mode2 < 0 || mode2 > 5)
				{
					cout << "���������� ������� ���������� ����� ����: ";
					cin >> mode2;
					cout << endl;
				}

				ifstream file("Default_Patterns/defaultPattern" + to_string(mode2) + ".txt");
				try_to_read_net(pattern, file);

				file.close();

				file.open("Default_Nets/defaultNet" + to_string(mode2) + ".txt");
				try_to_read_net(net, file);

				file.close();
			}
			else
			{
				string adress;
				cout << endl << "������� ���� �� ����� � ��������� ��� ��������: ";
				cin >> adress;

				ifstream file(adress);
				try_to_read_net(pattern, file);

				file.close();

				cout << endl << "������� ���� �� ����� � ����� ��� ��������: ";
				cin >> adress;

				file.open(adress);
				try_to_read_net(net, file);

				file.close();
			}
		}
		catch (Wrong_node_ex& ex)
		{
			cout << ex.what() << ex.index << endl;
			system("pause");
			return 0;
		}
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
		system("pause");
		return 0;
	}

	try
	{
		cout << "�� ������ ��������������� ���������� ���������� ���������� (������� �����, ������ � ������������) ��� ��������� ����?";
		cout << endl << "0: ������� ���������" << endl << "1: ��������� ����������������" << endl;

		cin >> mode1;

		while (mode1 < 0 || mode1 > 1)
		{
			cout << "����������, ������� ���������� �����: ";
			cin >> mode1;
			cout << endl;
		}


		if (mode1 == 0)
			try_to_read_vars("Default_Vars/defaultVars.txt");
		else
		{
			string adress;
			cout << endl << "������� ���� �� ����� � ����������� ��� ��������: ";
			cin >> adress;

			try_to_read_vars(adress);
		}

		cout << endl << "�������:" << endl;
		pattern.print();

		cout << endl << "����: " << endl;
		net.print();

	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
		system("pause");
		return 0;
	}

	int start = clock();

	testing_alignment(pattern, net);

	int end_time = clock();

	cout << endl << "time: " << end_time - start << endl;

	system("pause");
	return 0;
}
