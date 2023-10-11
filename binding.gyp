{
	'targets': [
		{ 
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [ '-fno-exceptions' ],
			'include_dirs' : [
				'<!@(node -p "require(\'node-addon-api\').include")'
			],
			'target_name': 'sqlanywhere',
			'sources': [ 'src/main.cpp', 'src/MyObject.cpp', 'src/sacapidll.c' ],
			'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
		}
	]
}
