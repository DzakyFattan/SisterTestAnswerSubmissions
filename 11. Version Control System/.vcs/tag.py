import argparse
import os
import sys

parser = argparse.ArgumentParser(description='Add a tag to a commit')
parser.add_argument('Commit', metavar='<commit>', type=str,
                    help='Commit to tag', default='')
parser.add_argument('Tag', metavar='<tag>',
                    type=str, help='Tag that refers to a commit', default='')

args = parser.parse_args()
tag = args.Tag
commit = args.Commit
repo_name = os.getcwd()

if not os.path.exists(repo_name + '/.repo') and os.path.exists(repo_name + '/.vcs'):
    print('Repository does not exist in ' + repo_name)
    sys.exit(1)

if not os.path.exists(repo_name + '/.repo/snapshots/' + commit):
    print('Commit does not exist in ' + repo_name)
    sys.exit(2)
else:
    print('Tagging commit ' + commit + ' with tag "' + str(tag) + '"')
    lines = []
    with open('.repo/snapshots/' + str(commit) + '/.commit', 'r') as fin:
        for line in fin:
            if line.startswith('tag: '):
                print('Replacing Tag')
                continue
            lines.append(line)
    with open('.repo/snapshots/' + str(commit) + '/.commit', 'w') as fout:
        for line in lines:
            fout.write(line)
        fout.write('tag: ' + str(tag) + '\n')
    for sym in os.listdir(repo_name + '/.repo/refs/tags/'):
        if sym.startswith(str(commit)):
            os.unlink(repo_name + '/.repo/refs/tags/' + sym)
    os.symlink('.repo/snapshots/' + str(commit), repo_name + '/.repo/refs/tags/' + str(commit) + '_' + str(tag))
    sys.exit(0)