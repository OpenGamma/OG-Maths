#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

import sys
import xml.etree.ElementTree as ET

import argparse
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('-o', '--output', help='Output file name', required=True)
parser.add_argument('files', nargs='+')
args = parser.parse_args()

stats = { 'disabled': 0,
          'tests':    0,
          'errors':   0,
          'time':     0,
          'failures': 0,
          'name':     'AllTests' }

roots = []

# Input XML and aggregate counts
for filename in args.files:
    tree = ET.parse(filename)
    root = tree.getroot()
    for item, count in root.items():
        if item == 'name':
            continue
        if item == 'time':
            value = float(count)
        else:
            value = int(count)
        stats[item] += value
    roots.append(root)

# Convert stats to string values for serialisation
for k, v in stats.iteritems():
    stats[k] = str(v)

# Create new root
newroot = ET.Element('testsuites', stats)

# Add the elements from each input root to the new root
for root in roots:
    for element in list(root):
        newroot.append(element)

# Write the output
newtree = ET.ElementTree(newroot)
newtree.write(args.output)
