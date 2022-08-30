# minishell
A project for School 21. Implements a small subset of Bash.

### Features
*tbd*

### To-do
- [x] Command execution
- [x] Pipes
- [ ] Find commands in `PATH`
- [ ] Relative paths
- [ ] Redirects
- [ ] Heredocs
- [ ] Signals (`ctrl + C`, `ctrl + D`, `ctrl + /`). `ctrl + D` seems to be working out of the box in `readline`.
- [ ] Expanding variables inside of double quotes
- [ ] Builtins
- [ ] `$?` for status of most recently executed foreground pipeline (should be set through `export` builtin)
- [ ] Non-tty mode (`echo "ls" | ./minishell` should run `ls` inside of `minishell`)

**misc**
- [ ] Use STDERR for errors. We can't use `fprintf`, that sucks.
- [ ] Correct error handling in general
