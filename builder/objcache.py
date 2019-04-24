import json, hashlib, os

class OBJ_CACHE:
    cache = []
    json_path = 'builder/cache.json'

    def __init__(self):
        if not os.path.exists(self.json_path):
            with open(self.json_path, 'w') as f:
                f.write(json.dumps([]))
        with open(self.json_path, 'r') as f:
            self.cache = json.loads(f.read())

    def json_write(self):
        with open(self.json_path, 'w') as f:
            f.write(json.dumps(self.cache))

    def sha256(self, s):
        return hashlib.sha256(s).hexdigest()

    def sha256_file(self, filename):
        with open(filename, 'rb') as f:
            return self.sha256(filename + "|" + self.sha256(f.read()))

    def is_cached(self, src_file, obj_file):
        obj_dir = os.path.dirname(obj_file)

        if not os.path.exists(obj_dir):
            os.makedirs(obj_dir)
            return False
        if not os.path.exists(obj_file):
            return False
        if not os.path.exists(src_file):
            return False

        hashId = self.sha256(self.sha256_file(src_file) + "|" + self.sha256_file(obj_file))
        for i in self.cache:
            if i == hashId:
                print("using cached '" + src_file + "'")
                return True
        return False
    
    def update(self, src_file, obj_file):
        if os.path.exists(src_file) and os.path.exists(obj_file):
            hashId = self.sha256(self.sha256_file(src_file) + "|" + self.sha256_file(obj_file))
            for i in self.cache:
                if i == hashId:
                    return
            self.cache.append(hashId)
            self.json_write()