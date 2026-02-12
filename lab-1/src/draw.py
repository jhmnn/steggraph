import cv2
import numpy as np
import matplotlib.pyplot as plt
import sys

out = "hist.png"
k = 1

def load_gray(path):
    img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    if img is None:
        raise ValueError(f"Не удалось загрузить изображение: {path}")
    return img


def calc_hist(img):
    hist = cv2.calcHist([img], [0], None, [256], [0, 256])
    return hist.flatten()

def bin_hist(hist, bin_size=8):
    n_bins = 256 // bin_size
    binned = hist.reshape(n_bins, bin_size).sum(axis=1)
    return binned

def plot_histograms(img1, img2, label1="исходное изображение", label2="стеганографическое изображение", bin_size=8):
    hist1 = calc_hist(img1)
    hist2 = calc_hist(img2)

    b1 = bin_hist(hist1, bin_size)
    b2 = bin_hist(hist2, bin_size)

    x = np.arange(len(b1))
    width = 0.4

    labels = [f"{i*bin_size}-{i*bin_size+bin_size-1}" for i in range(len(b1))]

    plt.figure(figsize=(14, 6))

    plt.bar(x - width/2, b1, width=width, label=label1)
    plt.bar(x + width/2, b2, width=width, label=label2)

    plt.xticks(x, labels, rotation=90)

    plt.xlabel("Диапазоны яркости")
    plt.ylabel("Количество пикселей")
    plt.title(f"Групповая столбчатая гистограмма (биннинг = {bin_size}; k = {k})")
    plt.legend()
    plt.grid(True, axis='y')

    plt.tight_layout()

    plt.savefig(out)
    # plt.show()


if __name__ == "__main__":
    if len(sys.argv) != 5:
        print(f"Использование: python3 {sys.argv[0]} src.bmp stego.bmp out.png k")
        sys.exit(1)

    src_path = sys.argv[1]
    stego_path = sys.argv[2]
    out = sys.argv[3]
    k = sys.argv[4]

    src = load_gray(src_path)
    stego = load_gray(stego_path)

    if src.shape != stego.shape:
        raise ValueError("Изображения должны быть одного размера")

    plot_histograms(src, stego)
