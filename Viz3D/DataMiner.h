#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include "Cell.h"

/*

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