/**
 * @file napiHelpers.h
 * @author Trey Tomes (George.Tomes@cityelectricsupply.com)
 * @brief Everything for writing a Node.js extension that isn't specific to sqlanywhere.
 * @version 0.1
 * @date 2022-08-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __NAPIHELPERS_H__
#define __NAPIHELPERS_H__

#include <node_api.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define APP_NAME "Node.js"

#define NAPI_FUNC(funcName) napi_value funcName(napi_env env, napi_callback_info info)
#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }
#define CREATE_BYTE_BLOB(value, length) createByteBlob(env, value, length)
#define CREATE_STRING(value) createString(env, value)
#define CREATE_INT8(value) createInt8(env, value)
#define CREATE_INT16(value) createInt16(env, value)
#define CREATE_INT32(value) createInt32(env, value)
#define CREATE_INT64(value) createInt64(env, value)
#define CREATE_UINT8(value) createUInt8(env, value)
#define CREATE_UINT16(value) createUInt16(env, value)
#define CREATE_UINT32(value) createUInt32(env, value)
#define CREATE_UINT64(value) createUInt64(env, value)
#define CREATE_DOUBLE(value) createDouble(env, value)
#define CREATE_BOOL(value) createBoolean(env, value)
#define CREATE_OBJECT() createObject(env)
#define GET_ARGUMENTS(args, argc) napi_value args[argc]; if (!getArguments(env, info, args, argc)) return nullptr
#define GET_VALUE(type, value, jsValue) type value; if (!getValue(env, jsValue, &value)) return nullptr
#define GET_VALUE_STRING(size, value, jsValue) char value[size]; if (!getValue(env, jsValue, value, size)) return nullptr
#define SET_PROPERTY(obj, name, value) assert(napi_set_named_property(env, obj, name, value) == napi_ok)
#define REGISTER_FUNCTION(name) registerFunction(env, exports, #name, name)

inline void registerFunction(napi_env env, napi_value exports, const char* name, napi_value (*func)(napi_env, napi_callback_info)) {
	napi_property_descriptor desc = DECLARE_NAPI_METHOD(name, func);
	napi_status status = napi_define_properties(env, exports, 1, &desc);
	assert(status == napi_ok);
}

inline napi_value createByteBlob(napi_env env, const char* value, int length) {
	napi_value result;
	napi_status status = napi_create_string_utf8(env, value, length, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createString(napi_env env, const char* value) {
	napi_value result;
	napi_status status = napi_create_string_utf8(env, value, strlen(value), &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createDouble(napi_env env, double value) {
	napi_value result;
	napi_status status = napi_create_double(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createDouble(napi_env env, bool value) {
	napi_value result;
	napi_status status = napi_create_double(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createBoolean(napi_env env, bool value) {
	napi_value result;
	napi_status status = napi_create_int32(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createInt8(napi_env env, char value) {
	napi_value result;
	napi_status status = napi_create_int32(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createInt16(napi_env env, short value) {
	napi_value result;
	napi_status status = napi_create_int32(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createInt32(napi_env env, int value) {
	napi_value result;
	napi_status status = napi_create_int32(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createInt64(napi_env env, int value) {
	napi_value result;
	napi_status status = napi_create_int64(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createUInt8(napi_env env, unsigned char value) {
	napi_value result;
	napi_status status = napi_create_uint32(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createUInt16(napi_env env, unsigned short value) {
	napi_value result;
	napi_status status = napi_create_uint32(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createUInt32(napi_env env, unsigned int value) {
	napi_value result;
	napi_status status = napi_create_uint32(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createUInt64(napi_env env, uint64_t value) {
	napi_value result;
	napi_status status = napi_create_bigint_uint64(env, value, &result);
	assert(status == napi_ok);
	return result;
}

inline napi_value createObject(napi_env env) {
	napi_value obj;
	napi_status status = napi_create_object(env, &obj);
	assert(status == napi_ok);
	return obj;
}

inline napi_value createPointer(napi_env env, void* ptr, napi_type_tag typeTag) {
	napi_status status;
	napi_value result;

	// Create a new, empty JS object.
	status = napi_create_object(env, &result);
	assert(status == napi_ok);

	// Tag the object to indicate that it holds a pointer to a `typeTag`.
	status = napi_type_tag_object(env, result, &typeTag);
	if (status != napi_ok) return NULL;

	status = napi_wrap(env, result, ptr, nullptr, nullptr, nullptr);
	assert(status == napi_ok);
	return result;
}

inline bool getArguments(napi_env env, napi_callback_info info, napi_value* args, size_t argc) {
	size_t expectedArgc = argc;

	napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
	assert(status == napi_ok);

	if (argc != expectedArgc) {
		char buffer[256];
		sprintf(buffer, "Expected %d arguments, found %d.", (int)expectedArgc, (int)argc);
		napi_throw_type_error(env, nullptr, buffer);
		return false;
	}
	return true;
}

inline bool getValue(napi_env env, napi_value value, char* result, int resultSize) {
	napi_status status;
	napi_valuetype type;

	status = napi_typeof(env, value, &type);
	assert(status == napi_ok);

	if (type != napi_string) {
		napi_throw_type_error(env, nullptr, "Expected a string.");
		return false;
	}

	size_t textLength;
	status = napi_get_value_string_utf8(env, value, result, resultSize, &textLength);
	assert(status == napi_ok);
	return true;
}

inline bool getValue(napi_env env, napi_value value, double* result) {
	napi_status status;
	napi_valuetype type;

	status = napi_typeof(env, value, &type);
	assert(status == napi_ok);

	if (type != napi_number) {
		napi_throw_type_error(env, nullptr, "Expected a number.");
		return false;
	}

	status = napi_get_value_double(env, value, result);
	assert(status == napi_ok);
	return true;
}

inline bool getValue(napi_env env, napi_value value, int64_t* result) {
	napi_status status;
	napi_valuetype type;

	status = napi_typeof(env, value, &type);
	assert(status == napi_ok);

	if (type != napi_number) {
		napi_throw_type_error(env, nullptr, "Expected a number.");
		return false;
	}

	status = napi_get_value_int64(env, value, result);
	assert(status == napi_ok);
	return true;
}

inline bool getValue(napi_env env, napi_value value, int32_t* result) {
	napi_status status;
	napi_valuetype type;

	status = napi_typeof(env, value, &type);
	assert(status == napi_ok);

	if (type != napi_number) {
		napi_throw_type_error(env, nullptr, "Expected a number.");
		return false;
	}

	status = napi_get_value_int32(env, value, result);
	assert(status == napi_ok);
	return true;
}

inline bool getValue(napi_env env, napi_value value, void** ptr, napi_type_tag typeTag) {
	// Check that the object passed as the first parameter has the previously applied tag.
	bool doesHandleMatch;
	napi_status status = napi_check_object_type_tag(env, value, &typeTag, &doesHandleMatch);
	if (status != napi_ok) {
		return false;
	}

	void* result;
	status = napi_unwrap(env, value, &result);
	*ptr = result;
	if (status != napi_ok) {
		return false;
	}
	
	return true;
}

#endif
