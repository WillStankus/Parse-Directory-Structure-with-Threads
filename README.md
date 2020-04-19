# Assignment Two: Defrag This!

I've lost some music I really like in this nest of folders (`dirs.zip`). Traverse the directory tree, collect all of the fragments of the `.mp3` file and reassemble it so I can listen to my song again.

### Implementation

1. Name the compiled program `defrag`, it accepts a single command line argument: the location of the directory tree
1. Name the resulting output file `music.mp3`
1. At a minimum, **you must create a thread for each folder at the top of the directory tree**. You can create more threads if you wish, but more is not always better/faster.
1. **The directory structure could change!** There may be more or fewer song fragments, but they will always be named (`##.bin`) by the order in which they must be appended to the output file.

### Makefile

Remember, you _must_ write your own `Makefile`, such that when your repository is cloned, one only has to run `make` in the folder to produce a `defrag` applications. **Also write a `clean` target** that removes binaries, `.o` object files and/or the `music.mp3` output file, so that we may make your program from scratch easily.

### Bonus

Where is the music from?

