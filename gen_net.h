#pragma once
#include<vector>
#include<iostream>
#include<queue>
#include<unordered_set>
#include<set>
#include<unordered_map>
#include<string>
#include<fstream>
using namespace std;

//списки смежности для реверсивных сетей
vector<vector<int>> rev_pat_edges;
vector<vector<int>> rev_net_edges;

//матрица для хранения весов уже выровненных вершин
vector<vector<double>> old_alg;

//максимальное количество невыровненных вершин
int max_cnt_unaligned;
//штраф за одну невыровненную вершину в результате
double fine_incomplete;
//средний допустимый штраф для выравнивания двух вершин
double avg_fine;
//штраф для растяжения
double fine_stretch;
//коэффициент для штрафа при растяжения 
double coeff_fine_stretch;
//штраф за расщепление
double fine_split;
//коэффициент для штрафа за расщепление
double coeff_fine_split;
//минимальная сумма для выравнивания
double min_weight;
//матрица замен нуклеотидов
unordered_map<char, unordered_map<char, double>> subst_matr;

//исключение для некорректного номера вершины
class Wrong_node_ex :public exception
{
public:
	int index;
	Wrong_node_ex(const char* msg, int ind) : exception(msg)
	{
		this->index = ind;
	}
};

//вершина графа
struct Node
{
	int index;
	vector<int> adjacent_nodes; //вектор индексов смежных вершин
	int characteristic; //какая-то характеристика, которая позволит, взяв две вершины и сравнив их, сказать, насколько они похожи

	void print()
	{
		cout << "index: " << index << endl << "adjacent_nodes: ";
		for (int adj : adjacent_nodes)
			cout << adj << " ";
		cout << endl << "characteristic" << characteristic << endl;
	}

};

//генная сеть
class Net
{
public:

	unordered_map<int, int> display; //Отображение, позволяющее по оригинальному индексу вершины найти соответствующий индекс в net
	vector<Node> net;
	int length;

	Net()
	{
		net.reserve(300);
		length = 0;
	}


	void AddNode(Node& const n)
	{
		if (this->display.count(n.index) != 0)
			throw Wrong_node_ex("Одна сеть не может содержать две вершины с одинаковыми индексами. Проблемная вершина: ", n.index);
		else
		{
			this->net.push_back(n);
			display[n.index] = net.size() - 1;
			length++;
		}
	}

	void print()
	{
		for (Node& x : net)
		{
			cout << "index: " << x.index << endl << "индексы смежных вершин: ";
			for (int y : x.adjacent_nodes)
				cout << y << ", ";
			cout << endl << "характеристика: " << x.characteristic << endl;
		}
		cout << endl;
	}

};

//структура для хранения промежуточных результатов выравнивания
struct Way
{
	unordered_set<int> actual_nodes; //актуальная вершина в пути — это такая выровненная вершина паттерна, из которой есть
									 //хотя бы одна дуга (или в которую ведет хотя бы одна дуга) в вершину паттерна, которая еще не выровнена ни с одной вершино сети
									 //индексы записаны в оригинальной нумерации
	vector<set<int>> conformity; //соответствие. Индекс вектора — это индекс паттерна, значение — это коллекция вершин в сети, которые выровнены с этой вершиной
								 //индекс вектора — в правильной нумерации, а элементы в set — в оригинальной
	double sum_alignment; //значение выравнивания
						  
	vector<int> count_split;//количество вершин в паттерне, с которыми выровнена указанная вершина сети

	friend bool operator<(const Way& const lhs, const Way& const rhs)
	{
		if (lhs.sum_alignment < rhs.sum_alignment)
			return 1;

		if (lhs.sum_alignment > rhs.sum_alignment)
			return 0;

		return !(lhs == rhs);
	}

	friend bool operator==(const Way& const w1, const Way& const w2)
	{
		if (w1.sum_alignment < w2.sum_alignment)
			return 0;

		if (w1.sum_alignment > w2.sum_alignment)
			return 0;

		if (w1.conformity.size() != w2.conformity.size())
			return 0;

		for (int i = 0; i < w1.conformity.size(); i++)
		{
			if (w1.conformity[i].size() != w2.conformity[i].size())
				return 0;

			for (int x : w1.conformity[i])
				if (w2.conformity[i].count(x) == 0)
					return 0;
		}

		return 1;
	}
};

