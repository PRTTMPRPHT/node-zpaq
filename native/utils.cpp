#include "utils.h"

using namespace std;

string NodeZpaqUtils::extractOptionalString(const Napi::CallbackInfo& info, const size_t position, const string defaultValue)
{
	if (info.Length() < position + 1 || info[position].IsNull() || info[position].IsUndefined()) {
		return defaultValue;
	}

	Napi::Value val = info[position];

	if (!val.IsString()) {
		throw std::runtime_error("Type mismatch: Optional value should be a string.");
	}

	return val.As<Napi::String>().Utf8Value();
}

bool NodeZpaqUtils::extractOptionalBoolean(const Napi::CallbackInfo& info, const size_t position, const bool defaultValue)
{
	if (info.Length() < position + 1 || info[position].IsNull() || info[position].IsUndefined()) {
		return defaultValue;
	}

	Napi::Value val = info[position];

	if (!val.IsBoolean()) {
		throw std::runtime_error("Type mismatch: Optional value should be a boolean.");
	}

	return val.As<Napi::Boolean>().Value();
}

NodeZpaqUtils::CompressionOptions NodeZpaqUtils::extractCompressionOptions(const Napi::CallbackInfo& info, int offset)
{
	NodeZpaqUtils::CompressionOptions options;
	options.method = NodeZpaqUtils::extractOptionalString(info, 1 + offset, "14,128,0");
	options.fileName = NodeZpaqUtils::extractOptionalString(info, 2 + offset, "");
	options.comment = NodeZpaqUtils::extractOptionalString(info, 3 + offset, "");
	options.sha1 = NodeZpaqUtils::extractOptionalBoolean(info, 4 + offset, true);
	return options;
}

Napi::Value NodeZpaqUtils::throwError(const Napi::Env& env, const string errorText)
{
	Napi::Error::New(env, errorText).ThrowAsJavaScriptException();
	return env.Null();
}
