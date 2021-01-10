#include "include/Vigener.h"

int threshold;
int keyLength;
int maxKeyLength;
std::string alphabet;


void checkFile(std::string fileName)
{
	std::ifstream temp(fileName);

	if (!temp.is_open())
	{
		std::cout << "Невозможно открыть файл '" << fileName << "'\n";
		system("pause");
		exit(0);
	}
	else
	{
		std::string tmp;
		while (temp)
		{
			getline(temp, tmp);
			if (!tmp.empty())
				return;
		}
		temp.close();

		std::cout << "Файл '" << fileName << "' пуст!\n";
		system("pause");
		exit(0);

	}
}

void readFile(std::string& text, std::string fileName)
{
	checkFile(fileName);
	std::ifstream in(fileName);

	std::string  str;
	while (!in.eof())
	{
		str.clear();
		getline(in, str);
		text.append(str);
	}
	in.close();
}

void deleteForbiddenSymbols(std::string& text)
{
	transform(text.begin(), text.end(), text.begin(), tolower);
	text.erase(remove_if(text.begin(), text.end(), isAlpha(alphabet)), text.end());
}


void decrypt(std::string key, std::vector<std::string>& columns)
{
	for (auto i = 0; i < key.size(); i++)
	{
		int keyPosition = alphabet.find_first_of(key[i]);
		for (auto j = 0; j < columns[i].size(); j++)
		{
			int pos = alphabet.find_first_of(columns[i][j]);
			int num = pos - keyPosition;

			if (num < 0)
				num += (int)alphabet.size();

			columns[i][j] = alphabet[num];
		}
	}
}

void save(std::string key, std::vector<std::string>& columns, std::string decryptedTextFileName)
{
	std::ofstream out(decryptedTextFileName, std::ios::app);

	out << "Ключ: " << key << "\n\n";

	out << "Открытый текст:\n";

	decrypt(key, columns);

	int rawMaxSize = 130 / (int)key.length();

	unsigned int k = 0;
	int tmp = 0;
	for (auto j = 0; j < columns.size(); j++)
	{
		if (k > columns[j].size())
			break;

		out << columns[j][k];

		tmp++;

		if (j != 0 && j % (key.size() - 1) == 0)
			out << " ";

		if (tmp == rawMaxSize * key.length())
		{
			out << "\n";
			tmp = 0;
		}

		if (j != 0 && j % (key.size() - 1) == 0)
		{
			k++;
			j = -1;
		}
	}

	out << "\n";

	for (auto i = 0; i < 100; i++)
		out << "-";
	out << "\n";

	out.close();
}

void displayDecryptedText(std::string key, std::vector<std::string> columns)
{
	decrypt(key, columns);

	int rawMaxSize = 130 / (int)key.length();

	unsigned int k = 0;
	int tmp = 0;
	for (auto j = 0; j < columns.size(); j++)
	{
		if (k > columns[j].size())
			break;

		std::cout << columns[j][k];

		tmp++;

		if (j != 0 && j % (key.size() - 1) == 0)
			std::cout << " ";

		if (tmp == rawMaxSize * key.length())
		{
			std::cout << "\n";
			tmp = 0;
		}

		if (j != 0 && j % (key.size() - 1) == 0)
		{
			k++;
			j = -1;
		}
	}

	std::cout << "\n";
}

void generaateKey(std::vector<Frequency>& columnsFrequency, std::string& key, int groupNumber, int shift, int cipherAlphabetSize)
{
	int j = 0;
	int shiftFor = shift;

	if (key[groupNumber] == 'а')
	{
		if (shift < 0)
			shift += cipherAlphabetSize;
		key[groupNumber] = alphabet[shift];
	}
	else
	{
		int superiority;
		while (key[groupNumber] != alphabet[++j]);
		shift += j;
		superiority = (int)shift / cipherAlphabetSize;
		if (abs(superiority) >= 1)
			shift -= superiority * cipherAlphabetSize;
		key[groupNumber] = alphabet[abs(shift)];
	}

	if (shiftFor > 0)
		for (auto i = 0; i < shiftFor; i++)
		{
			auto it = (columnsFrequency[groupNumber]).info.begin();
			(columnsFrequency[groupNumber]).info.push_back(*it);
			columnsFrequency[groupNumber].info.erase((columnsFrequency[groupNumber]).info.begin());
		}
	else
		for (auto i = 0; i < abs(shiftFor); i++)
		{
			auto it = (columnsFrequency[groupNumber]).info.end() - 1;
			columnsFrequency[groupNumber].info.insert((columnsFrequency[groupNumber]).info.begin(), *it);
			columnsFrequency[groupNumber].info.erase((columnsFrequency[groupNumber]).info.end() - 1);
		}

}

