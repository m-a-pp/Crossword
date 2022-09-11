#include "main.h"
int answer = -1;


class Board {
private:
	char** data;
	unsigned char least_x = 0;// ����������, �������� ������ �� �������� 
	unsigned char least_y = 0;// �� �����
	unsigned char most_x = 0;// �������� 
	unsigned char most_y = 0;// ������
	unsigned short place = 0;// ������� ����������� ����
	unsigned short n_words;// ����� ����
	unsigned char board_size;// ������ ����
	bool last_placed_was_verical = 0;
	struct words {
		string word;
		short try_ = 0;
		bool placed;
		bool match_founded = 0;
	};
	struct MORE_SUITABLE {
		unsigned char match;
		string word;
		unsigned char x;
		unsigned char y;
		bool vertical;
		bool word_will_established;
		unsigned short i;
	};
	vector<string> didnt_placed;
	vector<words> list;

public:
	Board(vector<string>& p, int n) {

		char* temp;
		board_size = n;
		data = new char* [n];
		for (int i = 0; i < n; i++) {
			data[i] = new char[n];
			for (int j = 0; j < n; j++)data[i][j] = '_';
		}

		struct words tmp;
		tmp.placed = 0;

		n_words = p.size();
		for (int i = 0; i < n_words; i++)
		{
			tmp.word = p[i];
			list.push_back(tmp);
		}

	}
	~Board() {}
	//������� ����� � ����
	char getSpot(unsigned char y, unsigned char x) {
		if (y > -1 && x > -1 && y < board_size && x < board_size)return data[y][x];
		else return -51;
	}
	//���������� ����� �� ����
	void setSpot(char c, unsigned char y, unsigned char x) {
		data[y][x] = c;
	}
	char** placeList() {
		MORE_SUITABLE more_suitable;
		MORE_SUITABLE tmp;

		// ����������� ����� ������� ����� �������� (�������������)
		for (unsigned char x = 0; x < list[0].word.size(); x++) {
			if (list[0].word.size() > board_size) {
				MessageBoxA(NULL, "������� ��������� ������ ����, ��� ������� ������ ����", NULL, MB_OK);
				return NULL;
			}
			setSpot((list[0].word[x]), board_size / 2, (board_size - list[0].word.size()) / 2 + x);
		}
		least_y = board_size / 2;
		least_x = (board_size - list[0].word.size()) / 2;
		most_x = (board_size - list[0].word.size()) / 2 + list[0].word.size();
		most_y = board_size / 2;
		list[0].placed = 1;
		place++;
		// ���� �� ��������� ����, ���� ��� �� ����� ���������� ��� �� ����� �������� ����� �������
		// "list" ��� ������ ���� ������������ � ��� ����� �������� �� ���������
		while (place < n_words) {
			memset(&more_suitable, 0, sizeof(MORE_SUITABLE));
			if (answer == 2)more_suitable.match = list[0].word.size() * 2;
			// ������� ���� ��� ������ �������� �������� ��� ���������
			for (unsigned short i = 1; i < list.size(); i++) {
				if (list[i].placed == 0 && findMatch(i)) {
					tmp = PlaceWord(list[i].word);
					switch (answer) {
					case 1:
						if (tmp.match > tmp.word.size() && more_suitable.match < tmp.match) {
							tmp.word_will_established = 1;
							memcpy((void*)&more_suitable, (void*)&tmp, sizeof(MORE_SUITABLE));
							more_suitable.i = i;
						}
						list[i].try_++;
						break;
					case 2:
						if (tmp.match < tmp.word.size() * 2 && more_suitable.match > tmp.match) {
							tmp.word_will_established = 1;
							memcpy((void*)&more_suitable, (void*)&tmp, sizeof(MORE_SUITABLE));
							more_suitable.i = i;
						}
						list[i].try_++;
						break;
					default:
						list[i].try_++;
						continue;
					}

				}
			}
			//��������� �������� �������� ����� � ����������� �� ��� ����������
			if (more_suitable.word_will_established) {
				if (more_suitable.vertical == 1) {
					for (unsigned char k = 0; k < more_suitable.word.size(); k++) {
						setSpot(more_suitable.word[k], more_suitable.y + k, more_suitable.x);
					}
					last_placed_was_verical = 1;

					//���������� �������, �� ��������� ������� ��� ������ ��������� ������������� �����
					if (more_suitable.y < least_y)
						least_y = more_suitable.y;
					if (more_suitable.y + more_suitable.word.size() - 1 > most_y)
						most_y = more_suitable.y + more_suitable.word.size() - 1;
				}
				else {
					for (unsigned char k = 0; k < more_suitable.word.size(); k++) {
						setSpot(more_suitable.word[k], more_suitable.y, more_suitable.x + k);
					}
					last_placed_was_verical = 0;
					//���������� �������, �� ��������� ������� ��� ������ ��������� ������������� �����
					if (more_suitable.x < least_x)
						least_x = more_suitable.x;
					if (more_suitable.x + more_suitable.word.size() - 1 > most_x)
						most_x = more_suitable.x + more_suitable.word.size() - 1;
				}

				list[more_suitable.i].placed = 1;
				place++;
			}
		}
		cout << "���������� ���������� �����: ";
		if (didnt_placed.size() > 0) {
			for (unsigned short j = 0; j < didnt_placed.size(); j++) {
				cout << endl << didnt_placed[j] << endl;
			}
		}
		return data;
	}

