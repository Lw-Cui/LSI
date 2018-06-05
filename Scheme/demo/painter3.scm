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

              (define make-vect cons)
(define (xcor-vect vect) (car vect))
(define (ycor-vect vect) (cdr vect))

(define (add-vect vect1 vect2)
  (make-vect (+ (xcor-vect vect1) (xcor-vect vect2))
             (+ (ycor-vect vect1) (ycor-vect vect2))))

(define (sub-vect vect1 vect2)
  (make-vect (- (xcor-vect vect1) (xcor-vect vect2))
             (- (ycor-vect vect1) (ycor-vect vect2))))

(define (scale-vect scale vect)
  (make-vect (* scale (xcor-vect vect))
             (* scale (ycor-vect vect))))

(define (make-frame origin edgeX edgeY)
  (cons origin (cons edgeX edgeY)))

(define (origin-frame frame) (car frame))
(define (edgeX-frame frame) (car (cdr frame)))
(define (edgeY-frame frame) (cdr (cdr frame)))

# default drawing board
(define default (make-frame (cons 0 0) (cons 1 0) (cons 0 1)))

# v in unit rectangle --> frame
(define (frame-coord-map frame)
  (lambda (v)
    (add-vect
     (origin-frame frame)
     (add-vect (scale-vect (xcor-vect v) (edgeX-frame frame))
               (scale-vect (ycor-vect v) (edgeY-frame frame))))))

(define (transform-painter painter origin corner1 corner2)
  (lambda (frame)
    (let ((m (frame-coord-map frame)))
      (let ((new-origin (m origin)))
        (painter (make-frame new-origin
                             (sub-vect (m corner1) new-origin)
                             (sub-vect (m corner2) new-origin)))))))

(define (flip-vert painter) (transform-painter painter
                                               (make-vect 0 1) (make-vect 1 1) (make-vect 0 0)))

(define (flip-horiz painter) (transform-painter painter
                                                (make-vect 1 0) (make-vect 0 0) (make-vect 1 1)))

(define (shrink-to-upper-right painter) (transform-painter painter
                                                           (make-vect 0.5 0.5) (make-vect 1 0.5) (make-vect 0.5 1)))

(define (shrink-to-upper-left painter) (transform-painter painter
                                                          (make-vect 0 0.5) (make-vect 0.5 0.5) (make-vect 0 1)))

(define (shrink-to-lower-right painter) (transform-painter painter
                                                           (make-vect 0.5 0) (make-vect 1 0) (make-vect 0.5 0.5)))

(define (shrink-to-lower-left painter) (transform-painter painter
                                                          (make-vect 0 0) (make-vect 0.5 0) (make-vect 0 0.5)))

(define (rotate90 painter) (transform-painter painter
                                              (make-vect 1 0) (make-vect 1 1) (make-vect 0 0)))

(define (beside painter1 painter2)
  (let ((split-point (make-vect 0.5 0)))
    (let ((paint-left (transform-painter painter1 (make-vect 0 0) split-point (make-vect 0 1)))
          (paint-right (transform-painter painter2
                                          split-point (make-vect 1 0) (make-vect 0.5 1))))
      (lambda (frame) (paint-left frame) (paint-right frame)))))

(define (below painter1 painter2)
  (let ((split-point (make-vect 0 0.5)))
    (let ((paint-up (transform-painter painter1 split-point (make-vect 1 0.5) (make-vect 0 1)))
          (paint-down (transform-painter painter2 (make-vect 0 0) (make-vect 1 0) split-point)))
      (lambda (frame) (paint-up frame) (paint-down frame)))))

(define board (make-frame (cons 0 0) (cons 1000 0) (cons 0 1000)))
(define (frame-real-coord-map frame)
  (lambda (v)
    (add-vect
     ((frame-coord-map board) (origin-frame frame))
     (add-vect (scale-vect (xcor-vect v) (edgeX-frame frame))
               (scale-vect (ycor-vect v) (edgeY-frame frame))))))

(define (st-painter frame)
  (#painter (map (sierpinskiTriangle (cons 0 0) (cons 500 866) (cons 1000 0)
                                     (/ (* 50 (sqrt 2)) (distance (add-vect (edgeY-frame frame) (edgeX-frame frame)) (cons 0 0))))
                 (frame-real-coord-map frame))))

(define (line-painter frame)
  (#painter (map (line (cons 0 0) (cons 100 100)) (frame-real-coord-map frame)))
  (#painter (map (line (cons 100 100) (cons 200 0)) (frame-real-coord-map frame))))

(sierpinskiTriangle-time (cons 0 0) (cons 500 866) (cons 1000 0) 5)