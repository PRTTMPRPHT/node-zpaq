import {CompressionOptions} from "./interfaces";

const nativeBindings = require("bindings")("node-zpaq");

/**
 * Decompresses a buffer of zpaq-compressed bytes asynchronously.
 * @param inputBuffer The buffer to decompress.
 */
export function decompress(inputBuffer: Buffer): Promise<Buffer> {
    if (!inputBuffer || !(inputBuffer instanceof Buffer)) {
        throw new Error("Need a buffer to decompress.");
    }

    return new Promise((resolve, reject) => {
        nativeBindings.decompressAsync(inputBuffer, (err: any, result: Buffer) => {
            if (err) {
                reject(err);
                return;
            }

            resolve(result);
        });
    });
}

/**
 * Decompresses a buffer of zpaq-compressed bytes synchronously.
 * @param inputBuffer The buffer to decompress.
 */
export function decompressSync(inputBuffer: Buffer): Buffer {
    if (!inputBuffer || !(inputBuffer instanceof Buffer)) {
        throw new Error("Need a buffer to decompress.");
    }

    return nativeBindings.decompressSync(inputBuffer);
}

/**
 * Validates that the compression options passed are typed correctly.
 * For more information on how to use these options, please see libzpaq.h.
 * @param compressionOptions The options provided by the consumer.
 */
function validateCompressionOptions(compressionOptions?: CompressionOptions) {
    if (!compressionOptions) {
        return;
    }

    if (typeof compressionOptions !== "object") {
        throw new Error("Type mismatch: Compression options must be an object.");
    }

    if (compressionOptions.method) {
        if (typeof compressionOptions.method !== "string") {
            throw new Error("Type mismatch: Compression method must be a string.");
        }
    }

    if (compressionOptions.fileName) {
        if (typeof compressionOptions.fileName !== "string") {
            throw new Error("Type mismatch: File name must be a string.");
        }
    }

    if (compressionOptions.comment) {
        if (typeof compressionOptions.comment !== "string") {
            throw new Error("Type mismatch: Comment must be a string.");
        }
    }

    if (compressionOptions.sha1 != null) {
        if (typeof compressionOptions.sha1 !== "boolean") {
            throw new Error("Type mismatch: sha1 must be a boolean.");
        }
    }
}

/**
 * Compresses a buffer of bytes with libzpaq synchronously.
 * For the options, please see libzpaq.h.
 * @param inputBuffer The buffer to compress.
 * @param compressionOptions Optional parameters for libzpaq.
 */
export function compressSync(inputBuffer: Buffer, compressionOptions?: CompressionOptions): Buffer {
    validateCompressionOptions(compressionOptions);

    return nativeBindings.compressSync(
        inputBuffer,
        compressionOptions?.method,
        compressionOptions?.fileName,
        compressionOptions?.method,
        compressionOptions?.sha1
    );
}

/**
 * Compresses a buffer of bytes with libzpaq asynchronously.
 * For the options, please see libzpaq.h.
 * @param inputBuffer The buffer to compress.
 * @param compressionOptions Optional parameters for libzpaq.
 */
export function compress(inputBuffer: Buffer, compressionOptions?: CompressionOptions): Promise<Buffer> {
    if (!inputBuffer || !(inputBuffer instanceof Buffer)) {
        throw new Error("Need a buffer to decompress.");
    }

    validateCompressionOptions(compressionOptions);

    return new Promise((resolve, reject) => {
        nativeBindings.compressAsync(
            inputBuffer,
            (err: any, result: Buffer) => {
                if (err) {
                    reject(err);
                    return;
                }

                resolve(result);
            },
            compressionOptions?.method,
            compressionOptions?.fileName,
            compressionOptions?.method,
            compressionOptions?.sha1
        );
    });
}
