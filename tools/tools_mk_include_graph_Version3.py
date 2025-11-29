import os
import re
import graphviz
from collections import defaultdict, deque
import argparse

def find_source_files(directory, extensions):
    """Trova ricorsivamente tutti i file sorgente C/C++"""
    source_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                source_files.append(os.path.join(root, file))
    return source_files

def extract_includes(file_path):
    """Estrae tutte le direttive #include da un file"""
    includes = []
    include_pattern = re.compile(r'^\s*#include\s*[<"]([^>"]+)[>"]')
    
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            for line in f:
                match = include_pattern.match(line)
                if match:
                    includes.append(match.group(1))
    except Exception as e:
        print(f"Errore nella lettura di {file_path}: {e}")
    
    return includes

def has_main_function(file_path):
    """Verifica se il file contiene una funzione main"""
    main_patterns = [
        re.compile(r'\bint\s+main\s*\('),
        re.compile(r'\bvoid\s+main\s*\('),
        re.compile(r'\bmain\s*\(')
    ]
    
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            # Rimuove commenti per evitare falsi positivi
            content = re.sub(r'//.*?$|/\*.*?\*/', '', content, flags=re.MULTILINE|re.DOTALL)
            
            for pattern in main_patterns:
                if pattern.search(content):
                    return True
    except Exception as e:
        print(f"Errore nella verifica main di {file_path}: {e}")
    
    return False

def build_dependency_graph(directory):
    """Costruisce il grafico delle dipendenze"""
    extensions = ['.c', '.cpp', '.cc', '.cxx', '.h', '.hpp', '.hh', '.hxx']
    files = find_source_files(directory, extensions)
    
    # Mappa nome file -> percorso completo
    file_map = {}
    for file_path in files:
        filename = os.path.basename(file_path)
        file_map[filename] = file_path
        # Aggiungi anche senza estensione per matching più flessibile
        name_without_ext = os.path.splitext(filename)[0]
        if name_without_ext not in file_map:
            file_map[name_without_ext] = file_path
    
    graph = defaultdict(list)
    main_files = []
    file_info = {}
    
    for file_path in files:
        filename = os.path.basename(file_path)
        includes = extract_includes(file_path)
        
        file_info[file_path] = {
            'filename': filename,
            'includes': includes,
            'has_main': has_main_function(file_path)
        }
        
        if file_info[file_path]['has_main']:
            main_files.append(file_path)
        
        # Collega il file corrente con i file inclusi
        for included_file in includes:
            # Prova a trovare il file incluso nella mappa
            included_path = None
            
            # Cerca per nome completo
            if included_file in file_map:
                included_path = file_map[included_file]
            else:
                # Cerca per nome base
                base_name = os.path.basename(included_file)
                if base_name in file_map:
                    included_path = file_map[base_name]
            
            if included_path and included_path != file_path:
                graph[file_path].append(included_path)
    
    return graph, file_info, main_files

def create_dependency_visualization(graph, file_info, main_files, output_file='dependencies'):
    """Crea la visualizzazione delle dipendenze"""
    dot = graphviz.Digraph(comment='Dipendenza C/C++', 
                          format='png',
                          engine='dot')
    
    dot.attr(rankdir='TB', splines='ortho')
    dot.attr('node', shape='box', style='filled', fontname='Arial')
    
    # Colori per diversi tipi di file
    color_source = '#E1F5FE'  # Azzurro per file sorgente
    color_header = '#E8F5E8'  # Verde chiaro per header
    color_main = '#FFEBEE'    # Rosso chiaro per file con main
    
    # Aggiungi nodi
    for file_path in graph.keys():
        filename = file_info[file_path]['filename']
        file_ext = os.path.splitext(filename)[1].lower()
        
        # Determina il colore in base al tipo di file e alla presenza di main
        if file_path in main_files:
            color = color_main
            label = f'{filename}\\n(contiene main)'
        elif file_ext in ['.h', '.hpp', '.hh', '.hxx']:
            color = color_header
            label = filename
        else:
            color = color_source
            label = filename
        
        dot.node(file_path, label, fillcolor=color)
    
    # Aggiungi archi
    for source, targets in graph.items():
        for target in targets:
            if target in graph:  # Solo se il target è nel grafo
                dot.edge(source, target)
    
    # Salva il file
    dot.render(output_file, cleanup=True)
    print(f"Grafico salvato come: {output_file}.png")
    print(f"File DOT salvato come: {output_file}")

def print_main_files(main_files):
    """Stampa la lista dei file che contengono main"""
    if main_files:
        print("\n" + "="*50)
        print("FILE CHE CONTENGONO LA FUNZIONE MAIN:")
        print("="*50)
        for i, file_path in enumerate(main_files, 1):
            print(f"{i}. {file_path}")
    else:
        print("\nNessun file contenente la funzione main trovato.")

def print_dependency_summary(file_info):
    """Stampa un riepilogo delle dipendenze"""
    print("\n" + "="*50)
    print("RIEPILOGO DEPENDENZE:")
    print("="*50)
    
    for file_path, info in file_info.items():
        print(f"\n{os.path.basename(file_path)}:")
        print(f"  - Include: {', '.join(info['includes']) if info['includes'] else 'Nessuna'}")
        print(f"  - Main: {'SÌ' if info['has_main'] else 'no'}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Analizza le dipendenze C/C++ e genera un grafico')
    parser.add_argument('directory', help='Directory da analizzare')
    parser.add_argument('-o', '--output', default='dependencies', 
                       help='Nome file output (senza estensione)')
    
    args = parser.parse_args()
    
    if not os.path.isdir(args.directory):
        print(f"Errore: La directory '{args.directory}' non esiste!")
        exit(1)
    
    print(f"Analisi della directory: {args.directory}")
    print("Ricerca file C/C++...")
    
    graph, file_info, main_files = build_dependency_graph(args.directory)
    
    print(f"Trovati {len(file_info)} file sorgente/header")
    print(f"Trovati {len(main_files)} file con funzione main")
    
    if file_info:
        create_dependency_visualization(graph, file_info, main_files, args.output)
        print_main_files(main_files)
        print_dependency_summary(file_info)
    else:
        print("Nessun file C/C++ trovato nella directory specificata.")
