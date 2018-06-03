(define (st-painter frame)
        (#painter (map (sierpinskiTriangle (cons 0 0) (cons 500 866) (cons 1000 0) 30)
                (frame-real-coord-map frame))))

(define rhombus-painter (below st-painter (flip-vert st-painter)))
((beside rhombus-painter rhombus-painter) default)
