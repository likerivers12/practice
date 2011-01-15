
;; M-x slime-cd

(defparameter *cur-dir-str* "d:/data/pr/labproject/2010-2/descriptor_lfb")

(defparameter *fname-in-1* "total_desc_libsvm.txt") ; OK
(defparameter *fold-fnames-1* 
  '(("x-train-libsvm-1-5.txt" "x-test-libsvm-1-5.txt")
    ("x-train-libsvm-2-5.txt" "x-test-libsvm-2-5.txt")
    ("x-train-libsvm-3-5.txt" "x-test-libsvm-3-5.txt")
    ("x-train-libsvm-4-5.txt" "x-test-libsvm-4-5.txt")
    ("x-train-libsvm-0-5.txt" "x-test-libsvm-0-5.txt")))

(defparameter *fname-in-2* "total_desc_libsvm_2.txt") ; OK
(defparameter *fold-fnames-2* 
  '(("x-train-libsvm-1-5_2.txt" "x-test-libsvm-1-5_2.txt")
    ("x-train-libsvm-2-5_2.txt" "x-test-libsvm-2-5_2.txt")
    ("x-train-libsvm-3-5_2.txt" "x-test-libsvm-3-5_2.txt")
    ("x-train-libsvm-4-5_2.txt" "x-test-libsvm-4-5_2.txt")
    ("x-train-libsvm-0-5_2.txt" "x-test-libsvm-0-5_2.txt")))

(defparameter *str-data-set* nil)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; (defparameter *raw-file-list* '("raw_particle_desc.txt"
;; 				"raw_non-particle1_desc.txt"
;; 				"raw_non-particle2_desc.txt"
;; 				"raw_unclassified_desc.txt"))

(progn
  ;; (init-data-set)
  ;; (load-data-set *f-in-1*) ;ok - 1
  ;; (trim-line (load-data-set *f-in-1*)) ;ok - 2
  ;; (setf *tmp* (split-each-data (trim-lines (load-data-set *f-in-1*)))) ;ok - 3 -for total...
  ;;(setf *tmp* (split-data-set #\: (split-lines (trim-lines (load-data-set *f-in-1*))))) ;ok - 4 - 전체 변환 완료
;;  (setf *tmp* (make-data-set-from-file *f-in-1*))
;;  (setf *tmp* (make-data-set-from-file-2 *f-in-1*))

  (setf *data-set* (make-data-set-from-file-3 *fname-in-1*))
  (setf *nfold-data-sets* (make-n-fold-data-sets *data-set* 5))
;;  (setf *fold-1* (get-fold-tt-set *nfold-data-sets* 1))
  (save-nfold-data-sets *fold-fnames-1* *nfold-data-sets*) ; n-fold data set

  )


(defun save-nfold-data-sets (fold-fnames nfold-data-sets)
  (maplist #'(lambda (fname-cdr fold-tt-set-cdr)
	       (save-data-set (car fname-cdr) (car fold-tt-set-cdr)))
	   (get-train-fnames fold-fnames)
	   (get-train-nfsets nfold-data-sets))
  (maplist #'(lambda (fname-cdr fold-tt-set-cdr)
	       (save-data-set (car fname-cdr) (car fold-tt-set-cdr)))
	   (get-test-fnames fold-fnames)
	   (get-test-nfsets nfold-data-sets)))
;; (maplist #'(lambda (x y) (list (car x) (car y))) '(1 2 3) '(1 2 3))
  

(defun get-train-fnames (fold-fnames)
  (mapcar #'car fold-fnames))
;; (get-train-fnames *fold-file-names-1*)

(defun get-train-nfsets (nfold-data-sets)
  (mapcar #'(lambda (fold-tt-set)
	      (get-train-set fold-tt-set))
	  nfold-data-sets))

(defun get-test-fnames (fold-fnames)
  (mapcar #'cadr fold-fnames))
;; (get-test-fnames *fold-file-names-1*)

(defun get-test-nfsets (nfold-data-sets)
  (mapcar #'(lambda (fold-tt-set)
	      (get-test-set fold-tt-set))
	  nfold-data-sets))


(defun save-data-set (fname data-set)
  "data set을 파일에 저장"
  (with-open-file (out fname
		       :direction :output
		       :if-exists :supersede)
    (mapcar #'(lambda (data)
		(progn
		  (format out "~A " (class-data data))
		  (mapcar #'(lambda (iv) 
			      (format out "~A:~A " (ind-iv iv) (fval-iv iv)))
			  (feature-vec-data data))
		  (format out "~%")))
	    data-set)))
;; (save-data-set "xxx-train-2.txt" (get-train-set *fold-1*))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun make-n-fold-data-sets (data-set fold-size)
  (mapcar #'(lambda (fold-num)
	      (split-data-set data-set fold-num fold-size))		      
	  (mod-range fold-size)))

