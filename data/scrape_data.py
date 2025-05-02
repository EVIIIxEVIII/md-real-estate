from bs4 import BeautifulSoup
from bs4 import BeautifulSoup
from playwright.sync_api import sync_playwright, TimeoutError as PlayTimeout
import pandas as pd
import numpy as np
import os

# ─── Settings ───────────────────────────────────────────────────────────────────
BASE      = "https://999.md"
links     = np.array(pd.read_csv("links.csv")["URL"])
CSV_FILE  = "apartments.csv"

# ─── Helpers ────────────────────────────────────────────────────────────────────
def ensure_columns(row_df: pd.DataFrame, csv_path: str) -> pd.DataFrame:
    """
    Make sure `row_df` has exactly the same columns as the CSV on disk.
    If new columns appear, rewrite the whole file with the expanded header.
    """
    if not os.path.isfile(csv_path):
        return row_df  # first run – nothing to align with

    # read only header
    current_cols = list(pd.read_csv(csv_path, nrows=0).columns)
    union_cols   = sorted(set(current_cols).union(row_df.columns))

    if union_cols != current_cols:
        # new fields detected → rewrite file with padded columns
        full_df = pd.read_csv(csv_path).reindex(columns=union_cols)
        full_df.to_csv(csv_path, index=False, encoding="utf-8")
    return row_df.reindex(columns=union_cols)

def scrape_features(soup: BeautifulSoup) -> dict:
    features = {}
    for li in soup.select('div[data-sentry-component="FeatureGroup"] li'):
        spans = li.find_all("span", limit=2)
        if len(spans) == 2:
            features[spans[0].get_text(strip=True)] = spans[1].get_text(strip=True)

    price_div = soup.select_one('div[data-sentry-component="Price"]')
    if price_div:
        spans = price_div.find_all("span")
        if len(spans) >= 2:
            features["price"] = spans[1].get_text(strip=True)
    return features

# ─── Main loop ──────────────────────────────────────────────────────────────────
with sync_playwright() as p:
    browser = p.chromium.launch(headless=True)
    context = browser.new_context()
    page    = context.new_page()

    for rel_link in links:
        url = BASE + rel_link
        print("fetching", url)

        try:
            page.goto(url, wait_until="domcontentloaded", timeout=60_000)
            page.wait_for_selector('div[data-sentry-component="Price"]', timeout=10_000)
        except PlayTimeout:
            print("timeout, skip", url)
            continue

        feat_dict = scrape_features(BeautifulSoup(page.content(), "html.parser"))
        row_df    = ensure_columns(pd.DataFrame([feat_dict]), CSV_FILE)

        row_df.to_csv(
            CSV_FILE,
            mode="a",
            index=False,
            header=not os.path.isfile(CSV_FILE),
            encoding="utf-8"
        )

    browser.close()

from playwright.sync_api import sync_playwright, TimeoutError as PlayTimeout
import pandas as pd
import numpy as np
import os

# ─── Settings ───────────────────────────────────────────────────────────────────
BASE      = "https://999.md"
links     = np.array(pd.read_csv("links.csv")["URL"])
CSV_FILE  = "apartments.csv"

# ─── Helpers ────────────────────────────────────────────────────────────────────
def ensure_columns(row_df: pd.DataFrame, csv_path: str) -> pd.DataFrame:
    """
    Make sure `row_df` has exactly the same columns as the CSV on disk.
    If new columns appear, rewrite the whole file with the expanded header.
    """
    if not os.path.isfile(csv_path):
        return row_df  # first run – nothing to align with

    # read only header
    current_cols = list(pd.read_csv(csv_path, nrows=0).columns)
    union_cols   = sorted(set(current_cols).union(row_df.columns))

    if union_cols != current_cols:
        # new fields detected → rewrite file with padded columns
        full_df = pd.read_csv(csv_path).reindex(columns=union_cols)
        full_df.to_csv(csv_path, index=False, encoding="utf-8")
    return row_df.reindex(columns=union_cols)

def scrape_features(soup: BeautifulSoup) -> dict:
    features = {}
    for li in soup.select('div[data-sentry-component="FeatureGroup"] li'):
        spans = li.find_all("span", limit=2)
        if len(spans) == 2:
            features[spans[0].get_text(strip=True)] = spans[1].get_text(strip=True)

    price_div = soup.select_one('div[data-sentry-component="Price"]')
    if price_div:
        spans = price_div.find_all("span")
        if len(spans) >= 2:
            features["price"] = spans[1].get_text(strip=True)
    return features

# ─── Main loop ──────────────────────────────────────────────────────────────────
with sync_playwright() as p:
    browser = p.chromium.launch(headless=True)
    context = browser.new_context()
    page    = context.new_page()

    for rel_link in links:
        url = BASE + rel_link
        print("fetching", url)

        try:
            page.goto(url, wait_until="domcontentloaded", timeout=60_000)
            page.wait_for_selector('div[data-sentry-component="Price"]', timeout=10_000)
        except PlayTimeout:
            print("timeout, skip", url)
            continue

        feat_dict = scrape_features(BeautifulSoup(page.content(), "html.parser"))
        row_df    = ensure_columns(pd.DataFrame([feat_dict]), CSV_FILE)

        row_df.to_csv(
            CSV_FILE,
            mode="a",
            index=False,
            header=not os.path.isfile(CSV_FILE),
            encoding="utf-8"
        )

    browser.close()

