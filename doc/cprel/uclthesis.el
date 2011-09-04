(add-to-list 'org-export-latex-classes
 	     '("uclthesis"
 	       "\\documentclass{article}
           [NO-DEFAULT-PACKAGES]
           [PACKAGES]
           [EXTRA]"
 	       ("\\section{%s}" . "\\section*{%s}")
 	       ("\\subsection{%s}" . "\\subsection*{%s}")
 	       ("\\subsubsection{%s}" . "\\subsubsection*{%s}")
 	       ("\\paragraph{%s}" . "\\paragraph*{%s}")
 	       ("\\subparagraph{%s}" . "\\subparagraph*{%s}")))
