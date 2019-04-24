import json, os, sys

import clean

with open("hostfile.json", "r") as f:
    hostfile = json.dumps(json.loads(f.read()))

rcs = {"**HOSTFILE**":hostfile}
src = ["main.c",
       "network/server.c",
       "network/network.c",
       "vendors/jsmn/jsmn.c"
       ]
 
def load_resources():
    with open("./src/resource/resource.c", "r") as f:
        source = f.read()
    for r in rcs:
        source = source.replace(r, rcs[r].replace('"', '\\"'))
    with open("./src/resource/resource-loaded.c", "w") as f:
        f.write(source)
    
def build(output, define, args=""):
    cmdline = "gcc"
    cmdline += reduce(lambda x, y: x + " src/" + y, [""] + src)
    cmdline += reduce(lambda x, y: x + " -D" + y, [""] + define)
    cmdline += " -o bin/" + output + " " + args
    print("build '" + output + "'...")
    os.system(cmdline)

print("Starting build, target '" + sys.platform + "'...\n")

load_resources()

if sys.platform == "linux2":
    build("linux", ["OS_LINUX"])
    build("linux-dbg", ["OS_LINUX", "DEBUG"])
elif sys.platform == "win32" or sys.platform == "cygwin": 
    build("win.exe", ["OS_WIN"])
    build("win-dbg.exe", ["OS_WIN", "DEBUG"])
elif sys.platform == "darwin":
    build("mac", ["OS_MAC"])
    build("mac-dbg", ["OS_MAC", "DEBUG"])
else:
    print("System '" + sys.platform + "' is not supported")
    
if __name__ == "__main__":
    raw_input("\nPress ENTER to exit...")

