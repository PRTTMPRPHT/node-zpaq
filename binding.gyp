{
	"targets": [{
		"cflags!": ["-fno-exceptions"],
		"cflags_cc!": ["-fno-exceptions"],
		"include_dirs": [
			"<!@(node -p \"require('node-addon-api').include\")",
			"native/zpaq"
		],
    "conditions": [
      ['OS=="mac"', {
        "defines": [
          "unix"
        ],
        "xcode_settings": {
          "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        }
      }
      ]
    ],
		"target_name": "node-zpaq",
		"sources": [
			"native/node-zpaq.cpp",
			"native/utils.cpp",
			"native/zpaq/libzpaq.cpp"
		],
		"defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
	}]
}
