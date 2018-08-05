# ONLY USE FOR TEST

(define (foo x) (+ x 1))
(define (add x y) (+ x y))
(define (Cons x y) (lambda (i) (if (not (= i 0)) x y)))
(define (Car pair) (pair 1))
(define (Cdr pair) (pair 0))

