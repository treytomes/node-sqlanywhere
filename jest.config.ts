import type { Config } from '@jest/types';
import 'dotenv/config'

const config: Config.InitialOptions = {
	preset: 'ts-jest',
	testEnvironment: 'node',
	verbose: true,
	automock: false,
};

export default config;
