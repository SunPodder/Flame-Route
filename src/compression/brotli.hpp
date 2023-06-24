#ifndef BROTLI_HPP
#define BROTLI_HPP

#include <brotli/encode.h>
#include <brotli/decode.h>
#include <brotli/types.h>
#include <cstdint>
#include <string>
#include <vector>

namespace Brotli {

/*
 * Compresses the given data using the Brotli algorithm.
 * @param data The data to compress.
 * @param quality The quality of the compression. 0 = fast, 11 = best.
 * @param window The window size of the compression. 0 = default, 10 = max.
 * @return The compressed data.
 */
std::string compress(std::string& data, BrotliEncoderMode mode = BROTLI_MODE_GENERIC,int quality = BROTLI_DEFAULT_QUALITY, int window = BROTLI_DEFAULT_WINDOW);

std::vector<uint8_t> compress(std::vector<uint8_t>& data, int quality = 11, int window = 0);

/*
 * Decompresses the given data using the Brotli algorithm.
 * @param data The data to decompress.
 * @return The decompressed data.
 */
std::string decompress(std::string& data);

std::vector<uint8_t> decompress(std::vector<uint8_t>& data);
}

#endif
