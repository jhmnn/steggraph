import requests
from bs4 import BeautifulSoup
import pandas as pd


def parse_salary(salary):
    if not salary:
        return None, None, None
    return salary.get("from"), salary.get("to"), salary.get("currency")


def parse_hh(vacancy, pages=1):
    url_template = "https://api.hh.ru/vacancies?text={}&page={}"
    results = []

    for page in range(pages):
        url = url_template.format(vacancy, page)
        response = requests.get(url)

        if response.status_code != 200:
            print(f"Ошибка {response.status_code} на странице {page}")
            continue

        soup = BeautifulSoup("<pre>" + response.text + "</pre>", "html.parser")

        json_data = response.json()
        for item in json_data.get("items", []):
            min_sal, max_sal, currency = parse_salary(item.get("salary"))

            results.append({
                "title": item.get("name"),
                "min_salary": min_sal,
                "max_salary": max_sal,
                "currency": currency,
                "company": item.get("employer", {}).get("name"),
                "location": item.get("area", {}).get("name"),
                "link": item.get("alternate_url"),
                "source": "hh.ru"
            })

    return results


if __name__ == "__main__":
    vacancy = input("vacancy: ")
    pages = int(input("pages: "))

    data = parse_hh(vacancy, pages)

    df = pd.DataFrame(data)
    print(df.head())

    df.to_json("vacancies.json", orient="records", force_ascii=False, indent=4)
    print("saved to vacancies.json")
