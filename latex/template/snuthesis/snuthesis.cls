% SNU Thesis Document Class for LaTeX2e
%%%=================================================================
%%%@LaTeX-class-file {
%%%    filename = "snuthesis.cls",
%%%    version  = "1.0",
%%%    date     = "2007/09/25",
%%%    author   = "You, Hyun Jo",
%%%	   codetable= "UTF-8"
%%%    keywords = "LaTeX, SNU, Seoul National University, thesis",
%%%	   docstring= "
%%%				 The class file is intended to meet the requirements
%%%				 for Seoul National University Ph.D. dissertations.
%%%
%%%				 The class file is based on:
%%%					 kaist-thesis.cls 1998-11-23 version made by Cho, Jin-Hwan,
%%%					 and huthesis.cls 2004-01-06 by Alex Barnett."
%%%}
%%%================================================================

%%%%%%%%%%%%%%%%         Identification           %%%%%%%%%%%%%%%%

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{snuthesis}[2007/09/25 v1.0 Seoul National University thesis class]


%%%%%%%%%%%%%%%%           Initial Code            %%%%%%%%%%%%%%%%

\newif\if@draft
\@draftfalse

\newif\if@screen
\@screenfalse

\newif\if@final
\@finalfalse

\newif\if@korean
\@koreanfalse



%%%%%%%%%%%%%%%%          Declaration of options    %%%%%%%%%%%%%%%%

\DeclareOption{korean}{%
	\@koreantrue%
	\typeout{SNU Thesis Class Option: `korean' mode}%
}%

\DeclareOption{draft}{%
	\@drafttrue%
	\typeout{SNU Thesis Class Option: `draft' mode}%
	\PassOptionsToClass{draft,a4paper}{report}%
}%

\DeclareOption{screen}{%
	\@screentrue%
	\typeout{SNU Thesis Class Option: `screen' mode}%
}

\DeclareOption{final}{%
	\@finaltrue%
	\typeout{SNU Thesis Class Option: `final' mode}%
}%





\DeclareOption{hangul}{\PassOptionsToPackage{hangul}{kotex}}
\DeclareOption{nojosa}{\PassOptionsToPackage{nojosa}{kotex}}
\DeclareOption{hanja}{\PassOptionsToPackage{hanja}{kotex}}
\DeclareOption{nonfrench}{\PassOptionsToPackage{nonfrench}{kotex}}
\DeclareOption{finemath}{\PassOptionsToPackage{finemath}{kotex}}
\DeclareOption{strictcharcheck}{\PassOptionsToPackage{strictcharcheck}{kotex}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}


%%%%%%%%%%%%%%%%           Execution of options       %%%%%%%%%%%%%%%%

%\ExecuteOptions{korean,hangul,nonfrench,finemath,a4paper,11pt,twoside,openright}
\ExecuteOptions{hangul,nonfrench,finemath,11pt,twoside,openright}


\ProcessOptions

%%%%%%%%%%%%%%%%            Package loading            %%%%%%%%%%%%%%%%

\if@korean
  \RequirePackageWithOptions{kotex}
\fi

% \usepackage[default]{dhucs-interword}
% \usehangulfontspec{ut}
% \usepackage[hangul]{dhucs-setspace}
% \usepackage{dhucs-gremph}
% \usepackage{dhucsfn}
% \usepackage{dhucs-trivcj}
% \usepackage{dhucs-midkor}
% \usepackage{dhucs-enumitem}
% \usepackage{dhucs-enumerate}
% \usepackage{dhucs-paralist}

%%%%%%%%%%%%%%%%              Special commands              %%%%%%%%%%%%%%%%

\AtBeginDocument{
}

%%%%%%%%%%%%%%%%            Special class file commands     %%%%%%%%%%%%%%%%

\LoadClassWithOptions{report}




% Layout
%================================================================
\if@draft
    \topmargin=-10mm
    \oddsidemargin=0.0mm
	\textwidth=150mm
	\textheight=220mm
	\oddsidemargin=10.0mm
	\evensidemargin=\oddsidemargin
    \footskip=10mm
    \headheight=10.0mm
    \headsep=10.0mm
    \overfullrule=0pt
\else\if@screen
    \paperwidth=145mm
    \paperheight=230mm
    \topmargin=-24.6mm
    \oddsidemargin=-20mm
    \evensidemargin=-20mm
    \textwidth=135mm
    \textheight=205mm
    \footskip=5mm
    \headheight=10.0mm
    \headsep=5.0mm
    \overfullrule=0pt
\else
    %% paper size
	%%--------------------------------
    %% 46배판  257 x 188   cf) b5paper 250 x 176
    \paperwidth  = 188mm
    \paperheight = 257mm

    \textwidth=135mm
    %\advance\textwidth by -60mm
    \textheight=205mm
    %\advance\textheight by -60mm

%    \topmargin  = -14.6mm
    \topmargin  = -17.6mm

    \headheight =  10mm
    %\headsep    =  10mm
    \headsep = 8mm
    %\advance\topmargin 19mm        % comment out if want exact top margin

    %% oddside horizontal:
    %--------------------------------
    \oddsidemargin=1.1mm
    %\advance\oddsidemargin 10mm    % comment out if want exact left margin
    \evensidemargin=1.1mm

    \footskip=10mm
    \overfullrule=0pt
    %%
\fi
\fi











% PREPARING A FOREIGN LANGUAGE VERSION:
%
%

\if@korean
    \def\contentsname{차례}
    \def\listfigurename{그림 차례}
    \def\listtablename{표 차례}
    \def\refname{참고문헌}
    \def\bibname{참고문헌}
    \def\indexname{찾아보기}
    \def\figurename{그림}
    \def\tablename{표}
	\def\appendixname{부록}
	\def\summaryname{국문초록}
	\def\ackname{감사의 글}
	\def\acknowledgename{감사의 글}
	%\def\esummaryname{Summary in Korean}
	%\def\eackname{Acknowledgement in Korean}
\fi
%  \expandafter\split@code\@code\eop
%  \ifx\D@char\@md\def\@degree{박사}\else\def\@degree{석사}\fi





%   ****************************************************************
%   * Front Matter
%   ****************************************************************



%% Declarations
% author
% degree
% title
% degreemonth
% degreeyear
% university
% department
% major
% advisor


\def\author#1{\def\@author{#1}}
%\def\ename#1{\def\@ename{#1}}
\def\studentid#1{\def\@studentid{#1}}

\def\title#1{\def\@title{#1}}
%\def\etitle#1{\def\@etitle{#1}}
\def\subtitle#1{\def\@subtitle{#1}}

% degree
% eg) 문학박사, 이학박사, 공학박사
\newcommand*{\@degree}{
  \ClassError{snuthesis}{
	You must specify the type of degree!}
  {use the \protect\degree{} command in the preamble.}
}
\def\degree#1{\def\@degree{#1}}
%\def\edegree#1{\def\@edegree{#1}}



