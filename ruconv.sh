#!/bin/sh
cat README_RU.utf8 | iconv -f utf8 -t cp1251 > README_RU.cp1251
cat README_RU.utf8 | iconv -f utf8 -t koi8r> README_RU.koi8r
