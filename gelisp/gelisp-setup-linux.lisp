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
(printIntSet (intSet 1 4))

(defvar sp (new_GlSpace))
(defvar i (intVar sp (intSet 1 5)))
(GlSpace_intbranch sp)
(searchAll sp)

;(GlSpace_info sp)
(write-line "end of execution!")
