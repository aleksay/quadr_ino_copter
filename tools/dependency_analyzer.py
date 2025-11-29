import os
import re
import argparse
from collections import defaultdict
import webbrowser
import tempfile

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
            content = re.sub(r'//.*?$|/\*.*?\*/', '', content, flags=re.MULTILINE|re.re.DOTALL)
            
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

def create_dot_file(graph, file_info, main_files, output_file='dependencies'):
    """Crea un file DOT per la visualizzazione delle dipendenze"""
    dot_content = ['digraph Dependencies {']
    dot_content.append('    rankdir="TB";')
    dot_content.append('    splines="ortho";')
    dot_content.append('    node [shape=box, style=filled, fontname="Arial"];')
    dot_content.append('')
    
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
        
        # Crea un ID sicuro per il nodo
        node_id = file_path.replace('\\', '_').replace('/', '_').replace('.', '_')
        dot_content.append(f'    {node_id} [label="{label}", fillcolor="{color}"];')
    
    dot_content.append('')
    
    # Aggiungi archi
    for source, targets in graph.items():
        source_id = source.replace('\\', '_').replace('/', '_').replace('.', '_')
        for target in targets:
            if target in graph:  # Solo se il target è nel grafo
                target_id = target.replace('\\', '_').replace('/', '_').replace('.', '_')
                dot_content.append(f'    {source_id} -> {target_id};')
    
    dot_content.append('}')
    
    # Salva il file DOT
    dot_filename = f"{output_file}.dot"
    with open(dot_filename, 'w', encoding='utf-8') as f:
        f.write('\n'.join(dot_content))
    
    return dot_filename

def create_html_visualization(graph, file_info, main_files, output_file='dependencies'):
    """Crea una visualizzazione HTML interattiva usando vis.js"""
    html_content = '''
<!DOCTYPE html>
<html>
<head>
    <title>Dipendenza C/C++</title>
    <script type="text/javascript" src="https://unpkg.com/vis-network/standalone/umd/vis-network.min.js"></script>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        #mynetwork { width: 100%; height: 800px; border: 1px solid #ccc; }
        .legend { margin: 20px 0; padding: 10px; background: #f5f5f5; border-radius: 5px; }
        .legend-item { display: inline-block; margin-right: 20px; }
        .color-box { display: inline-block; width: 20px; height: 20px; margin-right: 5px; vertical-align: middle; }
    </style>
</head>
<body>
    <h1>Analisi Dipendenze C/C++</h1>
    <div class="legend">
        <div class="legend-item"><div class="color-box" style="background-color:#E1F5FE;"></div>File Sorgente</div>
        <div class="legend-item"><div class="color-box" style="background-color:#E8F5E8;"></div>Header File</div>
        <div class="legend-item"><div class="color-box" style="background-color:#FFEBEE;"></div>Contiene Main</div>
    </div>
    <div id="mynetwork"></div>
    
    <script type="text/javascript">
        var nodes = new vis.DataSet([
'''
    
    # Aggiungi nodi
    nodes = []
    for file_path in graph.keys():
        filename = file_info[file_path]['filename']
        file_ext = os.path.splitext(filename)[1].lower()
        
        # Determina il colore in base al tipo di file e alla presenza di main
        if file_path in main_files:
            color = '#FFEBEE'
            border = '#F44336'
            title = f"{filename}\\nContiene la funzione main\\nPercorso: {file_path}"
        elif file_ext in ['.h', '.hpp', '.hh', '.hxx']:
            color = '#E8F5E8'
            border = '#4CAF50'
            title = f"{filename}\\nHeader file\\nPercorso: {file_path}"
        else:
            color = '#E1F5FE'
            border = '#2196F3'
            title = f"{filename}\\nFile sorgente\\nPercorso: {file_path}"
        
        nodes.append(f"            {{id: '{file_path}', label: '{filename}', color: {{background: '{color}', border: '{border}'}}, title: '{title}'}}")
    
    html_content += ',\n'.join(nodes)
    
    html_content += '''
        ]);

        var edges = new vis.DataSet([
'''
    
    # Aggiungi archi
    edges = []
    for source, targets in graph.items():
        for target in targets:
            if target in graph:  # Solo se il target è nel grafo
                edges.append(f"            {{from: '{source}', to: '{target}', arrows: 'to'}}")
    
    html_content += ',\n'.join(edges)
    
    html_content += '''
        ]);

        var container = document.getElementById('mynetwork');
        var data = {
            nodes: nodes,
            edges: edges
        };
        var options = {
            layout: {
                hierarchical: {
                    direction: 'UD',
                    sortMethod: 'directed'
                }
            },
            physics: {
                enabled: true
            },
            interaction: {
                dragNodes: true,
                dragView: true,
                zoomView: true
            }
        };
        var network = new vis.Network(container, data, options);
    </script>
</body>
</html>
'''
    
    # Salva il file HTML
    html_filename = f"{output_file}.html"
    with open(html_filename, 'w', encoding='utf-8') as f:
        f.write(html_content)
    
    return html_filename

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
    parser.add_argument('--format', choices=['dot', 'html', 'both'], default='html',
                       help='Formato di output: dot (DOT file), html (HTML interattivo), both (entrambi)')
    parser.add_argument('--open-browser', action='store_true',
                       help='Apri automaticamente il file HTML nel browser')
    
    args = parser.parse_args()
    
    if not os.path.isdir(args.directory):
        print(f"Errore: La directory '{args.directory}' non esiste!")
        exit(1)
    
    print(f"Analisi della directory: {args.directory}")
    print("Ricerca file C/C++...")
    
    graph, file_info, main_files = build_dependency_graph(args.directory)
    
    print(f"Trovati {len(file_info)} file sorgente/header")
    print(f"Trovati {len(main_files)} file con funzione main")
    
    if not file_info:
        print("Nessun file C/C++ trovato nella directory specificata.")
        exit(0)
    
    # Genera i file richiesti
    output_files = []
    
    if args.format in ['dot', 'both']:
        dot_file = create_dot_file(graph, file_info, main_files, args.output)
        output_files.append(dot_file)
        print(f"\nFile DOT generato: {dot_file}")
        print("Puoi visualizzarlo con:")
        print("  - Tool online: http://www.webgraphviz.com/")
        print("  - Convertirlo con: dot -Tpng -o output.png input.dot")
    
    if args.format in ['html', 'both']:
        html_file = create_html_visualization(graph, file_info, main_files, args.output)
        output_files.append(html_file)
        print(f"\nFile HTML generato: {html_file}")
        print("Apri il file nel browser per visualizzare il grafico interattivo.")
        
        if args.open_browser:
            webbrowser.open('file://' + os.path.abspath(html_file))
    
    print_main_files(main_files)
    print_dependency_summary(file_info)
    
    print(f"\nFile generati: {', '.join(output_files)}")
