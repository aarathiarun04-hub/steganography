# Steganography

Hide a text file inside a BMP image and extract it again — written in C with no
external dependencies. The payload is stored in the **least significant bits**
of the image pixel data, so the picture looks unchanged to the naked eye.

## Build

```sh
make            # builds ./stegno
make clean
```

## Usage

```sh
# Encode: hide secret.txt inside the cover image
./stegno -e replace_with_your_input.bmp secret.txt stego.bmp

# Decode: recover the message from an encoded image
./stegno -d replace_with_your_decoding_input.bmp decode.txt
```

Both output file names are optional — `stego.bmp` and `decode.txt` are used by
default.

## How it works

1. **Validation** — `read_and_validate_encode_args` checks the operation type,
   that the cover image exists and ends in `.bmp`, and that the secret file
   exists.
2. **Capacity** — the image size is read from the BMP header (width at offset
   18, height after it) giving `width * height * 3` bytes of pixel data; the
   secret plus its header must fit in the available bits.
3. **Encoding** — a magic string and the secret file size are written first,
   byte by byte, from the secret file, each input bit replacing the lowest bit
   of an image byte.
4. **Decoding** — the magic string and size are read back out of the image, then
   the payload is reconstructed byte by byte into the output file.

## Sample files

| File | Purpose |
| --- | --- |
| `replace_with_your_input.bmp` | Cover image — **replace it with your own picture** |
| `replace_with_your_decoding_input.bmp` | Already-encoded sample image for testing `-d` — **replace it with your own** |
| `secret.txt` | The message that gets hidden |
| `decode.txt` | The message recovered by decoding |

## Project structure

| File | Purpose |
| --- | --- |
| `common.h` | Shared status/operation enums and helpers |
| `types.h` | `Status` / `OperationType` types |
| `encode.h` | `EncodeInfo` struct and encoder prototypes |
| `decode.h` | `DecodeInfo` struct and decoder prototypes |
| `encode.c` | Capacity check, magic string, payload encoding |
| `decode.c` | Magic string verification and payload decoding |
| `test_encode.c` | Driver: parses `-e` / `-d` and runs the operation |
| `makefile` | Build and clean targets |
