globber = run_command('sh', '-c', 'find . -name "*.c" | sort', check: true)
src = globber.stdout().strip().split('\n')

sqlite = dependency('sqlite3', static: true)
executable('Hygieia', src, dependencies: [sqlite])
