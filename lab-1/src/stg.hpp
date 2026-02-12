#pragma once

#include <string>

namespace stg {

typedef char byte;
typedef unsigned char ubyte;

ubyte *gray_to_bin(const ubyte *src, int x, int y, int k);

int encrypt_message(ubyte *data, int x, int y, int k, const std::string &msg);

std::string decrypt_message(ubyte *data, int x, int y, int k, int length);

} // namespace stg
