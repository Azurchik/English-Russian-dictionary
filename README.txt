English-Russian dictionary
Version: 1.0.0.1

The essence of the project is that
I create my binary tree.

Words are stored as follows:
1) the English word;
2) translation of the word in Russian;
3) a counter that shows how many times
addressed this word.


The tree has two data saving modes:

1) alphabetically, summing up the values of all
the chars in word and dividing the sum by 
the length of the word to avoid collision.
{
 \\ a = 14;
 \\ g = 25;

 string word = "ag";

 getCode (word); \\ (14 + 25) / 2 = 19;
}

2) by the counter of word requests - the more often
the word will be requested, the higher it will be
in the tree.

In the 1st tree sorting mode, the depth search is used, 
in the 2nd - the breadth search.


In the dictiory of the exe-file is created
txt-file "dict.txt" in which word are stored.

The First line is used to initialize:
"numbers of words" ~space~
"list sorted?"(1/0) ~space~

(If sorted, write folowing, if not - skip and write ~end line~)
"was sorted by abc?"(1\0) ~end line~

Next are the word entries:
"english word" ~space~
"russian translate" ~space~
"counter" ~end line~