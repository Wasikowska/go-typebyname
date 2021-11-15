# script lang

the bash program is a command list, or many pipelines in order.

a command list is a collection of pipelines, separated by `&&`, `||`,
`;`, or `&`, and terminated by `;` or `&`. `;` can be replaced with
newline, `||` have shortcut semantics.

a pipeline is a collection of commands, separated by `|` or `|&`. a
pipeline has a subshell execution environment.

a commannd is either builtin, simple, or compound. compound command is
a syntax construct, and includes:
- loop construct
- conditional construct
  * [[ conditional-expression ]] and (( arithmatic-expression )) are
    VERY useful compounds
- grouping construct

compound commands are syntax construct, thus apply special
semantics. for example, words splitting and filename expansion is not
performed in arithmatic expression for [[, and parameters can be used
without the parameter expansion syntax


# script interpretation process
1. split tokens using metacharacters
   - quoting rule  
     quoting makes metacharactoers NOT meta anymore, thus prevent
     splitting
   - alias expansion  
     the tokens splitted are also called words
2. parse the token stream to commands, generating AST
   - compound commands are constructed
   - the exact program to run is NOT decided here
3. perform various expansion
4. word splitting on the expansion result using $IFS, then remove
   quotes
   - only apply to the expansions that is NOT inside the double quotes
   - this may change token streams
5. perform redirection, then remove redirection operators
6. command execution
   - the program to run is decided here, actual it is decided after
     word splitting

metacharacters include: `space`, `tab`, `newline`, `|`, `&`, `;`, `(`,
`)`, `<`, `>`.

default values of $IFS are: `space`, `tab`, `newline`

## various expansions
process substitution is awesome!

## how to write good/secure script

readonly
local

- set -a or set -o errexit  
  exit the whole script if a pipeline fails
- set -u  
  treat unset parameters and parameters as error during parameter
  expansion
  
options of set builtin can also be used as the shell invocation
parameters

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

## command execution environment
subshell environment is a fork of the shell process, separate
execution environment is a save/restore context in current shell
process to execute commands.

simple command run in a separate execution environment, while command substitution, commands grouped with parentheses, and async commands are invoked in a subshell environment. subshells inherit -e option


# command line arguments parsing

# debugging
set is used to change bash behaviors

- verbose mode
  set -v
- syntax checking mode
  set -n
- tracing mode
  set -x

# trap - signal handling

# coproc

# bash lsp
https://emacs-lsp.github.io/lsp-mode/page/lsp-bash/

bash-ls is written in nodejs, so need npm to install it

https://explainshell.com/ cool, also support local server

https://cdecl.org/
