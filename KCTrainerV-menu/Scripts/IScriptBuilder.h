#pragma once

// Script builder interface, all scripts that are classes and have tick events should use this class.
class IScriptBuilder
{
public:
	virtual ~IScriptBuilder() = default; // Important: Virtual destructor for base classes.

	virtual void Tick() = 0;
};