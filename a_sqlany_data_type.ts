/**
 * Specifies the data type being passed in or retrieved.
 */
enum a_sqlany_data_type {
	/**
	 * Invalid data type.
	 */
	A_INVALID_TYPE,

	/**
	 * Binary data.  Binary data is treated as-is and no character set conversion is performed.
	 */
	A_BINARY,

	/**
	 * String data.  The data where character set conversion is performed.
	 */
	A_STRING,

	/**
	 * Double data.  Includes float values.
	 */
	A_DOUBLE,

	/**
	 * 64-bit integer.
	 */
	A_VAL64,

	/**
	 * 64-bit unsigned integer.
	 */
	A_UVAL64,

	/**
	 * 32-bit integer.
	 */
	A_VAL32,

	/**
	 * 32-bit unsigned integer.
	 */
	A_UVAL32,

	/**
	 * 16-bit integer.
	 */
	A_VAL16,

	/**
	 * 16-bit unsigned integer.
	 */
	A_UVAL16,

	/**
	 * 8-bit integer.
	 */
	A_VAL8,

	/**
	 * 8-bit unsigned integer.
	 */
	A_UVAL8,
}

export default a_sqlany_data_type;
