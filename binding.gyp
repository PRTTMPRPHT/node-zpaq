{
	"targets": [{
		"cflags!": ["-fno-exceptions"],
		"cflags_cc!": ["-fno-exceptions"],
		"include_dirs": [
			"<!@(node -p \"require('node-addon-api').include\")",
			"native/zpaq"
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
