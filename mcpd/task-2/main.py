import requests
from bs4 import BeautifulSoup
import json

def main():
  headers = {
      'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
      'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
      'Accept-Language': 'ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7',
      'Accept-Encoding': 'gzip, deflate, br',
      'Connection': 'keep-alive',
      'Upgrade-Insecure-Requests': '1',
      'Sec-Fetch-Dest': 'document',
      'Sec-Fetch-Mode': 'navigate',
      'Sec-Fetch-Site': 'none',
      'Sec-Fetch-User': '?1',
      'Cache-Control': 'max-age=0'
  }

  hh_url = 'https://novosibirsk.hh.ru/search/vacancy?text=c%2B%2B+developer&salary=&ored_clusters=true&area=4&hhtmFrom=vacancy_search_list&hhtmFromLabel=vacancy_search_line'
  hh_request = requests.get(hh_url, headers=headers)
  hh_soup = BeautifulSoup(hh_request.text, 'html.parser')

  paginator = hh_soup.find("nav", {'data-qa': 'pager-block'})
  page_links = paginator.find_all('a', {'data-qa': 'pager-page'})

  page_numbers = []
  for link in page_links:
      page_num = int(link.get_text(strip=True))
      page_numbers.append(page_num)

  total_pages = max(page_numbers)

  vacancies_list = []

  for page in range(total_pages):
      url = f"{hh_url}&page={page}"
      response = requests.get(url, headers=headers)
      print(f"Статус: {response.status_code}")

      page_soup = BeautifulSoup(response.text, 'html.parser')
      vacancies = page_soup.find_all('a', {'data-qa': 'serp-item__title'})

      for vacancy in vacancies:
          title = vacancy.get_text(strip=True)
          vacancy_url = vacancy.get('href')

          salary_span = page_soup.find('span', {'class': 'magritte-text___pbpft_4-1-2 magritte-text_style-primary___AQ7MW_4-1-2 magritte-text_typography-label-1-regular___pi3R-_4-1-2'})
          salary = salary_span.get_text(strip=True) if salary_span else "З/п не указана"

          vacancy_data = {
              'title': title,
              'salary': salary,
              'url': vacancy_url
          }
          vacancies_list.append(vacancy_data)

          print(f"  - {title} | {salary}")

  with open('vacancies.json', 'w', encoding='utf-8') as f:
      json.dump(vacancies_list, f, ensure_ascii=False, indent=2)

  print("Данные сохранены в файл vacancies.json")

if __name__ == "__main__":
  main()
