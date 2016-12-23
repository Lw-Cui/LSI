((shrink-to-upper-right (flip-vert st-painter)) default)
((shrink-to-upper-left (flip-vert st-painter)) default)
((shrink-to-lower-left st-painter) default)
((shrink-to-lower-right st-painter) default)

(define (beside painter1 painter2)
  (let ((split-point (make-vect 500 0)))
    (let ((paint-left
           (transform-painter painter1
                              (make-vect 0 0)
                              split-point
                              (make-vect 0 1000)))
          (paint-right
           (transform-painter painter2
                              split-point
                              (make-vect 1000 0)
                              (make-vect 500 1000))))
      (lambda (frame)
        (paint-right frame)))))
