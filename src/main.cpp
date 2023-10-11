#include "napiHelpers.h"
#include "sacapidll.h"
#include "MyObject.h"

#pragma comment(lib, "C:\\Program Files\\SQL Anywhere 17\\SDK\\Lib\\X64\\dbcapi.lib")

#define CONNECT_STRING_MAX_LENGTH 256
#define QUERY_STRING_MAX_LENGTH 1024

// This value is the type tag for a connection handle.
// The command `uuidgen | sed -r -e 's/-//g' -e 's/(.{16})(.*)/0x\1, 0x\2/'`
// can be used to obtain the two values with which to initialize the structure.
static const napi_type_tag ConnectionHandleTypeTag = { 0x1edf75a38336451d, 0xa5ed9ce2e4c00c38 };

// This value is the type tag for a statment handle.
static const napi_type_tag StatementHandleTypeTag = { 0x9c73317f9fad44a3, 0x93c3920bf3b0ad6a };

inline napi_value createPointer(napi_env env, a_sqlany_connection* conn) {
	return createPointer(env, conn, ConnectionHandleTypeTag);
}

inline napi_value createPointer(napi_env env, a_sqlany_stmt* stmt) {
	return createPointer(env, stmt, StatementHandleTypeTag);
}

bool getValue(napi_env env, napi_value value, a_sqlany_connection** conn) {
	void* result;
	if (!getValue(env, value, &result, ConnectionHandleTypeTag)) {
		return false;
	}
	*conn = (a_sqlany_connection*)result;
	return true;
}

bool getValue(napi_env env, napi_value value, a_sqlany_stmt** stmt) {
	void* result;
	if (!getValue(env, value, &result, StatementHandleTypeTag)) {
		return false;
	}
	*stmt = (a_sqlany_stmt*)result;
	return true;
}

// Example: Returning a string.
NAPI_FUNC(hello) {
	return CREATE_STRING("world");
}

// Example: Using 2 arguments.
NAPI_FUNC(add) {
	GET_ARGUMENTS(args, 2);
	GET_VALUE(double, value0, args[0]);
	GET_VALUE(double, value1, args[1]);
	return createDouble(env, value0 + value1);
}

NAPI_FUNC(init) {
	sacapi_u32 api_version;
	bool status = sqlany_init(APP_NAME, SQLANY_API_VERSION_4, &api_version);
	return createBoolean(env, status);
}

NAPI_FUNC(fini) {
	sqlany_fini();
	return nullptr;
}

NAPI_FUNC(new_connection) {
	a_sqlany_connection* sqlany_conn = sqlany_new_connection();
	if (sqlany_conn == nullptr) {
		napi_throw_type_error(env, nullptr, "Failed to create a new connection.");
	}
	return createPointer(env, sqlany_conn);
}

NAPI_FUNC(free_connection) {
	GET_ARGUMENTS(args, 1);
	GET_VALUE(a_sqlany_connection*, sqlany_conn, args[0]);
	sqlany_free_connection(sqlany_conn);
	return nullptr;
}

NAPI_FUNC(connect) {
	GET_ARGUMENTS(args, 2);
	GET_VALUE(a_sqlany_connection*, sqlany_conn, args[0]);
	GET_VALUE_STRING(CONNECT_STRING_MAX_LENGTH, connectionString, args[1]);
	bool status = sqlany_connect(sqlany_conn, connectionString);
	return createBoolean(env, status);
}

NAPI_FUNC(disconnect) {
	GET_ARGUMENTS(args, 1);
	GET_VALUE(a_sqlany_connection*, sqlany_conn, args[0]);
	bool status = sqlany_disconnect(sqlany_conn);
	return createBoolean(env, status);
}

NAPI_FUNC(error) {
	GET_ARGUMENTS(args, 1);
	GET_VALUE(a_sqlany_connection*, sqlany_conn, args[0]);

	// Get the last error.
	char reason[SACAPI_ERROR_SIZE];
	sacapi_i32 code = sqlany_error(sqlany_conn, reason, SACAPI_ERROR_SIZE);

	// Construct the JSON object to return.
	napi_value obj = CREATE_OBJECT();
	SET_PROPERTY(obj, "code", CREATE_INT32(code));
	SET_PROPERTY(obj, "reason", CREATE_STRING(reason));
	return obj;
}

NAPI_FUNC(execute_direct) {
	GET_ARGUMENTS(args, 2);
	GET_VALUE(a_sqlany_connection*, sqlany_conn, args[0]);
	GET_VALUE_STRING(QUERY_STRING_MAX_LENGTH, queryText, args[1]);
	a_sqlany_stmt* stmt = sqlany_execute_direct(sqlany_conn, queryText);
	if (stmt == nullptr) {
		napi_throw_type_error(env, nullptr, "Failed to execute a direct statement.");
	}
	return createPointer(env, stmt);
}

NAPI_FUNC(free_stmt) {
	GET_ARGUMENTS(args, 1);
	GET_VALUE(a_sqlany_stmt*, stmt, args[0]);
	sqlany_free_stmt(stmt);
	return nullptr;
}

NAPI_FUNC(num_cols) {
	GET_ARGUMENTS(args, 1);
	GET_VALUE(a_sqlany_stmt*, stmt, args[0]);
	int result = sqlany_num_cols(stmt);
	return CREATE_INT32(result);
}

NAPI_FUNC(fetch_next) {
	GET_ARGUMENTS(args, 1);
	GET_VALUE(a_sqlany_stmt*, stmt, args[0]);
	bool result = sqlany_fetch_next(stmt);
	return CREATE_BOOL(result);
}

