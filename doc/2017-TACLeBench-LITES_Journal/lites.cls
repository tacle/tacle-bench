%%
%% This is file `lites.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% lites.dtx  (with options: `class')
%% 
%% -----------------------------------------------------------------
%% Author:     le-tex publishing services
%% 
%% This file is part of the lites package for preparing
%% LITES articles.
%% 
%%       Copyright (C) 2013 Schloss Dagstuhl
%% -----------------------------------------------------------------
\NeedsTeXFormat{LaTeX2e}[2011/06/27]
\ProvidesClass{lites}
    [2013/06/14 v1.00 LITES articles]
\emergencystretch1em
\advance\hoffset-1in
\advance\voffset-1in
\advance\hoffset2.95mm
\newif\if@nobotseplist  \@nobotseplistfalse
\def\@endparenv{%
  \addpenalty\@endparpenalty\if@nobotseplist\else\addvspace\@topsepadd\fi\@endpetrue}
\def\@doendpe{%
  \@endpetrue
  \def\par{\@restorepar
           \everypar{}%
           \par
           \if@nobotseplist
             \addvspace\topsep
             \addvspace\partopsep
             \global\@nobotseplistfalse
           \fi
           \@endpefalse}%
  \everypar{{\setbox\z@\lastbox}%
            \everypar{}%
            \if@nobotseplist\global\@nobotseplistfalse\fi
            \@endpefalse}}
