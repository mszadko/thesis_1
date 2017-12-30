%
% Klasa do pisania prac dyplomowych - inzynierskich i magisterskich - w SGGW
%
% Wersja 1.01 z 2017.12.13
% Autor: Leszek Chmielewski
% Zmiana: poprawka stylu podpisow rys i tab: \RequirePackage[...]{caption}

% Wersja 1.0 z 2017.01.16.
% Autor: Lukasz Adamczyk
%
\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{SGGW-thesis}[2017/01/06 LaTeX class for writing Thesis in SGGW]


\DeclareOption{multip}{\@multiptrue}
\DeclareOption{onep}{\@multipfalse}

\newif\if@multip
\ExecuteOptions{multip}
\@multipfalse

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}
\DeclareOption*{}
\ProcessOptions*\relax
\LoadClass[12pt,notitlepage,twoside]{report}


\RequirePackage[T1]{fontenc}
\RequirePackage{mathptmx} %czcionka Times New Roman
\RequirePackage[utf8]{inputenc}
\RequirePackage[nottoc,numbib]{tocbibind} %bibliografia z numerem i w spisie tresci
\RequirePackage{xifthen}
\RequirePackage[labelsep=period]{caption} %kropka oddziela numer od tytulu rysunku lub tabeli
%\RequirePackage[labelfont=bf,labelsep=period]{caption} %tytuly rys i tab: gruba czcionka, kropka

\RequirePackage[a4paper,top=2.5cm,bottom=2.5cm,inner=3.5cm,outer=2cm]{geometry}

\RequirePackage{polski}
\selecthyphenation{polish}

\linespread{1.25}

\RequirePackage{titlesec} % zmiana formatu chapter do (1 Nazwa)
\titleformat{\chapter}
  {\normalfont\LARGE\bfseries}{\thechapter}{1em}{}
\titlespacing*{\chapter}{0pt}{3.5ex plus 1ex minus .2ex}{2.3ex plus .2ex}

\renewcommand{\normalsize}{\fontsize{12}{13}\selectfont}

\newcommand\VeryLarge{\@setfontsize\VeryLarge{16pt}{21}}	%definicja czcionki 16pt
\newcommand\VeryVeryLarge{\@setfontsize\VeryLarge{24pt}{30}} %definicja czcionki 24pt

