




;;; Common Lisp Web Programming 









(require 'hunchentoot)    ; http://www.weitz.de/hunchentoot/
(require 'cl-who)         ; http://www.weitz.de/cl-who/

(require 'my-login-db)
;;(load "/home/suil/data/pr/lisp/klisper-seminar/my-login-db.lisp")

(defpackage :cl-web-exercise
  (:use :cl :hunchentoot :cl-who :my-login-db))

(in-package :cl-web-exercise)

;;(defparameter *port* 5000)
(defparameter *port* 4242)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 웹 서버 시작
(hunchentoot:start (make-instance 'hunchentoot:acceptor :port *port*))

;;; 훈첸투트 예제 
(hunchentoot:define-easy-handler (hello :uri "/hello") (name)
  (setf (hunchentoot:content-type*) "text/plain")
  (format nil "Hello~@[ ~A~]!" name))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 페이지 등록 예제
(defun page1 ()
  (format nil
    "<HTML>
     <HEAD>
        <TITLE>Page1</TITLE>
     </HEAD>
     <BODY>
        <CENTER>
           <H1>PAGE1</H1>
        </CENTER>
     </BODY>
     </HTML>"))

(push (create-prefix-dispatcher "/page1.html" 'page1) *dispatch-table*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; cl-who 사용
(defun page2 ()
  (cl-who:with-html-output-to-string (*standard-output*)
    (:HTML
     (:HEAD
      (:TITLE "Page2"))
     (:BODY
      (:CENTER
       (:H1 "PAGE2"))))))

(push (create-prefix-dispatcher "/page2.html" 'page2) *dispatch-table*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 먜크로를 사용해서 페이지 등록과 함수 정의를 한꺼번에 수행
(defmacro define-url-fn ((name) &body body)
  `(progn
     (defun ,name ()
       ,@body)
     (push (create-prefix-dispatcher ,(format nil "/~(~a~).html" name) ',name) *dispatch-table*)))

(define-url-fn (page3)
  (with-html-output-to-string (*standard-output*)
    (:HTML
     (:HEAD 
      (:TITLE "Page3"))
     (:BODY
      (:CENTER
       (:H1 "PAGE3"))))))

;;;; macroexpand-1
;; (PROGN
;;   (DEFUN PAGE3 ()
;;     (WITH-HTML-OUTPUT-TO-STRING (*STANDARD-OUTPUT*)
;;       (:HTML (:HEAD (:TITLE "Page3")) (:BODY (:CENTER (:H1 "PAGE3"))))))
;;   (PUSH (CREATE-PREFIX-DISPATCHER "/page3.html" 'PAGE3) *DISPATCH-TABLE*))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 매크로를 사용해서 표준 페이지 출력 제거
(defmacro standard-page ((&key title) &body body)
  `(with-html-output-to-string (*standard-output* nil :prologue t :indent t)
     (:HTML :XMLNS "http://www.w3.org/1999/xhtml"
	    :XML\:lang "en" 
	    :LANG "en"
	    (:HEAD 
	     (:META :HTTP-EQUIV "Content-Type" 
		    :CONTENT    "text/html;charset=utf-8")
	     (:TITLE ,title))
	    (:BODY 
	     (:CENTER
	      (:H1 "Korea Lisp Seminar - Common Lisp Web Programming")
	      (:H2 "Voting System")(:BR)
	      (:HR))))
	     ,@body))

(define-url-fn (page4)
  (standard-page (:TITLE "Page4")
    (:CENTER
     (:H1 "PAGE4"))))

(define-url-fn (page5)
  (standard-page (:TITLE "Page5")
    (:CENTER
     (:H1 "PAGE5"))))

;;;
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


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;
;;; 본격적으로 voting system page 제작에 들어간다.
;;;
;;; 참고 사이트 : http://www.adampetersen.se/articles/lispweb.htm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 설문 조사 클래스 로드하기
(load "/home/suil/data/pr/lisp/klisper-seminar/survey-item.lisp")
;;(load "./survey-item.lisp")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; voting 메인 페이지 만들기
(define-url-fn (survey)
  (standard-page (:TITLE "Survey")
    (:CENTER
     (:H1 "Vote for your favorite programming language!")
    (:P "Missing a item? Make it available for votes " 
	(:A :HREF "new-item.html" "here"))
    (:H2 "Current stand")
    (:TABLE
     (dolist (item (items))
       (htm  
	(:TR
	 (:TD
	  (:A :HREF (format nil "vote.html?name=~a" (name item)) "Vote!")
	  (fmt "~A with ~d/~d votes" (name item) (votes item) *total-votes*)))))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 아이템 추가 페이지 만들기
(define-url-fn (new-item)
  (standard-page (:TITLE "Add a new item")
    (:CENTER
     (:H1 "Add a new item to the chart")
     (:FORM :ACTION "/item-added.html" :METHOD "post" 
	    (:P "What is the name of the programming language?" (:BR)
		(:INPUT :TYPE "text"  
			:NAME "name" 
			:CLASS "txt"))
	    (:P (:INPUT :TYPE "submit" 
			:VALUE "Add" 
			:CLASS "btn"))))))

(define-url-fn (item-added)
  (let ((name (parameter "name")))
    (unless (or (null name) (zerop (length name)))
      (add-item name))
    (redirect "/survey.html")))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 투표 하기 
(define-url-fn (vote)
  (let ((item (item-from-name (parameter "name"))))
    (if item
	(vote-for item))
    (redirect "/survey.html")))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 1. 장식 만들기
;;; survey 에서 코드 추가
;;	       (:TD
;;		(fmt "~A" (stats "*" (votes item) *stat-range*))))))))

(define-url-fn (survey)
  (standard-page (:TITLE "Survey")
    (:CENTER
     (:H1 "Vote on your favorite programming language!")
    (:P "Missing a item? Make it available for votes " 
	(:A :HREF "new-item.html" "here"))
    (:H2 "Current stand")
    (:TABLE
     (dolist (item (items))
       (htm  
	(:TR
	 (:TD
	  (:A :HREF (format nil "vote.html?name=~a" (name item)) "Vote!")
	  (fmt "~A with ~d/~d votes" (name item) (votes item) *total-votes*))
;;
	 (:TD
	  (fmt "~A" (stats "*" (votes item) *stat-range*)))))))
;;
)))

(defun stats (mark votes range)
  (if (> *total-votes* 0)
      (let ((counts (floor (* (/ votes *total-votes*) range))))
	(with-output-to-string (out)
	  (dotimes (n counts)
	    (format out "~A" mark))
	  (dotimes (n (- range counts))
	    (format out "-"))))
      (with-output-to-string (out)
	(dotimes (n range)
	  (format out "-")))))

;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;
;;; 사용자 정보와 결합하기 
;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 사용자 정보 관리 파일 로드 하기
;(load "/home/suil/data/pr/lisp/klisper-seminar/my-login-db.lisp")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; login 페이지 만들기 
(define-url-fn (index)
  (standard-page (:TITLE "Login")
    (let ((user (session-value :user)))
      (if user
	  (htm
	   (:CENTER
	    (htm-user *standard-output*)(:BR)
	    (htm-main-link *standard-output* :login t)))
	  (htm 
	   (htm-login-form *standard-output*)
	   (htm-main-link *standard-output* :login nil))))))

(defun htm-login-form (s)
  (with-html-output (s)
    (:CENTER
     (:H2 "Login Page")
     (:FORM :NAME "login" :ACTION "/login-reply" :METHOD "POST" 
	    (:TABLE ;;:BORDER 1
	     (:TR  :ALIGN "left"
		   (:TD "ID:")
		   (:TD 
		    (:INPUT :TYPE "text" :NAME "id" :VALUE nil)))
	     (:TR  :ALIGN "left"
		   (:TD "PASSWD:")
		   (:TD
		    (:INPUT :TYPE "password" :NAME "passwd" :VALUE nil))))
	    (:INPUT :TYPE "submit" :NAME "submit" :VALUE "OK")))))

(defun htm-user (s)
  (with-html-output (s)
    (let ((user (session-value :user)))
      (if user 
	  (progn
	    (htm
	      (fmt "USER:(~A)" user)
	      (:A :HREF "/logout.html" "LogOut")(:BR))
	    nil)
	  (progn
	    (htm
	     (fmt "USER:(GUEST)")
	     (:A :HREF "/index.html" "LogIn")(:BR))
	   nil)))))

(defun htm-main-link (s &key login)
  (progn
    (if (not login)
	(with-html-output (s)
	  (:CENTER
	   (:A :HREF "/survey.html" "GUEST")(:BR)
	   (:A :HREF "/list-user.html" "LIST-USER")(:BR)
	   (:A :HREF "/register.html" "REGISTER")(:BR)))
	(with-html-output (s)
	  (:CENTER
	   (:A :HREF "/survey.html" "VOTE")(:BR))))
    nil))
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; register 페이지 만들기
(define-url-fn (register)
  (standard-page (:TITLE "Register")
    (:CENTER
     (:H2 "Register Page")
     (:FORM :NAME "register" :METHOD "POST" :ACTION "/register-reply"
	    (:TABLE
	     (:TR :ALIGN "left"
		   (:TD "ID:")
		   (:TD
		    (:INPUT :TYPE "text" :NAME "id" :VALUE nil)))
	     (:TR :ALIGN "left"
		   (:TD "PASSWD:")
		   (:TD
		    (:INPUT :TYPE "password" :NAME "passwd" :VALUE nil))))
	    (:INPUT :TYPE "submit" :NAME "submit" :VALUE "OK"))
     (:A :HREF "/index.html" "MAIN"))))

(define-easy-handler (register-reply :uri "/register-reply") (id passwd)
  (let ((res (add-record (make-user id passwd 0))))
    (standard-page (:TITLE "Register-Reply")
      (:CENTER
       (:H2 "Register Reply")
       (:TABLE :BORDER 1 :WIDTH "200"
	     (:TR
	      (:TD
	       (if res
		   (progn
		     (htm
		      "You are registered."(:BR)
		      "ID:(" (esc id) "),PASSWORD:(" (esc passwd) ")"))
		   (progn
		     (htm
		      (fmt "(~A) is already existing ID" id)(:BR)
		   )))
	       )))
     (:BR)
     "["(:A :HREF "/register.html" "BACK")"] [" ;(:nbsp)
     (:A :HREF "/index.html" "MAIN")
     "]"
))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Login 수행 하기
(define-easy-handler (login-reply :uri "/login-reply") (id passwd)
  (standard-page (:TITLE "Login-Reply")
    (:CENTER
     "You entered ID:(" (esc id) ") PASSWD:(" (esc passwd) ")" (:BR)
     (check-passwd *standard-output* id passwd))))

(defun check-passwd (s id passwd)
  (with-html-output (s) 
    (let ((record (car (select (where :id id)))))
      (if (eq record nil)
	  (progn
	    (htm
	     "You are not registered!"(:BR)
	     (:A :HREF "/index.html" "BACK!"))
	    nil)
	  (if (string= passwd (getf record :passwd))
	      (progn 
		(start-session)
		(setf (session-value :user) id)
		(redirect "/survey.html"))
	      (progn
		(htm		
		 "Incorrect password!"(:BR)
		 (:A :HREF "/index.html" "BACK"))
		nil))))))


;;; 2. 투표 페이지에서 사용자 정보 출력, 로그아웃 링크 추가하기
(define-url-fn (survey)
  (standard-page (:TITLE "Survey")
    (:CENTER
     (:H1 "Vote on your favorite programming language!")
    (:P "Missing a item? Make it available for votes " 
	(:A :HREF "new-item.html" "here"))
    (:H2 "Current stand")
;;;;;;
;; 2.
    (esc (htm-user *standard-output*))(:BR)(:BR)
;;;;;
    (:TABLE
     (dolist (item (items))
       (htm  
	(:TR
	 (:TD
	  (:A :HREF (format nil "vote.html?name=~a" (name item)) "Vote!")
	  (fmt "~A with ~d/~d votes" (name item) (votes item) *total-votes*))
	 (:TD
	  (fmt "~A" (stats "*" (votes item) *stat-range*)))))))
;;;;;;;
;; 2
    (:BR)
    (:A :HREF "/index.html" "MAIN"))))
;;;;;;;

;;; 로그 아웃 페이지 만들기
(define-url-fn (logout)
  (delete-session-value :user)
  (redirect "/index.html"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 로그인 상태에서만 투표할 수 있도록 하기
;;;;;;;
;; 투표 하기를 변경해야 한다.
;; (define-url-fn (vote)
;;   (let ((item (item-from-name (parameter "name"))))
;;     (if item
;; 	(vote-for item))
;;     (redirect "/survey.html")))

(define-url-fn (vote)
  (let ((user (session-value :user)))
    (if user 
	(let ((vote-cnt (get-vote-flag user)))
	  (if (<= vote-cnt 0)
	      (let ((item (item-from-name (parameter "name"))))
		(if item
		    (vote-user item user vote-cnt))
		(redirect "/survey.html"))
	      (redirect "/already-voted.html")))
	(redirect "/noti-login.html"))))

(defun vote-user (item user vote-cnt)
  (vote-for item)
  (update (where :id user) :vote-flag (+ vote-cnt 1)))

(defun get-vote-flag (user)
  (let ((record (car (select (where :id user)))))
    (getf record :vote-flag)))

(define-url-fn (already-voted)
  (standard-page (:TITLE "Notify to login")
    (:CENTER
     (esc (htm-user *standard-output*))
     (:H3 "You have already voted!")(:BR)
     (:A :HREF "/survey.html" "BACK"))))
    
(define-url-fn (noti-login)
  (standard-page (:TITLE "Notify to login")
    (:CENTER
     (:H3 "Please Log in to vote")(:BR)
     (:A :HREF "/survey.html" "BACK")(:BR)
     (:A :HREF "/index.html" "LOGIN")(:BR))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 사용자 리스트 보여주기 
(define-url-fn (list-user)
  (standard-page (:TITLE "List users")
    (:CENTER
     (:H2 "List users")
     (db-user-list *standard-output*)(:BR)
     (:A :HREF "/index.html" "MAIN"))))

(defun db-user-list (stream)
  (with-html-output (stream)
    (:TABLE :BORDER 1
     (loop for lst on (mapcar #'(lambda (lst)
				  (getf lst :id)) 
			      *db*)
	  do (htm
	      (:TR
	       (:TD (esc (car lst)))))))))

;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
