// Include default function provided by zpaq, so we do not have to include zpaq.cpp...

#include <stdexcept>
#include <cstring>
#include <new>

namespace libzpaq {
	void error(const char* msg) {
		if (strstr(msg, "ut of memory")) throw std::bad_alloc();
		throw std::runtime_error(msg);
	}
}

// Actual node bindings.

#include "utils.h"
#include <libzpaq.h>
#include <memory>

namespace NodeZpaq {

	// This executes the compression asynchronously for compressAsync().

	class AsyncCompressor : public Napi::AsyncWorker {
	public:
		AsyncCompressor(Napi::Function& callback, std::unique_ptr<char[]> data, size_t length, NodeZpaqUtils::CompressionOptions options) : AsyncWorker(callback), data(move(data)), length(length), options(options) {}
		~AsyncCompressor() {};
		AsyncCompressor(const AsyncCompressor&) = delete;
		AsyncCompressor& operator=(const AsyncCompressor&) = delete;

		void Execute() override {
			libzpaq::StringBuffer inBuf(this->length);
			inBuf.write(&this->data[0], this->length);
			libzpaq::compress(&inBuf, &outBuf, this->options.method.c_str(), this->options.fileName.c_str(), this->options.comment.c_str(), this->options.sha1);
		}

		void OnOK() override {
			// Data will be compressed in outBuf, so we copy it over to a node buffer now.
			unsigned char* compressedData = this->outBuf.data();
			const size_t compressedLength = this->outBuf.size();
			Napi::Buffer<unsigned char> resultBuffer = Napi::Buffer<unsigned char>::Copy(Env(), compressedData, compressedLength);

			// Return null as first argument, as that is the error argument, provided by the default OnError() implementation.
			Callback().Call({ Env().Null(), resultBuffer });
		}
	private:
		std::unique_ptr<char[]> data;
		size_t length;
		NodeZpaqUtils::CompressionOptions options;
		libzpaq::StringBuffer outBuf;
	};

	// This executes the decompression asynchronously for decompressAsync().

	class AsyncDecompressor : public Napi::AsyncWorker {
	public:
		AsyncDecompressor(Napi::Function& callback, std::unique_ptr<char[]> data, size_t length): AsyncWorker(callback), data(move(data)), length(length) {}
		~AsyncDecompressor() {};
		AsyncDecompressor(const AsyncDecompressor&) = delete;
		AsyncDecompressor& operator=(const AsyncDecompressor&) = delete;

		void Execute() override {
			libzpaq::StringBuffer inBuf(this->length);
			inBuf.write(&this->data[0], this->length);
			libzpaq::decompress(&inBuf, &this->outBuf);
		}

		void OnOK() override {
			// Data will be uncompressed in outBuf, so we copy it over to a node buffer now.
			unsigned char* decompressedData = this->outBuf.data();
			const size_t decompressedLength = this->outBuf.size();
			Napi::Buffer<unsigned char> resultBuffer = Napi::Buffer<unsigned char>::Copy(Env(), decompressedData, decompressedLength);
			
			// Return null as first argument, as that is the error argument, provided by the default OnError() implementation.
			Callback().Call({ Env().Null(), resultBuffer });
		}
	private:
		std::unique_ptr<char[]> data;
		size_t length;
		libzpaq::StringBuffer outBuf;
	};

	// Asynchronous binding for the compress() function.
	// We take in a buffer of uncompressed bytes and likewise, return a buffer of the zpaq-compressed bytes.
	// Call signature: compressAsync(buffer: Buffer, callback: Function, method?: string, fileName?: string, comment?: string, sha1?: boolean)

	Napi::Value compressAsync(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		try {
			// First argument must be a buffer and must be present.
			// Second argument must be a callback and must be present.

			if (info.Length() < 2 || !info[0].IsBuffer() || !info[1].IsFunction()) {
				return NodeZpaqUtils::throwError(env, "compressAsync: Need both a buffer and a callback.");
			}

			Napi::Buffer<char> sourceBuffer = info[0].As<Napi::Buffer<char>>();
			Napi::Function callback = info[1].As<Napi::Function>();

			// Extract optional arguments.

			NodeZpaqUtils::CompressionOptions opts = NodeZpaqUtils::extractCompressionOptions(info, 1);

			char* sourceData = sourceBuffer.Data();
			const size_t sourceLength = sourceBuffer.Length();

			std::unique_ptr<char[]> data(new char[sourceLength]);

			memcpy(&data[0], sourceData, sourceLength);

			AsyncCompressor* compressor = new AsyncCompressor(callback, move(data), sourceLength, opts);
			compressor->Queue();

			return env.Undefined();
		}
		catch (std::exception& e) {
			return NodeZpaqUtils::throwError(env, e.what());
		}
	}

