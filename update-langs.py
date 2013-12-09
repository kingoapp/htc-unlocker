#!/bin/env python
#-*- coding: UTF-8 -*-

import os.path
import fnmatch
import sys
import os,re
# import scan_qml_tr

def listLangDir(path = "."):
    lst = os.listdir(path)
    lcre = re.compile(r'^[a-z]{2}_[A-Z]{2,3}$')
    lre = re.compile(r'^[a-z]{2}$')
    lngs = []
    for p in lst:
        if not os.path.isdir(os.path.join(path,p)):
            continue
        if lcre.match(p) or lre.match(p):
            lngs.append(p)
    return lngs

def merge_ts(dst, src, target_lang = ""):
    cmd = 'lconvert'
    if target_lang != '':
        cmd += ' --target-language ' + target_lang
    cmd += ' -o "%s" %s' % (dst, src + " " + dst)
    print cmd
    os.system(cmd)

base = os.path.normpath(os.path.join(sys.path[0], '.'))
langSource = os.path.join(base, 'language')
langs = listLangDir(langSource)

# os.system('lupdate -pro "%s" -codecfortr utf8 -ts "%s"' % (
#     os.path.join(base, 'superflash.pro'),
#     os.path.join(langSource, 'project.ts'))
#     )
os.system('lupdate -recursive "%s" -codecfortr utf8 -ts "%s"' % (
    os.path.join(base, 'src'),
    os.path.join(langSource, 'project.ts'))
    )

# for lang in langs:
#     merge_ts(os.path.join(langSource, lang, 'lang.ts'), os.path.join(langSource, 'project.ts'), lang)
    # merge_ts(os.path.join(langSource, lang, 'extent_lang.ts'), os.path.join(langSource, 'lua_ts.ts'), lang)

# os.remove(os.path.join(langSource, 'project.ts'))
# os.remove(os.path.join(langSource, 'ui.ts'))
# os.remove(os.path.join(langSource, 'lua_ts.ts'))

#langs = [f for f in os.listdir(langSource) if fnmatch.fnmatch(f, "*.ts")]
#
#for f in langs:
#	print 'Parsing',f,'...'
#	os.system('lmerge -i "%s" "%s" -o "%s"' %(
#		os.path.join(langSource, 'winavi_untranslated.ts'),
#		os.path.join(langSource, f),
#		os.path.join(langSource, f)
#		))

print 'press any key to exit'
raw_input()
