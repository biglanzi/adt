#!/usr/bin/env bash
#set -exop
function getRandom()
{
    head -n 10 < /dev/urandom|md5sum|cut -c1-8
}

curpath="$(cd "$(dirname "$0")";pwd)"
from=${1:-HEAD}
to=${2:-HEAD}
readonly filter="username"

readonly tmpfile="${curpath}/$(getRandom)uu"
svn diff -r "${from}":"${to}" --summarize|tr -s ' '|cut -d ' ' -f 2|grep -E -v '.dll$|.py$|.user$|.filters$|.pdb$|.lib$|.vcxproj$|.sln$' >> "${tmpfile}"
while read file
do
if (($(svn log -q -r "$from":"$to" "$file"|grep -c "${filter}") != 0));then
    echo "$file"
fi
done < "$tmpfile"
rm -rf "$tmpfile"
