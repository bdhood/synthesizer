from ftplib import FTP
import os

ftp = FTP('bdhood.com')
ftp.login('hoody@bdhood.com', '2fr4D6=P~,&*xlq1;^^I&]*4')

def download_file(path):
    with open('..' + path, 'wb') as f:
        print("downloading '" + path + "'")
        ftp.retrbinary('RETR ' + path, f.write)
        
def recurse(path):
    ftp.cwd(path)
    for f in ftp.nlst():
        if f != "." and f != "..":
            p = path + "/" + f
            if '.' in f:
                download_file(p)
            else:
                try:
                    recurse(p)
                except:
                    continue
    
recurse('/base')

