
compile: make

execute helloWorld.bf: ./brainfuck helloWorld.bf && ./helloWorldGCC

brainfuck mods: ':' ',' output as number