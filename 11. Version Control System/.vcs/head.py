import argparse
import os
import sys
import shutil

parser = argparse.ArgumentParser(description='Revert to a commit')
parser.add_argument('versionid', metavar='<version-id>', type=str,
                    help='Commit to revert to', default='')

args = parser.parse_args()
vid = args.versionid
repo_name = os.getcwd()
dir_name = os.path.split(os.getcwd())[1]

if not (os.path.exists(repo_name + '/.repo') and os.path.exists(repo_name + '/.vcs')):
    print('Repository does not exist in ' + repo_name)
    sys.exit(1)
else:
    if not os.path.exists(repo_name + '/.repo/snapshots/' + vid):
        print('Commit does not exist in ' + repo_name)
        sys.exit(2)
    else:
        print('Reverting to commit ' + vid)
        for src in os.listdir(repo_name):
            if os.path.isdir(repo_name + '/' + src) and src != '.repo' and src != '.vcs':
                shutil.rmtree(repo_name + '/' + src)
            elif os.path.isfile(repo_name + '/' + src):
                os.remove(repo_name + '/' + src)
        for dst in os.listdir(repo_name + '/.repo/snapshots/' + vid):
            if os.path.isdir(repo_name + '/.repo/snapshots/' + vid + '/' + dst):
                shutil.copytree(repo_name + '/.repo/snapshots/' + vid + '/' + dst, repo_name + '/' + dst)
            elif os.path.isfile(repo_name + '/.repo/snapshots/' + vid + '/' + dst) and dst != '.commit':
                shutil.copy(repo_name + '/.repo/snapshots/' + vid + '/' + dst, repo_name)
        sys.exit(0)