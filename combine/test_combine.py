#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

"""Tests for the verinfo reading and combining."""

import unittest
from copy import deepcopy
from combine import read_version_info, validate_version_info, construct_combined_verinfo

class TestVerinfo(unittest.TestCase):

    def test_read_version_info(self):
        self.maxDiff = None
        verinfos = read_version_info(['test-verinfo.zip'])
        # Compare read dicts. This is a little fiddly because we are stuck on 2.6 with no
        # assertSequenceEqual.
        # Check we read one verinfo
        self.assertEqual(1, len(verinfos.keys()))
        # Get that verinfo and check the length of its keys
        verinfo = verinfos[verinfos.keys()[0]]
        self.assertEqual(9, len(verinfo.keys()))
        # Check root keys
        for k in ('artifacts', 'buildnumber', 'platforms', 'project', 'revision', 'version', \
                  'timestamp', 'localtime'):
            self.assertEqual(VERINFO_LNX_REFERENCE[k], verinfo[k])
        # Check keys in subprojects
        for k in ('buildnumber', 'platforms', 'project', 'revision', 'timestamp', 'localtime'):
            self.assertEqual(VERINFO_LNX_REFERENCE['subprojects'][0][k], verinfo['subprojects'][0][k])

    def test_validate_version_info(self):
        validate_version_info(VERINFO_REFERENCES)

    def test_invalid_version_info(self):
        def make_bad(key):
            refs = deepcopy(VERINFO_REFERENCES)
            refs['lnx'][key] = 'bad'
            return refs

        self.assertRaises(ValueError, validate_version_info, make_bad('project'))
        self.assertRaises(ValueError, validate_version_info, make_bad('version'))
        self.assertRaises(ValueError, validate_version_info, make_bad('revision'))

        def make_bad_subproject(key):
            refs = deepcopy(VERINFO_REFERENCES)
            refs['lnx']['subprojects'][0][key] = 'bad'
            return refs

        self.assertRaises(ValueError, validate_version_info, make_bad_subproject('project'))
        self.assertRaises(ValueError, validate_version_info, make_bad_subproject('revision'))

    def test_combine_version_info(self):
        combined = construct_combined_verinfo(VERINFO_REFERENCES, 1)
        # Again, a fiddly dict compare:
        self.assertEqual(9, len(combined.keys()))
        for k in ('project', 'revision', 'version', 'artifacts', 'buildnumber'):
            self.assertEquals(VERINFO_COMBINED_REFERENCE[k], combined[k])
        # Subproject check
        self.assertEquals(1, len(combined['subprojects'].keys()))
        self.assertEquals(VERINFO_COMBINED_REFERENCE['subprojects']['OG-Lapack'],
                          combined['subprojects']['OG-Lapack'])

        def combined_key_check(ckey):
            """Checks a ckey that takes values from all the platforms."""
            self.assertEquals(4, len(combined[ckey].keys()))
            for k in ('lnx', 'osx', 'win'):
                self.assertEquals(VERINFO_COMBINED_REFERENCE[ckey][k],
                                  combined[ckey][k])
                self.assertEquals(VERINFO_COMBINED_REFERENCE[ckey]['subprojects']['OG-Lapack'][k],
                                  combined[ckey]['subprojects']['OG-Lapack'][k])

        combined_key_check('buildnumbers')
        combined_key_check('timestamps')
        combined_key_check('localtimes')

VERINFO_LNX_REFERENCE = \
{'artifacts': ['jars/og-maths-lnx-0.1.0-SNAPSHOT.jar',
               'jars/og-maths-lnx-0.1.0-SNAPSHOT-javadoc.jar',
               'jars/og-maths-lnx-0.1.0-SNAPSHOT-sources.jar',
               'jars/og-maths-lnx-0.1.0-SNAPSHOT-tests.jar'],
 'buildnumber': 25,
 'localtime': '2014/04/30 11:37:10',
 'platforms': ['lnx'],
 'project': 'OG-Maths',
 'revision': 'a57fd89cfcb8b2c6711f6ec8bac19bb339cc24bc',
 'subprojects': [{'buildnumber': 9,
                  'localtime': '2014/04/30 11:15:52',
                  'platforms': ['lnx'],
                  'project': 'OG-Lapack',
                  'timestamp': 1398852952.053299,
                  'revision': '78bb7b167aea5f8b52015610d3ad69f360a84493'}],
 'timestamp': 1398854230.495819,
 'version': '0.1.0-SNAPSHOT'}

