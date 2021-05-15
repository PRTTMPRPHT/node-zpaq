#pragma once

#include <string>
#include <napi.h>

namespace NodeZpaqUtils {
	struct CompressionOptions {
		std::string method;
		std::string fileName;
		std::string comment;
		bool sha1;
	};

	// Helper function to ensure any optional argument is a string, otherwise return a default value.
	// If there is a type mismatch, this function will throw a C++ exception, which we need to catch!

	std::string extractOptionalString(const Napi::CallbackInfo& info, const size_t position, const std::string defaultValue);

	// Helper function to ensure any optional argument is a boolean, otherwise return a default value.
	// If there is a type mismatch, this function will throw a C++ exception, which we need to catch!
	
	bool extractOptionalBoolean(const Napi::CallbackInfo& info, const size_t position, const bool defaultValue);

	// Helper function to extract the options for compression from a node call.
	
	CompressionOptions extractCompressionOptions(const Napi::CallbackInfo& info, int offset);

	// Helper function to throw an error back to node.

	Napi::Value throwError(const Napi::Env& env, const std::string errorText);
}