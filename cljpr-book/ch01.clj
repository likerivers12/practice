;; p11
(defn average
  [numbers]
  (/ (apply + numbers) (count numbers)))

;; p12
(read-string "42")
(read-string "(+ 1 2)")
(eval (read-string "(+ 1 2)"))

;; p13
(pr-str [1 2 3])
(read-string "[1 2 3]")

\space
\newline
\formfeed
\return
\baskspace
\tab

(def person {:name "Sandra Cruz"
             :city "Portland, ME"})
(:city person)
(person :city)


;; p13
(def pizza {:name "Ramunto's"
            :location "Claremont, NH"
            ::location "43.3734,-72.3365"})
(:user/location pizza)

(name :user/location)
(namespace :user/location)
(namespace :location)




;;; Regular expressions

(re-seq #"(...) (...)" "foo bar")
(re-seq #"(\d+)-(\d+)" "1-3")



;;; Collection Literals
;; p19
'(a b :name 12.5)  ;; list
['a 'b :name 12.5] ;; vector
{:name "Chas" :age 31}  ;; map
#{1 2 3}                ;; set

*ns*


;;; Code Block: do
;; p25
(do
  (println "hi")
  (apply * [4 5 6]))

(let [a (inc (rand-int 6))
      b (inc (rand-int 6))]
  (do
    (println (format "You rolled a %s and a %s" a b))
    (+ a b)))



;;; Local Bindings: let
;; p
(defn hypot
  [x y]
  (let [x2 (* x x)
        y2 (* y y)]
    (Math/sqrt (+ x2 y2))))

(hypot 2 3)


;;;
;;
(def v [42 "foo" 99.2 [5 12]])

(first v)
(second v)
(last v)
(nth v 2)
(.get v 2)

(v 2)


;;; Sequential destructuring
;; p30
(def v [42 "foo" 99.2 [5 12]])

(let [[x y z] v]
  (+ x z))

(let [x (nth v 0)
      y (nth v 1)
      z (nth v 2)]
  (+ x z))

(let [[x _ _ [y z]] v]
  (+ x y z))

(let [[x & rest] v]
  rest)

(let [[x & rest] [1 2 3 4]]
  rest)

(let [[x _ z :as original-vector] v]
  (conj original-vector (+ x z)))



;;; Map destructuring
;; p32
(def m {:a 5 :b 6
        :c [7 8 9]
        :d {:e 10 :f 11}
        "foo" 88
        42 false})

(let [{a :a b :b} m]
  (+ a b))


;; p33
(def map-in-vector ["James" {:birthday (java.util.Date. 73 1 6)}])
(let [[name {bd :birthday}] map-in-vector]
  (str name " was born on " bd))


;; p35,6
(def user-info ["robert8990" 2011 :name "Bob" :city "Boston"])

(let [[username account-year & {:keys [name city]}] user-info]
  (format "%s is in %s" name city))

