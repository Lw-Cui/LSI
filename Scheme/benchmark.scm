(define (painter frame)
    (map (sierpinskiTriangle (cons 0 0) (cons 500 866) (cons 1000 0)
                (/ (* 50 (sqrt 2)) (distance (add-vect (edgeY-frame frame) (edgeX-frame frame)) (cons 0 0))))
                (frame-real-coord-map frame)))

