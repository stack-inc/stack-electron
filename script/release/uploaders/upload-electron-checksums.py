#!/usr/bin/env python

import argparse
import hashlib
import os
import shutil
import sys
import tempfile

sys.path.append(
  os.path.abspath(os.path.dirname(os.path.abspath(__file__)) + "/../.."))

from lib.util import download, rm_rf, safe_mkdir

DIST_URL = 'https://github.com/stack-inc/stack-electron/releases/download/'


def main():
  args = parse_args()
  dist_url = args.dist_url
  if dist_url[-1] != "/":
    dist_url += "/"

  url = dist_url + args.version + '/'
  directory, files = download_files(url, get_files_list(args.version))
  checksums = [
    create_checksum('sha1', directory, 'SHASUMS.txt', files),
    create_checksum('sha256', directory, 'SHASUMS256.txt', files)
  ]

  if args.target_dir is not None:
    copy_files(checksums, args.target_dir)

  rm_rf(directory)


def parse_args():
  parser = argparse.ArgumentParser(description='upload sumsha file')
  parser.add_argument('-v', '--version', help='Specify the version',
                      required=True)
  parser.add_argument('-u', '--dist-url',
                      help='Specify the dist url for downloading',
                      required=False, default=DIST_URL)
  parser.add_argument('-t', '--target-dir',
                      help='Specify target dir of checksums',
                      required=False)
  return parser.parse_args()

def get_files_list(version):
  return [
    { "filename": 'electron-api.json', "required": True },
    { "filename": 'electron-{0}-darwin-arm64.zip'.format(version), "required": True },
    { "filename": 'electron-{0}-darwin-x64.zip'.format(version), "required": True },
    { "filename": 'electron-{0}-win32-x64.zip'.format(version), "required": True },
    { "filename": 'electron.d.ts', "required": True },
  ]


def download_files(url, files):
  directory = tempfile.mkdtemp(prefix='electron-tmp')
  result = []
  for optional_f in files:
    required = optional_f['required']
    f = optional_f['filename']
    try:
      result.append(download(f, url + f, os.path.join(directory, f)))
    except Exception:
      if required:
        raise

  return directory, result


def create_checksum(algorithm, directory, filename, files):
  lines = []
  for path in files:
    h = hashlib.new(algorithm)
    with open(path, 'rb') as f:
      h.update(f.read())
      lines.append(h.hexdigest() + ' *' + os.path.relpath(path, directory))

  checksum_file = os.path.join(directory, filename)
  with open(checksum_file, 'w') as f:
    f.write('\n'.join(lines) + '\n')
  return checksum_file

def copy_files(source_files, output_dir):
  for source_file in source_files:
    output_path = os.path.join(output_dir, os.path.basename(source_file))
    safe_mkdir(os.path.dirname(output_path))
    shutil.copy2(source_file, output_path)

if __name__ == '__main__':
  sys.exit(main())
