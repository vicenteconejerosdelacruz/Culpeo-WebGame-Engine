#pragma once

#include <WinTypes.h>
#include <SimpleMath.h>

using namespace DirectX;

inline void PrintRect(std::string name, RECT& r)
{
	std::string title = "rect-" + name;
	std::string left = "left:" + std::to_string(r.left);
	std::string right = "right:" + std::to_string(r.right);
	std::string top = "top:" + std::to_string(r.top);
	std::string bottom = "bottom:" + std::to_string(r.bottom);
	
	std::cout << title << std::endl;
	std::cout << left << std::endl;
	std::cout << right << std::endl;
	std::cout << top << std::endl;
	std::cout << bottom << std::endl;
}

inline void PrintXMVector(XMVECTOR v, std::string name = "")
{
	std::cout << std::string(name + std::string((name != "") ? ":" : "") + std::to_string(v.vector4_f32[0]) + "," + std::to_string(v.vector4_f32[1]) + "," + std::to_string(v.vector4_f32[2]) + "," + std::to_string(v.vector4_f32[3])) << std::endl;
}

inline void PrintXMFloat3(XMFLOAT3 v, std::string name = "")
{
	std::cout << std::string(name + std::string((name != "") ? ":" : "") + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z)) << std::endl;
}

// #if defined(_EDITOR)
// inline void PrintImVec2(ImVec2 v, std::string name = "")
// {
// 	OutputDebugStringA(std::string(name + std::string((name != "") ? ":" : "") + std::to_string(v.x) + "," + std::to_string(v.y) + "\n").c_str());
// }
// #endif

inline std::string OutputV3(XMVECTOR V3)
{
	return std::string(std::to_string(V3.vector4_f32[0]) + "," + std::to_string(V3.vector4_f32[1]) + "," + std::to_string(V3.vector4_f32[2]));
}

inline void MatrixDump(std::string matrixName, DirectX::XMMATRIX m)
{
	std::string row1 = "[" +
		std::to_string(m.r[0].vector4_f32[0]) + "," +
		std::to_string(m.r[0].vector4_f32[1]) + "," +
		std::to_string(m.r[0].vector4_f32[2]) + "," +
		std::to_string(m.r[0].vector4_f32[3]) + "]";
	std::string row2 = "[" +
		std::to_string(m.r[1].vector4_f32[0]) + "," +
		std::to_string(m.r[1].vector4_f32[1]) + "," +
		std::to_string(m.r[1].vector4_f32[2]) + "," +
		std::to_string(m.r[1].vector4_f32[3]) + "]";
	std::string row3 = "[" +
		std::to_string(m.r[2].vector4_f32[0]) + "," +
		std::to_string(m.r[2].vector4_f32[1]) + "," +
		std::to_string(m.r[2].vector4_f32[2]) + "," +
		std::to_string(m.r[2].vector4_f32[3]) + "]";
	std::string row4 = "[" +
		std::to_string(m.r[3].vector4_f32[0]) + "," +
		std::to_string(m.r[3].vector4_f32[1]) + "," +
		std::to_string(m.r[3].vector4_f32[2]) + "," +
		std::to_string(m.r[3].vector4_f32[3]) + "]";
	std::string matrixDump = row1 + "\n" + row2 + "\n" + row3 + "\n" + row4 + "\n";

	std::cout << matrixName << std::endl;
	std::cout << matrixDump << std::endl;
}

inline void MatrixDump(std::string matrixName, DirectX::XMFLOAT4X4 m)
{
	std::string row1 = "[" +
		std::to_string(m._11) + "," +
		std::to_string(m._12) + "," +
		std::to_string(m._13) + "," +
		std::to_string(m._14) + "]";
	std::string row2 = "[" +
		std::to_string(m._21) + "," +
		std::to_string(m._22) + "," +
		std::to_string(m._23) + "," +
		std::to_string(m._24) + "]";
	std::string row3 = "[" +
		std::to_string(m._31) + "," +
		std::to_string(m._32) + "," +
		std::to_string(m._33) + "," +
		std::to_string(m._34) + "]";
	std::string row4 = "[" +
		std::to_string(m._41) + "," +
		std::to_string(m._42) + "," +
		std::to_string(m._43) + "," +
		std::to_string(m._44) + "]";
	std::string matrixDump = row1 + "\n" + row2 + "\n" + row3 + "\n" + row4 + "\n";

	std::cout << matrixName << std::endl;
	std::cout << matrixDump;
}

// template<typename T>
// void PrintUniquePtr(std::string ptrName, std::unique_ptr<T>& ptr)
// {
// 	std::string addr = std::format("{} address: {}\n", ptrName, (void*)ptr.get());
// 	OutputDebugStringA(addr.c_str());
// }