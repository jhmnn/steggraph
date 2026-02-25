import requests
import json

username = "jhmnn"
url = f"https://api.github.com/users/{username}/repos"

response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    with open("github_repos.json", "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=4)
    print("save to github_repos.json")
else:
    print("error:", response.status_code)
