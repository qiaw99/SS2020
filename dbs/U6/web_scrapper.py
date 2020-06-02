from bs4 import BeautifulSoup   
import requests
from collections import Counter


START_URL = "https://www.heise.de/thema/https"
SITE_NAME = "https://www.heise.de"
current_url = " "
title_arr = []
words_dict = Counter()
commas = 0
NUM_OF_CMN_WRDS = 3

def getPage(url):
    r = requests.get(url)
    data = r.text
    spobj = BeautifulSoup(data, "lxml")
    return spobj

def print_titles(titles_arr):
    for element in title_arr:
        print(element)

def print_common_words(arr):
    global commas, NUM_OF_CMN_WRDS
    for element in arr:
        words = element.split()
        for word in words:
            #deleting comma at the end of the word
            if word[-1] == ',':
                commas += 1
                word = word[:-1]
            
            words_dict[word] += 1
            
            
            '''
            without Counter() we would create a dict of words and then we would go through it 
            and chose 3 the most common words == 3 the biggest numbers
            
            adding to dict like this
            
            if word in words_dict:
                words_dict[word] += 1
            else:
                words_dict[word] = 1'''
                
    print(NUM_OF_CMN_WRDS, " most common words and number of their occurences are", words_dict.most_common(NUM_OF_CMN_WRDS))

def main():
    global START_URL, current_url
    new_url = START_URL
    while(new_url != current_url):
        current_url = new_url
        soup = getPage(current_url)
        for main_layout in soup.find_all('div', class_='a-layout__main'):   
            for article in main_layout.find_all('article'):
                title_arr.append((article.a.get('title')))
        for next_url in soup.find_all('a', {"class": "a-pagination__link", "rel": "next"}):
            new_url = SITE_NAME  + next_url.get('href')
        
    #print_titles(title_arr)
    print_common_words(title_arr)
    

if __name__ == '__main__':
    main()
