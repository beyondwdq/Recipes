# Custom command, counter and ref in Latex

## Beginning of the Story

Before a paper is accepted somewhere, usually I will need to revise the paper
based on the reviewers' comments. A document namely
"replies\_to\_reviewer\_comments.pdf" is necessary to answer the questions
raised by the reviewers and state the changes of the revised paper.

One day I got a decision of my previously submitted paper. The result is
"revise and resubmit". So sad. The comments are overwhelming. What I can do is
to carry out more experiments, ammend the manuscript and write the
"replies\_to\_reviewer\_comments" to answer the questions. The structure and
layout of the file is supposed to be like follows:

    Reviewer #1
      Comment #1-1
        Blah blah
        Reply: 
        Blah blah
      Comment #1-2
        Blah blah
        Reply: 
        Blah blah
    Reviewer #2
      Comment #2-1
        Blah blah
        Reply: 
        Blah blah

I want to add a `2.5mm` space between Comment and Reply and make the
text `Reply:` in bold font. Moreover, I don't like to repeat all this and manage
all the numbers like #1-2 manually. Therefore I Googled a bit and found it was
quite easy to do this with LaTeX.

## New Command

Let put aside the numbers and just look at the format: adding space and bold
font. I implemented these with three new commands: `\reviewer`, `\comment`
and `\reply`. 

Defining new command in Latex is easy. Just use the `\newcomamnd` or
`renewcommand` command, as follows:

    \newcommand{\reviewer}{%
    \section*{Reviewer }}

    \newcommand{\comment}{%
    \subsection*{Comment }}

    \newcommand{\reply}{%
    \vspace{5 mm}%
    {\bfseries Reply: }}

If you are interested in the details of `\newcommand`, please refer to the 
[wiki page][Customizing LaTeX].

## Managing Numbers: Counters

Now I have the three new commands and want to add the reviewer and comment
number to them. Managing the numbers manually is a disaster if there are many
comments. Fortunately, this can be easily solved by using counters. 

First, I define two counters `reviewerno` and `commentno` and set them to 0:

    \newcounter{reviewerno}
    \setcounter{reviewerno}{0}
    \newcounter{commentno}[reviewerno]
    \setcounter{commentno}{0}

The counter `commentno` is defined as a sub-counter of `reviewerno`. This means
that when `reviewerno` is changed, `commentno` will be reset to 0, which is
exactly what I want.

Now I modify the command `\reviewer` and `\comment` to add counters to them:

    \newcommand{\reviewer}{%
    \stepcounter{reviewerno}%
    \section*{Reviewer \#\arabic{reviewerno}}}

    \newcommand{\comment}{%
    \stepcounter{commentno}%
    \subsection*{Comment \#\arabic{reviewerno}-\arabic{commentno}}}

The command `\stepcounter{counter}` increases `counter` by 1. Command
`\arabic{counter}` outputs the value of `counter` in arabic numbers. More
operations on counter can be found [here][LaTex Counter].

Now I can write my replies like this:

    \reviewer
      \comment
    	Blah blah
    	\reply 
    	Blah blah

The nubmers will be automatically mantained and added by LaTeX.

## Label and Ref

The new commands and counters work pretty good, but now I have a new
requirement: two comments asked the same question. I want to answer it in the
first comment and put a reference to the answer in the other one. Of course I
can just hard code the comment number like "please refer to Comment #1-1 for
details", but this is not flexible. What if the nubmer of the comment changes? I
don't want to update all the references to it.

Now I need a reference to a custom counter. In LaTeX, we usually use the
`\label{label_name}` command to define a label and `\ref{label_name}` to
generate a reference to it. In fact, what `\label{label_name}` does is assigning the
name `label_name` to the current value of `\ref`. Correspondingly,
`\ref{label_name}` retrieves the value using `label_name`. 

Here the command needed is `\refstepcounter{counter}`. It increases `counter` by
1 and  sets the value of `\ref` to `counter`. We can simply replace the previous
`\stepcounter{count}` with `\refstepcounter{count}`. Now we try to make a reference: 

    \reviewer
      \comment
        \label{com:novelty}
    	Blah blah
    	\reply 
    	Blah blah
      \comment
        Blah blah
        \reply
        Please refere to our response to Comment \ref{com:novelty}

The output of the code is like this

    Reviewer #1 
      Comment #1-1
      Blah blah
      Reply:
      Blah blah
      Comment #1-2
      Blah blah
      Reply:
      Please refere to our response to Comment 1

It works, but not perfectly. What we want to see is a reference to `Comment #1-1`.
How can we set the `\ref` to two counters with formating in
`#reviewerno-commentno`?

The trick is that everytime you define a counter, say `counter`, LaTeX will
define a command `\thecounter` for it. `counter` is printed in LaTeX by using
`\thecounter`. Hence we can redefine this command for `commentno` to let `\ref`
print whatever we want. Now we define it with the following code:

    \renewcommand{\thecommentno}{\#\arabic{reviewerno}-\arabic{commentno}}

and it works like a charm.

More details can be found at this [link][Counters] and the discussion
[here][Discussion on ref].

## The Code and PDF File

The complete LaTeX source file and the generated PDF file can be found at my
GitHub <https://github.com/beyondwdq/Recipes/tree/master/latex/custom_ref>.

The code is also listed as follows:

    \documentclass[a4paper]{article}
    \newcounter{reviewerno}
    \setcounter{reviewerno}{0}
    \newcounter{commentno}[reviewerno]
    \setcounter{commentno}{0}

    \renewcommand{\thereviewerno}{\#\arabic{reviewerno}}
    \renewcommand{\thecommentno}{\#\arabic{reviewerno}-\arabic{commentno}}
    
    
    \newcommand{\reviewer}{%
    \refstepcounter{reviewerno}%
    \section*{Reviewer \thereviewerno}}
    
    \newcommand{\comment}{%
    \refstepcounter{commentno}%
    \subsection*{Comment \thecommentno}}
    
    \newcommand{\reply}{%
    \vspace{5 mm}%
    {\bfseries Reply: }} 
    
    \begin{document}
    \title{Replies to Review-Comments}
    
    \begin{center}
    {\bfseries Summary of Changes}
    \end{center}
    
    
    \reviewer
    
    \comment
    
    \label{com:novelty}
    
    The paper lacks novelty.
    
    \reply 
    
    The novelty of the paper is summarized as follows:
    
    \begin{itemize}
    	\item Novelty A
    	\item Novelty B
    \end{itemize}
    
    \comment 
    
    More simulations are need to fully evaluate the proposed method. The
    contributions of the paper is not clear.
    
    
    \reply 
    
    We have carried out extensive simulations based on the reviewer's suggestion.
    
    Please refer to Comment \ref{com:novelty} for the details of our contributions.
    
    \reviewer
    
    \comment
    
    The paper is well-written and can be published with minor changes.
    
    \reply 
    
    We have revised the manuscript based on the reviewer's suggestion.
    
    
    \end{document}



[Customizing LaTeX]: http://en.wikibooks.org/wiki/LaTeX/Customizing_LaTeX
[Latex Counter]: http://www.personal.ceu.hu/tex/counters.htm
[Counters]: http://www.public.asu.edu/~rjansen/latexdoc/ltx-3.html
[Discussion on ref]: http://www.latex-community.org/forum/viewtopic.php?f=44&t=10815
