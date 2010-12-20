;;;  (standard-page (:TITLE "Add-Numbers")
(define-url-fn (add-number)
  (with-html-output-to-string (*standard-output*)
    (:CENTER
     (:FORM :ACTION "/add-two.html" :METHOD "post"
	    (:INPUT :TYPE "text"
		    :NAME "num1")
	    (:INPUT :TYPE "text"
		    :NAME "num2")
	    (:INPUT :TYPE "submit"
		    :VALUE "Add")))))

(define-url-fn (add-two)
  (with-html-output-to-string (*standard-output*)
    (:CENTER 
     (let ((v1 (parse-integer (parameter "num1")))
	   (v2 (parse-integer (parameter "num2"))))
       (fmt "~A + ~A = ~A" v1 v2 (+ v1 v2))))))







(define-url-fn (add-number)
  (with-html-output-to-string (*standard-output*)
    (:CENTER
     (:FORM :ACTION "/add-two.html" :METHOD "post"
	    (:INPUT :TYPE "text"
		    :NAME "num1")
	    (:INPUT :TYPE "text"
		    :NAME "num2")
	    (:INPUT :TYPE "submit"
		    :VALUE "Add")))))

(define-url-fn (add-two)
  (with-html-output-to-string (*standard-output*)
    (:CENTER 
     (let ((v1 (parse-integer (parameter "num1")))
	   (v2 (parse-integer (parameter "num2"))))
       (fmt "~A + ~A = ~A" v1 v2 (+ v1 v2))))))

