import json, os, sys, subprocess, objcache

class GCC_WRAPPER:
    src_files = []
    incl_dirs = []
    resources = []
    libs = []
    output = ""

    def run_command_line(self, cmd_line):
        process = subprocess.Popen(cmd_line,
            stderr=subprocess.PIPE,
            stdout=subprocess.PIPE)
        failure = False
        while process.poll() == None:
            line = process.stderr.read()
            if line != "":
                print(line)
                failure = True
            line = process.stdout.read()
            if line != "":
                print(line)
        if failure:
            exit(1)

    def invoke_gcc(self, output, define_arr, gcc_args=""):
        cache = objcache.OBJ_CACHE()
        used_compiler = False
        for f in self.src_files: 
            f_obj = "obj/" + f.replace(".cpp", ".o").replace(".c", ".o")
            if not cache.is_cached(f, f_obj):
                used_compiler = True

                cmd_line = "g++ -c " 
                cmd_line += f
                cmd_line += reduce(lambda x, y: x + " -I" + y, [""] + self.incl_dirs)
                cmd_line += reduce(lambda x, y: x + " -D" + y, [""] + define_arr)
                cmd_line += reduce(lambda x, y: x + " -l" + y, [""] + self.libs)
                cmd_line += " " + gcc_args
                cmd_line += " -o " + f_obj

                print("compiling '" + f + "'")
                print(cmd_line)
                self.run_command_line(cmd_line)
                cache.update(f, f_obj)

        cmd_line = "g++ " 
        cmd_line += reduce(lambda x, y: x + " obj/" + y.replace(".cpp", ".o").replace(".c", ".o"), [""] + self.src_files)
        cmd_line += reduce(lambda x, y: x + " -I" + y, [""] + self.incl_dirs)
        cmd_line += reduce(lambda x, y: x + " -D" + y, [""] + define_arr)
        cmd_line += reduce(lambda x, y: x + " -l" + y, [""] + self.libs)
        cmd_line += " " + gcc_args 
        cmd_line += " -o " + output

        if not used_compiler and cache.is_cached(output, output):
            return

        print("linking...")
        print(cmd_line)
        self.run_command_line(cmd_line)
        print("build successful")
        cache.update(output, output)

    def build(self):
        if sys.platform == "win32" or sys.platform == "cygwin": 
            self.invoke_gcc(self.output, ["PA_USE_ASIO=1"], "")
        else:
            print("System '" + sys.platform + "' is not supported")