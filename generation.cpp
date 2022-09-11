#include "main.h"
int answer = -1;


class Board {
public:
	Board(vector<string>& p, int n) {
		char* temp;
		board_size = n;
		data = new char* [n];
		for (int i = 0; i < n; i++) {
			data[i] = new char[n];
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				data[i][j] = '_';
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
	//считать букву с поля
	char getSpot(int y, int x) {
		if (y > -1 && x > -1 && y < board_size && x < board_size)return data[y][x];
		else return -51;
	}
	//установить букву на поле
	void setSpot(char c, int y, int x) {
		data[y][x] = c;
	}
	char** placeList() {
		MORE_SUITABLE more_suitable;
		MORE_SUITABLE tmp;
		// цикл по установке слов, пока все не будут устновлены или не будет исчерпан лимит попыток
		// "list" уже должен быть отсортирован и все буквы заменены на заглавные
		while (place < n_words) {
			memset(&more_suitable, 0, sizeof(MORE_SUITABLE));
			if (answer == 2)more_suitable.match = list[0].word.size() * 2;
			for (int i = 0; i < list.size(); i++) {
				if (i == 0 && list[0].placed == 0) {
					for (int x = 0; x < list[0].word.size(); x++) {
						// расположить самое длинное слово поцентру (горизонтально)
						if (list[0].word.size() > board_size) {
							MessageBoxA(NULL, "Слишком маленький размер поля, для данного набора слов", NULL, MB_OK);
							return NULL;
						}
						setSpot((list[0].word[x]), board_size / 2, (board_size - list[0].word.size()) / 2 + x);
					}
					list[0].placed = 1;
					place++;

				} 
				//установка наиболее удачного слова в зависимости от его ориентации
				if (list[i].placed == 0 && findMatch(list[i].word, i)) {
					tmp = PlaceWord(list[i].word);
					if (tmp.word_will_established == 0)
					{
						list[i].try_++;
						continue;
					}
					else {
						if (answer == 1) {
							if (more_suitable.match < tmp.match) {
								memcpy((void*)&more_suitable, (void*)&tmp, sizeof(MORE_SUITABLE));
								more_suitable.i = i;
							}
						}
						else {
							if (more_suitable.match > tmp.match) {
								memcpy((void*)&more_suitable, (void*)&tmp, sizeof(MORE_SUITABLE));
								more_suitable.i = i;
							}
						}
					}

				}
			}
			if (more_suitable.word_will_established) {
				if (more_suitable.vertical == 1) {
					for (int k = 0; k < more_suitable.word.size(); k++) {
						setSpot(more_suitable.word[k], more_suitable.y + k, more_suitable.x);
					}
					last_placed_was_verical = 1;
				}
				else {
					for (int k = 0; k < more_suitable.word.size(); k++) {
						setSpot(more_suitable.word[k], more_suitable.y, more_suitable.x + k);
					}
					last_placed_was_verical = 0;
				}
				// записать координаты установленного слова
				list[more_suitable.i].placed = 1;
				place++;
			}
		}
		cout << "Could not place word: ";
		if (didnt_placed.size() > 0) {
			for (int j = 0; j < didnt_placed.size(); j++) {
				cout << endl << didnt_placed[j] << endl;
			}
		}
		return data;
	} 
private:
	struct MORE_SUITABLE {
		int match;
		string word;
		int x;
		int y;
		bool vertical;
		bool word_will_established;
		int i;
	};

public:

	MORE_SUITABLE PlaceWord(string word) {
		MORE_SUITABLE more_suitable;
		memset(&more_suitable, 0, sizeof(MORE_SUITABLE));
		if (answer == 2)more_suitable.match = list[0].word.size() * 2;

		//чередуем выставление слов горизонтально и вертикально
		//тогда решение получается более сбалансированным
		if (last_placed_was_verical) {
			more_suitable = PlaceHor(word, more_suitable);
			more_suitable = PlaceVert(word, more_suitable);
		}
		else {
			more_suitable = PlaceVert(word, more_suitable);
			more_suitable = PlaceHor(word, more_suitable);
		}

		if (answer == 1) {
			if (more_suitable.match > word.size())more_suitable.word_will_established = 1;
		}
		if (answer == 2) {
			if (more_suitable.match < word.size() * 2)more_suitable.word_will_established = 1;
		}
		return more_suitable;
	}

	MORE_SUITABLE PlaceVert(string word, MORE_SUITABLE more_suitable) {
		int match = 0;
		//перебор всех возможных позиций для установки слова
		//записывается та, которая набрала больше всего очков
		for (int x = 0; x < board_size; x++) {
			for (int y = 0; y < board_size + 1 - word.size(); y++) {
				for (int k = 0; k < word.size(); k++) {
					if (!CheckAdhesion(word, k, TRUE, x, y + k)) {
						match = 0;
						break;
					}
					//если буква пересекает пустую клетку +1
					//если буква пересекает такую же букву +2
					//иначе такое пересечение не нужно 
					if (getSpot(y + k, x) == '_') match++;
					else if (getSpot(y + k, x) == word[k])match += 2;
					else match = 0;
				} 
				//определение максимуму или минимума в зависимости от запроса пользователя
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
		int match = 0;
		//перебор всех возможных позиций для установки слова
		//записывается та, которая набрала больше всего очков
		for (int y = 0; y < board_size; y++) {
			for (int x = 0; x < board_size + 1 - word.size(); x++) {
				for (int k = 0; k < word.size(); k++) {
					if (!CheckAdhesion(word, k, FALSE, x + k, y)) {
						match = 0;
						break;
					}
					//если буква пересекает пустую клетку +1
					//если буква пересекает такую же букву +2
					//иначе такое пересечение не нужно 
					if (getSpot(y, x + k) == '_') match++;
					else if (getSpot(y, x + k) == word[k])
					{
						match += 2;
					}
					else match = 0;
				} 
				//определение максимуму или минимума в зависимости от запроса пользователя
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
	int findMatch(string word, int i) {
		//если было предпринято слишком много попыток для выставления слова, то 
		//оно перемещается в список невыставленных и попытки больше не совершаются
		if (list[i].try_ == n_words) {
			list[i].placed = 1;
			place++;
			didnt_placed.push_back(word);
			return 0;
		}
		for (int y = 0; y < board_size; y++) {
			for (int x = 0; x < board_size; x++) {
				for (int k = 0; k < word.size(); k++) {
					if (word[k] == getSpot(y, x)) return 1;
				}
			}
		}
		list[i].try_++;
		return 0;
	}
	int CheckAdhesion(string word, int i, bool vertical, int x, int y) {

		if (vertical) {
			//проверяем, чтобы слово не слипалось по вертикали
			if (i == 0 && y > 0 && getSpot(y - 1, x) != '_')return 0;
			if (i == 0 && y > 0 && getSpot(y + 1, x) != '_' && getSpot(y + 1, x) != word[1])return 0;
			if (i == (word.size() - 1) && y < (board_size - 1) && getSpot(y + 1, x) != '_')return 0;
			if (i == (word.size() - 1) && y < (board_size - 1) && getSpot(y - 1, x) != '_' && getSpot(y - 1, x) != word[i - 1])return 0;
			if (getSpot(y, x) == word[i])return 1;//если это условие верно, то справа/слева всегда будет буква
			//если же оно не выполнено, то проверяем, чтобы справа/слева не было буквы
			if (x > 0 && getSpot(y, x - 1) != '_')return 0;
			if (x < (board_size - 1) && getSpot(y, x + 1) != '_')return 0;
		}
		else {
			//проверяем, чтобы слово не слипалось по горизонтали
			if (i == 0 && x > 0 && getSpot(y, x - 1) != '_')return 0;
			if (i == 0 && x > 0 && getSpot(y, x + 1) != '_' && getSpot(y, x + 1) != word[1])return 0;
			if (i == (word.size() - 1) && x < (board_size - 1) && getSpot(y, x + 1) != '_')return 0;
			if (i == (word.size() - 1) && x < (board_size - 1) && getSpot(y, x - 1) != '_' && getSpot(y, x - 1) != word[i - 1])return 0;
			if (getSpot(y, x) == word[i])return 1;//если это условие верно, то сверху/снизу всегда будет буква
			//если же оно не выполнено, то проверяем, чтобы сверху/снизу не было буквы
			if (y > 0 && getSpot(y - 1, x) != '_')return 0;
			if (y < (board_size - 1) && getSpot(y + 1, x) != '_')return 0;
		}
		return 1;
	}
	void printBoard() {
		for (int y = 0; y < board_size; y++) {
			for (int x = 0; x < board_size; x++) {
				cout << data[y][x];
				// добавить пробелы между символами для визуального удобства
				cout << " ";
			}
			cout << endl;
		}
		return;
	}

private:
	char** data;
	int place;
	int n_words;
	int board_size;
	struct words {
		string word;
		int try_ = 0;
		bool placed;
		int x;
		int y;
	};
	vector<string> didnt_placed;
	vector<words> list;
	bool last_placed_was_verical = 0;
};

void ToUpper(vector<string>& lst) {
	for (int i = 0; i < lst.size(); i++) {
		for (int j = 0; j < lst[i].size(); j++) {
			lst[i][j] = toupper(lst[i][j]);
			if (lst[i][j] == 'я')lst[i][j] = 'Я';
		}
	}
}

// сортировка слов по длине по убыванию
void sortHighLow(vector<string>& list) {
	string temp;
	for (int i = 0; i < list.size() - 1; i++) {
		for (int j = 0; j < list.size() - i - 1; j++) {
			if (list[j].size() < list[j + 1].size()) {
				temp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = temp;
			}
		}
	}
}

bool checkForLetters(string word) {
	// проверяются, все буквы слова, подходят ли они
	for (int i = 0; i < word.size(); i++) {
		if (!(word[i] >= -64 && word[i] <= -1 || word[i] >= 65 && word[i] <= 90 || word[i] >= 97 && word[i] <= 122)) {
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
		MessageBoxA(NULL, "Неверный путь к файлу", NULL, MB_OK);
		return NULL;
	}

	while (!ifs.eof()) {
		getline(ifs, temp, '\n');
		if (checkForLetters(temp)) {
			lst.push_back(temp);
		}
		else {
			MessageBoxA(NULL, "В файле присутствует неподходящее слово", NULL, MB_OK);
			return NULL;
		}
	}
	lst.push_back(".");
	// отсортировать "lst" и сделать все буквы заглывными
	sortHighLow(lst);

	// перевести все буквы в верхниий регистр
	ToUpper(lst);

	// создать объект Board
	Board board(lst, size);

	// разместить "lst" на доске
	char** result = board.placeList();

	//clock_t time_spent = (clock() - t);
	//printf("\nВремя: %d\n", time_spent);

	//board.printBoard();
	return result;
} 