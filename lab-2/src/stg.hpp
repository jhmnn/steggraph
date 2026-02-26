#pragma once

#include <string>

namespace stg {

typedef char byte;
typedef unsigned char ubyte;

ubyte *gray_to_bin(const ubyte *src, int x, int y, int k);

std::size_t lsb_embed(ubyte *container, std::size_t container_size,
                      const ubyte *data, std::size_t data_size,
                      std::size_t key);

std::string lsb_extract(const ubyte *container, std::size_t container_size,
                        std::size_t data_size, std::size_t key);

std::size_t adaptive_embed(ubyte *container, std::size_t width,
                           std::size_t height, const ubyte *data,
                           std::size_t data_size, std::size_t key
                           );

std::string adaptive_extract(const ubyte *container, std::size_t width,
                             std::size_t height, std::size_t data_size,
                             std::size_t key);

double psnr(const ubyte *original, const ubyte *modified, std::size_t size);

} // namespace stg
