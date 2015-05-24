CS 214 TOKENZIER 
=========

I found that the main part of this assignment (getting a tokenstream to tokenize based on an argument of characters) was fairly easy to implement. The challenging part was the handling of escape characters, because what your program sees is not exactly what is entered into bash.
I would say 80-90% of the time I spent coding this assignment was dealing with these pesky edge-cases, which resulted in a whole lot of checks for backslash characters and what comes after them.

The program provides the following output:

**tokenstream**: { what the program sees as the incoming stream to be tokenized }

**delims** : { lists each delimiter the program is using to tokenize the stream }

**numdelims** : { number of delimiters }

**token**: { a single token, each on a new line}


All of the test­-cases provided on the assignment specification and given to us by the TA produce the correct output, and I provided ~10 more to ensure tricky cases with multiple escape characters are handled correctly. I found some of the wording in the spec to be a little ambiguous regarding how specifically to deal with escape characters as delimiters vs. escape characters in the tokenstream, but I hope my program produces the desired outputs.
