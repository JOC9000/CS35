import os
import sys
import zlib

def topo_order_commits():
   "hello" 


def find_git_dir():
   search_is_done = False
   base_directory = os.getcwd()

   while search_is_done == False and base_directory != "/":
      for dir_name in os.listdir(base_directory):
         if dir_name == ".git" and os.path.isdir(base_directory + "/" + dir_name):
            search_is_done = True
            base_directory = base_directory + "/.git"
            return search_is_done, base_directory

      base_directory = os.path.dirname(base_directory)

   return search_is_done, base_directory

def get_branch_names(directory):
   list_of_branches = list()
   list_of_hashes = list()
   for file_name in os.listdir(directory):
      if os.path.isdir(directory + "/" + file_name):
         temp_branch_list, temp_hash_list  = get_branch_names(directory + "/" + file_name)
         list_of_branches = list_of_branches + temp_branch_list
         list_of_hashes = list_of_hashes + temp_hash_list
      else:
         list_of_branches.append(file_name)
         f = open(directory + "/" + file_name, "r")
         if f.mode == 'r':
            list_of_hashes.append(f.read())
            f.close()
         else:
            print("reading of file " + file_name + " has failed!")

   return list_of_branches, list_of_hashes;

def create_branch_dict(list_of_branch, list_of_hash):
   hash_branches_pairs = dict()

   for i in range(len(list_of_branch)):
      if list_of_hash[i][-1] == "\n":
         list_of_hash[i] = list_of_hash[i][:-1]
      if list_of_hash[i] in hash_branches_pairs:
         hash_branches_pairs[list_of_hash[i]].append(list_of_branch[i])
      else:
         hash_branches_pairs.update({list_of_hash[i] : [list_of_branch[i]]})

   return hash_branches_pairs

class CommitNode:
   def __init__(self, commit_hash):
      """
      :type commmit_hash: str
      """
      self.commit_hash = commit_hash
      self.parents = set()
      self.children = set()

def obtain_parent_hashes(git_dir, hash_value):

   parent_hashes = set()
   full_file_access_path = git_directory + "/objects/" + hash_value[:2] + "/" + hash_value[2:]

   f = open(full_file_access_path, "rb")
   compressed_data = f.read()
   f.close()

   decompressed_data = zlib.decompress(compressed_data)
   nice_decompressed_data = decompressed_data.decode("utf-8")
   nice_decompressed_data = nice_decompressed_data.splitlines()

   for line in nice_decompressed_data:
      if line[:6] == "parent" and len(line) == 47:
         parent_hashes.add(line[7:])
   return parent_hashes

def build_commit_graph(git_dir, local_branch_heads):
   commit_nodes = dict()
   root_hashes = set() 
   visited = set()
   stack = local_branch_heads

   while stack:
      commit_hash = stack[0]
      stack.pop(0)
      if commit_hash in visited:
         continue

      visited.add(commit_hash)

      if commit_hash not in commit_nodes:
         commit_nodes.update({commit_hash : CommitNode(commit_hash)})
      
      commit_node = CommitNode(commit_hash)
      commit_node.parents = obtain_parent_hashes(git_dir, commit_hash)

      if not commit_node.parents:
         root_hashes.add(commit_hash)

      for p in commit_node.parents:
         if p not in visited:
            stack.append(p)
         if p not in commit_nodes:
            commit_nodes.update({p : CommitNode(p)})
            
         commit_nodes[commit_hash].parents.add(p)
         commit_nodes[p].children.add(commit_hash)

   return commit_nodes, root_hashes

def get_topo_ordered_commits(commit_nodes, root_hashes):
   order = []
   visited = set()
   temp_stack = []
   stack = sorted(root_hashes)

   while stack:
      v = stack.pop()
      if v in visited:
         continue

      visited.add(v)
      while len(temp_stack) != 0 and v not in commit_nodes[temp_stack[len(temp_stack)-1]].children:
         g = temp_stack.pop()
         order.append(g)

      temp_stack.append(v)

      for c in sorted(commit_nodes[v].children):
         if c in visited:
            continue 
         stack.append(c)

   while len(temp_stack) != 0:
      g = temp_stack.pop()
      order.append(g)

   return order

def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches):
   jumped = False

   for i in range((len(topo_ordered_commits))):
      commit_hash = topo_ordered_commits[i]

      if jumped:
         jumped = False
         sticky_hash = ' '.join(commit_nodes[commit_hash].children)
         print(f'={sticky_hash}')
      
      if commit_hash in head_to_branches:
         branches = sorted(head_to_branches[commit_hash])
      else:
         branches = ""

      if branches:
         print(commit_hash + ' ' + ' '.join(branches))
      else:
         print(commit_hash + ' ')

      if i + 1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
         jumped = True
         sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
         print(f'{sticky_hash}=\n')
                  
search_successful, git_directory = find_git_dir()

if search_successful == False:
   print("Not inside a git repository")
   sys.exit(1)

branch_directory = git_directory + "/refs/heads"
branch_list, hash_list = get_branch_names(branch_directory)
hash_branches_pairs = create_branch_dict(branch_list, hash_list)
list_of_parents = list()

commit_nodes, root_hashes = build_commit_graph(git_directory, list(hash_branches_pairs.keys()))

topo_ordered_commits = get_topo_ordered_commits(commit_nodes, root_hashes)

print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, hash_branches_pairs)
