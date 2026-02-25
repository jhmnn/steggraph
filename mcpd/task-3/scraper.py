from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

def get_trending_products():
    options = Options()
    options.add_argument("--start-maximized")

    driver = webdriver.Chrome(options=options)
    driver.get("https://www.mvideo.ru/noutbuki-planshety-komputery-8/noutbuki-118?from=homepage")

    wait = WebDriverWait(driver, 20)

    for _ in range(6):
        driver.execute_script("window.scrollBy(0, 1200)")
        time.sleep(0.5)

    cards = driver.find_elements(By.CSS_SELECTOR, "mvid-product-card-list")

    print("finded cards:", len(cards))

    products = []

    for card in cards:
        try:
            name = card.text.split("\n")[0]
            # print(card.text)
            link = card.get_attribute("href")

            products.append({
                "name": name,
                "link": link
            })
        except:
            continue

    driver.quit()

    print("collected products:", len(products))
    
    return products
