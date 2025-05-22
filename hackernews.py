# web scraper in Python to gather the latest ethical hacking news and save it to a formatted HTML file by Nathan W Jones nat@davaosecurity.com
# pip install requests beautifulsoup4
# Scraping Function: The scrape_hacking_news function takes a URL, sends a GET request, and parses the HTML to find articles.
# You will need to adjust the selectors based on the actual HTML structure of the website you are scraping.
# Saving to HTML: The save_to_html function takes the list of news items and writes them to an HTML file, formatting them as a list.
# Main Execution: The script runs the scraping function on a specified URL and saves the results to an HTML file.

import requests
from bs4 import BeautifulSoup
import html

def scrape_hacking_news(url):
    # Send a GET request to the website
    response = requests.get(url)
    response.raise_for_status()  # Raise an error for bad responses

    # Parse the HTML content
    soup = BeautifulSoup(response.text, 'html.parser')

    # Find news articles (this will depend on the website's structure)
    articles = soup.find_all('article')  # Adjust this based on the website's HTML structure

    news_items = []
    for article in articles:
        title = article.find('h2')  # Adjust based on the website's HTML structure
        link = article.find('a')['href']  # Adjust based on the website's HTML structure
        if title and link:
            news_items.append({
                'title': title.get_text(strip=True),
                'link': link
            })

    return news_items

def save_to_html(news_items, filename='ethical_hacking_news.html'):
    # Create a formatted HTML file
    with open(filename, 'w', encoding='utf-8') as f:
        f.write('<html><head><title>Latest Ethical Hacking News</title></head><body>')
        f.write('<h1>Latest Ethical Hacking News</h1>')
        f.write('<ul>')
        for item in news_items:
            f.write(f'<li><a href="{html.escape(item["link"])}">{html.escape(item["title"])}</a></li>')
        f.write('</ul>')
        f.write('</body></html>')

if __name__ == '__main__':
    # Example URL (you can replace this with a real news site that covers ethical hacking)
    url = 'https://www.example.com/ethical-hacking-news'  # Replace with a real URL
    news_items = scrape_hacking_news(url)
    save_to_html(news_items)
    print(f'Saved {len(news_items)} news items to ethical_hacking_news.html')