(defun get-fold-tt-set (nf-data-sets fold-num)
  (nth fold-num nf-data-sets))
;; (make-n-fold-data-sets *tmp* 5) 1)

(defun get-train-set (tt-set)
  (car tt-set))

(defun get-test-set (tt-set)
  (cadr tt-set))

(defun mod-range (e)
  (loop for i from 1 to e collect (if (= i e) 0 i)))
;; 5 -> (1 2 3 4 0)
;; 3 -> (1 2 0)

(defun split-data-set (data-set fold-num fold-size)
  "data-set을 n-fold 나눈다. 
data의 순서가 모드-fold-num이면 test-set으로 나머지는 train-set으로"
  (let ((train-set nil)
	(test-set nil)
	(n 0))
    (mapcar #'(lambda (data)
		(progn 
		  (incf n)
		  (if (= (mod n fold-size) fold-num)
		      (setf test-set (cons data test-set))
		      (setf train-set (cons data train-set)))))
	    data-set)
    (list (reverse train-set)
	  (reverse test-set))))
    
;; data-set : 전체 데이터
;; data     : 클래스, (인덱스:feature-val) vector
;; datum    : (인덱스:feature-val) 하나

;; (
;;  (0 (1 . 75.692) 
;;     (2 . 19.606) 
;;     (3 . 107.0) 
;; ...
;;  (0 (1 . 75.692) 
;;     (2 . 19.606) 
;;     (3 . 107.0) 
;; )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun make-data-set-from-file (file-name)
  (init-data-set)
  (construct-data-set #\: 
		      (split-lines 
		       (trim-lines 
			(load-data-set file-name)))))

(defun make-data-set-from-file-2 (file-name)
  (init-data-set)
  (construct-data-set #\: 
		      (split-lines 
			(load-data-set-2 file-name))))

(defun make-data-set-from-file-3 (file-name)
  (load-data-set-3 file-name))

(defun class-data (data)
  (car data))

(defun feature-vec-data (data)
  (cdr data))

(defun feature-data (fid data)
  (let ((fv (feature-vec-data data)))
    (let ((iv (assoc fid fv)))
      (if iv
	  (cdr iv)
	  (error "no fid in assoc data" fid)))))

(defun ind-iv (iv)
  (car iv))

(defun fval-iv (iv)
  (cdr iv))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun construct-data-set (ch data-set)
  "전체 data-set의 각 data에 대해서 클래스값과 
feature인덱스 - feature값)-리스트 형태로 구분"
  (mapcar #'(lambda (data)
	       (let ((class-val (car data))
		     (if-vec (cdr data)))
		 (cons (parse-integer class-val)
		       (split-if-vec-into-ivs ch if-vec))))
	  data-set))

(defun split-if-vec-into-ivs (ch data)
  "하나의 data에 대해서 인덱스와 값을 분리"
  (mapcar #'(lambda (x) 
	      (split-a-if-into-iv ch x)) 
	  data))
