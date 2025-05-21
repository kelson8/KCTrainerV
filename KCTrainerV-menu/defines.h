#pragma once

#include <iostream>

// Taken from defines.h in my ReVC project

static std::string kcTrainerString = "[KCTrainerV]: ";
static std::string kcErrorString = kcTrainerString + " Error: ";

template <typename T>
inline void
log_output(const T& message)
{
	//std::cout << message << std::endl;
	std::cout << kcTrainerString << message << std::endl;
}

template <typename T>
inline void
log_error(const T& message)
{
	std::cout << kcErrorString << message << std::endl;
}

//#define CONSOLELOG std::cout