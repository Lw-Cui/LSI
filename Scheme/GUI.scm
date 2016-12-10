(define x car)
(define y cdr)

(define (line begin end)
    (define (simple-line begin end)
        (define (line-iter cx cy ex d a2b2 a2 result)
        (if (or (< cx ex) (= cx ex))
            (if (< d 0)
                (line-iter (+ cx 1) (+ cy 1) ex (+ d a2b2) a2b2 a2 (cons (cons cx cy) result))
                (line-iter (+ cx 1) cy ex (+ d a2) a2b2 a2 (cons (cons cx cy) result)))
            result))
    (let ((a (- (y begin) (y end)))
        (b (- (x end) (x begin))))
        (line-iter (x begin) (y begin) (x end) (+ (* 2 a) b) (* 2 (+ a b)) (* 2 a) nil)))
    (define (reverse dx dy)
        (if (< (abs dy) (abs dx))
            (lambda (point) point)
            (lambda (point) (cons (y point) (x point)))))
    (define (op dx dy)
        (cond ((and (or (< 0 dx) (= dx 0)) (or (< 0 dy) (= 0 dy))) (lambda (point) point))
              ((and (< 0 dy) (< dx 0)) (lambda (point) (cons (- (x point)) (y point))))
              ((and (< 0 dx) (< dy 0)) (lambda (point) (cons (x point) (- (y point)))))
              (else (lambda (point) (cons (- (x point)) (- (y point)))))))
    (define (merge f g dx dy)
        (lambda (point) ((f dx dy) ((g dx dy) point))))
    (let ((dx (- (x end) (x begin)))
        (dy (- (y end) (y begin))))
        (map (simple-line ((merge reverse op dx dy) begin)
                          ((merge reverse op dx dy) end))
             (merge op reverse dx dy))))

(define (triangle p1 p2 p3)
    (reduce (list (line p1 p2) (line p2 p3) (line p3 p1)) append))

(define (sierpinskiTriangle p1 p2 p3)
    (define (st-aux p1 p2 p3)
        (define (mid-point p1 p2)
            (cons (/ (+ (x p1) (x p2)) 2) (/ (+ (y p1) (y p2)) 2)))
        (define (close-enough? p1 p2)
            (define (distance p1 p2)
                (+ (square (- (x p1) (x p2))) (square (- (y p1) (y p2)))))
            (if (< (distance p1 p2) 100) #t #f))
        (let ((m12 (mid-point p1 p2))
            (m23 (mid-point p2 p3))
            (m31 (mid-point p3 p1)))
        (if (close-enough? m12 m23) nil
            (reduce
            (list  (st-aux p1 m12 m31) (st-aux p2 m12 m23) (st-aux p3 m31 m23) (triangle m12 m23 m31))
            append))))
    (append (st-aux p1 p2 p3) (triangle p1 p2 p3)))