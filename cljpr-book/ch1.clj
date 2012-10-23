
(defn average
      [numbers]
      (/ (apply + numbers) (count numbers)))

(average [60 80 100 400])

;;;

(read-string "ABC")
(pr-string "123")

(class \c)




;;;
(def person {:name "Sandra Cruz"
     	    :city "Portland, ME"})

(person :name)





(def pizza {:name "Ramunto's"
     	   :location "Claremont, NH"
	   ::location "43.3734,-72.3365"})







;;;
(class #"(p|h)ail")

(re-seq #"(...) (...)" "foo bar")

(re-seq #"(\d+)-(\d+)" "1-3")


;;;
(read-string "(+ 1 2 #_(* 2 2 ) 8)")



;;;
'(a b :name 12.5)		;; LIST
['a 'b :name 12.5]		;; vector
{:name "Chas" :age 31}		;; map
#{1 2 3}      	   		;; set


;;;
*ns*




;;;
(defn hypot [x y]
      (let [x2 (* x x)
      	   y2 (* y y)]
	   (Math/sqrt (+ x2 y2))))




;;;
(def v [42 "foo" 9.2 [5 12]])

(first v)
(second v)
(last v)
(nth v 2)
(v 2)
(.get v 2)



;;;
(def strange-adder (fn adder-self-reference
     		    ([x] (adder-self-reference x 1))
		    ([x y] (+ x y))))


(letfn [(odd? [n]
       	      (even? (dec n)))
	(evn? [n]
	      (odd? (dec n)))]
	  (odd? 11))