	// Synchronous binding for the compress() function.
	// We take in a buffer of uncompressed bytes and likewise, return a buffer of the zpaq-compressed bytes.
	// Call signature: compressSync(buffer: Buffer, method?: string, fileName?: string, comment?: string, sha1?: boolean): Buffer

	Napi::Value compressSync(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		try {
			// First argument must be a buffer and must be present.

			if (info.Length() < 1 || !info[0].IsBuffer()) {
				return NodeZpaqUtils::throwError(env, "compressSync: Need a buffer as first argument.");
			}

			Napi::Buffer<char> sourceBuffer = info[0].As<Napi::Buffer<char>>();

			// Extract optional arguments.

			NodeZpaqUtils::CompressionOptions opts = NodeZpaqUtils::extractCompressionOptions(info, 0);
			
			const size_t sourceLength = sourceBuffer.Length();

			libzpaq::StringBuffer inBuf(sourceLength);
			libzpaq::StringBuffer outBuf;

			char* sourceData = sourceBuffer.Data();

			inBuf.write(sourceData, sourceLength);

			libzpaq::compress(&inBuf, &outBuf, opts.method.c_str(), opts.fileName.c_str(), opts.comment.c_str(), opts.sha1);

			unsigned char* compressedData = outBuf.data();
			const size_t compressedLength = outBuf.size();

			return Napi::Buffer<unsigned char>::Copy(env, compressedData, compressedLength);
		} catch (std::exception& e) {
			return NodeZpaqUtils::throwError(env, e.what());
		}
	}

	// Asynchronous binding for the libzpaq::decompress() function.
	// We take in a buffer of zpaq-compressed bytes and likewise, return a buffer of the decompressed bytes.
	// Call signature: decompressAsync(buffer: Buffer, callback: Function)

	Napi::Value decompressAsync(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		try {
			// First argument must be a buffer and must be present.
			// Second argument must be a callback and must be present.

			if (info.Length() < 2 || !info[0].IsBuffer() || !info[1].IsFunction()) {
				return NodeZpaqUtils::throwError(env, "decompressAsync: Need both a buffer and a callback.");
			}

			Napi::Buffer<char> sourceBuffer = info[0].As<Napi::Buffer<char>>();
			Napi::Function callback = info[1].As<Napi::Function>();

			char* sourceData = sourceBuffer.Data();
			const size_t sourceLength = sourceBuffer.Length();

			std::unique_ptr<char[]> data(new char[sourceLength]);
			
			memcpy(&data[0], sourceData, sourceLength);

			AsyncDecompressor* decompressor = new AsyncDecompressor(callback, move(data), sourceLength);
			decompressor->Queue();

			return env.Undefined();
		} catch (std::exception& e) {
			return NodeZpaqUtils::throwError(env, e.what());
		}
	}

	// Synchronous binding for the libzpaq::decompress() function.
	// We take in a buffer of zpaq-compressed bytes and likewise, return a buffer of the decompressed bytes.
	// Call signature: decompressSync(buffer: Buffer): Buffer

	Napi::Value decompressSync(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		try {
			// First argument must be a buffer and must be present.

			if (info.Length() != 1 || !info[0].IsBuffer()) {
				return NodeZpaqUtils::throwError(env, "decompressSync: Need a buffer as first argument.");
			}

			Napi::Buffer<char> sourceBuffer = info[0].As<Napi::Buffer<char>>();

			const size_t sourceLength = sourceBuffer.Length();

			libzpaq::StringBuffer inBuf(sourceLength);
			libzpaq::StringBuffer outBuf;

			char* sourceData = sourceBuffer.Data();

			inBuf.write(sourceData, sourceLength);

			libzpaq::decompress(&inBuf, &outBuf);

			unsigned char* decompressedData = outBuf.data();
			const size_t decompressedLength = outBuf.size();

			return Napi::Buffer<unsigned char>::Copy(env, decompressedData, decompressedLength);
		} catch (std::exception& e) {
			return NodeZpaqUtils::throwError(env, e.what());
		}
	}

	// Module initialization and function registration.

	Napi::Object init(Napi::Env env, Napi::Object exports) {
		exports.Set(
			Napi::String::New(env, "decompressSync"),
			Napi::Function::New(env, decompressSync)
		);

		exports.Set(
			Napi::String::New(env, "decompressAsync"),
			Napi::Function::New(env, decompressAsync)
		);

		exports.Set(
			Napi::String::New(env, "compressSync"),
			Napi::Function::New(env, compressSync)
		);

		exports.Set(
			Napi::String::New(env, "compressAsync"),
			Napi::Function::New(env, compressAsync)
		);

		return exports;
	}

	NODE_API_MODULE(node_zpaq, init);
}
