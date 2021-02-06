#ifndef CLASS_H
#define CLASS_H

#include <algorithm>
#include <vector>
#include <set>
#include "windows.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>


class Symbol{
public:
	char word;
	int count;

	Symbol(char word, int count) :word(word), count(count){};

	Symbol() :word(), count(){};
};

class Frequency{
public:
	std::vector<Symbol> info;

	Frequency(std::vector<Symbol> info) :info(info){};

	Frequency() :info(){};

};

class symbolsDistance{
public:
	std::vector<int> distance;

	symbolsDistance(std::vector<int> distance) :distance(distance){};

	symbolsDistance() :distance(){};

};

class SymbolsDistanceFrequency{
public:
	int distance;
	int count;
	SymbolsDistanceFrequency(int distance, int count) :distance(distance), count(count){};

	SymbolsDistanceFrequency() :distance(), count(){};

};

class index{
public:
	std::vector<long double> distance;

	index(std::vector<long double> distance) :distance(distance){};

	index() :distance(){};

};


class Compare {
	std::string alpha;
public:
	Compare(std::string alpha) : alpha(alpha){};
	bool operator() (Symbol a, Symbol b) {
		
		return alpha.find(a.word) < alpha.find(b.word);
	}
};

class CompareMaxCount {
public:
	 bool  operator() (Symbol a, Symbol b) {
		return a.count > b.count;
	}
};

class CompareFreqDisSmbl {
public:
	bool  operator() (SymbolsDistanceFrequency a, SymbolsDistanceFrequency b) {
		return a.count > b.count;
	}
};


//работа со строками
class isAlpha {
public:
	std::string str;

	isAlpha(std::string str) :str(str) {};

	bool operator () (char init) {
		if (str.find(init) != std::string::npos)
			return false;
		return true;
	}
};

class noIsAlpha {
public:
	std::string str;

	noIsAlpha(std::string str) :str(str) {};

	bool operator () (char init) {
		if (str.find(init) != std::string::npos)
			return true;
		return  false;
	}
};

class FindAlpha {
public:
	char str;

	FindAlpha(char str) :str(str) {};

	bool operator () (char init) {
		str ^= 32;
		if (str == init)
			return  true;
		return false;
	}
};


#endif CLASS_H