

(def h {[1 2] 3})

(h [1 2])

(conj (first (keys h)) 3)

(require 'clojure.string)
(clojure.string/lower-case "Clojure")


;;;
(map clojure.string/lower-case ["Java" "Imperative" "Weeping" "Clojure" "Learning" "Peace"])

(map * [1 2 3 4] [5 6 7 8])



;;;
(reduce max [ 0 -3 10 48])


;;;
(reduce
 (fn [m v]
   (assoc m v (* v v)))

 {}

 [1 2 3 4])

;;;
(#(filter string? %) ["a" 5 "b" 6])
(#(filter % ["a" 5 "b" 6]) string?)
(#(filter % ["a" 5 "b" 6]) number?)

(#(map * [1 2 3] [4 5 6] [7 8 9]))
(#(map * % %2 %3) [1 2 3] [4 5 6] [7 8 9])


(apply hash-map [:a 5 :b 6])



;;; p65
;;; partial

(def only-strings (partial filter string?))

(only-strings ["a" 5 "b" 6])


;;
(#(filter string? %) ["a" 5 "b" 6])

(#(filter % ["a" 5 "b" 6]) string?)

(#(filter % ["a" 5 "b" 6]) number?)


;;
;(#(map *) [1 2 3] [4 5 6] [7 8 9])

(#(map * % %2 %3) [1 2 3] [4 5 6] [7 8 9])

(#(apply map * %&) [1 2 3] [4 5 6] [7 8 9])

((partial map *) [1 2 3] [4 5 6] [7 8 9])




;;; p68 Composition of function
(defn negated-sum-str
  [& numbers]
  (str (- (apply + numbers))))

(negated-sum-str 10 12 3.4)


(def negated-sum-str (comp str - +))





;;
(require '[clojure.string :as str])

(def camel->keyword (comp keyword
			  str/join
			  (partial interpose \-)
			  (partial map str/lower-case)
			  #(str/split % #"(?<=[a-z])(?=[A-Z])")))

(camel->keyword "CamelCase")

(camel->keyword "lowerCamelCase")

(def camel-pairs->map (comp (partial apply hash-map)
                            (partial map-indexed (fn [i x]
                                                   (if (odd? i)
                                                     x
                                                     (camel->keyword x))))))
j



;;; Higher order function
;; p72

(defn adder
  [n]
  (fn [x] (+ n x)))

((adder 5) 18)

(defn doubler
  [f]
  (fn [& args]
    (* 2 (apply f args))))

(def double-+ (doubler +))

(double-+ 1 2 3)





;;; p72 ~

(defn print-logger
  [writer]
  #(binding [*out* writer]
     (println %)))

(def *out*-logger (print-logger *out*))

(*out*-logger "hello")






(def writer (java.io.StringWriter.))

(def retained-logger (print-logger writer))

(retained-logger "hello")

(str writer)





(require 'clojure.java.io)

(defn file-logger
  [file]
  #(with-open [f (clojure.java.io/writer file :append true)]
     ((print-logger f) %)))

(def log->file (file-logger "messages.log"))

(log->file "hello")




(defn multi-logger
  [& logger-fns]
  #(doseq [f logger-fns]
     (f %)))

(def log (multi-logger
          (print-logger *out*)
          (file-logger "messages.log")))

(log "hello again")




(defn timestamped-logger
  [logger]
  #(logger (format "[%1$tY-%1$tm-%1$te %1$tH:%1$tM:%1$tS] %2$s" (java.util.Date.) %)))

(def log-timestamped (timestamped-logger
                     (multi-logger
                      (print-logger *out*)
                      (file-logger "messages.log"))))
                                    
(log-timestamped "goodbye, now")

       


;;; p77
(require 'clojure.xml)

(defn twitter-followers
  [username]
  (->> (str "https://api.twitter.com/1/users/show.xml?screen_name=" username)
       clojure.xml/parse
       :content
       (filter (comp #{:folwers_count} :tag))
       first
       :content
       first
       Integer/parseInt))

;; (twitter-followers "ClojureBook")


;;; p79
(defn prime?
  [n]
  (cond
   (== 1 n) false
   (== 2 n) true
   (even? n) false
   :else (->> (range 3 (inc (Math/sqrt n)) 2)
	      (filter #(zero? (rem n %)))
	      empty?)))

(time (prime? 1125899906842679))

(let [m-prime? (memoize prime?)]
  (time (m-prime? 112589906842679))
  (time (m-prime? 112589906842679)))



(repeatedly 10 (partial rand-int 10))

(repeatedly 10 (partial (memoize rand-int) 10))

