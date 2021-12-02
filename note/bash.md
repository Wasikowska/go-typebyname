# script lang

the bash program is a command list, many pipelines in order.

a command list is a collection of pipelines, separated by `&&`, `||`,
`;`, or `&`, and terminated by `;` or `&`. `;` can be replaced with
newline, `||` have shortcut semantics.

a pipeline is a collection of commands, separated by `|` or `|&`.

a commannd is either simple(including builtin), or compound. compound
command is a syntax construct, includes:
- loop construct
- conditional construct
- grouping construct

compound commands are syntax construct, thus can have special
semantics.

## arithmatic/conditional expression
words splitting and filename expansion is not performed in arithmatic
expression

and parameters can be used without the parameter expansion syntax

## bash variables

defined by bash itself, exist only in current shell, can not be
exported??? should check!

## functions
function defines an alias command, to a compound command.

during function calling, the passing arguments become positional
parameters.

the function's exit status is the value of the builtin cmd `return`,
or otherwise the exit status of the last command executed.

you can also export a function so that the subshell can use this
function. `export` means definitions visible to child processes.

## parameters
this is the traditional variables we familiar with!

NAME=VALUE

this is a syntax form, expansions and quote removal are applied, but
no word splitting.

this form can prepend readonly or local

parameters need not to be explicitly declared before assigning a value
to it

in order to declare a parameter, use declare.

declare can also declare function

when used in a function, the name are local by default

parameters can be exported.

parameter has attributes(type is one kind of attribute), and can be
configured by declare

# script interpretation process
1. split tokens(also called words) by metacharacters
   - quoting rule  
     quoting makes metacharactoers NOT meta anymore, thus prevent
     splitting
   - alias expansion is applied in this phase
2. parse the token stream to various syntactic consturct
   - simple command can be thought of a AST node containing many
     tokens
   - compound command is a AST subtree which reflects the compound
     structure
   - pipeline and list are also AST subtrees
3. perform various expansion
4. word splitting on the expansion result using $IFS, then do quote
   removal
   - the word splitting may change token streams of simple command
   - the quote removal only apply to the expansions that is NOT inside
     the double quotes
5. perform redirection, then remove redirection operators
6. command execution
   - when a simple command runs, the first token is program, other
     tokens are arguments

metacharacters include: `space`, `tab`, `newline`, `|`, `&`, `;`, `(`,
`)`, `<`, `>`.

default values of $IFS are: `space`, `tab`, `newline`

## command execution environment
subshell environment is a fork of the shell process, command
substitution, commands grouped with parentheses, and async commands
use that to execute.

separate execution environment is a save/restore context in current
shell process, simple commands(other than builtin or function) use
that to execute. subshell environment inherit -e option.

# script programming
tips for good/secure script:
- use readonly to label const variable
- use local to label local variable in functions
- always use `bash -ue SCRIPT` to run a script
  * `-u`: treat unset parameters and parameters as error during
    parameter expansion  
  * `-e`: exit the whole script if any pipeline fails

by the way, `bash -ue SCRIPT` can be replaced by writing `set -ue` in
SCRIPT.

## debugging
- verbose mode (`set -v`)  
- syntax checking mode (`set -n`)  
  just do syntax check, don't run
- tracing mode(`set -x`)  
  print the intepretation process

## bash lsp
just refer to: https://emacs-lsp.github.io/lsp-mode/page/lsp-bash/

bash-ls is written in nodejs, so need npm to install it

https://explainshell.com/ cool, also support local server
https://cdecl.org/
