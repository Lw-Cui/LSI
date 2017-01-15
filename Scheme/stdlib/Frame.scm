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
(define (length-of-edgeX frame) (distance (edgeX-frame frame) (cons 0 0)))
(define (length-of-edgeY frame) (distance (edgeY-frame frame) (cons 0 0)))

# default drawing board
(define default (make-frame (cons 0 0) (cons 1 0) (cons 0 1)))
(define board (make-frame (cons 0 0) (cons 1000 0) (cons 0 1000)))

(define (frame-coord-map-ratio frame)
  (lambda (v)
    (add-vect
     (origin-frame frame)
     (add-vect (scale-vect (xcor-vect v) (edgeX-frame frame))
               (scale-vect (ycor-vect v) (edgeY-frame frame))))))

(define (transform-painter painter origin corner1 corner2)
  (lambda (frame)
    (let ((m (frame-coord-map-ratio frame)))
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

(define (frame-coord-map frame)
  (lambda (v)
    (add-vect
     (origin-frame frame)
     (add-vect (scale-vect (/ (xcor-vect v) (length-of-edgeX board)) (edgeX-frame frame))
               (scale-vect (/ (ycor-vect v) (length-of-edgeY board)) (edgeY-frame frame))))))

(define (ratio-to-reality frame)
  (define (scaleXY-vect scaleX scaleY vect)
    (make-vect (* scaleX (xcor-vect vect))
               (* scaleY (ycor-vect vect))))
  (let ((lenX (length-of-edgeX board))
        (lenY (length-of-edgeY board)))
    (make-frame (scaleXY-vect lenX lenY (origin-frame frame))
                (scaleXY-vect lenX lenY (edgeX-frame frame))
                (scaleXY-vect lenX lenY (edgeY-frame frame)))))

(define (st-painter frame)
    (let ((reality (ratio-to-reality frame)))
        (#painter (map (sierpinskiTriangle
                (cons 0 0) (cons 500 866) (cons 1000 0)
                (* 30 (/ (length-of-edgeX board) (length-of-edgeX reality))))
                (frame-coord-map reality)))))
(define (line-painter frame) (#painter (map (line (cons 0 0) (cons 100 100)) (frame-coord-map (ratio-to-reality frame)))))
