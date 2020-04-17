#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "CubeCell.h"

/*
	Załadowanie całego (30 MB danych) pliku do pamięci mogłoby być mało wydajne, więc
	klasa DataMiner otwiera plik podany w konstruktorze i pozwala:

	przeczytać rozmiar siatki i nazwy komórek 
	przeczytać komórkę w danej linijce pliku - GetCellAt(int index); 
	przeczytać komórkę w kolejnej linijce pliku - GetNextCell(int index); (szybsze od GetCellAt() jeżeli chcemy przeczytać parę komórek obok siebie)
*/

class DataMiner
{
private:
	const char* fileName;
	unsigned short meshSize[3];
	std::vector<std::string> valuesNames;
	std::ifstream file;
	std::map<int, std::vector<float>> colors;
public:
	DataMiner(const char* fileName);
	DataMiner(const std::string fileName);
	DataMiner(const DataMiner&) = delete;
	DataMiner() = delete;
	DataMiner& operator=(const DataMiner&) = delete;
	~DataMiner() noexcept;
	void closeFile() noexcept;
	//static std::vector<CubeCell> LoadCells(const char* file);
	CubeCell GetNextCell();
	CubeCell GetCellAt(int index);
	unsigned short* GetMeshSize() noexcept;
	std::vector<std::string> GetValuesNames()noexcept;
	std::vector<float> getColorOf(int id) noexcept;
};