void displayWorkSpace(Frequency analyticalTextFrequency, std::vector<Frequency> columnsFrequency, std::string key, int groupNumber)
{
	HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");

	std::cout << "           ";
	SetConsoleTextAttribute(hOUTPUT, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	for (auto i = 0; i < analyticalTextFrequency.info.size(); i++)
		std::cout << std::setw(4) << i + 1;

	SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << "\n           ";
	for (auto it : analyticalTextFrequency.info)
		std::cout << std::setw(4) << it.word;

	std::cout << "\n           ";
	for (auto it : analyticalTextFrequency.info)
		if (it.count >= threshold)
		{
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << std::setw(4) << it.count;
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else
			std::cout << std::setw(4) << it.count;

	int count = 0;
	std::cout << "\n";
	for (auto it : columnsFrequency)
	{
		count++;
		if (count - 1 == groupNumber)
		{
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			for (auto i = 0; i < 143; i++)
				std::cout << "-";
			std::cout << "\n";
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

		if (count >= 10)
			std::cout << count << "-я группа";
		else
			std::cout << count << "-я группа ";

		for (auto i : it.info)
			if (i.count >= threshold)
			{
				SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_INTENSITY);
				std::cout << std::setw(4) << i.count;
				SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else
				std::cout << std::setw(4) << i.count;

		std::cout << "\n";
		if (count - 1 == groupNumber)
		{
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			for (auto i = 0; i < 143; i++)
				std::cout << "-";
			std::cout << "\n";
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	}

	std::cout << "\nПредполагаемый ключ: " << key << "\n";
}

void determineKey(Frequency analyticalTextFrequency, std::vector<Frequency> columnsFrequency, int cipherAlphabetSize, std::vector<std::string> columns, std::string decryptedTextFileName)
{
	int maxGroup = 0;
	int groupNumber = 0;
	int shift;
	std::string key;

	for (auto i = 0; i < keyLength; i++)
		key += "а";

	while (true)
	{
		displayWorkSpace(analyticalTextFrequency, columnsFrequency, key, groupNumber);
		std::cout << "Группа: ";
		std::cin >> groupNumber;

		if (groupNumber == 0)
			break;

		if (groupNumber == -1)
		{
			displayDecryptedText(key, columns);
			system("pause");
			continue;
		}

		if (groupNumber < -2 || groupNumber > keyLength)
		{
			std::cout << "Неверные данные.\n";
			system("pause");
			continue;
		}

		if (groupNumber > maxGroup)
			maxGroup = groupNumber;

		if (groupNumber > 0)
			groupNumber -= 1;

		while (true)
		{
			std::cout << "Сдвиг: ";
			std::cin >> shift;

			if (shift == 0)
				break;

			if (groupNumber == -2 && abs(shift - 1) < keyLength)  //отмена всех действий для группы с номером shift -1
			{
				std::sort((columnsFrequency[shift - 1.0]).info.begin(), (columnsFrequency[shift - 1.0]).info.end(), Compare(alphabet));
				key[shift - 1.0] = 'а';

				displayWorkSpace(analyticalTextFrequency, columnsFrequency, key, groupNumber);
				std::cout << "Группа: " << groupNumber << "\n";
				continue;
			}

			if (abs(shift) > cipherAlphabetSize || abs(groupNumber - 1) > keyLength)
			{
				std::cout << "Неверные данные.\n";
				system("pause");
				continue;
			}

			shift *= -1;
			generaateKey(columnsFrequency, key, groupNumber, shift, cipherAlphabetSize);

			displayWorkSpace(analyticalTextFrequency, columnsFrequency, key, groupNumber);
			std::cout << "Группа: " << groupNumber + 1 << "\n";
		}
	}

	displayDecryptedText(key, columns);
	save(key, columns, decryptedTextFileName);
}

void findAlphabetSize(int& cipherAlphabetSize, std::string text)
{
	std::set<char> cipherAlphabet;

	for (auto it : text)
		cipherAlphabet.insert(it);

	cipherAlphabetSize = cipherAlphabet.size();
}

void calculateFrequency(std::string text, Frequency& freq)
{
	for (auto it : alphabet)
		freq.info.push_back(Symbol(it, 0));

	int n = text.length();
	for (auto i = 0; i < n; i++)
	{
		bool yes = true;

		for (auto j = freq.info.begin(); j != freq.info.end(); j++)
		{
			if ((*j).word == text[i])
			{
				Symbol temp((*j).word, (*j).count + 1);
				freq.info.erase(j);
				freq.info.push_back(temp);
				yes = false;
				break;
			}
		}
		if (yes)
			freq.info.push_back(Symbol(text[i], 1));
	}

	std::sort(freq.info.begin(), freq.info.end(), Compare(alphabet));
}

void createColumns(std::string encryptedText, std::vector<std::string>& columns)  // не использет последние символы (поледнюю строку т.к. она может быть не полной)
{
	std::string temp;
	int size = (int)encryptedText.size() / keyLength;

	for (auto j = 0; j < keyLength; j++)
	{
		temp.clear();
		for (auto i = 0; i < size; i++)
			temp += encryptedText[j + i * keyLength];

		columns.push_back(temp);
	}
}

void findKeyValue(std::string encryptedText, std::string analiticalTextFileName, std::string decryptedTextFileName)
{
	std::string analiticalText;
	std::vector<std::string> columns;
	std::vector<Frequency>  columnsFrequency;
	Frequency analyticalTextFrequency;

	createColumns(encryptedText, columns);  //формируем массив строк из шифртекста по столбцам


	readFile(analiticalText, analiticalTextFileName);  //считываем аналилитечкий текст
	deleteForbiddenSymbols(analiticalText);

	if (analiticalText.size() < columns[1].size()) {
		std::cout << "Аналитический текст получился меньше зашифрованного текста!\nВозможно несоответствие алфавитов!\n";
		system("pause");
		exit(0);
	}

	analiticalText.erase(columns[1].size(), analiticalText.size());  // удаляем лишний аналитический текст

	calculateFrequency(analiticalText, analyticalTextFrequency);  // частотный анализ аналитического текста

	Frequency buffEncryptedTextFrequency;
	for (auto it : columns)  //в цикле формируем частотный анализ шифроткста по группам (по столбикам)
	{
		buffEncryptedTextFrequency.info.clear();
		calculateFrequency(it, buffEncryptedTextFrequency);
		columnsFrequency.push_back(buffEncryptedTextFrequency);
	}

	int cipherAlphabetSize;
	findAlphabetSize(cipherAlphabetSize, encryptedText);

	determineKey(analyticalTextFrequency, columnsFrequency, cipherAlphabetSize, columns, decryptedTextFileName);

}

void findKeyLength(std::string encryptedText)
{
	int count;
	std::vector<double> coincidenceCount;
	std::string buffText(encryptedText);

	for (auto i = 0; i < maxKeyLength; i++)
	{
		count = 0;
		buffText.push_back(buffText.front());
		buffText.erase(0, 1);

		for (auto it = 0; it < encryptedText.size(); it++)
			if (encryptedText[it] == buffText[it])
				count++;

		coincidenceCount.push_back((double)count / encryptedText.size());
	}

	std::cout << "Вероятные длины ключей: \n";
	for (auto it = 0; it < maxKeyLength; it++)
		std::cout << std::setw(3) << it + 1 << " - " << coincidenceCount[it] << "\n";

	std::cout << "Введите длину ключа: \n";
	std::cin >> keyLength;
}


bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

std::string getCmdOption(int argc, char* argv[], const std::string& option)
{
	std::string cmd;

	for (short i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];

		if (0 == arg.find(option))
			if (i + 1 < argc)
				return argv[i + 1];
	}

	return cmd;
}

void parse_arguments(int argc, char* argv[], std::string& analiticalTextFileName, std::string& encryptedTextFileName, std::string& decryptedTextFileName)
{
	if (cmdOptionExists(argv, argv + argc, "-h"))
	{
		std::cout << "Использование: Vigener -i [путь к файлу с шифртекстом] -ia [путь к файлу с аналитическим текстом]\n\n";
		std::cout << "Параметры:\n\n";
		std::cout << "-i Задает путь к файлу с шифртекстом\n";
		std::cout << "-ia Задает путь к файлу с аналитическим текстом\n";
		std::cout << "-o Задает путь к файлу для сохранения результата работы программы. По умолчанию - путь файла шифртекста с меткой {processed}\n";
		std::cout << "-a Задает алфавит шифртекста. По умолчанию - кириллица\n";
		std::cout << "-af Задает путь к файлу с алфавитом шифртекста\n";
		std::cout << "-t Задает порог для выделения статистики. По умолчанию 6\n";
		std::cout << "-k Задает длину ключа\n";
		std::cout << "-mk Задает максимальную длину ключа\n\n";

		std::cout << "При вводе номера группы также можно указать следуюшие значения:\n\n";
		std::cout << "0: выход из программы с сохранением результата работы. Ctrl + C - выход без сохранения результата\n";
		std::cout << "-1: вывести декодированный шифртекст при текущем ключе\n";
		std::cout << "-2: перейти в режим отмены сдвигов в группах. При вводе сдвига нужно указать номер группу,\nдля которой необходимо отменить все действия.\nДля выхода из режима необходимо ввести значение 0 в поле номера группы\n\n";
		std::cout << "При вводе сдвига группы указывается положительное или отрицательное значение для\nциклического сдвига статистики вправо или влево соответсвенно.\nДля выхода из текушей группы необходимо ввести значение 0 в поле сдвига\n\n";

		exit(0);
	}

	// Инициализация encryptedTextFileName
	if (cmdOptionExists(argv, argv + argc, "-i"))
		encryptedTextFileName = getCmdOption(argc, argv, "-i");
	else
	{
		std::cout << "Необходимо указать путь к файлу с шифртекстом.\nИспользуйте параметр -i.\n\n";
		exit(0);
	}

	// Инициализация analiticalTextFileName
	if (cmdOptionExists(argv, argv + argc, "-ia"))
		analiticalTextFileName = getCmdOption(argc, argv, "-ia");
	else
	{
		std::cout << "Необходимо указать путь к файлу с аналитическим текстом.\nИспользуйте параметр -ia.\n\n";
		exit(0);
	}

	// Инициализация decryptedTextFileName
	if (cmdOptionExists(argv, argv + argc, "-o"))
		decryptedTextFileName = getCmdOption(argc, argv, "-o");
	else
	{
		decryptedTextFileName = encryptedTextFileName;

		size_t pos = decryptedTextFileName.find(".");
		if (pos != std::string::npos)
			decryptedTextFileName.insert(pos, " {processed}");
		else
			decryptedTextFileName += " {processed}";
	}

	// Инициализация alphabet
	if (cmdOptionExists(argv, argv + argc, "-a"))
		alphabet = getCmdOption(argc, argv, "-a");
	else if (cmdOptionExists(argv, argv + argc, "-af"))
		readFile(alphabet, getCmdOption(argc, argv, "-af"));
	else
		alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

	// Инициализация threshold
	if (cmdOptionExists(argv, argv + argc, "-t"))
	{
		try
		{
			threshold = std::stoi(getCmdOption(argc, argv, "-t"));

			if (threshold < 0)
				throw "Error";
		}
		catch (...)
		{
			std::cout << "Неверно указан порог - положитльное число.\n\n";
			exit(0);
		}
	}
	else
		threshold = 6;

	// Инициализация keyLength
	if (cmdOptionExists(argv, argv + argc, "-k"))
	{
		try
		{
			keyLength = std::stoi(getCmdOption(argc, argv, "-k"));

			if (keyLength < 0)
				throw "Error";
		}
		catch (...)
		{
			std::cout << "Неверно указана длина ключа - положитльное число.\n\n";
			exit(0);
		}
	}
	else
		keyLength = 0;

	// Инициализация maxKeyLength
	if (cmdOptionExists(argv, argv + argc, "-mk"))
	{
		try
		{
			maxKeyLength = std::stoi(getCmdOption(argc, argv, "-mk"));

			if (maxKeyLength <= 0)
				throw "Error";
		}
		catch (...)
		{
			std::cout << "Неверно указана максимальная длина ключа - положитльное число.\n\n";
			exit(0);
		}
	}
	else
		maxKeyLength = 26;
}


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(.1251);
	SetConsoleOutputCP(.1251);

	std::string encryptedTextFileName = "tests/Encrypted text.txt";
	std::string decryptedTextFileName = "tests/Encrypted text {processed}.txt";
	std::string analiticalTextFileName = "tests/Analytical text.txt";

	parse_arguments(argc, argv, analiticalTextFileName, encryptedTextFileName, decryptedTextFileName);

	std::string encryptedText;

	readFile(encryptedText, encryptedTextFileName);
	deleteForbiddenSymbols(encryptedText);

	if (!keyLength)
		findKeyLength(encryptedText);

	findKeyValue(encryptedText, analiticalTextFileName, decryptedTextFileName);

	return 0;
}