import requests
from bs4 import BeautifulSoup
import os

def downloadImage(url, destination, file_name):
    print("Download", file_name, "...")
    res = requests.get(url, headers={'referer': "https://readmanganato.com/", 'accept': "image/avif,image/webp,*/*"})
    if res.status_code == 200:
        if not os.path.isdir(destination):
            os.mkdir(destination)
        with open(destination + file_name, 'wb') as f:
            f.write(res.content)
        print('Image sucessfully Downloaded: ', file_name)
        return 1
    else:
        print('Image Couldn\'t be retrieved')
        return 0

def dumpChapter(url, destination):
    print("Dump chapter", url, "...")
    page = requests.get(url)
    soup = BeautifulSoup(page.content, "html.parser")
    container = soup.find_all("div", class_="container-chapter-reader")[0] # get images container

    for imageElement in container: # browse each chapter images
        if (imageElement.name != "img"):
            continue
        imgUrl = imageElement["src"] # extract image's url
        filename = imgUrl.rsplit('/', 1)[-1] # get image filename
        if not downloadImage(imgUrl, destination, filename): # download image
            break # an error ocurred
    chapterName = url.rsplit('/', 1)[-1] # get image filename
    print("chapter", chapterName, "downloaded.")


def dumpManga(url, destination):
    page = requests.get(url)
    soup = BeautifulSoup(page.content, "html.parser")
    linkList = soup.find_all("a", class_="chapter-name")
    for link in linkList:
        chapterUrl = link["href"]
        chapterName = link.text
        chapterName = chapterName.replace(" ", "_")
        dumpChapter(chapterUrl, destination+chapterName+"/")

dumpManga("https://readmanganato.com/manga-fh982716", "./data/")
