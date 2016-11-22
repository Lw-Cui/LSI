(define (add-list l) (if (null? l) 0 (+ (car l) (add-list (cdr l)))))
(define (- num . args) (+ num (~ (add-list args))))
(define (not expr) (if expr #f #t))
