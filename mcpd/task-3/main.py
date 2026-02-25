from scraper import get_trending_products
from mongo_db import save_product, get_all_products

def main():
    products = get_trending_products()

    for product in products:
        save_product(product)

    print(f"saved to mongodb: {len(products)}")
    for p in get_all_products()[:5]:
        print(p)

if __name__ == "__main__":
    main()
