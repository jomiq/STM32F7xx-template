#! /bin/python2
import subprocess, argparse, sys, pprint, json
from os import path

# From https://gist.github.com/tjoslin/e40a9361d384f8ed1d37f0618487980a
# Read the output of Chibios build to generate .vscode/c_cpp_properties.json
# E.g. make | python2 c_cpp_properties.py -C arm-none-eabi-gcc > .vscode/c_cpp_properties.json


parser = argparse.ArgumentParser()
parser.add_argument('-C', '--compiler', dest='compiler', default='gcc')
parser.add_argument('-x', '--language', dest='language', default='c++')
parser.add_argument('--configuration', dest='configuration', default='Linux')
parser.add_argument('--intellisense-mode', dest='intelliSenseMode', default='clang-x64')

cc_parser = argparse.ArgumentParser()
cc_parser.add_argument('-I', dest='includePath', action='append', default=[])
cc_parser.add_argument('-D', dest='defines', action='append')

def main(args = None):
    if args is None:
        args = sys.argv

    args = parser.parse_args(args[1:])
    includePath, defines = parseCommandLine(args)
    includePath  = compilerIncludePath(args) + includePath
    json.dump({ 
        'configurations': [
            {
                "name": args.configuration,
                "includePath": includePath,
                "browse": {
                    "limitSymbolsToIncludedHeaders": True,
                    "databaseFilename": "",
                    "path": includePath,
                    "defines": defines,
                    "intelliSenseMode": "clang-x64"
                }
            }
        ],
        "version": 3
    },
    sys.stdout,
    indent=4)

def compilerIncludePath(args):
    compiler = subprocess.Popen(
        [args.compiler, '-v', '-x', args.language, '-E', '-' ], 
        stdin = subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    includeSection = False
    includePath = []
    out, err = compiler.communicate(input='')
    for line in err.splitlines():
        if includeSection and line.startswith(' '):
            includePath.append(path.abspath(line.strip()))
        elif line.startswith('#include'):
            includeSection = True
        elif line.startswith('End'):
            includeSection = False
    return includePath

    while not next(line).starts_with('#include'):
        pass
    
    

def parseCommandLine(args):
    # This is naive, since include order can matter, but I'm going
    # to assume it doesn't for now
    includePath = set()
    defines = {}

    for line in sys.stdin.readlines():
        cc_args = line.split()
        if len(cc_args) and cc_args[0] == args.compiler:
            cc_args, unknown =  cc_parser.parse_known_args(cc_args[1:])
            includePath |= set(cc_args.includePath)
            for define in cc_args.defines:
                parts = define.split('=')
                if len(parts) == 1:
                    defines[parts[0]] = True
                elif len(parts) == 2:
                    name, value = parts
                    defines[name] = value
    return list(includePath), defines


if __name__ == "__main__":
    main()
