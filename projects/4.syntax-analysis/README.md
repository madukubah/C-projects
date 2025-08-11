# SIMPLE SYNTAX ANALYSER
# Features
- Convert Context-Free Grammar(CFG) into Parser
- determin that parser can be build for the given CFG
- insert a sentence and the produced parser will evaluate this sentence is acceptible or not

# Format
%%
<definition>
%%
<rules>

------------------
%%

%token ...
%start S


%%
S : A


S is production head
: is arrow -> to mean "is defined as"
A is body

- example
%%
%token id + *
%%

S  : E
E  : TE`
E` : + TE` | E`
T  : FT`
T` : * FT` | E
F  : id 

# run
./x filename.txt

# LIMITATION
- UPPERCASE letter mean nonterminal
- lowercase letter mean terminal
- e mean null
- not support regex
