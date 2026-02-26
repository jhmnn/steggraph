import cv2
import sys
import numpy as np
from skimage.metrics import structural_similarity as ssim


def calculate_metrics(img1_path, img2_path):
    img1 = cv2.imread(img1_path, cv2.IMREAD_GRAYSCALE)
    img2 = cv2.imread(img2_path, cv2.IMREAD_GRAYSCALE)

    if img1 is None or img2 is None:
        raise ValueError("Ошибка загрузки изображений")

    if img1.shape != img2.shape:
        raise ValueError("Изображения должны иметь одинаковый размер")

    mse = np.mean((img1.astype(np.float64) - img2.astype(np.float64)) ** 2)

    if mse == 0:
        psnr = float("inf")
    else:
        max_pixel = 255.0
        psnr = 20 * np.log10(max_pixel / np.sqrt(mse))

    ssim_value = ssim(img1, img2)

    return mse, psnr, ssim_value


if __name__ == "__main__":
    img1_path = sys.argv[1]
    img2_path = sys.argv[2]

    mse, psnr, ssim_value = calculate_metrics(img1_path, img2_path)

    print(f"MSE  = {mse}")
    print(f"PSNR = {psnr}")
    print(f"SSIM = {ssim_value}")
