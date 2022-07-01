import argparse
import os
import sys
import shutil

parser = argparse.ArgumentParser(description='Init a repository')
parser.add_argument('Directory', metavar='<directory>',
                    type=str, help='Directory to init')

args = parser.parse_args()
dir = args.Directory
commit = os.path.dirname(os.path.abspath('.vcs\commit.py')) + '\\commit.py'
log = os.path.dirname(os.path.abspath('.vcs\log.py')) + '\\log.py'
print(commit)

if os.path.isfile(dir):
    print('{} is a file'.format(dir))
    sys.exit(1)

if not os.path.exists(dir):
    os.makedirs(dir)

if not (os.path.exists(dir + '/.repo') and os.path.exists(dir + '/.vcs')):
    os.chdir(dir)
    os.makedirs('.repo/refs/tags')
    os.makedirs('.repo/snapshots')
    os.makedirs('.vcs')
    shutil.copyfile(commit, '.vcs/commit.py')
    shutil.copyfile(log, '.vcs/log.py')
    print('Initialized repository in ' + dir)
    sys.exit(0)
else:
    print('Repository already exists in ' + dir)
    sys.exit(2)
