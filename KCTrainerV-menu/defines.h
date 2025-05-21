#pragma once

// Taken from defines.h in my ReVC project

template <typename T>
inline void
log_output(const T& message)
{
	std::cout << message << std::endl;
}

//#define CONSOLELOG std::cout