#include <stg.hpp>

#include <algorithm>
#include <random>
#include <vector>

namespace stg {

static byte get_bit(byte c, int b);
static byte set_bit(byte &c, int b, int x);
static ubyte get_bit(ubyte c, int b);
static ubyte set_bit(ubyte &c, int b, int x);

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

std::size_t lsb_embed(ubyte *container, std::size_t container_size,
                      const ubyte *data, std::size_t data_size,
                      std::size_t key) {
  if (container == nullptr || data == nullptr) {
    return 0;
  }

  std::vector<std::size_t> indices(container_size);
  for (std::size_t i = 0; i < container_size; i++) {
    indices[i] = i;
  }

  std::mt19937 gen(key);
  std::shuffle(indices.begin(), indices.end(), gen);

  std::size_t bit = 0;
  std::size_t byte = 0;

  for (std::size_t i = 0; i < container_size; i++) {
    byte = bit / 8;
    if (byte == data_size) {
      goto exit;
    }

    auto &c = container[indices[i]];
    set_bit(c, 0, get_bit(data[byte], bit % 8));
    bit++;
  }

exit:
  return bit / 8;
}

std::string lsb_extract(const ubyte *container, std::size_t container_size,
                        std::size_t data_size, std::size_t key) {
  if (container == nullptr) {
    return {};
  }

  if (data_size == 0) {
    data_size = container_size;
  }

  std::vector<std::size_t> indices(container_size);
  for (std::size_t i = 0; i < container_size; i++) {
    indices[i] = i;
  }

  std::mt19937 gen(key);
  std::shuffle(indices.begin(), indices.end(), gen);

  std::string msg;

  std::size_t bit = 0;
  std::size_t byte = 0;

  for (std::size_t i = 0; i < indices.size(); i++) {
    byte = bit / 8;
    if (byte == data_size) {
      goto exit;
    }

    if (bit % 8 == 0) {
      msg += "w";
    }

    auto &c = container[indices[i]];
    set_bit(msg[byte], bit % 8, get_bit(c, 0));
    bit++;
  }

exit:
  return msg;
}

std::size_t adaptive_embed(ubyte *container,
                               std::size_t width,
                               std::size_t height,
                               const ubyte *data,
                               std::size_t data_size,
                               std::size_t key)
{
  if (!container || !data)
    return 0;

  const std::size_t block = 8;

  struct Block {
    std::size_t index;   // индекс первого пикселя блока
    int gradient;
  };

  std::vector<Block> blocks;

  // 1️⃣ считаем градиент каждого блока
  for (std::size_t bx = 0; bx + block < height; bx += block) {
    for (std::size_t by = 0; by + block < width; by += block) {

      int sum = 0;

      for (std::size_t x = 0; x < block - 1; x++) {
        for (std::size_t y = 0; y < block - 1; y++) {

          std::size_t pos = (bx + x) * width + (by + y);

          sum += std::abs(container[pos] -
                          container[pos + 1]);

          sum += std::abs(container[pos] -
                          container[pos + width]);
        }
      }

      blocks.push_back({bx * width + by, sum});
    }
  }

  // 2️⃣ сортируем блоки по убыванию текстурности
  std::sort(blocks.begin(), blocks.end(),
            [](const Block &a, const Block &b) {
              return a.gradient > b.gradient;
            });

  std::mt19937 gen(key);
  std::shuffle(blocks.begin(), blocks.end(), gen);

  std::size_t bit = 0;

  // 3️⃣ встраиваем по блокам
  for (const auto &blockInfo : blocks) {

    std::size_t bx = blockInfo.index / width;
    std::size_t by = blockInfo.index % width;

    for (std::size_t x = 0; x < block; x++) {
      for (std::size_t y = 0; y < block; y++) {

        if (bit / 8 >= data_size)
          return bit / 8;

        std::size_t pos = (bx + x) * width + (by + y);

        set_bit(container[pos], 0,
                get_bit(data[bit / 8], bit % 8));

        bit++;
      }
    }
  }

  return bit / 8;
}

std::string adaptive_extract(const ubyte *container, std::size_t width,
                             std::size_t height, std::size_t data_size,
                             std::size_t key) {
  if (!container)
    return {};

  std::size_t size = width * height;

  struct Pixel {
    std::size_t index;
    int gradient;
  };

  std::vector<Pixel> pixels;
  pixels.reserve(size);

  // градиент считаем по стегоконтейнеру
  // (LSB меняет ±1, порядок практически сохраняется)
  for (std::size_t x = 0; x < height - 1; x++) {
    for (std::size_t y = 0; y < width - 1; y++) {

      std::size_t pos = x * width + y;

      int g = std::abs(container[pos] - container[pos + 1]) +
              std::abs(container[pos] - container[pos + width]);

      pixels.push_back({pos, g});
    }
  }

  std::sort(pixels.begin(), pixels.end(), [](const Pixel &a, const Pixel &b) {
    return a.gradient > b.gradient;
  });

  std::mt19937 gen(key);
  std::shuffle(pixels.begin(), pixels.end(), gen);

  std::string msg;
  std::size_t bit = 0;

  for (std::size_t i = 0; i < pixels.size(); i++) {

    if (bit / 8 >= data_size)
      break;

    if (bit % 8 == 0)
      msg += "w";

    set_bit(msg[bit / 8], bit % 8, get_bit(container[pixels[i].index], 0));

    bit++;
  }

  return msg;
}

double psnr(const ubyte *original, const ubyte *modified, std::size_t size) {
  double mse = 0.0;

  for (std::size_t i = 0; i < size; i++) {
    double diff = (double)original[i] - modified[i];
    mse += diff * diff;
  }

  mse /= size;

  if (mse == 0) {
    return 99.0;
  }

  return 10.0 * std::log10((255.0 * 255.0) / mse);
}

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

} // namespace stg
