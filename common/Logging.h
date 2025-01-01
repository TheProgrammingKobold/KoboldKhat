#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <chrono>

class Log
{
public:

	static void Info(const std::string& input);
	static void Error(const std::string& input);
	static void ExceptError(const std::string& input);
	static void Warning(const std::string& input);

private:

	static std::chrono::system_clock::time_point currnetTime;
	static std::string getCurrentTime();

};

