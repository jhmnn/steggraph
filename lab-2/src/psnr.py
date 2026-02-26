import sys
import numpy as np
from PIL import Image


def psnr(original, modified):
    mse = np.mean(
        (original.astype(np.float64) -
         modified.astype(np.float64)) ** 2
    )

    if mse == 0:
        return 99.0

    return 10 * np.log10((255.0 ** 2) / mse)


def load_image(path):
    img = Image.open(path).convert("L")
    return np.array(img)


def main():
    if len(sys.argv) != 5:
        print("Usage:")
        print("python calc_psnr.py <original> <container> <type> <out_file>")
        sys.exit(1)

    original_path = sys.argv[1]
    container_path = sys.argv[2]
    dataset_type = sys.argv[3]
    out_file = sys.argv[4]

    original = load_image(original_path)
    container = load_image(container_path)

    if original.shape != container.shape:
        print("Error: image sizes do not match")
        sys.exit(1)

    value = psnr(original, container)

    with open(out_file, "a") as f:
        f.write(f"{dataset_type},{value:.6f}\n")

    print(f"PSNR = {value:.6f} dB")
    print(f"Appended to {out_file}")


if __name__ == "__main__":
    main()
