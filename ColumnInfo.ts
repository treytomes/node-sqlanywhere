import type sqlanywhere from './types/sqlanywhere';
import a_sqlany_native_type from './a_sqlany_native_type';
import a_sqlany_data_type from './a_sqlany_data_type';

class ColumnInfo implements sqlanywhere.IColumnInfo {
	public maxSize: number;
	public name: string;
	public nativeType: a_sqlany_native_type;
	public nullable: boolean;
	public precision: number;
	public scale: number;
	public type: a_sqlany_data_type;

	constructor(info: sqlanywhere.IColumnInfo) {
		this.maxSize = info.maxSize;
		this.name = info.name;
		this.nativeType = info.nativeType;
		this.nullable = info.nullable;
		this.precision = info.precision;
		this.scale = info.scale;
		this.type = info.type;
	}

	public get nativeTypeName(): string {
		return a_sqlany_native_type[this.nativeType];
	}
	public get typeName(): string {
		return a_sqlany_data_type[this.type];
	}
}

export default ColumnInfo;
