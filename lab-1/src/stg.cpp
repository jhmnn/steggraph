#include <stg.hpp>

namespace stg {

static byte get_bit(byte c, int b) { return 0b1 & (c >> b); }

static byte set_bit(byte &c, int b, int x) {
  if (b < 0 || b > 7 || (x != 0 && x != 1)) {
    return 0;
  }

  if (x == 0) {
    c &= ~(0b1 << b);
  } else {
    c |= (0b1 << b);
  }

  return c;
}

static ubyte get_bit(ubyte c, int b) { return 0b1 & (c >> b); }

static ubyte set_bit(ubyte &c, int b, int x) {
  if (b < 0 || b > 7 || (x != 0 && x != 1)) {
    return 0;
  }

  if (x == 0) {
    c &= ~(0b1 << b);
  } else {
    c |= (0b1 << b);
  }

  return c;
}

ubyte *gray_to_bin(const ubyte *src, int x, int y, int k) {
  if (src == nullptr || x < 1 || y < 1 || k < 1 || k > 8) {
    return nullptr;
  }

  ubyte *data = new ubyte[x * y];
  if (data == nullptr) {
    return nullptr;
  }

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      data[i * x + j] = 255 * (0b1 & ((unsigned int)src[i * x + j] >> (k - 1)));
    }
  }

  return data;
}

int encrypt_message(ubyte *data, int x, int y, int k, const std::string &msg) {
  if (data == nullptr || x < 1 || y < 1 || k < 1 || k > 8) {
    return -1;
  }

  k--;

  int bit = 0;
  int byte = 0;

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      int idx = i * x + j;
      if (static_cast<std::size_t>(byte) == msg.length()) {
        goto exit;
      }

      auto &c = data[idx];
      set_bit(c, k, get_bit(msg[byte], bit++));

      if (bit == 8) {
        bit = 0;
        byte++;
      }
    }
  }

exit:
  return byte;
}

std::string decrypt_message(ubyte *data, int x, int y, int k, int length) {
  if (data == nullptr || x < 1 || y < 1 || k < 1 || k > 8 || length < 0) {
    return {};
  }

  k--;

  std::string msg = "w";

  int bit = 0;
  int byte = 0;
  int idx = -1;

  if (length == 0) {
    length = x * y;
  }

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      idx = i * x + j;

      auto &c = data[idx];
      set_bit(msg[byte], bit++, get_bit(c, k));

      if (bit == 8) {
        byte++;
        if (byte == length) {
          goto exit;
        }
        msg += "w";
        bit = 0;
      }
    }
  }

exit:
  return msg;
}

} // namespace stg