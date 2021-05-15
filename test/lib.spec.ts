import {compress, compressSync, decompressSync, decompress} from "../src";

// A "compressed" file containing only the string "Hello World".
// This can be verified by writing it out to the disk and using
// the zpaq binary to extract it.

const HELLO_WORLD_PAYLOAD = [
    55, 107, 83, 116, 160, 49, 131, 211, 140, 178, 40, 176, 211, 122, 80, 81, 2, 1, 14, 0, 9, 16, 0, 20, 0, 0, 18, 104,
    135, 255, 88, 114, 56, 0, 1, 0, 49, 50, 32, 0, 0, 0, 0, 1, 63, 1, 46, 1, 239, 255, 47, 13, 4, 12, 20, 28, 55, 1, 55,
    2, 55, 3, 55, 4, 56, 203, 130, 80, 71, 8, 131, 88, 7, 1, 223, 0, 47, 51, 71, 1, 55, 2, 66, 175, 3, 239, 0, 47, 30,
    2, 207, 3, 55, 3, 66, 215, 2, 80, 15, 3, 175, 7, 129, 55, 3, 66, 215, 3, 80, 67, 143, 5, 88, 71, 1, 55, 1, 63, 10,
    66, 215, 2, 80, 26, 26, 71, 3, 55, 1, 7, 1, 223, 1, 47, 61, 67, 239, 2, 47, 56, 66, 175, 1, 223, 1, 47, 21, 66, 215,
    1, 80, 15, 2, 66, 175, 1, 129, 129, 55, 2, 66, 215, 1, 80, 26, 26, 63, 26, 66, 215, 1, 80, 7, 2, 207, 2, 72, 66,
    175, 3, 129, 55, 2, 66, 215, 2, 80, 26, 26, 26, 71, 2, 55, 1, 63, 189, 7, 1, 223, 2, 47, 57, 7, 3, 235, 39, 52, 66,
    55, 6, 67, 55, 7, 15, 3, 71, 1, 201, 88, 2, 170, 131, 88, 15, 4, 65, 139, 80, 31, 2, 67, 239, 0, 47, 8, 26, 69, 96,
    17, 9, 57, 63, 243, 65, 55, 4, 7, 6, 15, 3, 209, 80, 7, 7, 137, 88, 4, 55, 1, 7, 1, 223, 3, 47, 43, 67, 239, 1, 47,
    38, 66, 175, 1, 223, 1, 47, 20, 66, 215, 1, 80, 15, 2, 175, 1, 129, 129, 55, 2, 66, 215, 1, 80, 26, 26, 63, 9, 66,
    215, 1, 80, 26, 71, 4, 55, 1, 63, 207, 7, 1, 223, 4, 47, 34, 67, 239, 7, 47, 29, 15, 4, 66, 96, 57, 9, 65, 55, 4,
    66, 215, 8, 80, 67, 143, 8, 88, 7, 2, 2, 55, 2, 223, 0, 47, 3, 4, 55, 1, 56, 0, 92, 144, 202, 216, 216, 222, 64,
    174, 222, 228, 216, 200, 66, 0, 0, 0, 0, 0, 253, 46, 247, 189, 230, 8, 206, 84, 4, 233, 125, 95, 4, 47, 149, 248,
    159, 28, 35, 40, 113, 255
];

describe("node-zpaq", () => {
    it("should compress a buffer synchronously", () => {
        const sourceBuffer = Buffer.from("Hello World!");
        const compressedBuf = compressSync(sourceBuffer);
        const compressedArr = Array.from(compressedBuf);
        expect(compressedArr).toEqual(HELLO_WORLD_PAYLOAD);
    });

    it("should compress a buffer asynchronously", async () => {
        const sourceBuffer = Buffer.from("Hello World!");
        const compressedBuf = await compress(sourceBuffer);
        const compressedArr = Array.from(compressedBuf);
        expect(compressedArr).toEqual(HELLO_WORLD_PAYLOAD);
    });

    it("should decompress a buffer synchronously", () => {
        const decompressedBuf = decompressSync(Buffer.from(HELLO_WORLD_PAYLOAD));
        const decompressedStr = decompressedBuf.toString("utf-8");
        expect(decompressedStr).toEqual("Hello World!");
    });

    it("should decompress a buffer asynchronously", async () => {
        const decompressedBuf = await decompress(Buffer.from(HELLO_WORLD_PAYLOAD));
        const decompressedStr = decompressedBuf.toString("utf-8");
        expect(decompressedStr).toEqual("Hello World!");
    });

    it("should compress and decompress a buffer synchronously", () => {
        const sourceBuffer = Buffer.from("So long, and thanks for all the bytes!");
        const compressedBuf = compressSync(sourceBuffer);

        const decompressedBuf = decompressSync(compressedBuf);
        const decompressedStr = decompressedBuf.toString("utf-8");
        expect(decompressedStr).toEqual("So long, and thanks for all the bytes!");
    });

    it("should compress and decompress a buffer asynchronously", async () => {
        const sourceBuffer = Buffer.from("So long, and thanks for all the bytes!");
        const compressedBuf = await compress(sourceBuffer);

        const decompressedBuf = await decompress(compressedBuf);
        const decompressedStr = decompressedBuf.toString("utf-8");
        expect(decompressedStr).toEqual("So long, and thanks for all the bytes!");
    });

    it("should throw an error when compressing synchronously without a buffer", () => {
        expect(() => compressSync(null)).toThrow();
    });

    it("should throw an error when compressing asynchronously without a buffer", () => {
        expect(async() => await compress(null)).rejects.toThrow();
    });

    it("should throw an error when decompressing synchronously without a buffer", () => {
        expect(() => decompressSync(null)).toThrow();
    });

    it("should throw an error when decompressing asynchronously without a buffer", () => {
        expect(async() => await decompress(null)).rejects.toThrow();
    });

    it("should throw an error when compress options are of the wrong type", () => {
        expect(() => compressSync(Buffer.from("Hello World!"), 42 as any)).toThrow();
    });

    it("should throw an error when method is of the wrong type", () => {
        expect(() => compressSync(Buffer.from("Hello World!"), {method: 42 as any})).toThrow();
    });

    it("should throw an error when fileName is of the wrong type", () => {
        expect(() => compressSync(Buffer.from("Hello World!"), {fileName: 42 as any})).toThrow();
    });

    it("should throw an error when comment is of the wrong type", () => {
        expect(() => compressSync(Buffer.from("Hello World!"), {comment: 42 as any})).toThrow();
    });

    it("should throw an error when sha1 is of the wrong type", () => {
        expect(() => compressSync(Buffer.from("Hello World!"), {sha1: 42 as any})).toThrow();
    });

    it("should not throw an error when compression options are of the right type", () => {
        expect(() => compressSync(Buffer.from("Hello World!"), {
            method: "14,128,0",
            comment: "Some comment.",
            fileName: "TestFile.txt",
            sha1: true
        })).not.toThrow();
    });
});
