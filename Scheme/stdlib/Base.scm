# Basic function set

(define (and expr . args)
    (define (list-and l)
        (cond ((null? l) #t)
              ((car l) (list-and (cdr l)))
              (else #f)))
    (if expr (list-and args) #f))

(define (not expr) (if expr #f #t))

(define (or expr . args)
    (define (list-or l)
        (cond ((null? l) #f)
              ((car l) #t)
              (else (list-or (cdr l)))))
    (if expr #t (list-or args)))

(define (= expr . args)
    (define (equal x y)
        (and (not (< x y)) (not (< y x))))
    (define (equal-list l)
        (cond ((null? (cdr l)) #t)
              ((equal (car l) (car (cdr l))) (equal-list (cdr l)))
              (else #f)))
    (if (equal expr (car args)) (equal-list args) #f))

(define (> expr . args)
  (define (greater x y)
    (if (not (or (< x y) (= x y))) #t #f))
  (define (greater-list l)
    (cond ((null? (cdr l)) #t)
          ((greater (car l) (car (cdr l))) (greater-list (cdr l)))
          (else #f)))
  (if (greater expr (car args)) (greater-list args) #f))

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


(define (remainder a b)
  (if (< a b) a (remainder (- a b) b)))

(define (reverse l)
    (define (reverse-iter li res)
        (if (null? li) res (reverse-iter (cdr li) (cons (car li) res))))
    (reverse-iter l nil))

(define (append l1 . more)
    (define (append-iter l1 l2)
        (if (null? l1) l2 (append-iter (cdr l1) (cons (car l1) l2))))
  (define (append-list l1 more)
    (if (null? more) l1
        (append-list (append-iter (reverse l1) (car more)) (cdr more))))
  (if (null? more) l1 (append-list l1 more)))

(define (map seq op)
    (define (map-iter seq res)
        (if (null? seq) res (map-iter (cdr seq) (cons (op (car seq)) res))))
    (map-iter (reverse seq) nil))

(define (length seq)
    (define (length-iter seq n)
        (if (null? seq) n (length-iter (cdr seq) (+ n 1))))
    (length-iter seq 0))

(define (abs x) ((if (< 0 x) + -) x))

(define (reduce seq op init)
    (define (reduce-iter seq res)
        (if (null? seq) res (reduce-iter (cdr seq) (op res (car seq)))))
    (reduce-iter seq init))

(define (half x) (/ x 2))

(define (square x) (* x x))

(define (sqrt x)
    (define (average x y) (/ (+ x y) 2))
    (define (close-enough? guess) (< (abs (- (square guess) x)) 0.01))
    (define (improve guess) (average guess (/ x guess)))
    (define (sqrt-iter guess) (if (close-enough? guess) guess (sqrt-iter (improve guess))))
    (sqrt-iter 1.0))

# Y-combinator
(define Y
    (lambda (fn)
    ((lambda (f)
        (f f)) (lambda (f)
            (fn (lambda (s) ((f f) s)))))))

# end basic function set
