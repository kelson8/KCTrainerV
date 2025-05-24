#pragma once
#include <iostream>

// Taken from defines.h in my ReVC project

static std::string kcTrainerString = "[KCTrainerV]: ";
static std::string kcErrorString = kcTrainerString + " Error: ";

template <typename T>
inline void
log_output(const T& message)
{
	std::cout << kcTrainerString << message << std::endl;
	std::cout << std::flush; // Crucial for immediate console output
}

template <typename T>
inline void
log_error(const T& message)
{
	std::cout << kcErrorString << message << std::endl;
}
