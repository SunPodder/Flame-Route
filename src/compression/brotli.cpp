#include <brotli/decode.h>
#include <brotli/encode.h>
#include <brotli/types.h>
#include <compression/brotli.hpp>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace Brotli {
std::string compress(std::string& data, BrotliEncoderMode mode, int quality,
					 int window) {
	const size_t input_size = data.size();
	const size_t buffer_size = BrotliEncoderMaxCompressedSize(input_size);
	std::string compressed_data(buffer_size, '\0');

	size_t output_size = buffer_size;

	if (BrotliEncoderCompress(
			quality, window, mode, input_size,
			reinterpret_cast<const uint8_t*>(data.data()), &output_size,
			reinterpret_cast<uint8_t*>(&compressed_data[0])) == BROTLI_FALSE) {
		return "";
	}
	compressed_data.resize(output_size);
	return compressed_data;
}

std::vector<uint8_t> compress(std::vector<uint8_t>& data, int quality,
							  int window) {
	std::vector<uint8_t> compressed_data;
	compressed_data.resize(BrotliEncoderMaxCompressedSize(data.size()));
	size_t encoded_size = compressed_data.size();
	BrotliEncoderCompress(quality, window, BROTLI_MODE_GENERIC, data.size(),
						  data.data(), &encoded_size, compressed_data.data());
	compressed_data.resize(encoded_size);
	return compressed_data;
}

std::string decompress(std::string& data) {
	const size_t kOutputBufferSize = 4096;
	BrotliDecoderState* state =
		BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);
	if (state == nullptr) {
		return "";
	}

	std::string decompressed_data;
	std::vector<uint8_t> output_buffer(kOutputBufferSize);

	size_t available_in = data.size();
	const uint8_t* next_in = reinterpret_cast<const uint8_t*>(data.data());

	while (available_in > 0) {
		size_t available_out = output_buffer.size();
		uint8_t* next_out = output_buffer.data();

		BrotliDecoderResult result = BrotliDecoderDecompressStream(
			state, &available_in, &next_in, &available_out, &next_out, nullptr);

		if (result == BROTLI_DECODER_RESULT_ERROR) {
			BrotliDecoderDestroyInstance(state);
			return "";
		}

		size_t decompressed_size = output_buffer.size() - available_out;
		decompressed_data.append(
			reinterpret_cast<const char*>(output_buffer.data()),
			decompressed_size);

		if (result == BROTLI_DECODER_RESULT_SUCCESS) {
			break;
		}

		if (available_out == 0) {
			output_buffer.resize(output_buffer.size() * 2);
		}
	}
	BrotliDecoderDestroyInstance(state);
	return decompressed_data;
}

} // namespace Brotli
