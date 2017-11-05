from distutils.core import setup
setup(name='ROIspendfrom',
      version='1.0',
      description='Command-line utility for roicoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@roicoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