NAPI_FUNC(get_column) {
	GET_ARGUMENTS(args, 2);
	GET_VALUE(a_sqlany_stmt*, stmt, args[0]);
	GET_VALUE(int, columnIndex, args[1]);

	a_sqlany_data_value fetched_value;

	if (!sqlany_get_column(stmt, columnIndex, &fetched_value)) {
		char buffer[256];
		sprintf(buffer, "Unable to get column %d.", columnIndex);
		napi_throw_type_error(env, nullptr, buffer);
		return nullptr;
	}

	a_sqlany_column_info columnInfo;
	if (!sqlany_get_column_info(stmt, columnIndex, &columnInfo)) {
		char buffer[256];
		sprintf(buffer, "Column index %d is out of range.", columnIndex);
		napi_throw_error(env, nullptr, buffer);
	}

	char* buffer = fetched_value.buffer;
	size_t buffer_size = fetched_value.buffer_size;
	sacapi_bool is_null = *fetched_value.is_null;
	size_t length = *fetched_value.length;

	napi_value obj = CREATE_OBJECT();
	SET_PROPERTY(obj, "buffer_size", CREATE_INT32(buffer_size));
	SET_PROPERTY(obj, "is_null", CREATE_BOOL(is_null));
	SET_PROPERTY(obj, "length", CREATE_INT32(length));

	switch (columnInfo.type) {
		case A_BINARY:
		case A_STRING:
			SET_PROPERTY(obj, "buffer", CREATE_BYTE_BLOB(buffer, length));
			break;
		case A_VAL64:
			SET_PROPERTY(obj, "buffer", CREATE_INT64(*(int64_t*)buffer));
			break;
		case A_UVAL64:
			SET_PROPERTY(obj, "buffer", CREATE_UINT64(*(uint64_t*)buffer));
			break;
		case A_VAL32:
			SET_PROPERTY(obj, "buffer", CREATE_INT32(*(int32_t*)buffer));
			break;
		case A_UVAL32:
			SET_PROPERTY(obj, "buffer", CREATE_UINT32(*(uint32_t*)buffer));
			break;
		case A_VAL16:
			SET_PROPERTY(obj, "buffer", CREATE_INT16(*(uint16_t*)buffer));
			break;
		case A_UVAL16:
			SET_PROPERTY(obj, "buffer", CREATE_UINT16(*(uint16_t*)buffer));
			break;
		case A_VAL8:
			SET_PROPERTY(obj, "buffer", CREATE_UINT8(*(uint8_t*)buffer));
			break;
		case A_UVAL8:
			SET_PROPERTY(obj, "buffer", CREATE_UINT8(*(uint8_t*)buffer));
			break;
		case A_DOUBLE:
			SET_PROPERTY(obj, "buffer", CREATE_DOUBLE(*(double*)buffer));
			break;
		default:
			SET_PROPERTY(obj, "buffer", nullptr);
			break;
	}

	return obj;
}

NAPI_FUNC(get_column_info) {
	GET_ARGUMENTS(args, 2);
	GET_VALUE(a_sqlany_stmt*, stmt, args[0]);
	GET_VALUE(int, columnIndex, args[1]);

	a_sqlany_column_info columnInfo;
	sacapi_bool result = sqlany_get_column_info(stmt, columnIndex, &columnInfo);
	if (!result) {
		char buffer[256];
		sprintf(buffer, "Column index %d is out of range.", columnIndex);
		napi_throw_error(env, nullptr, buffer);
	}

	size_t max_size = columnInfo.max_size;
	char* name = columnInfo.name;
	a_sqlany_native_type native_type = columnInfo.native_type;
	sacapi_bool nullable = columnInfo.nullable;
	unsigned short precision = columnInfo.precision;
	unsigned short scale = columnInfo.scale;
	a_sqlany_data_type type = columnInfo.type;

	napi_value obj = CREATE_OBJECT();
	SET_PROPERTY(obj, "maxSize", CREATE_INT32(max_size));
	SET_PROPERTY(obj, "name", CREATE_STRING(name));
	SET_PROPERTY(obj, "nativeType", CREATE_UINT32(native_type));
	SET_PROPERTY(obj, "nullable", CREATE_BOOL(nullable));
	SET_PROPERTY(obj, "precision", CREATE_UINT32(precision));
	SET_PROPERTY(obj, "scale", CREATE_UINT32(scale));
	SET_PROPERTY(obj, "type", CREATE_UINT32(type));
	return obj;
}

NAPI_MODULE_INIT() {
	exports = MyObject::Init(env, exports);

	registerFunction(env, exports, "hello", hello);
	registerFunction(env, exports, "add", add);
	registerFunction(env, exports, "init", init);
	registerFunction(env, exports, "fini", fini);
	registerFunction(env, exports, "new_connection", new_connection);
	registerFunction(env, exports, "free_connection", free_connection);
	registerFunction(env, exports, "connect", connect);
	registerFunction(env, exports, "disconnect", disconnect);
	registerFunction(env, exports, "error", error);
	registerFunction(env, exports, "execute_direct", execute_direct);
	registerFunction(env, exports, "free_stmt", free_stmt);
	registerFunction(env, exports, "num_cols", num_cols);
	registerFunction(env, exports, "fetch_next", fetch_next);
	registerFunction(env, exports, "get_column", get_column);
	registerFunction(env, exports, "get_column_info", get_column_info);
	
	return exports;
}
