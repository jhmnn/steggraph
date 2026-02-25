import mongomock

client = mongomock.MongoClient()
db = client["lenta_db"]
collection = db["trending_products"]

def save_product(product: dict):
    collection.insert_one(product)

def get_all_products():
    return list(collection.find())
