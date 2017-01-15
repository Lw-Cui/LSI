(define (painter frame)
    (let ((reality (ratio-to-reality frame)))
        (map (sierpinskiTriangle
                (cons 0 0) (cons 500 866) (cons 1000 0)
                (* 10 (/ (length-of-edgeX board) (length-of-edgeX reality))))
                (frame-coord-map reality))))

((beside (below painter (flip-vert st-painter)) (below st-painter (flip-vert st-painter))) default)
