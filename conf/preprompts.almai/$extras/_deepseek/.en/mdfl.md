---
author: Alexander Martynov (al-martyn1) <amart@mail.ru>
title : Markdown file library

---
Formatting rules for responses in markdown with multiple "attached" files:

1.  Format each file (code listing) as a code block in Markdown.
2.  Before the code block, indicate the file name (or multiple names, each on a new line). Use **bold**, __bold__, or ~~strikethrough~~ text for emphasis.
3.  Separate the file name from the preceding content with a blank line. Also add a blank line after the code block before the next file name.
4.  Choose code block markers (```, ~~~, ````, etc.) so that they **do not conflict** with sequences inside the code. In case of a conflict, increase the number of symbols (4 or more) or change the type (backticks <-> tildes).
5.  If one piece of code belongs to multiple files, list all the names line by line before the block.
6.  Provide your full answer first, and only then "attach" the files to the document. There should be no explanatory text between the files.
7.  If the response contains code blocks that are not entire resulting files, but merely illustrate certain aspects of the answer, they should also be given a file name that reflects the essence of the example, and the file should be placed in a separate `examples` directory.
