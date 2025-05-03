from bs4 import BeautifulSoup
from playwright.sync_api import sync_playwright, TimeoutError as PlayTimeout
import pandas as pd
import os

BASE = "https://999.md"
url = ("https://999.md/ro/list/real-estate/apartments-and-rooms"
       "?appl=1&selected_currency=eur&ef=16,6,2307,2200&eo=13859,12885,12900,12912"
       "&o_16_1=776&page=")
pages = 230
links = []

for i in range(0, pages + 1):
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=True)
        page = browser.new_page()
        try:
            print(url + str(i))
            page.goto(url + str(i), timeout=60_000)
            page.wait_for_timeout(2_000)
        except PlayTimeout:
            print(f"skip {url}: took too long")
            browser.close()
            continue

        soup = BeautifulSoup(page.content(), "html.parser")
        adlist_div = soup.find("div", attrs={"data-sentry-component": "AdList"})
        if adlist_div:
            for a in adlist_div.find_all("a"):
                href = a.get("href")
                if href:
                    links.append(href)
            print("links collected:", len(links))
        else:
            print("No apartments found on this page")
        browser.close()

links = list(set(links))
pd.DataFrame(links, columns=["URL"]).to_csv("links.csv", index=False)

