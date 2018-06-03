(define make-point cons)
(define xcor-point car)
(define ycor-point cdr)

(define (distance p1 p2)
  (sqrt (+ (square (- (xcor-point p1) (xcor-point p2))) (square (- (ycor-point p1) (ycor-point p2))))))

# line-drawing routine
(define (line begin end)
  (define (simple-line begin end)
    (define (line-iter cx cy ex d a2b2 a2 result)
      (if (or (< cx ex) (= cx ex))
          (if (< d 0)
              (line-iter (+ cx 1) (+ cy 1) ex (+ d a2b2) a2b2 a2 (make-point (make-point cx cy) result))
              (line-iter (+ cx 1) cy ex (+ d a2) a2b2 a2 (make-point (make-point cx cy) result)))
          result))
    (let ((a (- (ycor-point begin) (ycor-point end)))
          (b (- (xcor-point end) (xcor-point begin))))
      (line-iter (xcor-point begin) (ycor-point begin) (xcor-point end) (+ (* 2 a) b) (* 2 (+ a b)) (* 2 a) nil)))
  (define (reverse dx dy)
    (if (< (abs dy) (abs dx))
        (lambda (point) point)
        (lambda (point) (make-point (ycor-point point) (xcor-point point)))))
  (define (op dx dy)
    (cond ((and (or (< 0 dx) (= dx 0)) (or (< 0 dy) (= 0 dy))) (lambda (point) point))
          ((and (< 0 dy) (< dx 0)) (lambda (point) (make-point (- (xcor-point point)) (ycor-point point))))
          ((and (< 0 dx) (< dy 0)) (lambda (point) (make-point (xcor-point point) (- (ycor-point point)))))
          (else (lambda (point) (make-point (- (xcor-point point)) (- (ycor-point point)))))))
  (define (merge f g dx dy)
    (lambda (point) ((f dx dy) ((g dx dy) point))))
  (let ((dx (- (xcor-point end) (xcor-point begin)))
        (dy (- (ycor-point end) (ycor-point begin))))
    (map (simple-line ((merge reverse op dx dy) begin)
                      ((merge reverse op dx dy) end))
         (merge op reverse dx dy))))

# triangle-drawing routine
(define (triangle p1 p2 p3)
  (reduce (list (line p1 p2) (line p2 p3) (line p3 p1)) append nil))

# sierpinskiTrangle-drawing routine
(define (sierpinskiTriangle p1 p2 p3 accuracy)
  (define (st-aux p1 p2 p3)
    (define (mid-point p1 p2)
      (make-point (/ (+ (xcor-point p1) (xcor-point p2)) 2) (/ (+ (ycor-point p1) (ycor-point p2)) 2)))
    (define (close-enough? p1 p2)
      (if (< (distance p1 p2) accuracy) #t #f))
    (let ((m12 (mid-point p1 p2))
          (m23 (mid-point p2 p3))
          (m31 (mid-point p3 p1)))
      (if (close-enough? m12 m23) nil
          (reduce
           (list  (st-aux p1 m12 m31) (st-aux p2 m12 m23) (st-aux p3 m31 m23) (triangle m12 m23 m31))
           append nil))))
  (append (st-aux p1 p2 p3) (triangle p1 p2 p3)))

# sierpinskiTrangle-drawing routine
(define (sierpinskiTriangle-time p1 p2 p3 times)
  (define (st-aux p1 p2 p3 times)
    (define (mid-point p1 p2)
      (make-point (/ (+ (xcor-point p1) (xcor-point p2)) 2) (/ (+ (ycor-point p1) (ycor-point p2)) 2)))
    (let ((m12 (mid-point p1 p2))
          (m23 (mid-point p2 p3))
          (m31 (mid-point p3 p1)))
      (if (< times 0) nil
          ((lambda (unused)
             (st-aux p1 m12 m31 (- times 1))
             (st-aux p2 m12 m23 (- times 1))
             (st-aux p3 m31 m23 (- times 1))
             (#painter (triangle m12 m23 m31))) 42))))
  (#painter (triangle p1 p2 p3))
  (st-aux p1 p2 p3 times))

# Circle-drawing routine
(define (circle point radius)
  (define (eighth-circle radius)
    (define (circle-iter d x y l)
      (if (> x y) l
          (if (> d 0)
              (circle-iter (- (+ x d 1) y) (+ x 1) (- y 1) (make-point (make-point x y) l))
              (circle-iter (+ d 3 x) (+ x 1) y (make-point (make-point x y) l)))))
    (circle-iter (- 1 radius) 0 radius nil))
  (let ((eighth (eighth-circle radius)))
    (let ((fourth (append eighth (map eighth (lambda (p) (make-point (ycor-point p) (xcor-point p)))))))
      (let ((second (append fourth (map fourth (lambda (p) (make-point (xcor-point p) (- (ycor-point p))))))))
        (let ((first (append second (map second (lambda (p) (make-point (- (xcor-point p)) (ycor-point p)))))))
          (map first (lambda (p) (make-point (+ (xcor-point point) (xcor-point p)) (+ (ycor-point point) (ycor-point p))))))))))


(define (rectangle left right)
  (map
   (list
    (line left (make-point (xcor-point right) (ycor-point left)))
    (line left (make-point (xcor-point left) (ycor-point right)))
    (line right (make-point (xcor-point left) (ycor-point right)))
    (line right (make-point (xcor-point right) (ycor-point left))))
   #painter))

(define (first-point p1 p2)
  (make-point (+ (* 2 (/ (xcor-point p1) 3)) (/ (xcor-point p2) 3))
              (+ (* 2 (/ (ycor-point p1) 3)) (/ (ycor-point p2) 3))))
(define (second-point p1 p2)
  (make-point (+ (* 2 (/ (xcor-point p2) 3)) (/ (xcor-point p1) 3))
              (+ (* 2 (/ (ycor-point p2) 3)) (/ (ycor-point p1) 3))))