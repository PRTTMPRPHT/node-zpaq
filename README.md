# node-zpaq

[![Build Status](https://www.travis-ci.com/PRTTMPRPHT/node-zpaq.svg?branch=master)]()

Node bindings for the compression and decompression functions of libzpaq, written by Matt Mahoney (http://mattmahoney.net/dc/zpaq.html).

## Usage

### Decompression

These methods handle the decompression of any given zpaq-compressed buffer. Note that this will only decompress the contents, not handle decryption or processing archive structure.

#### Async (Promise)
```
const {decompress} = require("node-zpaq");

async function fn(buffer: Buffer) {
    const result = await decompress(buffer);
    // ...
} 
```
#### Sync
```
const {decompressSync} = require("node-zpaq");

function fn(buffer: Buffer) {
    const result = decompressSync(buffer);
    // ...
} 
```

### Compression

These methods handle the compression of any given buffer. For a detailed documentation of the compression options, please refer to [libzpaq.h](https://github.com/zpaq/zpaq/blob/master/libzpaq.h).

#### Async (Promise)
```
const {compress} = require("node-zpaq");

const OPTIONS = {
    method: "14,128,0",
    comment: "Some comment",
    fileName: "MyFile.hex",
    sha1: true,
};

async function fn(buffer: Buffer) {
    const result = await compress(buffer, OPTIONS);
    // ...
} 
```
#### Sync
```
const {compressSync} = require("node-zpaq");

const OPTIONS = {
    method: "14,128,0",
    comment: "Some comment",
    fileName: "MyFile.hex",
    sha1: true,
};

function fn(buffer: Buffer) {
    const result = compressSync(buffer, OPTIONS);
    // ...
} 
```

## Installing

`npm install node-zpaq`

## License

node-zpaq is released under the [Unlicense](http://unlicense.org/), which applies to all files except the files included in `/native/zpaq/`.

For these files, the original notice applies:

```
zpaq is written by Matt Mahoney and released to the public domain.
It includes code from libdivsufsort 2.0 (C) Yuta Mori, 2003-2008, MIT license,
public domain code for AES from libtomcrypt by Tom St Denis
and public domain code for salsa20 by D. J. Bernstein.
```

By contributing, you agree that your contributions will be licensed under the project's [Unlicense](http://unlicense.org/).
