;; Load quicklisp (the easiest way to get cffi in most of the systems)
;; More info: http://www.quicklisp.org/beta/

(ql:quickload "cffi")
;; Load the foreign function interface
(require :cffi)
;; Register the library
(cffi:define-foreign-library libgelisp
  (:unix (:or "libgelisp.so"))
  (t (:default "libgelisp")))
(cffi:use-foreign-library libgelisp)

;; Load the swig generated file
(load "gelisp.lisp")



(test)

(defvar sp (new_GlSpace))
(defvar r (relVar sp))
(GlSpace_relBranch sp)
(searchAll sp)

(delete_GlSpace sp)
(write-line "end of execution!")
