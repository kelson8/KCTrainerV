#pragma once

#ifdef LUA_TEST

enum class LuaNativeReturnType
{
    None,
    Bool,
    Int,
    UInt,
    Float,
    String,
    Vector3
};

struct LuaVector3
{
    alignas(8) float X = 0.f;
    alignas(8) float Y = 0.f;
    alignas(8) float Z = 0.f;

    LuaVector3() = default;

    LuaVector3(float x, float y, float z) : X(x), Y(y), Z(z)
    {
    }
};

#endif //LUA_TEST