	MORE_SUITABLE PlaceWord(string word) {
		MORE_SUITABLE more_suitable;
		memset(&more_suitable, 0, sizeof(MORE_SUITABLE));
		if (answer == 2)more_suitable.match = list[0].word.size() * 2;

		//�������� ����������� ���� ������������� � �����������
		//����� ������� ���������� ����� ����������������
		if (last_placed_was_verical) {
			more_suitable = PlaceHor(word, more_suitable);
			more_suitable = PlaceVert(word, more_suitable);
		}
		else {
			more_suitable = PlaceVert(word, more_suitable);
			more_suitable = PlaceHor(word, more_suitable);
		}

		return more_suitable;
	}

	MORE_SUITABLE PlaceVert(string word, MORE_SUITABLE more_suitable) {
		unsigned char match = 0;
		//������� ���� ��������� ������� ��� ��������� �����
		//������������ ��, ������� ������� ������ ����� �����
		for (unsigned char x = least_x; x <= most_x; x++) {
			for (unsigned char y = 0; y <= most_y && y < board_size + 1 - word.size(); y++) {
				for (unsigned char k = 0; k < word.size(); k++) {
					if (!CheckAdhesion(word, k, TRUE, x, y + k)) {
						match = 0;
						break;
					}
					//���� ����� ���������� ������ ������ +1
					//���� ����� ���������� ����� �� ����� +2
					//����� ����� ����������� �� ����� 
					if (getSpot(y + k, x) == '_') match++;
					else if (getSpot(y + k, x) == word[k])match += 2;
					else {
						match = 0;
						break;
					}
				}
				//����������� ��������� ��� �������� � ����������� �� ������� ������������
				if (answer == 1) {
					if (match > 0 && match > more_suitable.match) {
						more_suitable.match = match;
						more_suitable.word = word;
						more_suitable.vertical = 1;
						more_suitable.x = x;
						more_suitable.y = y;
					}

				}
				if (answer == 2) {
					if (match > word.size() && match < more_suitable.match) {
						more_suitable.match = match;
						more_suitable.word = word;
						more_suitable.vertical = 1;
						more_suitable.x = x;
						more_suitable.y = y;
					}
				}
				match = 0;

			}
		}
		return more_suitable;
	}

	MORE_SUITABLE PlaceHor(string word, MORE_SUITABLE more_suitable) {
		unsigned char match = 0;
		//������� ���� ��������� ������� ��� ��������� �����
		//������������ ��, ������� ������� ������ ����� �����
		for (unsigned char y = least_y; y <= most_y; y++) {
			for (unsigned char x = 0; x <= most_x && x < board_size + 1 - word.size(); x++) {
				for (unsigned char k = 0; k < word.size(); k++) {
					if (!CheckAdhesion(word, k, FALSE, x + k, y)) {
						match = 0;
						break;
					}
					//���� ����� ���������� ������ ������ +1
					//���� ����� ���������� ����� �� ����� +2
					//����� ����� ����������� �� ����� 
					if (getSpot(y, x + k) == '_') match++;
					else if (getSpot(y, x + k) == word[k])
					{
						match += 2;
					}
					else {
						match == 0;
						break;
					}
				}
				//����������� ��������� ��� �������� � ����������� �� ������� ������������
				if (answer == 1) {
					if (match > word.size() && match > more_suitable.match) {
						more_suitable.match = match;
						more_suitable.word = word;
						more_suitable.vertical = 0;
						more_suitable.x = x;
						more_suitable.y = y;
					}
				}
				if (answer == 2) {
					if (match > word.size() && match < more_suitable.match) {
						more_suitable.match = match;
						more_suitable.word = word;
						more_suitable.vertical = 0;
						more_suitable.x = x;
						more_suitable.y = y;
					}
				}
				match = 0;

			}
		}
		return more_suitable;
	}

