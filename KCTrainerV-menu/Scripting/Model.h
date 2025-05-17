/*
* Menyoo PC - Grand Theft Auto V single-player trainer mod
* Copyright (C) 2019  MAFINS
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*/
#pragma once

// TODO Fix this file.

#undef MENYOO_SCRIPT_FILES
#ifdef MENYOO_SCRIPT_FILES

#include "..\Util\GTAmath.h"

#include <string>

typedef unsigned long DWORD, Hash;
typedef unsigned __int64 UINT64;
enum VehicleHash : DWORD;

namespace GTAmodel
{
	class ModelDimensions
	{
	public:
		//Vector3M Dim1;
		Vector3 Dim1;
		//Vector3M Dim2;
		Vector3 Dim2;

		ModelDimensions();
		//ModelDimensions(const Vector3M& d1, const Vector3M& d2);
		ModelDimensions(const Vector3& d1, const Vector3& d2);
	};

	class Model // Inconsistent with other class names - fix?
	{
	public:
		Hash hash;

		Model();
		Model(const std::string& name);
		Model(Hash newHash);
		Model(VehicleHash newHash);

		friend bool operator == (const Model& left, const Model& right);
		friend bool operator != (const Model& left, const Model& right);
		bool Equals(const Model& right) const;
		Model& operator = (const Model& right);

		UINT64 MemoryAddress() const;

		std::string VehicleDisplayName(bool properName) const;
		std::string VehicleModelName() const;

		ModelDimensions Dimensions() const;
		//void Dimensions(Vector3M& dim1C, Vector3M& dim2C) const;
		//void Dimensions(Vector3M_t& dim1, Vector3M_t& dim2) const;
		void Dimensions(Vector3& dim1, Vector3& dim2) const;
		//Vector3M Dim1() const;
		//Vector3M Dim2() const;
		Vector3 Dim1() const;
		Vector3 Dim2() const;

		bool IsBicycle() const;
		bool IsBike() const;
		bool IsQuadbike() const;
		bool IsBoat() const;
		bool IsCar() const;
		bool IsHeli() const;
		bool IsPlane() const;
		bool IsTrain() const;
		bool IsFastBoat() const;
		bool IsCargobob() const;
		bool IsBus() const;
		bool IsPoliceVehicle() const;
		bool HasSiren() const;
		bool IsWheelChangingSupportedVehicle() const;
		bool IsBennySupportedVehicle();

		bool IsVehicle() const;
		bool IsPed() const;

		void Load() const;
		bool Load(DWORD timeOut) const;
		void LoadAndWait() const;
		bool IsLoaded() const;
		void Unload() const;

		void LoadCollision() const;
		bool LoadCollision(DWORD timeOut) const;
		bool IsCollisionLoaded() const;
		void LoadCollisionAndWait() const;


		bool IsValid() const;
		bool IsInCdImage() const;
		bool Exists() const;

	};
}

using namespace GTAmodel;

#endif //MENYOO_SCRIPT_FILES