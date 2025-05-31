#pragma once
#include <inc/natives.h>
#include <inc/types.h>
#include "../IScriptBuilder.h"

namespace Scripts
{
	class Marker : public IScriptBuilder
	{
public:
	// Instance of Marker
	static Marker& GetInstance()
	{
		static Marker instance;
		return instance;
	}

	void Tick() override;

	void Draw(int markerType, Vector3 position, Vector3 scale,
		int colorR, int colorG, int colorB, int colorA);

	// Marker toggles
	bool markerEnabled = false;

	// Marker position and scale
	float markerX = 236.2f;
	float markerY = -874.9f;
	//float markerZ = 35.25;
	float markerZ = 29.25;
	
	Vector3 scale = Vector3(2.0f, 2.0f, 2.0f);

	// Marker colors
	int colorR = 120;
	int colorG = 255;
	int colorB = 120;
	int colorA = 155;

private:
	Marker() {} // Private constructor to prevent external instantiation
	//Marker();
	Marker(const Marker&) = delete;
	Marker& operator=(const Marker&) = delete;

	// This value isn't in use anymore
	//Vector3 markerPosition = Vector3(markerX, markerY, markerZ);

};

}

//class MarkerScripts : public IScriptBuilder
//{
//public:
//	// Instance of the VehicleScripts
//	static MarkerScripts& GetInstance()
//	{
//		static MarkerScripts instance;
//		return instance;
//	}
//
//	void Tick() override;
//
//	void Draw(int markerType, Vector3 position, Vector3 scale,
//		int colorR, int colorG, int colorB, int colorA);
//
//private:
//	//MarkerScripts() {} // Private constructor to prevent external instantiation
//	MarkerScripts();
//	MarkerScripts(const MarkerScripts&) = delete;
//	MarkerScripts& operator=(const MarkerScripts&) = delete;
//};




