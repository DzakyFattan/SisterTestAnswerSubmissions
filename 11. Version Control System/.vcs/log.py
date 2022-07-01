import os
import sys
import shutil

repo_name = os.getcwd()

if not (os.path.exists(repo_name + '/.repo') and os.path.exists(repo_name + '/.vcs')):
    print('Repository does not exist in ' + repo_name)
    sys.exit(1)
else:
    snaps = os.listdir(repo_name + '/.repo/snapshots')
    for snap in snaps:
        print('snapshot: ' + snap)
        with open(repo_name + '/.repo/snapshots/' + snap + '/.commit', 'r') as f:
            print(f.read())
    sys.exit(0)