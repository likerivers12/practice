;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 이미지 픽셀 설정 테스트 2 - 성공
(require racket/gui)
(require racket/class)

(define (comment x)
    1)

(define (show-bitmap bm title)
    (let* ((width (send bm get-width))
	   (height (send bm get-height))
	   (frame (new frame% (label title)))
	   (canvas (new canvas% (parent frame) 
			(min-width width) (min-height height)
			(paint-callback 
			 (lambda (canvas dc) 
			  (send dc draw-bitmap bm 0 0))))))
      (send frame show #t)))

(define (read-bitmap df-name)
    (make-object bitmap% df-name))

;;; kind : 'png 'jpeg 'xbm 'xpm 'bmp
(define (save-bitmap bm df-name kind)
    (send bm save-file df-name kind))

(define (make-bitmap width height bits)
    (make-object bitmap% width height bits))

(define (make-bm-dc bm)
    (make-object bitmap-dc% bm))

(define (get-image-data bm-dc bm)
    (let* ((width (send bm get-width))
	   (height (send bm get-height))
	   (depth (send bm get-depth))
	   (img-data (make-bytes (* width height 4))))
      (begin
       (send bm-dc get-argb-pixels 0 0 width height img-data)
       img-data)))

(define (get-image-pixel bm img-data x y)
    (let* ((width (send bm get-width))
	   (height (send bm get-height))
	   (loc (+ (* y width 4) x)))
      (bytes-ref img-data loc)))
	  
(define (set-image-pixel! bm img-data x y v)
    (let* ((width (send bm get-width))
	   (height (send bm get-height))
	   (loc (+ (* y width 4) x)))
      (bytes-set! img-data loc v)
      (bytes-set! img-data (+ loc 1) v)
      (bytes-set! img-data (+ loc 2) v)))
;      (bytes-set! img-data (+ loc 3) v)))

(define (set-image-data! bm-dc bm img-data)
    (let* ((width (send bm get-width))
	   (height (send bm get-height))
	   (depth (send bm get-depth)))
      (send bm-dc set-argb-pixels 0 0 width height img-data)))

(define (test-get-set-and-show bm)
    (let* ((bm-dc (make-bm-dc bm))
	   (img-data (get-image-data bm-dc bm)))
      (begin
       (display (get-image-pixel bm img-data 25 50))
       (set-image-pixel! bm img-data 25 50 255)
       (set-image-data! bm-dc bm img-data)
       (show-bitmap bm "A"))))

(comment
'
(test-get-set-and-show (make-bitmap 50 100 8))
)

;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (make-image-object bm)
    (let* ((bm-dc (make-bm-dc bm))
	   (image (get-image-data bm-dc bm)))
      (make-object image-class bm bm-dc image)))

(define image-class
    (class object%
	   (super-new)

	   (init-field bm)
	   (init-field bm-dc)
	   (init-field image)

	   (define/public (get-pixel x y)
	       (get-image-pixel bm image x y))

	   (define/public (set-pixel! x y v)
	       (set-image-pixel! bm image x y v))

	   (define/public (show)
	       (begin
		(set-image-data! bm-dc bm image)
		(show-bitmap bm "A")))

	   (define/public (get-bitmap)
	       bm)

	   (define/public (flush)
	       (set-image-data! bm-dc bm image))
))

(comment
'
(begin
; (define img (make-object image-class 1 2 3))
 (define img (make-image-object (make-bitmap 10 20 8)))
 (define image (get-field image img))
 (define pixel (send img get-pixel 5 10))
 (send img set-pixel! 5 10 255)
 (send img show)

 (define img-file-in "./image/img3.jpg")
 (define img-file-out "./image/img3-2.jpg")
 (define img2 (make-image-object (read-bitmap img-file-in)))
 (send img2 show)
 (let loop1 ([i 100])
      (or (zero? i)
	       (send img2 set-pixel! i 100 200)
	       (loop1 (- i 1))))
		
 (send img2 set-pixel! 100 100 0)
 (send img2 set-pixel! 100 101 0)
 (send img2 set-pixel! 101 100 0)
 (send img2 set-pixel! 101 101 0)
 (send img2 show)
 (save-bitmap (send img2 get-bitmap) img-file-out 'jpeg)
)
)

