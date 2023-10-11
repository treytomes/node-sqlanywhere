import a_sqlany_native_type from '../a_sqlany_native_type';
import a_sqlany_data_type from '../a_sqlany_data_type';

declare namespace sqlanywhere {
	interface IColumnInfo {
		maxSize: number;
		name: string;
		nativeType: a_sqlany_native_type;
		nullable: boolean;
		precision: number;
		scale: number;
		type: a_sqlany_data_type;
	}

	class Connection {}

	class Statement {}

	class Error {
		code: number;
		reason: string;
	}

	function hello(): string;
	function add(n1: number, n2: number): number;

	function init(): boolean;
	function fini(): void;

	function new_connection(): Connection;
	function free_connection(cn: Connection): void;
	function connect(cn: Connection, connectionString: string): boolean;
	function disconnect(cn: Connection): boolean;
	function error(cn: Connection): Error;
	function execute_direct(cn: Connection, queryText: string): Statement;
	function free_stmt(statementHandle: Statement): void;
	function num_cols(statementHandle: Statement): number;
	function fetch_next(statementHandle: Statement): boolean;
	function get_column(
		statementHandle: Statement,
		columnIndex: number
	): {
		buffer: string;
		buffer_size: number;
		is_null: boolean;
		length: number;
	};
	function get_column_info(statementHandle: Statement, columnIndex: number): IColumnInfo;

	class MyObject {
		public value: number;

		constructor(value: number);
		plusOne(): number;
		multiply(value: number | null = null): MyObject;
	}
}

type sqlanywhere = typeof sqlanywhere;
export default sqlanywhere;
