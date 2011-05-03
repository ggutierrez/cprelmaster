;;Load dependencies
(defvar *deps_path* "/Users/saschavancauwelaert/Documents/EPL/Memoire/Final/")
(defvar *_asdf* (concatenate 'string *deps_path* "asdf/asdf.lisp"))
(load *_asdf*)

;; trivial-features
(defvar *_tf* (concatenate 'string *deps_path* "trivial-features_0.6/"))
(pushnew *_tf* asdf:*central-registry* :test #'equal)
(asdf:operate 'asdf:load-op :trivial-features)

;; alexandria
(defvar *_alx* (concatenate 'string *deps_path* "alexandria/"))
(pushnew *_alx* asdf:*central-registry* :test #'equal)
(asdf:operate 'asdf:load-op :alexandria)

;; babel
(defvar *_bbl* (concatenate 'string *deps_path* "babel_0.3.0/"))
(pushnew *_bbl* asdf:*central-registry* :test #'equal)
(asdf:operate 'asdf:load-op :babel)

;; cffi
(defvar *__cffi* (concatenate 'string *deps_path* "cffi_0.10.6/"))
(pushnew *__cffi* asdf:*central-registry* :test #'equal)
(asdf:operate 'asdf:load-op :cffi)

;; Load the foreign function interface
;(require :cffi)
;; Register the library
(cffi:define-foreign-library libgelisp
  (:darwin (:or "/Users/saschavancauwelaert/Documents/EPL/Memoire_sans_dropbox/buildminimal/build/libgelisp.so"))
  (t (:default "libgelisp")))
(cffi:use-foreign-library libgelisp)
;; Load the swig generated file
(load "/Users/saschavancauwelaert/Documents/EPL/Memoire_sans_dropbox/buildminimal/build/gelisp.lisp")

(test)
(printIntSet (intSet 1 4))

(defvar sp (new_GlSpace))
(defvar i (intVar sp (intSet 1 5)))
(GlSpace_intbranch sp)
(searchAll sp)

;(GlSpace_info sp)
(write-line "end of execution!")
