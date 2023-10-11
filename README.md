# sqlanywhere

My DIY Node.js module for connecting to a SQL Anywhere database.

You will need TypeScript installed globally: `npm install -g typescript`

Build the extension with `npm install`.
Run the tests with `npm run test`.
Run the example with `npm run dev`.

You will need to have the SQL Anywhere 17 API installed here: C:\Program Files\SQL Anywhere 17\SDK

## Tech Stack

- [Jest](https://jestjs.io/)
- [N-API](https://nodejs.org/api/n-api.html)
- [SQL Anywhere C API](https://infocenter.sybase.com/help/index.jsp?topic=/com.sybase.help.sqlanywhere.12.0.1/dbprogramming/programming-sacpp-sacapi-h-fil-sqlany-connect-met.html)

## TODO

- Use the [wrap](https://nodejs.org/api/n-api.html#napi_wrap) and [unwrap](https://nodejs.org/api/n-api.html#napi_unwrap) functions on all of my pointers. Implement the cleanup abilities on object wrapping.

## References

- [Node Add-on Examples](https://github.com/nodejs/abi-stable-node-addon-examples)
