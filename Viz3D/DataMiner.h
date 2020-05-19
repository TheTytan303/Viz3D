#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include "CubeCell.h"

/*
	Za�adowanie ca�ego (30 MB danych) pliku do pami�ci mog�oby by� ma�o wydajne, wi�c
	klasa DataMiner otwiera plik podany w konstruktorze i pozwala:

	przeczyta� rozmiar siatki i nazwy kom�rek 
	przeczyta� kom�rk� w danej linijce pliku - GetCellAt(int index); 
	przeczyta� kom�rk� w kolejnej linijce pliku - GetNextCell(int index); (szybsze od GetCellAt() je�eli chcemy przeczyta� par� kom�rek obok siebie)
*/

class DataMiner
{
public:
	const char* fileName;
	unsigned short meshSize[3];
	//int fileHeight;
	//std::vector<std::string> valuesNames;
	std::ifstream file;
	//std::map<int, std::vector<float>> colors;
	std::vector<int> grains;
public:
	DataMiner(const char* fileName);
	DataMiner(const std::string fileName);
	DataMiner(const std::wstring fileName);
	DataMiner(const DataMiner&) = delete;
	DataMiner() = delete;
	DataMiner& operator=(const DataMiner&) = delete;
	~DataMiner() noexcept;
	void closeFile() noexcept;
	//int GetFileHeight() noexcept;
	//static std::vector<CubeCell> LoadCells(const char* file);
	std::unique_ptr<Cell> GetNextCell();
	std::unique_ptr<Cell> GetCellAt(int index);
	unsigned short* GetMeshSize() noexcept;
	std::vector<std::string> GetValuesNames()noexcept;
	void initGrain(int id) noexcept;
};