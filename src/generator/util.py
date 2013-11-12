# Copyright (C) 2008-2013 Martin Sandve Alnes and Anders Logg
#
# This file is part of UFL.
#
# UFL is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# UFL is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with UFL. If not, see <http://www.gnu.org/licenses/>.

def camel2underscore(name):
    "Convert a CamelCaps string to underscore_syntax."
    letters = []
    lastlower = False
    for l in name:
        thislower = l.islower()
        if not thislower:
            # Don't insert _ between multiple upper case letters
            if lastlower:
                letters.append("_")
            l = l.lower()
        lastlower = thislower
        letters.append(l)
    return "".join(letters)
