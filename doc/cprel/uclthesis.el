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

;(setq org-export-latex-listings 'minted)
;(setq org-export-latex-custom-lang-environments
;      '(
;	(emacs-lisp "common-lispcode")
;	))
;(setq org-export-latex-minted-options
;      '(
;	("bgcolor" "codebg")
;	("frame" "single")
;	("fontsize" "\\scriptsize")
;	("linenos" "")))
;(setq org-latex-to-pdf-process
;      '("pdflatex -shell-escape -interaction nonstopmode -output-directory %o %f"
;	"pdflatex -shell-escape -interaction nonstopmode -output-directory %o %f"
;	"pdflatex -shell-escape -interaction nonstopmode -output-directory %o %f"))