	bool findMatch(unsigned short i) {
		//���� ���� ����������� ������� ����� ������� ��� ����������� �����, �� 
		//��� ������������ � ������ �������������� � ������� ������ �� �����������
		if (list[i].try_ == n_words) {
			list[i].placed = 1;
			place++;
			didnt_placed.push_back(list[i].word);
			return 0;
		}
		//���� ������� ��� ���� ������� ��������� ������� ��� �����
		//��� ������ ��������� ��� ���
		//������ ��� return ����� �� ��� �� �����
		if (list[i].match_founded)
			return 1;
		for (unsigned char k = 0; k < list[i].word.size(); k++) {
			for (unsigned char x = least_x; x <= most_x; x++) {
				for (unsigned char y = least_y; y <= most_y; y++) {
					if (list[i].word[k] == getSpot(y, x)) {
						list[i].match_founded = 1;
						return 1;
					}
				}
			}
		}
		list[i].try_++;
		return 0;
	}
	bool CheckAdhesion(string word, unsigned short i, bool vertical, unsigned char x, unsigned char y) {

		if (vertical) {
			//���������, ����� ����� �� ��������� �� ���������
			if (i == 0 && y > 0 && getSpot(y - 1, x) != '_')return 0;
			if (i == 0 && y > 0 && getSpot(y + 1, x) != '_' && getSpot(y + 1, x) != word[1])return 0;
			if (i == (word.size() - 1) && y < (board_size - 1) && getSpot(y + 1, x) != '_')return 0;
			if (i == (word.size() - 1) && y < (board_size - 1) && getSpot(y - 1, x) != '_' && getSpot(y - 1, x) != word[i - 1])return 0;
			if (getSpot(y, x) == word[i])return 1;//���� ��� ������� �����, �� ������/����� ������ ����� �����
			//���� �� ��� �� ���������, �� ���������, ����� ������/����� �� ���� �����
			if (x > 0 && getSpot(y, x - 1) != '_')return 0;
			if (x < (board_size - 1) && getSpot(y, x + 1) != '_')return 0;
		}
		else {
			//���������, ����� ����� �� ��������� �� �����������
			if (i == 0 && x > 0 && getSpot(y, x - 1) != '_')return 0;
			if (i == 0 && x > 0 && getSpot(y, x + 1) != '_' && getSpot(y, x + 1) != word[1])return 0;
			if (i == (word.size() - 1) && x < (board_size - 1) && getSpot(y, x + 1) != '_')return 0;
			if (i == (word.size() - 1) && x < (board_size - 1) && getSpot(y, x - 1) != '_' && getSpot(y, x - 1) != word[i - 1])return 0;
			if (getSpot(y, x) == word[i])return 1;//���� ��� ������� �����, �� ������/����� ������ ����� �����
			//���� �� ��� �� ���������, �� ���������, ����� ������/����� �� ���� �����
			if (y > 0 && getSpot(y - 1, x) != '_')return 0;
			if (y < (board_size - 1) && getSpot(y + 1, x) != '_')return 0;
		}
		return 1;
	}
	void printBoard() {
		char** pi = data;
		char* pj;
		for (pi; pi < data + board_size; pi++) {
			pj = *pi;
			for (pj; pj < (*pi) + board_size; pj++) {
				cout << *pj;
				cout << " ";
			}
			cout << endl;
		}
		return;
	}
};

void ToUpper(vector<string>& lst) {
	for (unsigned short i = 0; i < lst.size(); i++) {
		for (unsigned short j = 0; j < lst[i].size(); j++) {
			lst[i][j] = toupper(lst[i][j]);
			if (lst[i][j] == '�')lst[i][j] = '�';
		}
	}
}

void findMax(vector<string>& list) {
	string temp;
	unsigned short max_i = 0;
	unsigned char max = 0;
	for (int i = 0; i < list.size() - 1; i++) {
		if (max < list[i].size()) {
			max_i = i;
			max = list[i].size();
		}
	}
	list[max_i].swap(list[0]);
}

bool checkForLetters(string word) {
	// �����������, ��� ����� �����, �������� �� ���
	// ������������ �������� ascii ��� �������� � ������� ��������

	for (int i = 0; i < word.size(); i++) {
		if (word[i] < -64 || word[i] > -1 && word[i] < 65 || word[i]>122 || word[i] > 90 && word[i] < 97) {
			return false;
		}

	}

	return true;
}


char** generate(int size, char* c_path, int ans) {
	//clock_t t;
	//t = clock();
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	answer = ans;
	vector<string> lst;
	string temp;

	ifstream ifs;
	string path = c_path;
	ifs.open(path);


	while (!ifs) {
		MessageBoxA(NULL, "�������� ���� � �����", NULL, MB_OK);
		return NULL;
	}


	while (!ifs.eof()) {
		getline(ifs, temp, '\n');
		if (checkForLetters(temp)) {
			lst.push_back(temp);
		}
		else {
			MessageBoxA(NULL, "� ����� ������������ ������������ �����", NULL, MB_OK);
			return NULL;
		}
	}
	lst.push_back(".");

	findMax(lst);
	// ��������� ��� ����� � �������� �������
	ToUpper(lst);

	// ������� ������ Board
	Board board(lst, size);

	// ���������� "lst" �� �����
	char** result = board.placeList();
	//clock_t time_spent = (clock() - t);
	//printf("\n�����: %d\n", time_spent);
	//board.printBoard();

	return result;
}
