#!/usr/bin/env python3
"""
Scans repository for #include and produces:
 - includes.dot (Graphviz)
 - includes.csv
 - detects cycles and prints topological order if acyclic
"""
import os
import re
from collections import defaultdict, deque

ROOT = './src'
#BUILD = './'
INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"]([^">]+)[">]')

# file extensions to scan
EXTS = ( 'cpp','.cc', '.cxx', '.h', '.hpp', '.ino')

def find_sources(root):
    files=[]
    for dirpath, dirs, filenames in os.walk(root):
        # skip .git and build dirs
        if '.git' in dirpath.split(os.sep): continue
        if dirpath.startswith('./build') or dirpath.startswith('./.pio'): continue
        for f in filenames:
            if f.endswith(EXTS):
                files.append(os.path.join(dirpath, f))
    return files

def parse_includes(files):
    includes = defaultdict(set)
    filenames_map = {}
    for f in files:
        try:
            with open(f, 'r', encoding='utf-8', errors='ignore') as fh:
                for ln, line in enumerate(fh,1):
                    m = INCLUDE_RE.match(line)
                    if m:
                        inc = m.group(1)
                        includes[f].add(inc)
            filenames_map[os.path.basename(f)] = f
        except Exception as e:
            print(f"Warning reading {f}: {e}")
    return includes, filenames_map

def resolve_include_target(inc, filenames_map):
    # try to resolve include name to a repository file path when possible
    base = os.path.basename(inc)
    return filenames_map.get(base, inc)  # if not found, leave as external include

def build_graph(includes, filenames_map):
    nodes = set()
    edges = []
    for src, incs in includes.items():
        nodes.add(src)
        for inc in incs:
            tgt = resolve_include_target(inc, filenames_map)
            edges.append((src, tgt))
            nodes.add(tgt)
    return nodes, edges

def write_dot(nodes, edges, path='includes.dot'):
    with open(path, 'w') as fh:
        fh.write('digraph includes {\n')
        for n in sorted(nodes):
            label = os.path.basename(n)
            fh.write(f'  "{n}" [label="{label}"];\n')
        for a,b in edges:
            fh.write(f'  "{a}" -> "{b}";\n')
        fh.write('}\n')
    print(f"Wrote {path}")

def write_csv(edges, path='includes.csv'):
    with open(path, 'w') as fh:
        fh.write('from,to\n')
        for a,b in edges:
            fh.write(f'"{a}","{b}"\n')
    print(f"Wrote {path}")

def detect_cycles(nodes, edges):
    # Build adjacency limited to repo-resolved nodes
    adj = defaultdict(list)
    in_degree = defaultdict(int)
    for a,b in edges:
        if b.startswith('.') or b.startswith('/'):  # resolved to file path
            adj[a].append(b)
            in_degree[b] += 1
            if a not in in_degree:
                in_degree[a] = in_degree.get(a, 0)
    # Kahn's algorithm for topological sort
    q = deque([n for n in in_degree if in_degree[n]==0])
    order=[]
    while q:
        n=q.popleft()
        order.append(n)
        for m in adj.get(n,()):
            in_degree[m]-=1
            if in_degree[m]==0:
                q.append(m)
    # if order covers all nodes with in_degree records it's acyclic
    if len(order) != len(in_degree):
        return True, order  # has cycles
    return False, order

def main():
    files = find_sources(ROOT)
    includes, filenames_map = parse_includes(files)
    nodes, edges = build_graph(includes, filenames_map)
    write_dot(nodes, edges, './tools/include.dot')
    #write_csv(edges)
    has_cycles, order = detect_cycles(nodes, edges)
    if has_cycles:
        print("Cycles detected in includes (partial order shown):")
    else:
        print("No cycles detected. Topological order (leaf first):")
    #for p in order:
    #    print(p)

if __name__ == '__main__':
    main()
