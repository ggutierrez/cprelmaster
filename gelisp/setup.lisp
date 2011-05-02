
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

;;(load "/Users/ggutierrez/Downloads/Final/asdf/asdf.lisp")
;;(pushnew "/Users/ggutierrez/Downloads/Final/trivial-features_0.6/" asdf:*central-registry* :test #'equal)
;;(asdf:operate 'asdf:load-op :trivial-features)
;;(pushnew "/Users/ggutierrez/Downloads/Final/alexandria/" asdf:*central-registry* :test #'equal)
;;(asdf:operate 'asdf:load-op :alexandria)
;;(pushnew "/Users/ggutierrez/Downloads/Final/babel_0.3.0/" asdf:*central-registry* :test #'equal)
;;(asdf:operate 'asdf:load-op :babel)
;;(pushnew "/Users/ggutierrez/Downloads/Final/cffi_0.10.6/" asdf:*central-registry* :test #'equal)
;;(asdf:operate 'asdf:load-op :cffi)

; Instructions to build the library
; gcc -arch i386 -c -fPIC example.c
; gcc -arch i386 -dynamiclib -o libexample.dylib -dylib example.o
; swig -cffi example.i

; only works if the library is 32 bits


(cffi:define-foreign-library libgelisp
    (:darwin "/Users/saschavancauwelaert/Documents/EPL/Memoire/gelisp/build/libgelisp.so")
    (t (:default "libgelisp")))

(load "/Users/saschavancauwelaert/Documents/EPL/Memoire/gelisp/build/gelisp.lisp")
(cffi:use-foreign-library libgelisp)