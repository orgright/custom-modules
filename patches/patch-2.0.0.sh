#!/bin/sh
# script to copy patched files to contributed modules
# for orgright release 2.0.0 - 27 Jun 2011

# determine path location and change working directory to there
cd `dirname "$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"`
 
# Date module 6.x-2.7
cp -bv date/date_popup.module-2.0.0  ../../date/date_popup/date_popup.module