%definicje zmiennych
\def\@album{}
\newcommand{\album}[1]{
  \def\@album{#1}
}

\def\@university{}
\newcommand{\university}[1]{
  \def\@university{#1}
}
\def\@Etitle{}
\newcommand{\Etitle}[1]{
  \def\@Etitle{#1}
}
\def\@thesis{}
\newcommand{\thesis}[1]{
  \def\@thesis{#1}
}
\def\@course{}
\newcommand{\course}[1]{
  \def\@course{#1}
}
\def\@promotor{}
\newcommand{\promotor}[1]{
  \def\@promotor{#1}
}
\def\@pworkplace{}
\newcommand{\pworkplace}[1]{
  \def\@pworkplace{#1}
}
\def\@dep{}
\newcommand{\dep}[1]{
  \def\@dep{#1}
}


\renewcommand{\maketitle}{ %definicja strony tytulowej
	\begin{center}
		\thispagestyle{empty}
		{\VeryLarge{\@university}}\\
		{\VeryLarge{\@dep}}\\
		\vspace{3cm}
		{\Large{\@author}}\\
		{\large{\@album}}\\
		\vspace{3cm}
		{\VeryVeryLarge{\@title}}\\
		\vspace{1cm}
		{\VeryLarge{\@Etitle}}\\
		\vspace{1cm}
		{\Large{\@thesis}}\\
		\vspace{0.1cm}
		{\Large{na kierunku \@course}}\\

	\begin{flushright}
		\vfill
		{\large{Praca wykonana pod kierunkiem}}\\
		{\large{\@promotor}}\\
		{\large{\@pworkplace}}\\
		\vspace{1cm}
	\end{flushright}
	{\large{Warszawa \@date}}\\
	\end{center}
	\newpage
	\if@multip
	\else
		
		\null
		\pagestyle{empty}
		\newpage
		\pagestyle{plain}
	\fi
}
\newcommand{\twoppage}[6]{ %definicja strony z dwoma autorami pracy
	\null
	\vfill
	\noindent{\large{Praca przygotowana zespo{\l}owo przez:}}\\
	\begin{quote}
	\noindent{\Large{1. #1}}\\
	{\large{#2\\
	kt\'{o}ry jest autorem:\\
	#3\\
	}}
	\end{quote}
	\vfill
	\begin{quote}
	\noindent{\Large{2. #4}}\\
	{\large{#5\\
	kt\'{o}ry jest autorem:\\
	#6\\
	}}
	\end{quote}
	\vfill
	\thispagestyle{empty}
	\newpage
	\pagestyle{plain}
}

\newcommand{\threeppage}[9]{ %definicja strony z trzema autorami pracy
	\null
	\vfill
	\noindent{\large{Praca przygotowana zespo{\l}owo przez:}}\\
	\begin{quote}
		\noindent{\Large{1. #1}}\\
		{\large{#2\\
		kt\'{o}ry jest autorem:\\
		#3\\
		}}
	\end{quote}
	\vfill
	\begin{quote}
		\noindent{\Large{2. #4}}\\
		{\large{#5\\
		kt\'{o}ry jest autorem:\\
		#6\\
		}}
	\end{quote}
	\vfill
	\begin{quote}
		\noindent{\Large{3. #7}}\\
		{\large{#8\\
		kt\'{o}ry jest autorem:\\
		#9\\
		}}
	\end{quote}	
	\vfill
	\thispagestyle{empty}
	\newpage
	\pagestyle{plain}	
}
\newcommand{\statementpage}{ %definicja strony z oswiadczeniami
	\null
	\vfill
	\begin{center}
		{\textbf{O\'{s}wiadczenie promotora pracy}}\\
	\end{center}
		O\'{s}wiadczam, że
		\if@multip
			wskazane przez autora rozdzia{\l}y pracy dyplomowej przygotowanej zespo{\l}owo zosta{\l}y przygotowane pod moim kierunkiem i stwierdzam, że spe{\l}niaj\k{a} one
		\else
			niniejsza praca zosta{\l}a przygotowana pod moim kierunkiem i stwierdzam, że spe{\l}nia ona
		\fi
		 warunki do przedstawienia tej pracy w post\k{e}powaniu o nadanie tytu{\l}u zawodowego.\\
		
		
		\noindent
		Data \dotfill
		\hspace{2cm}
		Podpis promotora pracy \dotfill

	\vfill
	\begin{center}
		{\textbf{O\'{s}wiadczenie autora pracy}}\\
	\end{center}
		\'{S}wiadom odpowiedzialno\'{s}ci prawnej, w tym odpowiedzialno\'{s}ci karnej za z{\l}ożenie fa{\l}szywego o\'{s}wiadczenia, o\'{s}wiadczam, że
		\if@multip
			wskazane przeze mnie rozdzia{\l}y pracy dyplomowej przygotowanej zespo{\l}owo zosta{\l}y napisane przeze mnie samodzielnie i nie zawieraj\k{a}
		\else
			niniejsza praca dyplomowa zosta{\l}a napisana przeze mnie samodzielnie i nie zawiera
		\fi
		  tre\'{s}ci uzyskanych w spos\'{o}b niezgodny z obowi\k{a}zuj\k{a}cymi przepisami prawa, w szczeg\'{o}lno\'{s}ci z ustaw\k{a} z dnia
		4~lutego 1994~r.\ o~prawie autorskim i~prawach pokrewnych (Dz.\ U.\ Nr 90 poz.\ 631 z p\'{o}\'{z}n.\ zm.)\\[2ex]
		O\'{s}wiadczam, że przedstawiona praca nie by{\l}a wcze\'{s}niej podstaw\k{a} żadnej procedury zwi\k{a}zanej z nadaniem dyplomu lub uzyskaniem tytu{\l}u zawodowego.\\%[2ex]
		
		\noindent
		O\'{s}wiadczam, że niniejsza wersja pracy jest identyczna z za{\l}\k{a}czon\k{a} wersj\k{a} elektroniczn\k{a}.
		Przyjmuj\k{e} do wiadomo\'{s}ci, że praca dyplomowa poddana zostanie procedurze antyplagiatowej.
		\\
		
		\noindent
		Data \dotfill
		\hspace{2cm}
		Podpis autora pracy \dotfill
	\vfill
	\pagestyle{empty}	
	\newpage
	\null
	\pagestyle{empty}
	\newpage
	\pagestyle{plain}
}
\newcommand{\abstractpage}[6]{ %definicja strony ze streszczeniami
	\null
	\vfill
	\begin{center}
		\textbf{Streszczenie}\\
	\end{center}
		\noindent
		\textbf{#1}\\
		{#2}
		\\[4ex]
		S{\l}owa kluczowe -- {#3}
	
	\vfill
	\begin{center}
		\textbf{Summary}\\
	\end{center}
		\noindent
		\textbf{#4}\\
		{\selecthyphenation{english}#5}
		\\[4ex]
		Keywords -- {\selecthyphenation{english}#6}
	\vfill
	\pagestyle{empty}
	\newpage
	\null
	\pagestyle{empty}
	\newpage
	\pagestyle{plain}
}
\newcommand{\beforelastpage}[1][]{ %definicja strony przedostatniej
	\newpage
	\thispagestyle{plain}
	\null
	\vfill
	\begin{flushleft}
		Wyrażam zgod\k{e} na udost\k{e}pnienie mojej pracy w~czytelniach Biblioteki SGGW w~tym w~Archiwum Prac Dyplomowych SGGW%
		\ifthenelse{\isempty{#1}}{}{po roku #1}.
	\end{flushleft}
	\vspace{2cm}
	\begin{flushright}
		\hfill\hfill\dotfill\\
		(czytelny podpis autora pracy)
	\end{flushright}
	\vfill
}
