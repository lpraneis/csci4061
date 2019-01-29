import requests
from zipfile import ZipFile
from os import listdir,remove
from bs4 import BeautifulSoup

site_url = "https://www-users.cs.umn.edu/~kauffman/4061/schedule.html"

page_url = "https://www-users.cs.umn.edu/~kauffman/4061/"


def get_links():
    r = requests.get(site_url)

    soup = BeautifulSoup(r.content, 'html5lib')

    links = soup.findAll('a')

    code_links = [page_url + link['href'] for link in links if link['href'].endswith('zip')]
    notes_links = [page_url + link['href'] for link in links if link['href'].endswith('pdf')]
    labs_links = [page_url + link['href'] for link in links if link['href'].endswith('html')]


    
    return code_links, notes_links, labs_links

def download_labs(labs_links):
    current_labs = listdir("labs")
    for lab in labs_links:
        if (lab.split('/')[-1].split('.')[0]+"-code") not in current_labs:
            r = requests.get(lab)
            soup = BeautifulSoup(r.content, 'html5lib')
            links = soup.findAll('a')
            labs_links = [page_url + link['href'] for link in links if link['href'].endswith('zip')]

            for link in labs_links:
                    file_name = link.split('/')[-1]
                    r = requests.get(link, stream = True)
                    with open(file_name, 'wb') as f:
                        for chunk in r.iter_content(chunk_size = 1024):
                            if chunk:
                                f.write(chunk)
                    zip_ref = ZipFile(file_name, 'r')
                    zip_ref.extractall('labs')
                    zip_ref.close()
                    remove(file_name)
            


def download_code_zips(code_links):
    currently_downloaded = listdir("code")



    for link in code_links:
        file_name = link.split('/')[-1]
        if file_name.split(".")[0] not in currently_downloaded and file_name[0:2].isdigit():
            r = requests.get(link, stream = True)

            with open(file_name, 'wb') as f:
                for chunk in r.iter_content(chunk_size = 1024):
                    if chunk:
                        f.write(chunk)
            zip_ref = ZipFile(file_name, 'r')
            zip_ref.extractall('code')
            zip_ref.close()
            remove(file_name)




def download_lecture_notes(notes_links):
    currently_downloaded = listdir("lecture_slides")
    for link in notes_links:
        file_name = link.split('/')[-1]
        if file_name.split(".")[0] not in currently_downloaded:
            r = requests.get(link, stream = True)
            file_name = "lecture_slides/"+file_name
            with open(file_name, 'wb') as f:
                    f.write(r.content)





links = get_links()
download_code_zips(links[0])
download_lecture_notes(links[1])
download_labs(links[2])

