#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "CubeCell.h"
class DataMiner
{
private:
	const char* fileName;
	unsigned short meshSize[3];
	std::ifstream file;
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
	unsigned short* GetMeshSize() noexcept;
};