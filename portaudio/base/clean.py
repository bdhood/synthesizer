import os
def delete(path):
    try:
        print("delete '" + path + "'")
        os.remove(path)
        return 1
    except:
        return 0
    
def listdir(path):
    n = 0
    for f in os.listdir(path):
        p = path + "/" + f
        if os.path.isdir(p):
            n += listdir(p)
        elif f[0] == '.' or f[-4:] == '.pyc' or f[-9:] == "-loaded.c":
            n += delete(p)
    return n        


print("Cleaning files...\n")
print("\t" + str(listdir(".")) + " file(s) deleted\n")
if __name__ == "__main__":
    raw_input("Press ENTER to exit...")
