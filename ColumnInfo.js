"use strict";
exports.__esModule = true;
var a_sqlany_native_type_1 = require("./a_sqlany_native_type");
var a_sqlany_data_type_1 = require("./a_sqlany_data_type");
var ColumnInfo = /** @class */ (function () {
    function ColumnInfo(info) {
        this.maxSize = info.maxSize;
        this.name = info.name;
        this.nativeType = info.nativeType;
        this.nullable = info.nullable;
        this.precision = info.precision;
        this.scale = info.scale;
        this.type = info.type;
    }
    Object.defineProperty(ColumnInfo.prototype, "nativeTypeName", {
        get: function () {
            return a_sqlany_native_type_1["default"][this.nativeType];
        },
        enumerable: false,
        configurable: true
    });
    Object.defineProperty(ColumnInfo.prototype, "typeName", {
        get: function () {
            return a_sqlany_data_type_1["default"][this.type];
        },
        enumerable: false,
        configurable: true
    });
    return ColumnInfo;
}());
exports["default"] = ColumnInfo;
