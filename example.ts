require('dotenv').config()

import ColumnInfo from './ColumnInfo';

// eslint-disable-next-line import/no-unresolved
import type sqlanywhere from './types/sqlanywhere';

// eslint-disable-next-line @typescript-eslint/no-var-requires
const sa: sqlanywhere = require('bindings')('sqlanywhere');

const uid = process.env.WMSLNK_UID;
const pwd = process.env.WMSLNK_PWD;
const eng = process.env.WMSLNK_ENG;
const ip = process.env.WMSLNK_IP;
const port = process.env.WMSLNK_PORT;

console.log(uid, pwd, eng, ip, port);

function initialize(): sqlanywhere.Connection {
	sa.init();
	const cn = sa.new_connection();
	if (!sa.connect(cn, `uid=${uid};pwd=${pwd};eng=${eng};links=tcpip(ip=${ip};port=${port})`)) {
		console.error("Couldn't connect to the database.");
		console.error(sa.error(cn));
	}
	return cn;
}

function getColumns(stmt: sqlanywhere.Statement): ColumnInfo[] {
	const numColumns = sa.num_cols(stmt);
	const columnInfos = [];
	for (let columnIndex = 0; columnIndex < numColumns; columnIndex++) {
		columnInfos.push(new ColumnInfo(sa.get_column_info(stmt, columnIndex)));
	}
	return columnInfos;
}

function getRows(stmt: sqlanywhere.Statement): any[] {
	const rows = [];
	while (sa.fetch_next(stmt)) {
		const row = [];
		for (let columnIndex = 0; columnIndex < sa.num_cols(stmt); columnIndex++) {
			const data = sa.get_column(stmt, columnIndex);
			row.push(data.buffer);
		}
		rows.push(row);
	}
	return rows;
}

function mergeRowsWithColumns(rows: any[], columns: ColumnInfo[]): any[] {
	const mergedRows: any = [];
	for (let rowIndex = 0; rowIndex < rows.length; rowIndex++) {
		const row = rows[rowIndex];
		const mergedRow: any = {};
		for (let columnIndex = 0; columnIndex < columns.length; columnIndex++) {
			const column = columns[columnIndex];
			const data = row[columnIndex];
			mergedRow[column.name] = data;
		}
		mergedRows.push(mergedRow);
	}
	return mergedRows;
}

function describeColumns(columnInfos: ColumnInfo[]) {
	console.info('# columns:', columnInfos.length);
	console.table(
		columnInfos.map((x) => ({
			name: x.name,
			nativeType: `${x.nativeTypeName} (${x.nativeType})`,
			type: `${x.typeName} (${x.type})`,
		}))
	);
}

const cn: sqlanywhere.Connection = initialize();
if (cn) {
	try {
		const stmt = sa.execute_direct(cn, 'select top 3 prn_name, prn_desc, prn_ip, prn_port from erp.printers');
		if (stmt) {
			try {
				if (sa.num_cols(stmt) > 0) {
					const columns = getColumns(stmt);
					describeColumns(columns);

					const rows = getRows(stmt);
					const mergedRows = mergeRowsWithColumns(rows, columns);
					console.info(`# rows: ${mergedRows.length}`);
					console.table(mergedRows);
					console.log('END RESULTS');
				} else {
					console.log('No results.');
				}
			} finally {
				sa.free_stmt(stmt);
			}
		}
	} catch (err) {
		console.error(err);
	} finally {
		if (!sa.disconnect(cn)) {
			console.error("Couldn't disconnect from the database.");
			console.error(sa.error(cn));
		}
		sa.free_connection(cn);
		sa.fini();
	}
}
