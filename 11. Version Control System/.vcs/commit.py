import argparse
import datetime
import os
import sys
import shutil

parser = argparse.ArgumentParser(description='Commit a change')
parser.add_argument('Message', metavar='<message>',
                    type=str, help='Message, a summary to the changes that has been commited', default='')

args = parser.parse_args()
message = args.Message
repo_name = os.getcwd()

if not (os.path.exists(repo_name + '/.repo') and os.path.exists(repo_name + '/.vcs')):
    print('Repository does not exist in ' + repo_name)
    sys.exit(1)
else:
    print('Committing changes in ' + repo_name)
    i = 0
    while os.path.exists('.repo/snapshots/' + str(i)):
      i += 1
    shutil.copytree(repo_name, '.repo/snapshots/' + str(i), ignore=shutil.ignore_patterns('*.vcs', '*.repo'))
    with open('.repo/snapshots/' + str(i) + '/.commit', 'w') as f:
        f.write('username: ' + os.getlogin() + '\n')
        f.write('date: ' + str(datetime.datetime.now().strftime('%a, %b %d %Y, %H:%M:%S')) + '\n')
        f.write('message: ' + message + '\n')
    sys.exit(0)