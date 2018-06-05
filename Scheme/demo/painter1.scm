(define (st-painter frame)
        (#painter (map (sierpinskiTriangle (cons 0 0) (cons 500 866) (cons 1000 0) 10)
                (frame-real-coord-map frame))))

(define flip-painter (flip-vert st-painter))
((shrink-to-upper-right (shrink-to-upper-right flip-painter)) default)
((shrink-to-upper-right (shrink-to-upper-left flip-painter)) default)
((shrink-to-upper-left (shrink-to-upper-right flip-painter)) default)
((shrink-to-upper-left (shrink-to-upper-left flip-painter)) default)
