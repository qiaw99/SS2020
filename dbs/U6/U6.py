import requests                 
from bs4 import BeautifulSoup 

url = "https://www.heise.de/thema/https"

def getTitles():
	titles = []

	for pages in range (6):
		if(pages == 0):
			html = requests.get(url).text
		else:
			html = requests.get(url + "/seite-" + str(pages)).text

		soup = BeautifulSoup(html, 'lxml')
		a = soup.findAll('a')["title"]

		# aside tag that contains articles
		#aside = soup.find('aside', class_='recommendations')
        # divs that contains articles
		#divs = aside.findAll('div', class_='recommendation')
		# headers containing article headers
		#headers = aside.findAll('header')
        # extract string in header tag & append to titles
		#titles += map(lambda x: x.string.strip(), headers)
	return titles

def countWords(titles):
    # to store word counts
    counts = dict()

    for title in titles:
        words = title.split()
        for word in words:
            # if word not yet counted
            if word not in counts.keys():
                counts[word] = 1
            else:
                counts[word] += 1
    return counts		
    	
def main():
	print("Scraping...")
	titles = getTitles()
	counts = countWords(titles)

	counts = sorted(counts.items(), key = lambda x: x[1], reverse = True)

	print("The top 3 words in all titles are: ")
	for word in counts[:3]: 
		print(str(word[0]) + ": " + str(word[1]))

if __name__ == '__main__':
    main()
