#!/usr/bin/env python3

import sys
import re
import optparse
import subprocess

CXX = 'clang++'
PLUGIN = 'orthodoxy.so'

def main():
    global CXX
    global PLUGIN

    parser = optparse.OptionParser()
    parser.add_option("--cxx")
    parser.add_option("--plugin")
    opts, args = parser.parse_args()

    if opts.cxx is not None: CXX = opts.cxx
    if opts.plugin is not None: PLUGIN = opts.plugin

    success = True
    for src in args: success = process(src) and success

    if not success:
        sys.exit(1)

def get_actual_diagnostics(src):
    diags = set()
    for line in src.split('\n'):
        m = re.search(r'.*:([0-9]+):[0-9]+: warning: .* \[orthodoxy::(.*)\]', line)
        if m is not None: diags.add((int(m.group(1), 10), m.group(2)))
    return diags

def get_expected_diagnostics(src):
    diags = set()
    lineno = 0
    for line in open(src, 'r').readlines():
        lineno += 1
        m = re.search(r'EXPECT[(]([^)]*)[)]', line)
        if m is not None: diags.add((lineno, m.group(1)))
    return diags

def process(src):
    cmd = [CXX, "-fplugin=" + PLUGIN,
           "-fplugin-arg-orthodoxy--warn-only",
           "-fsyntax-only", "--", src]

    proc = subprocess.run(cmd, stderr=subprocess.PIPE, text=True)
    proc.check_returncode()

    actual = get_actual_diagnostics(proc.stderr)
    expected = get_expected_diagnostics(src)

    sys.stderr.write("\tEXPECTED: %s\n" % (expected))
    sys.stderr.write("\tACTUAL: %s\n" % (actual))
    return actual == expected

if __name__ == '__main__':
    main()
