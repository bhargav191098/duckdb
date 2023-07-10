//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/core_functions/scalar/operators_functions.hpp
//
//
//===----------------------------------------------------------------------===//
// This file is automatically generated by scripts/generate_functions.py
// Do not edit this file manually, your changes will be overwritten
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/function/function_set.hpp"

namespace duckdb {

struct BitwiseAndFun {
	static constexpr const char *Name = "&";
	static constexpr const char *Parameters = "left,right";
	static constexpr const char *Description = "Bitwise AND";
	static constexpr const char *Example = "91 & 15";

	static ScalarFunctionSet GetFunctions();
};

struct BitwiseOrFun {
	static constexpr const char *Name = "|";
	static constexpr const char *Parameters = "left,right";
	static constexpr const char *Description = "Bitwise OR";
	static constexpr const char *Example = "32 & 3";

	static ScalarFunctionSet GetFunctions();
};

struct BitwiseNotFun {
	static constexpr const char *Name = "~";
	static constexpr const char *Parameters = "input";
	static constexpr const char *Description = "Bitwise NOT";
	static constexpr const char *Example = "~15";

	static ScalarFunctionSet GetFunctions();
};

struct LeftShiftFun {
	static constexpr const char *Name = "<<";
	static constexpr const char *Parameters = "input";
	static constexpr const char *Description = "bitwise shift left";
	static constexpr const char *Example = "1 << 4";

	static ScalarFunctionSet GetFunctions();
};

struct RightShiftFun {
	static constexpr const char *Name = ">>";
	static constexpr const char *Parameters = "input";
	static constexpr const char *Description = "bitwise shift right";
	static constexpr const char *Example = "8 >> 2";

	static ScalarFunctionSet GetFunctions();
};

struct BitwiseXorFun {
	static constexpr const char *Name = "xor";
	static constexpr const char *Parameters = "left,right";
	static constexpr const char *Description = "Bitwise XOR";
	static constexpr const char *Example = "xor(17, 5)";

	static ScalarFunctionSet GetFunctions();
};

} // namespace duckdb
