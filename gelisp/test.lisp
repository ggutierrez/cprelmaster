;; Load the foreign function interface
(require :cffi)
;; Register the library
(cffi:define-foreign-library lisptest
  (:unix (:or "liblisptest.so"))
  (t (:default "liblisptest")))
(cffi:use-foreign-library lisptest)
;; Load the swig generated file
(load "lisptest.lisp")

;; Work with the functions defined by the library
(Test_out (new_Test "mundo"))
(write-line "end of execution!")