/**
 * An enumeration of the native types of values as described by the server.
 *
 * The value types correspond to the embedded SQL data types.
 * For more information about embedded SQL data types, see
 * [Embedded SQL data types](http://dcx.sap.com/goto?page=sa160/en/dbprogramming/esqlvar.html).
 */
enum a_sqlany_native_type {
	/**
	 * No data type.
	 */
	DT_NOTYPE = 0,

	/**
	 * Null-terminated character string that is a valid date.
	 */
	DT_DATE = 384,

	/**
	 * Null-terminated character string that is a valid time.
	 */
	DT_TIME = 388,

	/**
	 * Null-terminated character string that is a valid timestamp.
	 */
	DT_TIMESTAMP = 392,

	/**
	 * Varying length character string, in the CHAR character set, with a two-byte length field.
	 * The maximum length is 32765 bytes. When sending data, you must set the length field.
	 * When fetching data, the database server sets the length field. The data is not null-terminated or blank-padded.
	 */
	DT_VARCHAR = 448,

	/**
	 * Fixed-length blank-padded character string, in the CHAR character set.
	 * The maximum length, specified in bytes, is 32767. The data is not null-terminated.
	 */
	DT_FIXCHAR = 452,

	/**
	 * Long varying length character string, in the CHAR character set.
	 */
	DT_LONGVARCHAR = 456,

	/**
	 * Null-terminated character string, in the CHAR character set.
	 * The string is blank-padded if the database is initialized with blank-padded strings.
	 */
	DT_STRING = 460,

	/**
	 * 8-byte floating-point number.
	 */
	DT_DOUBLE = 480,

	/**
	 * 4-byte floating-point number.
	 */
	DT_FLOAT = 482,

	/**
	 * Packed decimal number (proprietary format).
	 */
	DT_DECIMAL = 484,

	/**
	 * 32-bit signed integer.
	 */
	DT_INT = 496,

	/**
	 * 16-bit signed integer.
	 */
	DT_SMALLINT = 500,

	/**
	 * Varying length binary data with a two-byte length field. The maximum length is 32765 bytes.
	 * When supplying information to the database server, you must set the length field.
	 * When fetching information from the database server, the server sets the length field.
	 */
	DT_BINARY = 524,

	/**
	 * Long binary data.
	 */
	DT_LONGBINARY = 528,

	/**
	 * 8-bit signed integer.
	 */
	DT_TINYINT = 604,

	/**
	 * 64-bit signed integer.
	 */
	DT_BIGINT = 608,

	/**
	 * 32-bit unsigned integer.
	 */
	DT_UNSINT = 612,

	/**
	 * 16-bit unsigned integer.
	 */
	DT_UNSSMALLINT = 616,

	/**
	 * 64-bit unsigned integer.
	 */
	DT_UNSBIGINT = 620,

	/**
	 * 8-bit signed integer.
	 */
	DT_BIT = 624,

	/**
	 * Null-terminated character string, in the NCHAR character set. The string is blank-padded if the database is initialized with blank-padded strings.
	 */
	DT_NSTRING = 628,

	/**
	 * Fixed-length blank-padded character string, in the NCHAR character set. The maximum length, specified in bytes, is 32767. The data is not null-terminated.
	 */
	DT_NFIXCHAR = 632,

	/**
	 * Varying length character string, in the NCHAR character set, with a two-byte length field.
	 * The maximum length is 32765 bytes. When sending data, you must set the length field.
	 * When fetching data, the database server sets the length field.
	 * The data is not null-terminated or blank-padded.
	 */
	DT_NVARCHAR = 636,

	/**
	 * Long varying length character string, in the NCHAR character set.
	 */
	DT_LONGNVARCHAR = 640,
}

export default a_sqlany_native_type;
