;; Load the foreign function interface
(require :cffi)
;; Register the library
(cffi:define-foreign-library libgelisp
  (:unix (:or "libgelisp.so"))
  (t (:default "libgelisp")))
(cffi:use-foreign-library libgelisp)

(cffi::defcfun "domain" :pointer
	       (d :pointer)
	       (size :int))

;; Load the swig generated file
(load "gelisp.lisp")



(cffi::with-foreign-object 
 (array :int 10)
 ;; Initialize array.
 (loop for i from 0 and n in '(7 2 10 4 3 5 1 6 9 8)
       do (setf (cffi::mem-aref array :int i) n))
 (defvar ddd (domain array 10))
 (loop for i from 0 below 10
       collect (cffi::mem-aref array :int i)))

(test)
(printIntSet ddd)

(defvar sp (new_GlSpace))
(defvar i (intVar sp ddd))
(defvar r (relVar sp))
(GlSpace_intBranch sp)
(GlSpace_relBranch sp)
(searchAll sp)

(delete_GlSpace sp)
(write-line "end of execution!")