template <typename T>
void try_to_read_var(T& var_name, ifstream& file_name)
{
	if (!file_name.eof())
		file_name >> var_name;
	else
	{
		throw exception("Недостаточно данных в файле с переменными");
		return;
	}
}

vector<string> split(string& input, char regex = ' ')
{
	vector<string> result;
	string buffer = "";

	for (char c : input)
	{
		if (c == regex)
		{
			result.push_back(buffer);
			buffer = "";
		}
		else
			buffer += c;
	}
	result.push_back(buffer);
	return result;
}

bool IsNumber(const string &s) {
	return all_of(s.cbegin(), s.cend(), [](const char &c) {
		return isdigit(c);
	});
}

void try_to_read_node(Node& out_node, ifstream& in_file) 
{
	if (in_file.eof())
	{
		throw exception("Недостаточно данных в файле с сетями");
		return;
	}

	Node result;

	in_file >> result.index;

	if (in_file.eof())
	{
		throw exception("Недостаточно данных в файле с сетями");
		return;
	}

	string str;

	getline(in_file, str);
	getline(in_file, str);
	vector<string> indexes = split(str);

	for (string index : indexes)
	{
		int digit = INT_MIN;

		for (int i = 0; i < index.length(); i++)
			if (int(index[i]) < 48 || int(index[i]) > 57)
			{
				throw exception("Некорректный список смежных вершин в файле с сетями");
				return;
			}
			else
			{
				if (digit == INT_MIN)
					digit = 0;
				digit = digit * 10 + (int(index[i]) - 48);
			}

		if (digit != INT_MIN)
			result.adjacent_nodes.push_back(digit);
	}

	if (in_file.eof())
	{
		throw exception("Недостаточно данных в файле с сетями");
		return;
	}

	in_file >> result.characteristic;

	out_node = result;
	
	return;
}

void try_to_read_net(Net& out_net, ifstream& in_file)
{
	if (in_file.is_open())
		while (!in_file.eof())
		{
			Node n;
			try_to_read_node(n, in_file);

			out_net.AddNode(n);
		}
	else
		cout << "не удалось открыть файл";
}	

//заполняет матрицу точечных замен
void fill_subst_matr(ifstream& in_file)
{
	auto nucleotides = { 'A', 'T', 'G', 'C' };

	for (char c1: nucleotides)
		for (char c2: nucleotides)
			try_to_read_var(subst_matr[c1][c2], in_file);
}

void try_to_read_vars(string file_name)
{
	ifstream in_file(file_name);

	if (in_file.is_open())
	{
		try
		{
			fill_subst_matr(in_file);

			try_to_read_var(max_cnt_unaligned, in_file);

			try_to_read_var(fine_incomplete, in_file);

			try_to_read_var(avg_fine, in_file);

			try_to_read_var(fine_stretch, in_file);

			try_to_read_var(coeff_fine_stretch, in_file);

			try_to_read_var(fine_split, in_file);

			try_to_read_var(coeff_fine_split, in_file);

			in_file.close();
		}
		catch (exception& ex)
		{
			cout << ex.what() << endl;
		}
	}
	else
	{
		cout << "Не удалось открыть файл с данными" << endl;
		system("pause");
		return;
	}

}

//для заданной последовательности нуклеотидов возвращает максимальный возможный вес классического выравнивания
double find_max_weight(string& s)
{
	double result = 0;
	
	for (char c : s)
		result += subst_matr[c][c];

	return result;
}

//возвращает штраф за выравнивание двух последовательностей
double alg_sequence(string& const s1, string& const s2)
{
	double max_weight;
	if (s1.length() > s2.length())
		max_weight = find_max_weight(s1);
	else
		max_weight = find_max_weight(s2);

	vector<vector<int>> matr(s1.length() + 1);

	matr[0].push_back(0);

	for (int i = 1; i < s2.length() + 1; ++i)
		matr[0].push_back(matr[0][i - 1] - 2);

	for (int i = 1; i < s1.length() + 1; ++i)
		for (int j = 0; j < s2.length() + 1; ++j)
		{
			if (j == 0)
				matr[i].push_back(matr[i - 1][0] - 2);
			else
			{
				matr[i].push_back(max(double(matr[i][j - 1] - 2), 
					max(double(matr[i - 1][j] - 2),
						matr[i - 1][j - 1] + subst_matr[s1[i - 1]][s2[j - 1]])));
			}
		}

	return -(max_weight - matr[s1.length()][s2.length()]);
}