\def\degreedate#1{\gdef\@degreedate{#1}}
\def\degreeyear#1{\gdef\@degreeyear{#1}}
\def\degreemonth#1{\gdef\@degreemonth{#1}}


\def\department#1{\def\@department{#1}}
\def\major#1{\def\@major{#1}}
%\def\edept#1{\def\@edept{#1}}
%\def\emajor#1{\def\@emajor{#1}}

\def\submitdate#1#2{\def\@submitdate{#1년 #2월}}
\def\signdate#1#2{\def\@signdate{#1년 #2월}}

\def\advisor#1{\def\@advisor{#1}}

\def\supervisorA#1{\def\@supervisorA{#1}}
\def\supervisorB#1{\def\@supervisorB{#1}}
\def\supervisorC#1{\def\@supervisorC{#1}}
\def\supervisorD#1{\def\@supervisorD{#1}}
\def\supervisorE#1{\def\@supervisorE{#1}}


%\def\code#1{\def\@code{#1}}


%\def\advisorsigndate#1#2#3{\def\@advisorsigndate{#1. #2. #3.}}
%\def\supervisorsigndate#1#2#3{\def\@supervisorsigndate{#1년 #2월 #3일}}



%\def\npages#1{\def\@npages{#1}}
%\def\@npages{\pageref{paperlastpagelabel}}
%\def\D@char{D}
%\def\@md\code
%\def\split@code#1#2\eop{\def\@md{#1}\def\@department{#2}}
%%
% \def\insert@hss#1#2{#1#2\hss}
 \def\maketitlebreakwork{\def\titlebreak{\\\ignorespaces}}
 \def\maketitlebreaknotwork{\def\titlebreak{\ }}
 \maketitlebreaknotwork
 \pagestyle{plain}



