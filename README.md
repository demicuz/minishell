# minishell
A project for School 21. Implements a small subset of Bash in C. Code style is ugly due to mandatory [Norm](https://github.com/MagicHatJo/-42-Norm/blob/master/norme.en.pdf). Yes, 25 lines per function. Yes, tabs. Yes, only while loops. And so on.

### Features
*tbd*

### To-do
- [x] Command execution
- [x] Pipes
- [x] Find commands in `PATH`
- [x] Redirects
- [x] Heredocs (currently weird behavior with `cltr + D`)
- [ ] Relative paths
- [ ] Signals (`ctrl + C`, `ctrl + D`, `ctrl + /`). `ctrl + D` seems to be working out of the box in `readline`.
- [ ] Expanding variables inside of double quotes
- [ ] Builtins
- [ ] `$?` for status of most recently executed pipeline (should be set through `export` builtin)
- [ ] Non-tty mode (`echo "ls" | ./minishell` should run `ls` inside of `minishell`)

**misc**
- [x] Use STDERR for errors. We can't use `fprintf`, that sucks.
- [ ] Correct error handling in general
- [ ] Better naming
