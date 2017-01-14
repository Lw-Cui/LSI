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
(define default (make-frame (cons 0 0) (cons 1000 0) (cons 0 1000)))

(define (frame-coord-map frame)
  (lambda (v)
    (add-vect
     (origin-frame frame)
     (add-vect (scale-vect (/ (xcor-vect v) (xcor-vect (edgeX-frame default))) (edgeX-frame frame))
               (scale-vect (/ (ycor-vect v) (ycor-vect (edgeY-frame default))) (edgeY-frame frame))))))

(define (transform-painter painter origin corner1 corner2)
  (lambda (frame)
    (let ((m (frame-coord-map frame)))
      (let ((new-origin (m origin)))
        (painter (make-frame new-origin
                             (sub-vect (m corner1) new-origin)
                             (sub-vect (m corner2) new-origin)))))))


(define (flip-vert painter) (transform-painter painter 
    (make-vect 0 (length-of-edgeY default)) 
    (make-vect (length-of-edgeX default) (length-of-edgeY default)) 
    (make-vect 0 0)))

(define (flip-horiz painter) (transform-painter painter 
    (make-vect (length-of-edgeX default) 0) 
    (make-vect 0 0)
    (make-vect (length-of-edgeX default) (length-of-edgeY default))))

(define (shrink-to-upper-right painter) (transform-painter painter 
    (make-vect (half (length-of-edgeX default)) (half (length-of-edgeY default)))
    (make-vect (length-of-edgeX default) (half (length-of-edgeY default)))
    (make-vect (half (length-of-edgeX default)) (length-of-edgeY default))))

(define (shrink-to-upper-left painter) (transform-painter painter
    (make-vect 0 (half (length-of-edgeY default)))
    (make-vect (half (length-of-edgeX default)) (half (length-of-edgeY default)))
    (make-vect 0 (length-of-edgeY default))))

(define (shrink-to-lower-right painter) (transform-painter painter
    (make-vect (half (length-of-edgeX default)) 0)
    (make-vect (length-of-edgeX default) 0)
    (make-vect (half (length-of-edgeX default)) (half (length-of-edgeY default)))))

(define (shrink-to-lower-left painter) (transform-painter painter
    (make-vect 0 0)
    (make-vect (half (length-of-edgeX default)) 0)
    (make-vect 0 (half (length-of-edgeY default)))))

(define (rotate90 painter) (transform-painter painter
    (make-vect (length-of-edgeX default) 0)
    (make-vect (length-of-edgeX default) (length-of-edgeY default))
    (make-vect 0 0)))

(define (beside painter1 painter2)
  (let ((split-point (make-vect (half (length-of-edgeX default)) 0)))
    (let ((paint-left (transform-painter painter1 (make-vect 0 0) split-point (make-vect 0 (length-of-edgeY default))))
          (paint-right (transform-painter painter2
              split-point
              (make-vect (length-of-edgeX default) 0)
              (make-vect (half (length-of-edgeX default)) (length-of-edgeY default)))))
      (lambda (frame) (paint-left frame) (paint-right frame)))))

(define (below painter1 painter2)
  (let ((split-point (make-vect 0 (half (length-of-edgeY default)))))
    (let ((paint-up (transform-painter painter1
            split-point
            (make-vect (length-of-edgeX default) (half (length-of-edgeY default)))
            (make-vect 0 (length-of-edgeY default))))
          (paint-down (transform-painter painter2 (make-vect 0 0) (make-vect (length-of-edgeX default) 0) split-point)))
      (lambda (frame) (paint-up frame) (paint-down frame)))))

(define (st-painter frame)
    (#painter (map (sierpinskiTriangle
            (cons 0 0) (cons 500 866) (cons 1000 0)
            (* 10 (/ (length-of-edgeX default) (length-of-edgeX frame)))) (frame-coord-map frame))))
(define (line-painter frame) (#painter (map (line (cons 0 0) (cons 100 100)) (frame-coord-map frame))))