%% Title page and approval page
%%================================================================
%% 표지
%% \maketitle
%%  
%% \fontsize{17}{34}\selectfont
%% OO박사학위논문 14pt
%% 제목 18pt
%% 날짜 16pt
%% 서울대학교 대학원 20pt
%% --학과 --전공 16pt
%% 저자명 16pt
\if@final
    \def\maketitle{%
    	\typeout{making tile page}

        %%표지
        \newpage
        \pagestyle{empty}
         \vspace*{0mm}
        {\centering
            \renewcommand{\baselinestretch}{1.0}
            \fontsize{14}{14}\selectfont\@degree{}학위논문\\
            \vspace*{20mm}
            \fontsize{22}{22}\selectfont\@title\\
            \renewcommand{\baselinestretch}{1.66}
            \fontsize{16}{16}\selectfont\@subtitle\\
            \vfill
            \renewcommand{\baselinestretch}{1.0}
            %\fontsize{14}{14}\selectfont\@degreeyear{}년 \@degreemonth{}월\\
            \fontsize{14}{14}\selectfont\@degreedate\\
            \vspace*{40mm}
            \fontsize{16}{16}\selectfont 서울대학교 대학원\\
            \renewcommand{\baselinestretch}{2.20}
            \fontsize{14}{14}\selectfont\@department{} \@major 전공\\
            \fontsize{16}{16}\selectfont\@author{}\\
            \vspace*{5mm}
        }
        %% 인준지
		\cleardoublepage
        %\newpage
        %\thispagestyle{empty}
  }
  \def\makesignedpage{
    	{\centering
            \renewcommand{\baselinestretch}{1.66}
            	{\maketitlebreakwork
                  \fontsize{22}{22}\selectfont\@title\\
                  \fontsize{16}{16}\@subtitle
                  \maketitlebreaknotwork}\\
		  	\vfill%----------------------------------------------------------------
            \fontsize{14}{14}\selectfont 지도교수\hspace{2pc}
            \fontsize{14}{14}\@advisor\\
          	\vfill%----------------------------------------------------------------
            \fontsize{16}{16}\selectfont
                이 논문을 {\@degree} 학위논문으로 제출함.\\
            \fontsize{14}{14}\selectfont \@submitdate\\
          	\vfill%----------------------------------------------------------------
            \fontsize{16}{16}\selectfont 서울대학교 대학원\\
            \fontsize{14}{14}\selectfont
                \@department{} \@major{}전공\\
            \fontsize{16}{16}\selectfont\@author{}\\
          	\vfill%----------------------------------------------------------------
            \fontsize{16}{16}\selectfont \@author{}의 \@degree{} 학위논문을 인준함.\\
            \fontsize{14}{14}\selectfont\@signdate\\
          	\vfill%----------------------------------------------------------------
            \fontsize{14}{14}\selectfont
            $$\vbox{
            	\baselineskip = 36pt
                                    \tabskip = 10pt
              	\halign{
                	##&
                	\underline{\hspace{5cm}}##&
    				\small##
              	\cr
              		위\hfill 원\hfill 장&		     &(인)\cr
              		부\hfill 위\hfill 원\hfill 장&   &(인)\cr
              		위\hfill 원& 					 &(인)\cr
              		위\hfill 원& 					 &(인)\cr
              		위\hfill 원& 					 &(인)\cr
            }}$$
         }

	}% end of \def\maketitle
\fi



  %% Dedication
  %% 바치는 글





  %% Abstract in Korean
  %% 국문초록
  \def\summary{%
    \newpage
    \pagenumbering{roman}\thispagestyle{plain}
    \if@korean
	\addcontentsline{toc}{chapter}{\summaryname}
    \else
    	\addcontentsline{toc}{chapter}{\esummaryname}
    \fi
    \noindent\summaryname
    \vspace*{5mm}%
    \begin{center}
        \maketitlebreakwork\bfseries
        % title
        \fontsize{13pt}{13pt}\selectfont
        \@title\\
        % subtitle
        \fontsize{11pt}{11pt}\selectfont
        \@subtitle
        \maketitlebreaknotwork
    \end{center}
    \vspace*{5mm}%
  }
  % \def\endsummary{\newpage\pagenumbering{arabic}\addtocounter{page}{1}}



  % 백색 별지

  % 차례(Contents)
  \def\makecontents{%
    % {\let\thispagestyle\@gobble \tableofcontents}%
    \tableofcontents
    \newpage{\pagestyle{empty}\cleardoublepage}
    \listoftables
    \newpage{\pagestyle{empty}\cleardoublepage}
    \listoffigures
    \newpage{\pagestyle{empty}\cleardoublepage}
    \pagenumbering{arabic}%
  }




%****************************************************************
%* Main Matter
%****************************************************************

\setcounter{tocdepth}{4}
\setcounter{secnumdepth}{4}