\def\enumerate{%
  \ifnum \@enumdepth >\thr@@\@toodeep\else
    \advance\@enumdepth\@ne
    \edef\@enumctr{enum\romannumeral\the\@enumdepth}%
    \expandafter
    \list
      \csname label\@enumctr\endcsname
      {\advance\partopsep\topsep
       \topsep\z@\@plus\p@
       \ifnum\@listdepth=\@ne
         \labelsep0.72em
       \else
         \ifnum\@listdepth=\tw@
           \labelsep0.3em
         \else
           \labelsep0.5em
         \fi
       \fi
       \usecounter\@enumctr\def\makelabel##1{\hss\llap{##1}}}%
  \fi}
\def\endenumerate{\ifnum\@listdepth=\@ne\global\@nobotseplisttrue\fi\endlist}
\def\itemize{%
  \ifnum \@itemdepth >\thr@@\@toodeep\else
    \advance\@itemdepth\@ne
    \edef\@itemitem{labelitem\romannumeral\the\@itemdepth}%
    \expandafter
    \list
      \csname\@itemitem\endcsname
      {\advance\partopsep\topsep
       \topsep\z@\@plus\p@
       \ifnum\@listdepth=\@ne
         \labelsep0.83em
       \else
         \ifnum\@listdepth=\tw@
           \labelsep0.75em
         \else
           \labelsep0.5em
         \fi
      \fi
      \def\makelabel##1{\hss\llap{##1}}}%
  \fi}
\def\enditemize{\ifnum\@listdepth=\@ne\global\@nobotseplisttrue\fi\endlist}
\def\@sect#1#2#3#4#5#6[#7]#8{%
  \ifnum #2>\c@secnumdepth
    \let\@svsec\@empty
  \else
    \refstepcounter{#1}%
    \protected@edef\@svsec{\@seccntformat{#1}\relax}%
  \fi
  \@tempskipa #5\relax
  \ifdim \@tempskipa>\z@
    \begingroup
      #6{%
        \@hangfrom{\hskip #3\relax
          \ifnum #2=1
            \colorbox[rgb]{0.99,0.78,0.07}{\kern0.15em\@svsec\kern0.15em}\quad
          \else
            \@svsec\quad
          \fi}%
          \interlinepenalty \@M #8\@@par}%
    \endgroup
    \csname #1mark\endcsname{#7}%
    \addcontentsline{toc}{#1}{%
      \ifnum #2>\c@secnumdepth \else
        \protect\numberline{\csname the#1\endcsname}%
      \fi
      #7}%
  \else
    \def\@svsechd{%
      #6{\hskip #3\relax
      \@svsec #8}%
      \csname #1mark\endcsname{#7}%
      \addcontentsline{toc}{#1}{%
        \ifnum #2>\c@secnumdepth \else
          \protect\numberline{\csname the#1\endcsname}%
        \fi
        #7}}%
  \fi
  \@xsect{#5}}
\def\@seccntformat#1{\csname the#1\endcsname}
\def\@biblabel#1{\textcolor{darkgray}{\sffamily\bfseries#1}}
\def\copyrightline{%
  \scriptsize
  \vtop{\hsize\textwidth
    \nobreakspace\\
    \@Copyright
    \textit{Leibniz Transactions on Embedded Systems}%
    \ifx\@Volume\@empty\else,\space Vol.\nobreakspace\@Volume\fi
    \ifx\@Issue\@empty\else,\space Issue\nobreakspace\@Issue\fi
    ,\space pp. \thepage--\pageref{LastPage}\\
    \setbox\@tempboxa\hbox{\includegraphics[height=14\p@,trim=0 15 0 0]{lites-logo}}%
    \hspace*{\wd\@tempboxa}\enskip
    \href{http://www.dagstuhl.de/lites}%
         {Leibniz Transactions on Embedded Systems}\\
    \smash{\unhbox\@tempboxa}\enskip
    \href{http://www.dagstuhl.de}%
         {Schloss Dagstuhl -- Leibniz-Zentrum f{\"u}r Informatik, Dagstuhl Publishing, Germany}}}
\def\ps@plain{\let\@mkboth\@gobbletwo
  \let\@oddhead\@empty
  \let\@evenhead\@empty
  \let\@evenfoot\copyrightline
  \let\@oddfoot\copyrightline}
\def\lites@opterrshort{Option  "\CurrentOption" not supported}
\def\lites@opterrlong{The option "\CurrentOption" from article.cls is not supported by lites.cls.}
\DeclareOption{a5paper}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{b5paper}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{legalpaper}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{executivepaper}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{landscape}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{10pt}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{11pt}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{12pt}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{oneside}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{twoside}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{titlepage}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{notitlepage}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{onecolumn}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{twocolumn}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{fleqn}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{openbib}{\@latexerr{\lites@opterrshort}{\lites@opterrlong}}
\DeclareOption{a4paper}{\PassOptionsToClass{\CurrentOption}{article}
                        \advance\hoffset-2.95mm
                        \advance\voffset8.8mm}
\DeclareOption{numberwithinsect}{\let\numberwithinsect\relax}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions
\LoadClass[twoside,notitlepage,fleqn]{article}
\renewcommand\normalsize{%
   \@setfontsize\normalsize\@xpt{13}%
   \abovedisplayskip 10\p@ \@plus2\p@ \@minus5\p@
   \abovedisplayshortskip \z@ \@plus3\p@
   \belowdisplayshortskip 6\p@ \@plus3\p@ \@minus3\p@
   \belowdisplayskip \abovedisplayskip
   \let\@listi\@listI}
\normalsize
\renewcommand\small{%
   \@setfontsize\small\@ixpt{11.5}%
   \abovedisplayskip 8.5\p@ \@plus3\p@ \@minus4\p@
   \abovedisplayshortskip \z@ \@plus2\p@
   \belowdisplayshortskip 4\p@ \@plus2\p@ \@minus2\p@
   \def\@listi{\leftmargin\leftmargini
               \topsep 4\p@ \@plus2\p@ \@minus2\p@
               \parsep 2\p@ \@plus\p@ \@minus\p@
               \itemsep \parsep}%
   \belowdisplayskip \abovedisplayskip
}
\renewcommand\footnotesize{%
   \@setfontsize\footnotesize{8.5}{9.5}%
   \abovedisplayskip 6\p@ \@plus2\p@ \@minus4\p@
   \abovedisplayshortskip \z@ \@plus\p@
   \belowdisplayshortskip 3\p@ \@plus\p@ \@minus2\p@
   \def\@listi{\leftmargin\leftmargini
               \topsep 3\p@ \@plus\p@ \@minus\p@
               \parsep 2\p@ \@plus\p@ \@minus\p@
               \itemsep \parsep}%
   \belowdisplayskip \abovedisplayskip
}
\renewcommand\large{\@setfontsize\large{10.5}{13}}
\renewcommand\Large{\@setfontsize\Large{12}{14}}
\setlength\parindent{1.5em}
\setlength\headheight{3mm}
\setlength\headsep   {10mm}
\setlength\footskip{3mm}
\setlength\textwidth{148mm}%was 140
\setlength\textheight{222mm}
\setlength\oddsidemargin{28mm}%was 32
\setlength\evensidemargin{34mm}%was 38
\setlength\marginparwidth{25mm}
\setlength\topmargin{13mm}
\setlength{\skip\footins}{2\baselineskip \@plus 4\p@ \@minus 2\p@}
\def\@listi{\leftmargin\leftmargini
            \parsep\z@ \@plus\p@
            \topsep 8\p@ \@plus2\p@ \@minus4\p@
            \itemsep \parsep}
\let\@listI\@listi
\@listi
\def\@listii {\leftmargin\leftmarginii
              \labelwidth\leftmarginii
              \advance\labelwidth-\labelsep
              \topsep    4\p@ \@plus2\p@ \@minus\p@
              \parsep\z@ \@plus\p@
              \itemsep   \parsep}
\def\@listiii{\leftmargin\leftmarginiii
              \labelwidth\leftmarginiii
              \advance\labelwidth-\labelsep
              \topsep    2\p@ \@plus\p@\@minus\p@
              \parsep    \z@
              \partopsep \p@ \@plus\z@ \@minus\p@
              \itemsep   \z@ \@plus\p@}
\def\ps@headings{%
    \def\@evenhead{\large\sffamily\bfseries
                   \llap{\hbox to0.5\oddsidemargin{\thepage\hss}}\leftmark\hfil}%
    \def\@oddhead{\large\sffamily\bfseries\rightmark\hfil
                  \rlap{\hbox to0.5\oddsidemargin{\hss\thepage}}}%
    \def\@oddfoot{\hfil
                  \rlap{%
                    \vtop{%
                      \vskip10mm
                      \colorbox[rgb]{0.99,0.78,0.07}
                                    {\@tempdima\evensidemargin
                                     \advance\@tempdima1in
                                     \advance\@tempdima\hoffset
                                     \hb@xt@\@tempdima{%
                                       \textcolor{darkgray}{\normalsize\sffamily
                                       \bfseries\quad
                                       \expandafter\textsolittle
                                       \expandafter{\@EventShortName}}%
                                     \strut\hss}}}}}
    \let\@evenfoot\@empty
    \let\@mkboth\markboth
  \let\sectionmark\@gobble
  \let\subsectionmark\@gobble}
\pagestyle{headings}
\renewcommand\maketitle{\par
  \begingroup
    \renewcommand\thefootnote{\@fnsymbol\c@footnote}%
    \if@twocolumn
      \ifnum \col@number=\@ne
        \@maketitle
      \else
        \twocolumn[\@maketitle]%
      \fi
    \else
      \newpage
      \global\@topnum\z@   % Prevents figures from going at top of page.
      \@maketitle
    \fi
    \thispagestyle{plain}\@thanks
  \endgroup
  \setcounter{footnote}{0}%
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\@title\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\date\relax
  \global\let\and\relax
}
\newwrite\tocfile
\def\@maketitle{%
  \newpage
  \null\vskip-\baselineskip
  \vskip-\headsep
  \ifLineNumbers\nolinenumbers\let\linno@n\relax\fi
  \@titlerunning
  \@authorrunning
  \let \footnote \thanks
  \parindent\z@ \raggedright
    {\LARGE\sffamily\bfseries\mathversion{bold}\@title \par}%
    \vskip 1.5em%
    \ifnum\c@authors=0 %
      \@latexerr{No \noexpand\author given}%
        {Provide at least one author. See the LITES class documentation.}%
    \else
      \@author
    \fi
    \bgroup
      \let\footnote\@gobble
      \immediate\openout\tocfile=\jobname.vtc
      \protected@write\tocfile{}{%
        \string\contitem
        \string\title{\@title}%
        \string\author{\AB@authfortoc}%
        \string\page{\thepage}}%
      \closeout\tocfile
    \egroup
  \par%
  \ifx\linno@n\relax\linenumbers\fi}
\renewcommand\tableofcontents{%
  \ifLineNumbers\nolinenumbers\let\linno@n\relax\fi
  \section*{\contentsname}%
  \@starttoc{toc}%
  \ifx\linno@n\relax\linenumbers\fi}
\setcounter{secnumdepth}{4}
\renewcommand\section{\@startsection {section}{1}{\z@}%
                                   {-3.5ex \@plus -1ex \@minus -.2ex}%
                                   {2.3ex \@plus.2ex}%
                                   {\sffamily\Large\bfseries\raggedright}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
                                     {-3.25ex\@plus -1ex \@minus -.2ex}%
                                     {1.5ex \@plus .2ex}%
                                     {\sffamily\Large\bfseries\raggedright}}
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
                                     {-3.25ex\@plus -1ex \@minus -.2ex}%
                                     {1.5ex \@plus .2ex}%
                                     {\sffamily\Large\bfseries\raggedright}}
\renewcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
                                    {-3.25ex \@plus-1ex \@minus-.2ex}%
                                    {1.5ex \@plus .2ex}%
                                    {\sffamily\large\bfseries\raggedright}}
\renewcommand\subparagraph{\@startsection{subparagraph}{5}{\z@}%
                                       {3.25ex \@plus1ex \@minus .2ex}%
                                       {-1em}%
                                      {\sffamily\normalsize\bfseries}}
\setlength\leftmargini  \parindent
\setlength\leftmarginii {1.2em}
\setlength\leftmarginiii{1.2em}
\setlength\leftmarginiv {1.2em}
\setlength\leftmarginv  {1.2em}
\setlength\leftmarginvi {1.2em}
\renewcommand\labelenumi{%
  \textcolor{darkgray}{\sffamily\bfseries\mathversion{bold}\theenumi.}}
\renewcommand\labelenumii{%
  \textcolor{darkgray}{\sffamily\bfseries\mathversion{bold}\theenumii.}}
\renewcommand\labelenumiii{%
  \textcolor{darkgray}{\sffamily\bfseries\mathversion{bold}\theenumiii.}}
\renewcommand\labelenumiv{%
  \textcolor{darkgray}{\sffamily\bfseries\mathversion{bold}\theenumiv.}}
\renewcommand\labelitemi{%
  \textcolor[rgb]{0.6,0.6,0.61}{\ifnum\@listdepth=\@ne
                                  \rule{0.67em}{0.33em}%
                                \else
                                  \rule{0.45em}{0.225em}%
                                \fi}}
\renewcommand\labelitemii{%
  \textcolor[rgb]{0.6,0.6,0.61}{\rule{0.45em}{0.225em}}}
\renewcommand\labelitemiii{%
  \textcolor[rgb]{0.6,0.6,0.61}{\sffamily\bfseries\textasteriskcentered}}
\renewcommand\labelitemiv{%
  \textcolor[rgb]{0.6,0.6,0.61}{\sffamily\bfseries\textperiodcentered}}
\renewenvironment{description}
               {\list{}{\advance\partopsep\topsep\topsep\z@\@plus\p@
                        \labelwidth\z@ \itemindent-\leftmargin
                        \let\makelabel\descriptionlabel}}
               {\ifnum\@listdepth=\@ne\global\@nobotseplisttrue\fi\endlist}
\renewcommand*\descriptionlabel[1]{%
  \hspace\labelsep\textcolor{darkgray}{\sffamily\bfseries\mathversion{bold}#1}}
\newsavebox{\metadatabox}
\newenvironment{colbox}
  {\begin{lrbox}{\metadatabox}%
   \begin{minipage}{\dimexpr\columnwidth-2\fboxsep\relax}}
  {\end{minipage}\end{lrbox}%
   \begin{center}
   \colorbox[rgb]{0.85,0.85,0.86}{\usebox{\metadatabox}}
   \end{center}}
\renewenvironment{abstract}
  {\vskip\bigskipamount
   \ifLineNumbers\nolinenumbers\let\linno@n\relax\fi
   \multicolsep\z@
   \columnsep\parindent
   \begin{multicols}{2}%
     [\noindent
      \rlap{\color[rgb]{0.51,0.50,0.52}\vrule\@width\textwidth\@height1\p@}%
      {\hspace*{4.5mm}\fboxsep1.5mm\colorbox[rgb]{1,1,1}{\raisebox{-0.4ex}{%
       \large\selectfont\sffamily\bfseries\abstractname}}}%
      \vskip3\p@]%
   \fontsize{9}{12}\selectfont
   \noindent\ignorespaces}
  {\end{multicols}%
   \vskip0.5\baselineskip\noindent
   \begin{colbox}
     \fontsize{9}{12}\selectfont
     \vskip0.3\baselineskip\noindent
     \subjclassHeading
     \ifx\@subjclass\@empty
      \textcolor{red}{Author: Please fill in subjclass macro}
     \else
      \@subjclass
     \fi
     \vskip0.1\baselineskip\noindent
     \keywordsHeading
     \ifx\@keywords\@empty
      \textcolor{red}{Author: Please fill in keywords macro}
     \else
      \@keywords
     \fi
     \vskip0.1\baselineskip\noindent
     \doiHeading
     \ifx\@DOI\@empty
      \textcolor{blue}{To be completed by Dagstuhl editorial office}
     \else
       \doi{\@DOI}%
     \fi
     \vskip0.1\baselineskip\noindent
     \DateSubmissionHeading
     \ifx\@DateSubmission\@empty
      \textcolor{red}{Author: Please fill in DateSubmission macro}
     \else
      \@DateSubmission.\space
     \fi
     \DateAcceptanceHeading
     \ifx\@DateAcceptance\@empty
      \textcolor{blue}{To be completed by Dagstuhl editorial office}
     \else
      \@DateAcceptance.\space
     \fi
     \DatePublishedHeading
     \ifx\@DatePublished\@empty
      \textcolor{blue}{To be completed by Dagstuhl editorial office}
     \else
      \@DatePublished.\space
     \fi
     \ifx\@SectionAreaEditor\@empty\else
      \vskip0.9\baselineskip\noindent
      \SectionAreaEditorHeading\@SectionAreaEditor
     \fi
     \ifx\@SpecialIssue\@empty\else
      \vskip0.1\baselineskip\noindent
      \SpecialIssueHeading\@SpecialIssue
     \fi
      \vskip0.3\baselineskip\noindent
     \par
   \end{colbox}
   \ifx\linno@n\relax\linenumbers\fi}
\renewenvironment{thebibliography}[1]
  {\if@noskipsec \leavevmode \fi
   \par
   \ifLineNumbers\nolinenumbers\let\linno@n\relax\fi
   \@tempskipa-3.5ex \@plus -1ex \@minus -.2ex\relax
   \@afterindenttrue
   \@tempskipa -\@tempskipa \@afterindentfalse
   \if@nobreak
     \everypar{}%
   \else
     \addpenalty\@secpenalty\addvspace\@tempskipa
   \fi
   \columnsep\parindent
   \begin{multicols}{2}%
     [\noindent
      \rlap{\color[rgb]{0.51,0.50,0.52}\vrule\@width\textwidth\@height1\p@}%
      {\hspace*{4.5mm}\fboxsep1.5mm\colorbox[rgb]{1,1,1}{\raisebox{-0.4ex}{%
      \normalsize\sffamily\bfseries\refname}}}%
      \@xsect{1ex \@plus.2ex}%
     \vspace{-\multicolsep}]%
     \list{\@biblabel{\@arabic\c@enumiv}}%
        {\leftmargin6mm
         \labelsep\leftmargin
         \settowidth\labelwidth{\@biblabel{#1}}%
         \advance\labelsep-\labelwidth
         \usecounter{enumiv}%
         \let\p@enumiv\@empty
         \renewcommand\theenumiv{\@arabic\c@enumiv}}%
     \fontsize{8}{9.5}\selectfont
     \sloppy
     \clubpenalty10000
     \@clubpenalty \clubpenalty
     \widowpenalty10000%
     \sfcode`\.\@m}
  {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
     \endlist
   \end{multicols}%
   \ifx\linno@n\relax\linenumbers\fi}
\renewcommand\footnoterule{%
  \kern-8\p@
  {\color[rgb]{0.60,0.60,0.61}\hrule\@width40mm\@height1\p@}%
  \kern6.6\p@}
\renewcommand\@makefntext[1]{%
    \parindent\z@\hangindent1em
    \leavevmode
    \hb@xt@1em{\@makefnmark\hss}#1}
\usepackage[utf8]{inputenc}
\IfFileExists{lmodern.sty}{\RequirePackage{lmodern}}{}
\RequirePackage[T1]{fontenc}
\RequirePackage{textcomp}
\RequirePackage[mathscr]{eucal}
\RequirePackage{amssymb}
\RequirePackage{soul}
\sodef\textsolittle{}{.12em}{.5em\@plus.08em\@minus.06em}%
        {.4em\@plus.275em\@minus.183em}
\RequirePackage{color}
\definecolor{darkgray}{rgb}{0.31,0.31,0.33}
\RequirePackage{babel}
\RequirePackage[tbtags,fleqn]{amsmath}
\RequirePackage{amsthm}
\thm@headfont{%
  \textcolor{darkgray}{$\blacktriangleright$}\nobreakspace\sffamily\bfseries}
\def\th@remark{%
  \thm@headfont{%
    \textcolor{darkgray}{$\blacktriangleright$}\nobreakspace\sffamily}%
  \normalfont % body font
  \thm@preskip\topsep \divide\thm@preskip\tw@
  \thm@postskip\thm@preskip
}
\def\@endtheorem{\endtrivlist}%\@endpefalse
\renewcommand\qedsymbol{\textcolor{darkgray}{\ensuremath{\blacktriangleleft}}}
\renewenvironment{proof}[1][\proofname]{\par
  \pushQED{\qed}%
  \normalfont \topsep6\p@\@plus6\p@\relax
  \trivlist
  \item[\hskip\labelsep
        \color{darkgray}\sffamily\bfseries
    #1\@addpunct{.}]\ignorespaces
}{%
  \popQED\endtrivlist%\@endpefalse
}
\theoremstyle{plain}
\newtheorem{theorem}{Theorem}
\newtheorem{lemma}[theorem]{Lemma}
\newtheorem{corollary}[theorem]{Corollary}
\theoremstyle{definition}
\newtheorem{definition}[theorem]{Definition}
\newtheorem{example}[theorem]{Example}
\theoremstyle{remark}
\newtheorem*{remark}{Remark}
\ifx\numberwithinsect\relax
  \@addtoreset{theorem}{section}
  \edef\thetheorem{\expandafter\noexpand\thesection\@thmcountersep\@thmcounter{theorem}}
\fi
\RequirePackage{graphicx}
\RequirePackage{array}
\let\@classzold\@classz
\def\@classz{%
   \expandafter\ifx\d@llarbegin\begingroup
     \toks \count@ =
     \expandafter{\expandafter\small\the\toks\count@}%
   \fi
   \@classzold}
\RequirePackage{multirow}
\RequirePackage{tabularx}
\RequirePackage[online]{threeparttable}
\def\TPTtagStyle#1{#1)}
\def\tablenotes{\small\TPT@defaults
  \@ifnextchar[\TPT@setuptnotes\TPTdoTablenotes} % ]
\RequirePackage{listings}
\lstset{basicstyle=\small\ttfamily,%
        backgroundcolor=\color[rgb]{0.85,0.85,0.86},%
        frame=single,framerule=0pt,xleftmargin=\fboxsep,xrightmargin=\fboxsep}
\RequirePackage{multicol}[2011/06/27]
\multicolsep = 13pt plus 4pt minus 3pt
\RequirePackage[left,mathlines]{lineno}
\linenumbers
\renewcommand\linenumberfont{\normalfont\tiny\sffamily}
%%%% patch to cope with amsmath
%%%% http://phaseportrait.blogspot.de/2007/08/lineno-and-amsmath-compatibility.html
\newcommand*\patchAmsMathEnvironmentForLineno[1]{%
  \expandafter\let\csname old#1\expandafter\endcsname\csname #1\endcsname
  \expandafter\let\csname oldend#1\expandafter\endcsname\csname end#1\endcsname
  \renewenvironment{#1}%
     {\linenomath\csname old#1\endcsname}%
     {\csname oldend#1\endcsname\endlinenomath}}%
\newcommand*\patchBothAmsMathEnvironmentsForLineno[1]{%
  \patchAmsMathEnvironmentForLineno{#1}%
  \patchAmsMathEnvironmentForLineno{#1*}}%
\AtBeginDocument{%
  \patchBothAmsMathEnvironmentsForLineno{equation}%
  \patchBothAmsMathEnvironmentsForLineno{align}%
  \patchBothAmsMathEnvironmentsForLineno{flalign}%
  \patchBothAmsMathEnvironmentsForLineno{alignat}%
  \patchBothAmsMathEnvironmentsForLineno{gather}%
  \patchBothAmsMathEnvironmentsForLineno{multline}}
\RequirePackage{lastpage}
\IfFileExists{doi.sty}
  {\RequirePackage{doi}%
   \renewcommand*{\doitext}{}}
  {\RequirePackage{hyperref}%
   \def\doi##1{##1}}
\hypersetup{pdfborder={0 0 0}}
\RequirePackage[labelsep=space,singlelinecheck=false,%
  font={up,small},labelfont={sf,bf},%
  listof=false]{caption}%"listof" instead of "list" for backward compatibility
\@ifpackagelater{hyperref}{2009/12/09}
  {\captionsetup{compatibility=false}}%cf. http://groups.google.de/group/comp.text.tex/browse_thread/thread/db9310eb540fbbd8/42e30f3b7b3aa17a?lnk=raot
  {}
\DeclareCaptionLabelFormat{boxed}{%
  \kern0.05em{\color[rgb]{0.99,0.78,0.07}\rule{0.73em}{0.73em}}%
  \hspace*{0.67em}\bothIfFirst{#1}{~}#2}
\captionsetup{labelformat=boxed}
\captionsetup[table]{position=top}
\RequirePackage[figuresright]{rotating}
\RequirePackage{subfig}
\def\titlerunning#1{\gdef\@titlerunning{{\let\footnote\@gobble\markboth{#1}{#1}}}}
\def\authorrunning#1{%
  \gdef\@authorrunning{\expandafter\def\expandafter\@tempa\expandafter{#1}%
                       \ifx\@tempa\@empty\else\markright{#1}\fi}}
\titlerunning{\@title}
\authorrunning{\AB@authrunning}
\def\Volume#1{\gdef\@Volume{#1}}
\Volume{\textcolor{blue}{To be completed by Dagstuhl editorial office}}
\def\Issue#1{\gdef\@Issue{#1}}
\Issue{\textcolor{blue}{To be completed by Dagstuhl editorial office}}
\RequirePackage{authblk}
\renewcommand*\Authand{{ and }}
\renewcommand*\Authfont{\Large\bfseries\mathversion{bold}}
\renewcommand*\AB@authnote[1]{\textsuperscript{#1}}
\renewcommand*\AB@affilnote[1]{\protect\item[#1]}
\renewcommand*\Affilfont{\fontsize{9.5}{12}\selectfont}
\setlength\affilsep{\baselineskip}
\newcommand\AB@authrunning{}
\newcommand\AB@authfortoc{}
\renewcommand\author[2][]%
      {\ifnewaffil\addtocounter{affil}{1}%
       \edef\AB@thenote{\arabic{affil}}\fi
      \if\relax#1\relax\def\AB@note{\AB@thenote}\else\def\AB@note{#1}%
        \setcounter{Maxaffil}{0}\fi
      \ifnum\value{authors}>1\relax
      \@namedef{@sep\number\c@authors}{\Authsep}\fi
      \addtocounter{authors}{1}%
      \begingroup
          \let\protect\@unexpandable@protect \let\and\AB@pand
          \def\thanks{\protect\thanks}\def\footnote{\protect\footnote}%
         \@temptokena=\expandafter{\AB@authors}%
         {\def\\{\protect\\[\@affilsep]\protect\Affilfont
              \protect\AB@resetsep}%
              \xdef\AB@author{\AB@blk@and#2}%
       \ifnewaffil\gdef\AB@las{}\gdef\AB@lasx{\protect\Authand}\gdef\AB@as{}%
           \xdef\AB@authors{\the\@temptokena\AB@blk@and}%
       \else
          \xdef\AB@authors{\the\@temptokena\AB@as\AB@au@str}%
          \global\let\AB@las\AB@lasx\gdef\AB@lasx{\protect\Authands}%
          \gdef\AB@as{\Authsep}%
       \fi
       \gdef\AB@au@str{#2}}%
         \@temptokena=\expandafter{\AB@authlist}%
         \let\\=\authorcr
         \xdef\AB@authlist{\the\@temptokena
           \protect\@nameuse{@sep\number\c@authors}%
           \protect\Authfont#2\AB@authnote{\AB@note}}%
         %new
         \@temptokena=\expandafter{\AB@authrunning}%
         \let\\=\authorcr
         \xdef\AB@authrunning{\the\@temptokena
           \protect\@nameuse{@sep\number\c@authors}#2}%
         %
         %new
         \@temptokena=\expandafter{\AB@authfortoc}%
         \let\\=\authorcr
         \xdef\AB@authfortoc{\the\@temptokena
           \expandafter\noexpand\csname @sep\number\c@authors\endcsname#2}%
         %
      \endgroup
      \ifnum\value{authors}>2\relax
      \@namedef{@sep\number\c@authors}{\Authands}\fi
      \newaffilfalse
}
\renewcommand\affil[2][]%
   {\newaffiltrue\let\AB@blk@and\AB@pand
      \if\relax#1\relax\def\AB@note{\AB@thenote}\else\def\AB@note{#1}%
        \setcounter{Maxaffil}{0}\fi
      \begingroup
        \let\protect\@unexpandable@protect
        \def\thanks{\protect\thanks}\def\footnote{\protect\footnote}%
        \@temptokena=\expandafter{\AB@authors}%
        {\def\\{\protect\\\protect\Affilfont}\xdef\AB@temp{#2}}%
         \xdef\AB@authors{\the\@temptokena\AB@las\AB@au@str
         \protect\\[\affilsep]\protect\Affilfont\AB@temp}%
         \gdef\AB@las{}\gdef\AB@au@str{}%
        {\xdef\AB@temp{#2}}%
        \@temptokena=\expandafter{\AB@affillist}%
        \xdef\AB@affillist{\the\@temptokena \AB@affilsep
          \AB@affilnote{\AB@note}\protect\Affilfont\AB@temp}%
      \endgroup
       \let\AB@affilsep\AB@affilsepx}
\renewcommand\@author{\ifx\AB@affillist\AB@empty\AB@authrunning\else
      \ifnum\value{affil}>\value{Maxaffil}\def\rlap##1{##1}%
    \AB@authlist\\[\affilsep]
    \labelwidth6mm\labelsep\z@\leftmargini\labelwidth
    \edef\@enumctr{enumi}%
    \list\theenumi{\usecounter\@enumctr\def\makelabel##1{\rlap{##1}\hss}}%
      \AB@affillist
    \endlist
    \else  \AB@authors\fi\fi}
\newcommand*\Copyright[1]{%
  \def\@Copyright{%
      \setbox\@tempboxa\hbox{\includegraphics[height=14\p@,clip]{cc-by}}%
      \hspace*{\wd\@tempboxa}\enskip\ifx#1\@empty \else \textcopyright\ #1;\fi\\
      \href{http://creativecommons.org/licenses/by/3.0/}%
           {\smash{\unhbox\@tempboxa}}\enskip
            licensed under Creative Commons License CC-BY\\}}
\Copyright{\textcolor{red}{Author: Please fill in Copyright macro}}
\def\keywords#1{\def\@keywords{#1}}
\let\@keywords\@empty
\def\keywordsHeading{%
  \textcolor{darkgray}{\fontsize{9}{12}\sffamily\bfseries
                       Keywords and phrases\enskip}}
\def\subjclass#1{\gdef\@subjclass{#1}}
\let\@subjclass\@empty
\def\subjclassHeading{%
  \textcolor{darkgray}{\fontsize{9}{12}\sffamily\bfseries
                       2012 ACM Subject Classification\enskip}}
\def\doiHeading{%
  \textcolor{darkgray}{\fontsize{9}{12}\sffamily\bfseries
                       Digital Object Identifier\enskip}}
\def\DateSubmission#1{\gdef\@DateSubmission{#1}}
\def\DateSubmissionHeading{%
  \textcolor{darkgray}{\fontsize{9}{12}\sffamily\bfseries
                       Received\enskip}}
\let\@DateSubmission\@empty
\def\DateAcceptance#1{\gdef\@DateAcceptance{#1}}
\def\DateAcceptanceHeading{%
  \textcolor{darkgray}{\fontsize{9}{12}\sffamily\bfseries
                       Accepted\enskip}}
\let\@DateAcceptance\@empty
\def\DatePublished#1{\gdef\@DatePublished{#1}}
\def\DatePublishedHeading{%
  \textcolor{darkgray}{\fontsize{9}{12}\sffamily\bfseries
                       Published\enskip}}
\let\@DatePublished\@empty
\def\SectionAreaEditor#1{\gdef\@SectionAreaEditor{#1}}
\def\SectionAreaEditorHeading{%
  \textcolor{darkgray}{\fontsize{9}{12}\sffamily\bfseries
                      Editor\enskip}}
\let\@SectionAreaEditor\@empty
\def\SpecialIssue#1{\gdef\@SpecialIssue{#1}}
\def\SpecialIssueHeading{%
  \textcolor{darkgray}{\fontsize{9}{12}\sffamily\bfseries
                      Special issue\enskip}}
\let\@SpecialIssue\@empty
\def\EventShortName#1{\gdef\@EventShortName{#1}}
\EventShortName{}
\def\DOI#1{\gdef\@DOI{#1}}
\DOI{}
\endinput
%%
%% End of file `lites.cls'.
