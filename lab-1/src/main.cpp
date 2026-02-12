#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stg.hpp>

#include <stb_image.h>
#include <stb_image_write.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

std::string prog_name;

std::string file_read(const std::string &filename) {
  std::ifstream file(filename);
  std::stringstream data;
  data << file.rdbuf();
  file.close();
  return data.str();
}

void file_write(const std::string &filename, const std::string &str) {
  std::ofstream file(filename);
  file << str;
  file.close();
}

void print_usage() {
  std::cout << "usage: " << prog_name
            << " bin <src_filepath> <out_filepath> <k>\n       " << prog_name
            << " enc <img_filepath> <msg_filepath> <out_filepath> <k>\n       "
            << prog_name
            << " dec <src_filepath> <out_filepath> <k> <msg_length>\n";
}

int main(int argc, char *argv[]) {
  prog_name = argv[0];

  if (argc < 2) {
    print_usage();
    return 0;
  }

  int x = -1;
  int y = -1;
  int channels = -1;

  if (strcmp(argv[1], "bin") == 0) {
    if (argc != 5) {
      print_usage();
      return -1;
    }

    std::string src_path = argv[2];
    std::string out_path = argv[3];
    int k = std::strtol(argv[4], NULL, 10);

    if (k < 1 || k > 8) {
      std::cerr << "k must be in the range [1, 8]\n";
      return -1;
    }

    stbi_uc *data = stbi_load(src_path.c_str(), &x, &y, &channels, 1);
    if (x < 0) {
      std::cerr << "can't load '" << src_path << "': " << stbi_failure_reason()
                << "\n";
      return -1;
    }

    stbi_uc *out = stg::gray_to_bin(data, x, y, k);

    stbi_write_bmp(out_path.c_str(), x, y, 1, out);
  } else if (strcmp(argv[1], "enc") == 0) {
    if (argc != 6) {
      print_usage();
      return -1;
    }

    std::string img_path = argv[2];
    std::string msg_path = argv[3];
    std::string out_path = argv[4];
    int k = std::strtol(argv[5], NULL, 10);

    if (k < 1 || k > 8) {
      std::cerr << "k must be in the range [1, 8]\n";
      return -1;
    }

    stbi_uc *data = stbi_load(img_path.c_str(), &x, &y, &channels, 1);
    if (x < 0) {
      std::cerr << "can't load '" << img_path << "': " << stbi_failure_reason()
                << "\n";
      return -1;
    }

    auto msg = file_read(msg_path);
    int length = stg::encrypt_message(data, x, y, k, msg);
    stbi_write_bmp(out_path.c_str(), x, y, 1, data);

    std::cout << "encrypted " << length << " symbols from '" << img_path
              << "' to '" << out_path << "'\n";
  } else if (strcmp(argv[1], "dec") == 0) {
    if (argc != 6) {
      print_usage();
      return -1;
    }

    std::string src_path = argv[2];
    std::string out_path = argv[3];
    int k = std::strtol(argv[4], NULL, 10);
    int length = std::strtol(argv[5], NULL, 10);

    if (k < 1 || k > 8) {
      std::cerr << "k must be in the range [1, 8]\n";
      return -1;
    }

    stbi_uc *enc = stbi_load(src_path.c_str(), &x, &y, &channels, 1);
    if (x < 0) {
      std::cerr << "can't load '" << src_path << "': " << stbi_failure_reason()
                << "\n";
      return -1;
    }

    auto dec = stg::decrypt_message(enc, x, y, k, length);

    file_write(out_path, dec);

    std::cout << "decrypted " << dec.length() << " symbols from '" << src_path
              << "'\n";
  } else {
    print_usage();
  }

  return 0;
}
