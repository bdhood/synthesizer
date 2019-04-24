import json, os, sys, clean

class GCC_WRAPPER:
    src_files = []
    resources = []

    def load_json_file(self, filepath):
        with open(filepath, "r") as f:
            try:
                return json.dumps(json.loads(f.read()))
            except:
                print("failed to read '" + filepath + "'")
                
    def load_resources(self):
        with open("./src/resource/resource.c", "r") as f:
            source = f.read()
        for r in self.resources:
            source = source.replace(r, self.resources[r].replace('"', '\\"'))
        with open("./src/resource/resource-loaded.c", "w") as f:
            f.write(source)

    def invoke_gcc(self, output, define_arr, gcc_args=""):
        cmd_line = "gcc"
        cmd_line += reduce(lambda x, y: x + " src/" + y, [""] + self.src_files)
        cmd_line += reduce(lambda x, y: x + " -D" + y, [""] + define_arr)
        cmd_line += " -o bin/" + output + " " + gcc_args
        print("build '" + output + "'...")
        os.system(cmd_line)

    def build(self):
        if sys.platform == "linux2":
            self.invoke_gcc("linux", ["OS_LINUX"])
            self.invoke_gcc("linux-dbg", ["OS_LINUX", "DEBUG"])
        elif sys.platform == "win32" or sys.platform == "cygwin": 
            self.invoke_gcc("win.exe", ["OS_WIN"])
            self.invoke_gcc("win-dbg.exe", ["OS_WIN", "DEBUG"])
        elif sys.platform == "darwin":
            self.invoke_gcc("mac", ["OS_MAC"])
            self.invoke_gcc("mac-dbg", ["OS_MAC", "DEBUG"])
        else:
            print("System '" + sys.platform + "' is not supported")

if __name__ == "__main__":
    print("Starting build, target '" + sys.platform + "'...\n")
    
    gcc = GCC_WRAPPER()
    gcc.src_files = [
        "main.c",
        "network/server.c",
        "network/network.c",
        "vendors/jsmn/jsmn.c"
    ]
    gcc.resources = {
        "**HOSTFILE**":"./hostfile.json"
    }

    gcc.load_resources()
    gcc.build()
    raw_input("\nPress ENTER to exit...")
