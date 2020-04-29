#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include "CubeCell.h"

/*
	Za³adowanie ca³ego (30 MB danych) pliku do pamiêci mog³oby byæ ma³o wydajne, wiêc
	klasa DataMiner otwiera plik podany w konstruktorze i pozwala:

	przeczytaæ rozmiar siatki i nazwy komórek 
	przeczytaæ komórkê w danej linijce pliku - GetCellAt(int index); 
	przeczytaæ komórkê w kolejnej linijce pliku - GetNextCell(int index); (szybsze od GetCellAt() je¿eli chcemy przeczytaæ parê komórek obok siebie)
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
	std::unique_ptr<CubeCell> GetNextCell(Graphics& gfx);
	std::unique_ptr<CubeCell> GetCellAt(int index, Graphics& gfx);
	unsigned short* GetMeshSize() noexcept;
	std::vector<std::string> GetValuesNames()noexcept;
	std::vector<float> getColorOf(int id) noexcept;
};