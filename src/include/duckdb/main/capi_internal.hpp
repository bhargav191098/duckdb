//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/main/capi_internal.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb.h"
#include "duckdb.hpp"
#include "duckdb/common/types/data_chunk.hpp"
#include <cstring>
#include <cassert>

#ifdef _WIN32
#ifndef strdup
#define strdup _strdup
#endif
#endif

#ifdef GetCValue
#undef GetCValue
#endif

namespace duckdb {

struct DatabaseData {
	unique_ptr<DuckDB> database;
};

struct PreparedStatementWrapper {
	unique_ptr<PreparedStatement> statement;
	vector<Value> values;
};

struct ArrowResultWrapper {
	unique_ptr<MaterializedQueryResult> result;
	unique_ptr<DataChunk> current_chunk;
};

duckdb_type ConvertCPPTypeToC(LogicalType type);
idx_t GetCTypeSize(duckdb_type type);
duckdb_state duckdb_translate_result(MaterializedQueryResult *result, duckdb_result *out);

}