;;(split-data #\: '("1:23" "2:34"))

(defun split-a-if-into-iv (ch datum)
  "datum문자열에서 ch문자기준으로 두개로 나눈다.
이들은 index와 값에 해당되는데, 이를 숫자로 바꾼다"
  (let ((pos (find-ch-pos ch datum)))
    (if (not pos)
	(error "error : str do not contains a character :" ch)
	(let ((ind (subseq datum 0 pos))
	      (val (subseq datum (+ pos 1))))
	  (cons (parse-integer ind)            ;; 타입변환
		(read-from-string val))))))
;;(split-a-datum #\: "12:34")


(defun find-ch-pos (ch str)
  "str문자열에서 ch문자의 위치를 찾아문자"
  (position ch str :test #'equal))
;;(position #\: "1:23" :test #'equal)
;;(find-ch-pos #\: "1:23")



(defun trim-lines (str-data-set)
  "데이터의 각 줄마다 양끝의 스페이스 제거"
  (mapcar #'(lambda (x) 
	      (string-trim " " x)) str-data-set))

(defun split-lines (str-data-set)
  "스페이스를 구분자로 해서 리스트로 설정"
  (mapcar #'split-by-one-space str-data-set))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun init-data-set ()
  (setf *str-data-set* nil))

(defun load-data-set (fname)
  "파일을 읽어서 한 줄씩 리스트 원소로 할당하여 *raw-data*에 설정"
  (with-open-file (in fname
		      :direction :input)
    (loop for line = (read-line in nil)
       while line
       do (setf *str-data-set* (cons line *str-data-set*)))
    (close in))
  (setf *str-data-set* (reverse *str-data-set*)))


(defun load-data-set-2 (fname)
  "파일을 읽어서 한 줄씩 리스트 원소로 할당하여 *raw-data*에 설정
2:trim-line"
  (with-open-file (in fname
		      :direction :input)
    (loop for line = (read-line in nil)
       while line
       do (let ((trimed-line (string-trim " " line)))
	    (setf *str-data-set* (cons trimed-line *str-data-set*))))
    (close in))
  (setf *str-data-set* (reverse *str-data-set*)))

(defun load-data-set-3 (fname)
  "파일을 읽어서 한 줄씩 리스트 원소로 할당하여 *raw-data*에 설정
2:trim-line
3:split-iv"
  (let ((data-set nil))
    (with-open-file (in fname
			:direction :input)
      (loop for line = (read-line in nil)
	 while line
	 do (let* ((trimed-line (string-trim " " line))
		   (splited-line (split-by-one-space trimed-line)))
	      (let ((class-val (car splited-line))
		    (if-vec (cdr splited-line)))
		(let* ((iv (split-if-vec-into-ivs #\: if-vec))
		       (data-line (cons (parse-integer class-val)
					iv)))
	    	  (setf data-set (cons data-line data-set))))))
      (close in))
    (reverse data-set)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun join-string-list (string-list)
  "Concatenates a list of strings
and puts spaces between the elements."
  (format nil "~{~A~^ ~}" string-list))

(defun split-by-one-space (string)
  "Returns a list of substrings of string
divided by ONE space each.
Note: Two consecutive spaces will be seen as
if there were an empty string between them."
  (loop for i = 0 then (1+ j)
	as j = (position #\Space string :start i)
	collect (subseq string i j)
	while j))

(loop for i = 0 then (1+ j)
      as j = (+ i 1)
      collect (print i)
      while (< j 10))

(loop for x in '(1 2 3 4 5 0 6)
      when (> x 0)
      collect (list x))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 파일 읽기
;; 1글자
;; (with-open-file  (in *f-in-1*
;; 		     :direction :input)
;; 		 (with-standard-io-syntax
;; 		  (print (read in)))) 

;; ;; 1 줄 읽기
;; (let ((*f-in-1* "test.txt"))
;;   (with-open-file (in *f-in-1*
;; 		      :direction :input)
;; 		  (format t "~a~%" (read-line in))))

;; ;;; 모든 줄 읽기
;; (with-open-file (in *f-in-1*
;; 		    :direction :input)
;; 		(loop for line = (read-line in nil)
;; 		      while line do (format t "~a~%" line))
;; 		(close in))

;; (with-open-file (in *f-in-1*
;; 		    :direction :input)
;; 		(loop for line = (read-line in nil)
;; 		      while line
;; 		      collect (split-by-one-space line))
;; 		(close in))

;; (with-open-file (in *f-in-1*
;; 		    :direction :input)
;; 		(loop for line = (read-line in nil)
;; 		      while line
;; 		      do (setf *total-data* (cons (list line) *total-data*)))
;; 		(close in))
  
		    
;; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ;;; 파일에 쓰기
;; (with-open-file (out "test.txt" 
;; 		     :direction :output)
;; 		(format out "Test"))


;; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ;;(shell "ls -l")
;; (shell "ls *.bat")
;; (shell "ls *.sh")
;; ;;(shell "desc-test-auto0-5.bat")

;; (run-program "ls" '("*.sh") :search t :output *standard-output*)
