(define (cons x y) (lambda (i) (if i x y)))
(define (car pair) (pair 0))
(define (cdr pair) (pair 1))

