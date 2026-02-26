import pandas as pd
import numpy as np
import scipy.stats as stats


def confidence_interval(data, alpha=0.05):
    n = len(data)
    mean = np.mean(data)
    std = np.std(data, ddof=1)
    t_value = stats.t.ppf(1 - alpha/2, df=n-1)
    margin = t_value * std / np.sqrt(n)
    return mean, std, mean - margin, mean + margin


df = pd.read_csv("out/psnr.csv")

alpha = 0.05

for dataset in df["dataset"].unique():
    values = df[df["dataset"] == dataset]["psnr"].values
    mean, std, lower, upper = confidence_interval(values, alpha)
    
    print(f"{dataset}:")
    print(f"  Mean = {mean:.3f}")
    print(f"  95% CI = [{lower:.3f}, {upper:.3f}]")
    print()
