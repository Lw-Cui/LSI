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

(define (make-frame origin edge1 edge2)
  (cons origin (cons edge1 edge2)))

(define (origin-frame frame) (car frame))
(define (edge1-frame frame) (car (cdr frame)))
(define (edge2-frame frame) (cdr (cdr frame)))

(define (frame-coord-map frame)
  (lambda (v)
    (add-vect
     (origin-frame frame)
     (add-vect (scale-vect (/ (xcor-vect v) 1000) (edge1-frame frame))
               (scale-vect (/ (ycor-vect v) 1000) (edge2-frame frame))))))

(define (transform-painter painter origin corner1 corner2)
  (lambda (frame)
    (let ((m (frame-coord-map frame)))
      (let ((new-origin (m origin)))
        (painter (make-frame new-origin
                             (sub-vect (m corner1) new-origin)
                             (sub-vect (m corner2) new-origin)))))))

(define default (make-frame (cons 0 0) (cons 1000 0) (cons 0 1000)))

(define (flip-vert painter) (transform-painter painter (make-vect 0 1000) (make-vect 1000 1000) (make-vect 0 0)))
(define (shrink-to-upper-right painter) (transform-painter painter (make-vect 500 500) (make-vect 1000 500) (make-vect 500 1000)))
(define (shrink-to-upper-left painter) (transform-painter painter (make-vect 0 500) (make-vect 500 500) (make-vect 0 1000)))
(define (shrink-to-lower-right painter) (transform-painter painter (make-vect 500 0) (make-vect 1000 0) (make-vect 500 500)))
(define (shrink-to-lower-left painter) (transform-painter painter (make-vect 0 0) (make-vect 500 0) (make-vect 0 500)))
(define (rotate90 painter) (transform-painter painter (make-vect 1000 0) (make-vect 1000 1000) (make-vect 0 0)))

(define (st-painter frame) (#painter (map (sierpinskiTriangle (cons 0 0) (cons 500 866) (cons 1000 0) 30) (frame-coord-map frame))))
(define (line-painter frame) (#painter (map (line (cons 0 0) (cons 100 100)) (frame-coord-map frame))))