double alg_sequence(int a, int b)
{
	return  -double(abs(a - b));
}

//оболочка для штрафов за выравнивание последовательностей на случай, если они уже были выровнены
double fine(Node& const n1, Node& const n2, unordered_map<int, int>& const in_pat_dis, unordered_map<int, int>& const in_net_dis)
{
	if (old_alg[in_pat_dis[n1.index]][in_net_dis[n2.index]] < 0)
		return old_alg[in_pat_dis[n1.index]][in_net_dis[n2.index]];

	old_alg[in_pat_dis[n1.index]][in_net_dis[n2.index]] = alg_sequence(n1.characteristic, n2.characteristic);

	return old_alg[in_pat_dis[n1.index]][in_net_dis[n2.index]];
}

double fine(int index1, int index2, string& const s1, string& const s2)
{
	if (old_alg[index1][index2] < 0)
		return old_alg[index1][index2];

	old_alg[index1][index2] = alg_sequence(s1, s2);

	return old_alg[index1][index2];
}

//вывод выравнивания сети
void print_way(const Way& w, Net& const in_pat, Net& const in_net)
{
	for (int i = 0; i < w.conformity.size(); i++)
	{
		cout << "index_pattern: " << in_pat.net[i].index << "; index_net: ";
		for (int x : w.conformity[i])
			cout << x << " ";
		cout << endl;
	}

	cout << "sum_alignment: " << w.sum_alignment << endl << endl;
}

//заполнение списков смежности с обратными дугами
void fill_reverse_edges(Net& const pat, Net& const in_net)
{
	rev_pat_edges.resize(pat.length);
	rev_net_edges.resize(in_net.length);

	for (int i = 0; i < pat.length; ++i)
		for (int j : pat.net[i].adjacent_nodes)
			rev_pat_edges[pat.display[j]].push_back(pat.net[i].index);

	for (int i = 0; i < in_net.length; ++i)
		for (int j : in_net.net[i].adjacent_nodes)
			rev_net_edges[in_net.display[j]].push_back(in_net.net[i].index);
}

//Удаляет из Way неактуальные вершины и возвращает все удаленные
unordered_set<int> correct_actual_nodes(Way& in_way, Net& const in_pat)
{
	unordered_set<int> result;

	bool f;

	for (int i : in_way.actual_nodes)
	{
		f = 1; //вершину i нужно удалить из актуальных?

		//проверяем все вершины в прямом направлении
		for (int j : in_pat.net[in_pat.display[i]].adjacent_nodes)
			if (in_way.conformity[in_pat.display[j]].empty())
			{
				f = 0;
				break;
			}

		if (!f)
			continue;

		//также проверяем в обратном направлении
		for (int j : rev_pat_edges[in_pat.display[i]])
		{
			if (in_way.conformity[in_pat.display[j]].empty())
			{
				f = 0;
				break;
			}
		}

		if (f)
		{
			result.insert(i);
			in_way.actual_nodes.erase(i);
		}
	}

	return result;

}

//добавляет в очередь новое выравнивание на основе старого
void add_way_to_queue(Net& const in_pat, 
	Net& const in_net,
	Way& in_way, 
	set<Way>& in_queue, 
	int index_next_node_in_pattern,
	int index_next_node_in_net,
	bool stretching) // если stretching == 1, то происходит растяжение 
{
	//если рассматриваемые вершины в петтерне и сети уже выровнены друг с другом, то выходим
	if (in_way.conformity[in_pat.display[index_next_node_in_pattern]].count(index_next_node_in_net) == 1)
		return;

	//суммарный штраф за все(выравнивание, растяжение, расщепление)
	double fine_stre = 0;

	if (in_way.count_split[in_net.display[index_next_node_in_net]] > 0)
		fine_stre += fine_split * in_way.count_split[in_net.display[index_next_node_in_net]];

	if (stretching)
		fine_stre = fine_stretch * coeff_fine_stretch * in_way.conformity[in_pat.display[index_next_node_in_pattern]].size();

	double current_fine = fine(in_pat.net[in_pat.display[index_next_node_in_pattern]], 
		in_net.net[in_net.display[index_next_node_in_net]],
		in_pat.display,
		in_net.display)
		+ fine_stre;

	if (in_way.sum_alignment + current_fine > min_weight)
	{
		in_way.conformity[in_pat.display[index_next_node_in_pattern]].insert(index_next_node_in_net);
		unordered_set<int> deleted;

		//если происходит растяжение, то коллекция актуальных вершин не изменяется
		if (!stretching)
		{
			in_way.actual_nodes.insert(index_next_node_in_pattern);
			deleted = correct_actual_nodes(in_way, in_pat);
		}

		in_way.count_split[in_net.display[index_next_node_in_net]]++;

		in_queue.insert({ in_way.actual_nodes,
			in_way.conformity, 
			in_way.sum_alignment + current_fine, 
			in_way.count_split });

		//Way на выходе должен быть таким же, как и на входе, поэтому выполняем обратные действия
		in_way.count_split[in_net.display[index_next_node_in_net]]--;

		in_way.conformity[in_pat.display[index_next_node_in_pattern]].erase(index_next_node_in_net);

		if (!stretching)
		{
			in_way.actual_nodes.erase(index_next_node_in_pattern);
			in_way.actual_nodes.insert(deleted.begin(), deleted.end());
		}
	}
}

