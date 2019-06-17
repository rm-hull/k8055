#!/usr/bin/env python

import os,sys,string
from distutils.core import setup, Extension

__revision__ = "$Id: setup.py,v 1.3 2007/03/28 10:17:57 pjetur Exp $"

if 'VERSION' in os.environ:
	version=os.environ['VERSION']
else:
	from subprocess import *
	try:
		version = Popen(["grep ^VERSION ../Makefile | cut -d '=' -f 2 | tr -d '\n'"], stdout=PIPE, shell=True).communicate()[0].decode('ascii')
	except:
		version='?.?'

build_modules = [Extension('_pyk8055',
		define_macros = [('VERSION', "\"%s\"" % str(version))],
		libraries=["usb"],
		sources=['libk8055.i',"../libk8055.c"])]
		
setup(
	name='pyk8055',
	version=version,
	author='Pjetur G. Hjaltason',
	author_email='pjetur@pjetur.net',
	description='K8055 library wrapper',
	url='http://libk8055.sourceforge.net/',
	ext_modules =build_modules,
	py_modules=['pyk8055']
)
