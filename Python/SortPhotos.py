# Sort photos from camera into dated folders
import os
import shutil
import datetime
import time

photos_folder = os.path.abspath("D:/Photos")
ingest_folder = os.path.abspath("D:/Photos/INGEST")

photos_folder = os.path.abspath("D:/Photos/SCRIPT_TEST")
ingest_folder = os.path.abspath("D:/Photos/INGEST_TEST")

class Photo:
    def __init__(self, file, filepath, date):
        self.file = file
        self.filepath = filepath
        self.date = date

class Folder:
    def __init__(self, foldername, folderpath):
        self.foldername = foldername
        self.folderpath = folderpath
        self.exists = os.path.exists(self.folderpath)
        self.photos = []
 


files = os.listdir(ingest_folder)
photos = []

for file in files:
    filepath = os.path.join(ingest_folder, file)

    file_mtime = os.path.getmtime(filepath)
    formatted_time = datetime.datetime.strptime(time.ctime(file_mtime), "%a %b %d %H:%M:%S %Y")
    formatted_date = str(formatted_time).split(" ")[0]

    photos.append( Photo(file, filepath, formatted_date) )

# for photo in photos:
#     print(photo.file, photo.filepath, photo.date)

folders = []

for photo in photos:
    folderpath = os.path.join(photos_folder, photo.date)

    if not any(folderpath in folder.folderpath for folder in folders):
        folders.append( Folder(photo.date, folderpath) )
    
    for folder in folders:
        if folderpath == folder.folderpath:
            folder.photos.append(photo)

for folder in folders:
    print(folder.folderpath)
    print([photo.file for photo in folder.photos])

for folder in folders:
    shutil.    
# print([folder.folderpath for folder in folders])
