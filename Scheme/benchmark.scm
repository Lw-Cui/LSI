(define (painter frame)
    (let ((reality (ratio-to-reality frame)))
        (map (sierpinskiTriangle
                (cons 0 0) (cons 500 866) (cons 1000 0)
                (* 40 (/ (length-of-edgeX board) (length-of-edgeX reality))))
                (frame-coord-map reality))))

(define (l-painter frame)
    (map (line (cons 100 100) (cons 200 0)) (frame-coord-map (ratio-to-reality frame))))