VERINFO_OSX_REFERENCE = \
{'artifacts': ['jars/og-maths-osx-0.1.0-SNAPSHOT.jar',
               'jars/og-maths-osx-0.1.0-SNAPSHOT-javadoc.jar',
               'jars/og-maths-osx-0.1.0-SNAPSHOT-sources.jar',
               'jars/og-maths-osx-0.1.0-SNAPSHOT-tests.jar'],
 'buildnumber': 6,
 'localtime': '2014/04/30 11:37:11',
 'platforms': ['osx'],
 'project': 'OG-Maths',
 'revision': 'a57fd89cfcb8b2c6711f6ec8bac19bb339cc24bc',
 'subprojects': [{'buildnumber': 9,
                  'localtime': '2014/04/30 11:15:53',
                  'platforms': ['osx'],
                  'project': 'OG-Lapack',
                  'timestamp': 1398852953.053299,
                  'revision': '78bb7b167aea5f8b52015610d3ad69f360a84493'}],
 'timestamp': 1398854231.495819,
 'version': '0.1.0-SNAPSHOT'}

VERINFO_WIN_REFERENCE = \
{'artifacts': ['jars/og-maths-win-0.1.0-SNAPSHOT.jar',
               'jars/og-maths-win-0.1.0-SNAPSHOT-javadoc.jar',
               'jars/og-maths-win-0.1.0-SNAPSHOT-sources.jar',
               'jars/og-maths-win-0.1.0-SNAPSHOT-tests.jar'],
 'buildnumber': 7,
 'localtime': '2014/04/30 11:37:12',
 'platforms': ['win'],
 'project': 'OG-Maths',
 'revision': 'a57fd89cfcb8b2c6711f6ec8bac19bb339cc24bc',
 'subprojects': [{'buildnumber': 10,
                  'localtime': '2014/04/30 11:15:54',
                  'platforms': ['win'],
                  'project': 'OG-Lapack',
                  'timestamp': 1398852954.053299,
                  'revision': '78bb7b167aea5f8b52015610d3ad69f360a84493'}],
 'timestamp': 1398854232.495819,
 'version': '0.1.0-SNAPSHOT'}

VERINFO_REFERENCES = { 'lnx': VERINFO_LNX_REFERENCE, 'osx': VERINFO_OSX_REFERENCE, 'win': VERINFO_WIN_REFERENCE }

VERINFO_COMBINED_REFERENCE = \
{'artifacts': ['og-maths-0.1.0-SNAPSHOT.jar',
               'og-maths-0.1.0-SNAPSHOT-javadoc.jar',
               'og-maths-0.1.0-SNAPSHOT-sources.jar',
               'og-maths-0.1.0-SNAPSHOT-tests.jar'],
 'buildnumber': 1,
 'buildnumbers': {'lnx': 25,
                  'osx': 6,
                  'subprojects': {'OG-Lapack': {'lnx': 9,
                                                'osx': 9,
                                                'win': 10}},
                  'win': 7},
 'timestamps': {'lnx': 1398854230.495819,
                'osx': 1398854231.495819,
                'subprojects': { 'OG-Lapack': {'lnx': 1398852952.053299,
                                               'osx': 1398852953.053299,
                                               'win': 1398852954.053299 }},
                'win': 1398854232.495819 },
 'localtimes': {'lnx': '2014/04/30 11:37:10',
                'osx': '2014/04/30 11:37:11',
                'subprojects': { 'OG-Lapack': {'lnx': '2014/04/30 11:15:52',
                                               'osx': '2014/04/30 11:15:53',
                                               'win': '2014/04/30 11:15:54' }},
                'win': '2014/04/30 11:37:12' },

 'project': 'OG-Maths',
 'revision': 'a57fd89cfcb8b2c6711f6ec8bac19bb339cc24bc',
 'subprojects': {'OG-Lapack': '78bb7b167aea5f8b52015610d3ad69f360a84493'},
 'version': '0.1.0-SNAPSHOT'}

if __name__ == '__main__':
    unittest.main()
