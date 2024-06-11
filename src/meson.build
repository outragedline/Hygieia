src = ['./db.c', './main.c']
tests = ['./db.c', './dbtests.c']

sqlite = dependency('sqlite3', static: true)
dependencies = [
  sqlite
]
dbtests = executable('dbtests', tests, dependencies:dependencies)
test('dbtests', dbtests)
executable('Hygieia', src, dependencies:dependencies)
