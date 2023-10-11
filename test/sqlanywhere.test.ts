// eslint-disable-next-line import/no-unresolved
import sqlanywhere from '../types/sqlanywhere';

// eslint-disable-next-line @typescript-eslint/no-var-requires
const sa: sqlanywhere = require('bindings')('sqlanywhere');

beforeAll(() => {
	expect(sa).not.toBeUndefined();
	expect(sa.init()).toBeTruthy();
});

afterAll(() => {
	sa.fini();
});

describe('Generic C++', function () {
	test('Native functions.', () => {
		expect(sa.hello()).toEqual('world');
		expect(sa.add(7, 5)).toEqual(12);
	});

	test('Native objects.', function () {
		const obj = new sa.MyObject(10);
		expect(obj).not.toBeNull();
		expect(obj.value).toEqual(10);

		expect(obj.plusOne()).toEqual(11);
		expect(obj.plusOne()).toEqual(12);
		expect(obj.plusOne()).toEqual(13);

		expect(obj.multiply().value).toEqual(13);
		expect(obj.multiply(10).value).toEqual(130);

		const newobj = obj.multiply(-1);
		expect(newobj).not.toBeNull();
		expect(newobj.value).toEqual(-13);

		expect(obj).not.toBe(newobj);
	});
});

const uid = process.env.WMSLNK_UID;
const pwd = process.env.WMSLNK_PWD;

describe('Connection', function () {
	let cn: sqlanywhere.Connection;

	beforeEach(() => {
		cn = sa.new_connection();
	});

	afterEach(() => {
		sa.free_connection(cn);
	});

	test('Can connect to the database.', function () {
		const eng = process.env.WMSLNK_ENG;
		const ip = process.env.WMSLNK_IP;
		const port = process.env.WMSLNK_PORT;

		expect(sa.connect(cn, `uid=${uid};pwd=${pwd};eng=${eng};links=tcpip(ip=${ip};port=${port})`)).toBeTruthy();
		expect(sa.disconnect(cn)).toBeTruthy();
	});

	test('Can read the last SQL error.', function () {
		const eng = process.env.WMSLNK_ENG;
		const ip = '12.13.14.15'; // <-- This port is intentionally wrong.
		const port = process.env.WMSLNK_PORT;

		expect(sa.connect(cn, `uid=${uid};pwd=${pwd};eng=${eng};links=tcpip(ip=${ip};port=${port})`)).toBeFalsy();

		const err = sa.error(cn);
		expect(err.code).toEqual(-100);
		expect(err.reason).toEqual('Database server not found');
	});
});

describe('Query', function () {
	const eng = process.env.WMSLNK_ENG;
	const ip = process.env.WMSLNK_IP;
	const port = process.env.WMSLNK_PORT;

	let cn: sqlanywhere.Connection;

	beforeEach(() => {
		cn = sa.new_connection();
		expect(sa.connect(cn, `uid=${uid};pwd=${pwd};eng=${eng};links=tcpip(ip=${ip};port=${port})`)).toBeTruthy();
	});

	afterEach(() => {
		expect(sa.disconnect(cn)).toBeTruthy();
		sa.free_connection(cn);
	});

	test('Can execute a direct query.', function () {
		const expectedRowCount = 10;
		const pic_loc = 782;
		const pic_state = 'cancel';

		let stmt: sqlanywhere.Statement;
		try {
			stmt = sa.execute_direct(cn, `select top ${expectedRowCount} pic_loc, pic_number, pic_state, pic_order, pic_shiptype from erp.pickticket where pic_loc=${pic_loc} and pic_state='${pic_state}'`);
		} catch (err) {
			throw [err, sa.error(cn)];
		}
		stmt = stmt!;

		expect(sa.num_cols(stmt!)).toEqual(5);
		expect(sa.get_column_info(stmt, 0).name).toEqual('pic_loc');
		expect(sa.get_column_info(stmt, 1).name).toEqual('pic_number');
		expect(sa.get_column_info(stmt, 2).name).toEqual('pic_state');
		expect(sa.get_column_info(stmt, 3).name).toEqual('pic_order');
		expect(sa.get_column_info(stmt, 4).name).toEqual('pic_shiptype');

		let actualRowCount = 0;
		while (sa.fetch_next(stmt)) {
			expect(sa.get_column(stmt, 0).buffer).toEqual(pic_loc);
			expect(sa.get_column(stmt, 2).buffer).toEqual(pic_state);
			actualRowCount++;
		}
		expect(actualRowCount).toEqual(expectedRowCount);

		sa.free_stmt(stmt);
	});
});