%%
%% Chapter
%%

  \def\@chapapp{}
  \def\@makechapterhead#1{%
    \vspace*{5mm}%
    {\parindent=0pt
      \begin{center}
        \ifnum\c@secnumdepth>\m@ne\LARGE\bfseries\@chapapp{ }\thechapter.{ }\fi
        \LARGE\bfseries#1\par\nobreak
        \vspace{5mm}
      \end{center}%
    }%
  }
  \def\@makeschapterhead#1{%
    \vspace*{5mm}%
    {\parindent=0pt
      \begin{center}
        \LARGE\bfseries#1\par\nobreak
        \vspace{5mm}
      \end{center}%
    }%
  }



%%
%% section
%%
  \def\section{\@startsection 
    {section}{1}{\z@}% name, level, indent
    {3.5ex plus 1ex minus .2ex}% beforeskip
    {2.3ex plus .2ex}% afterskip
    {\normalfont\Large\bfseries}}% style
%%
%% subsection
%%

  \def\subsection{\@startsection
    {subsection}{2}{\z@}% name, level, indent
    {3.25ex plus 1ex minus .2ex}% beforeskip
    {1.5ex plus .2ex}% afaterskip
    {\normalfont\large\bfseries}}% style

%%
%% subsubsection
%%

  \def\subsubsection{\@startsection
    {subsubsection}{3}{\z@}% name, level, indent
    {3.25ex plus 1ex minus .2ex}% beforeskip
    {1.0ex plus .2ex}% afterskip
    {\normalfont\bfseries}}% style



  %% 부록
  %% Appendix
  \def\appendix{%
    \par\setcounter{chapter}{0}\setcounter{section}{0}
    \if@korean
		\def\@chapapp{부록}
	\else
		\def\@chapapp{Appendix}
	\fi
    \def\thechapter{\Alph{chapter}}%
  }

  %% 참고문헌
  %% References
  \def\thebibliography#1{%
    \chapter*{\bibname\@mkboth{\MakeUppercase\bibname}{\MakeUppercase\bibname}}%
    \addcontentsline{toc}{chapter}{\protect\bibname}%
    \list{\@biblabel{\@arabic\c@enumiv}}%
    {\settowidth\labelwidth{\@biblabel{#1}}%
      \leftmargin\labelwidth
      \advance\leftmargin\labelsep
      \itemsep\medskipamount
      \@openbib@code
      \usecounter{enumiv}%
      \let\p@enumiv\@empty
      \renewcommand\theenumiv{\@arabic\c@enumiv}}%
    \sloppy
    \clubpenalty4000
    \@clubpenalty \clubpenalty
    \widowpenalty4000%
    \sfcode`\.\@m%
  }


  %% 찾아보기
  %% Index


  %% Abstract 
  %% 영문초록
  \def\abstract{%
    \newpage
    % \pagenumbering{roman}\setcounter{page}{4}
    \addcontentsline{toc}{chapter}{ABSTRACT}
    \hbox{%
      \parbox[t]{30mm}{\sffamily\fontsize{10}{12pt}\selectfont
        % \strut\@code\\\@studentid
      }%
      \advance\linewidth by-30mm
      \def\@dot{.\hspace{.5em}}

      % \parbox[t]{\linewidth}{\sffamily\grfamily\fontsize{10}{12pt}\selectfont
      %   \@author\@dot\@ename\@dot\@etitle\@dot\@title\@dot
      %   Department of \@edept\@dot\@covedate\@dot\@npages{p}\@dot
      %   \ifx\@coadvisor\@@@@undefined@@@@ Advisor Prof.~\@advisor
      %   \else Advisors Prof.~\@advisor, Prof.~\@coadvisor\fi\@dot
      %   \ifx\@korean\@@@@undefined@@@@ Text in English.\fi}%
    }%

    \begin{center}
      \fontsize{11pt}{11pt}\selectfont\bfseries ABSTRACT
    \end{center}
    \vspace*{5mm}
  }
  % \def\endabstract{\newpage\pagenumbering{arabic}\addtocounter{page}{1}}


  %% Acknowledgements
  %% 감사의 글
  % 
  \def\acknowledgement{%
    \label{paperlastpagelabel}
    \chapter*{\ackname}
    % \thispagestyle{empty}\pagestyle{empty}
    \if@korean 
	    % \addtocontents{toc}{\protect\contentsline{chapter}{\eackname}{}}
    	\addcontentsline{toc}{chapter}{\ackname}%
    \else
	    % \addtocontents{toc}{\protect\contentsline{chapter}{\ackname}{}}
    	\addcontentsline{toc}{chapter}{\eackname}%
    \fi
  }


  %% 백색 별지

  %% 뒤표지(Back Cover)


\endinput