void testing_alignment(Net& const pattern, Net& const in_net)
{
	//Считаем, что паттерн состоит из одной компоненты связности
	//Будем полагать, что нас удовлетворяют и те пути, которые имеют хорошее выравнивание, но которые не покрывают весь паттерн

	min_weight = pattern.length * avg_fine;

	fill_reverse_edges(pattern, in_net);

	old_alg.assign(pattern.length, vector<double>(in_net.length, 0));

	set<Way> out_ways;

	set<Way> ways;


	//добавляем первый выравнивания (перебор по одной вершине)
	vector<set<int>> alignment(pattern.length);
	vector<int> count_split(in_net.length, 0);

	for (int i = 0; i < in_net.length; i++)
	{
		for (int j = 0; j < pattern.length; j++)
		{
			alignment[j].insert(in_net.net[i].index);
			count_split[i] = 1;

			ways.insert({ { pattern.net[j].index },
				alignment,
				0 + fine(pattern.net[j],in_net.net[i], pattern.display, in_net.display),
				count_split });

			count_split[i] = 0;
			alignment[j].erase(in_net.net[i].index);
		}
	}

	while (true)
	{
		if (ways.empty())
		{
			for (Way x : out_ways)
				print_way(x, pattern, in_net);
			cout << "The end." << endl;
			return;
		}

		Way best_now_way = *--ways.end();
		ways.erase(--ways.end());

		int count;
		count = 0;
		for (set<int>& x : best_now_way.conformity)
		{
			if (x.empty())
				count++;
		}
		if (count <= max_cnt_unaligned && best_now_way.sum_alignment + fine_incomplete * count > min_weight)
		{
			best_now_way.sum_alignment += fine_incomplete * count;
			out_ways.insert(best_now_way);
			best_now_way.sum_alignment -= fine_incomplete * count;
		}

		unordered_set<int> actual_nodes(best_now_way.actual_nodes);

		for (int i : actual_nodes)
			for (int x : best_now_way.conformity[pattern.display[i]]) //для каждой вершины, выровненной с актуальной
				for (int index_next_node_in_net : in_net.net[in_net.display[x]].adjacent_nodes) // для каждой вершины в сети, смежной с какой-либо из тех, с кем веровнена актуальная
					for (int index_next_node_in_pattern : pattern.net[pattern.display[i]].adjacent_nodes) // для каждой вершины в паттерне, смежной с актуальной
					{
						add_way_to_queue(pattern, in_net, best_now_way, ways, index_next_node_in_pattern, index_next_node_in_net, 0);
						add_way_to_queue(pattern, in_net, best_now_way, ways, i, index_next_node_in_net, 1);
					}
		for (int i : actual_nodes)
			for (int x : best_now_way.conformity[pattern.display[i]]) //для каждой вершины, выровненной с актуальной
				for (int index_previous_node_in_net : rev_net_edges[in_net.display[x]]) // для каждой вершины в сети, смежной с какой-либо из тех, с кем веровнена актуальная
					for (int index_next_node_in_pattern : rev_pat_edges[pattern.display[i]]) // для каждой вершины в паттерне, смежной с актуальной
					{
						add_way_to_queue(pattern, in_net, best_now_way, ways, index_next_node_in_pattern, index_previous_node_in_net, 0);
						add_way_to_queue(pattern, in_net, best_now_way, ways, i, index_previous_node_in_net, 1);
					}
	}
}
