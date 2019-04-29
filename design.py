class Object(object):
    def get_content(self):
        raise NotImplementedError()
class Blob(object):
    def __init__(self, content):
        self.content = content
    def get_content(self):
        return content 
class Commit(object):
    def __init__(self, parent_id, tree_id, description):
        self.parent_id = parent_id
        self.tree_id = tree_id
        self.description = description
    def get_content(self):
        return "\n".join(self.description, self.parent_id, self.tree_id)
    def get_parent(self, db):
        return db.load_object(self.parent_id)
class Tree(object):
    def __init__(self, blob_names):
        self.blob_names = blob_names

class ObjectDb(object):
    def __init__(self, db=None, id_func=id):
        self.db = db or {}
        self.id_func = id_func
    def save(self, obj):
        obj_id = self.id_func(obj)
        self.db[obj_id] = obj
        return obj_id
    def load(self, obj_id):
        return self.db[obj_id]
class Index(object):
    def __init__(self, object_db=None, blob_names=None):
        self.object_db = object_db or ObjectDb()
        self.blob_names = blob_names or {}
    def add(self, name, blob):
        obj_id = self.object_db.save(blob)
        self.blob_names[name] = obj_id
class GitDb(object):
    def __init__(self, refs=None, head=None):
        self.refs = refs or {}
        self.head = head or None # Handle detached HEAD, currently None?

class Db(object):
    # DB should handle index, objects and configuration.
    def __init__(self, git_db=None, index=None):
        self.git_db = git_db or GitDb()
        self.index = index or Index()
    def save_object(self, obj):
        return self.index.object_db.save(obj)
    def load_object(self, obj_id):
        return self.index.object_db.load(obj_id)
    def index_add(self, name, blob):
        return self.index.add(name, blob)
    def get_index_blob_names(self):
        return self.index.blob_names
    @property
    def refs(self):
        return self.git_db.refs
    @property
    def head(self):
        return self.git_db.head
    @head.setter
    def head(self, value):
        self.git_db.head = value

class Fs(object):
    def get_blob(self, name):
        return Blob("Mock content of {}".format(name))

class Repo(object):
    def __init__(self, db=None, fs=None):
        self.db = db or Db()
        self.fs = fs or Fs()
    def add(self, name):
        blob = self.fs.get_blob(name)
        return self.db.index_add(name, blob)
    def commit(self, description):
        tree = Tree(self.db.get_index_blob_names())
        tree_id = self.db.save_object(tree)
        if self.db.head is None:
            self.db.head = 'master'
            parent_id = None
        else:
            parent_id = self.db.refs[self.db.head] 
        commit = Commit(parent_id, tree_id, description)
        commit_id = self.db.save_object(commit)
        self.db.refs[self.db.head] = commit_id
        return commit_id

def main():
    repo = Repo()
    repo.add('1')
    commit_1_id = repo.commit('1')
    repo.add('2')
    commit_2_id = repo.commit('2')

    commit_1 = repo.db.load_object(commit_1_id)
    commit_2 = repo.db.load_object(commit_2_id)

    assert commit_2.get_parent(repo.db) == commit_1

if __name__ == '__main__':
    main()