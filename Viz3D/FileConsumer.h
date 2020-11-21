#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include "CubeCell.h"

class FileConsumer
{
public:
	FileConsumer(const char* fileName);
	FileConsumer(const std::string fileName);
	FileConsumer(const std::wstring fileName);
	FileConsumer(const FileConsumer&) = delete;

	~FileConsumer() noexcept;
	FileConsumer& operator=(const FileConsumer&) = delete;
	FileConsumer() = delete;
	void closeFile() noexcept;

};