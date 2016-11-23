(define (- num . args)
    (define (add-list l)
        (if (null? l) 0 (+ (car l) (add-list (cdr l)))))
    (if (null? args)
        (#opposite num)
        (+ num (#opposite (add-list args)))))

(define (/ num . args)
    (define (multiply-list l)
        (if (null? l) 1 (* (car l) (multiply-list (cdr l)))))
    (* num (#reciprocal (multiply-list args))))

(define (not expr) (if expr #f #t))

(define (and expr . args)
    (define (list-and l)
        (if (null? l) #t (if (car l) (list-and (cdr l)) #f)))
    (if expr (list-and args) #f))

(define (or expr . args)
    (define (list-or l)
        (if (null? l) #f (if (car l) #t (list-or (cdr l)))))
    (if expr #t (list-or args)))

(define (= expr . args)
    (define (equal x y)
        (and (not (< x y)) (not (< y x))))
    (define (equal-list l)
        (if (null? (cdr l)) #t (if (equal (car l) (car (cdr l))) (equal-list (cdr l)) #f)))
    (if (equal expr (car args)) (equal-list args) #f))


(define (remainder a b)
  (if (< a b)
      a
      (remainder (- a b) b)))
