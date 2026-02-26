#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stg.hpp"

#include "stb_image.h"
#include "stb_image_write.h"

#include <cmath>
#include <iostream>
// #include <vector>

#define KEY 12345
#define THRESHOLD 257

using namespace stg;

std::string prog_name;

// double psnr(unsigned char *orig, unsigned char *stego, int size) {
//   double mse = 0.0;

//   for (int i = 0; i < size; i++) {
//     double diff = orig[i] - stego[i];
//     mse += diff * diff;
//   }

//   mse /= size;

//   if (mse == 0)
//     return 100.0;

//   return 10.0 * log10((255.0 * 255.0) / mse);
// }

// double local_variance(unsigned char *img, int x, int y, int width, int
// height) {
//   int window = 1;
//   double mean = 0.0;
//   double var = 0.0;
//   int count = 0;

//   for (int dy = -window; dy <= window; dy++) {
//     for (int dx = -window; dx <= window; dx++) {
//       int nx = x + dx;
//       int ny = y + dy;

//       if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
//         mean += img[ny * width + nx];
//         count++;
//       }
//     }
//   }

//   mean /= count;

//   for (int dy = -window; dy <= window; dy++) {
//     for (int dx = -window; dx <= window; dx++) {
//       int nx = x + dx;
//       int ny = y + dy;

//       if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
//         double diff = img[ny * width + nx] - mean;
//         var += diff * diff;
//       }
//     }
//   }

//   return var / count;
// }

// void adaptive_embed(unsigned char *container, const std::vector<int> &bits,
//                     int width, int height) {
//   std::size_t bit_index = 0;
//   std::size_t size = bits.size();

//   for (int y = 0; y < height && bit_index < size; y++) {
//     for (int x = 0; x < width && bit_index < size; x++) {
//       double var = local_variance(container, x, y, width, height);

//       if (var > 100.0) {
//         int idx = y * width + x;
//         container[idx] = (container[idx] & 0xFE) | bits[bit_index++];
//       }
//     }
//   }
// }

// std::vector<int> adaptive_extract(unsigned char *container, int width,
//                                   int height, int msg_length) {
//   std::vector<int> bits;
//   bits.reserve(msg_length);

//   for (int y = 0;
//        y < height && bits.size() < static_cast<std::size_t>(msg_length); y++)
//        {
//     for (int x = 0;
//          x < width && bits.size() < static_cast<std::size_t>(msg_length);
//          x++) {

//       double var = local_variance(container, x, y, width, height);

//       if (var > 100.0) {
//         int idx = y * width + x;
//         bits.push_back(container[idx] & 1);
//       }
//     }
//   }

//   return bits;
// }

void print_usage() {
  std::cout << "usage: " << prog_name
            << " emb [lsb|adaptive] <container_path> <logo_path> <out_path>\n"
            << "       " << prog_name
            << " ext [lsb|adaptive] <container_path> <out_path> <logo_size>\n";
}

int main(int argc, char *argv[]) {
  prog_name = argv[0];

  if (argc == 6) {
    const std::string action = argv[1];
    const std::string mode = argv[2];
    const std::string container_path = argv[3];
    const std::string logo_path = argv[4];
    const std::string out_path = argv[5];

    if (action == "emb") {
      int container_width = -1;
      int container_heigth = -1;
      int container_channels = -1;
      int logo_width = -1;
      int logo_heigth = -1;
      int logo_channels = -1;

      unsigned char *container =
          stbi_load(container_path.c_str(), &container_width, &container_heigth,
                    &container_channels, 1);
      if (container == NULL) {
        std::cout << "fail to load container: " << stbi_failure_reason()
                  << "\n";
        return -1;
      }

      unsigned char *logo = stbi_load(logo_path.c_str(), &logo_width,
                                      &logo_heigth, &logo_channels, 1);
      if (logo == NULL) {
        std::cout << "fail to load logo: " << stbi_failure_reason() << "\n";
        return -1;
      }

      std::size_t container_size = container_width * container_heigth;
      std::size_t logo_size = logo_width * logo_heigth;
      std::size_t embedded_count = 0;

      if (mode == "lsb") {
        embedded_count =
            lsb_embed(container, container_size, logo, logo_size, KEY);
      } else if (mode == "adaptive") {
        embedded_count =
            adaptive_embed(container, container_width, container_heigth, logo,
                           logo_size, KEY);
      } else {
        std::cout << "wrong mode\n\n";
        print_usage();
        return -1;
      }

      stbi_write_bmp(out_path.c_str(), container_width, container_heigth, 1,
                     container);

      std::cout << "embedded " << embedded_count << " bytes to '" << out_path
                << "' (logo_size = " << logo_size << ")\n";

      stbi_image_free(container);
      stbi_image_free(logo);
    } else if (action == "ext") {
      const std::string container_path = argv[3];
      const std::string out_path = argv[4];
      std::size_t logo_size = std::strtol(argv[5], NULL, 10);

      int container_width = -1;
      int container_heigth = -1;
      int container_channels = -1;

      unsigned char *container =
          stbi_load(container_path.c_str(), &container_width, &container_heigth,
                    &container_channels, 1);
      if (container == NULL) {
        std::cout << "fail to load container: " << stbi_failure_reason()
                  << "\n";
        return -1;
      }

      std::size_t container_size = container_width * container_heigth;
      std::string extracted;

      if (mode == "lsb") {
        extracted = lsb_extract(container, container_size, logo_size, KEY);
      } else if (mode == "adaptive") {
        extracted =
            adaptive_extract(container, container_width, container_heigth,
                             logo_size, KEY);
      } else {
        std::cout << "wrong mode\n\n";
        print_usage();
        return -1;
      }

      std::size_t side = (int)std::sqrt(extracted.size());
      stbi_write_bmp(out_path.c_str(), side, side, 1, extracted.c_str());

      std::cout << "extracted " << extracted.size() << " bytes to '" << out_path
                << "'\n";

      stbi_image_free(container);
    } else {
      std::cout << "wrong action\n\n";
      print_usage();
      return -1;
    }
  } else {
    print_usage();
    return -1;
  }

  // double psnr = psnr(container, stego);
  // cout << "psnr: " << psnr << endl;

  return 0;
}
