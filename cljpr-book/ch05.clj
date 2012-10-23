

;;;===============================================
;; p233
(defmacro foreach [[sym coll] & body]
  `(loop [coll# ~coll]
     (when-let [[~sym & xs#] (seq coll#)]
       ~@body
       (recur xs#))))


(foreach [x [1 2 3]]
  (println x))





;;;===============================================
;; unless

(defn unless [expr form]
  (if expr nil form))

(unless true
        (println "Not"))


(defmacro unless [expr form]
  (list 'if expr nil form))

(unless true
        (println "Not"))






;; p235
(defmacro print-keyword [x]
  `(println (keyword ~x)))

(print-keyword "foo")





;;;===============================================
;; MACRO: reverse-it
(require '(clojure [string :as str]
                   [walk :as walk]))

(defmacro reverse-it
  [form]
  (walk/postwalk #(if (symbol? %)
                    (symbol (str/reverse (name %)))
                    %)
                 form))

(reverse-it
 (qesod [gra (egnar 5)]
        (nltnirp (cni gra))))


(macroexpand-1 '(reverse-it
                 (qesod [gra (egnar 5)]
                        (nltnirp (cni gra)))))

(macroexpand '(reverse-it
                 (qesod [gra (egnar 5)]
                        (nltnirp (cni gra)))))


(require '(clojure [pprint :as pp]))

(pp/pprint (macroexpand '(reverse-it
                 (qesod [gra (egnar 5)]
                        (nltnirp (cni gra))))))





;;;===============================================
(macroexpand '(cond a b c d))

(require '[clojure.walk :as w])

(w/macroexpand-all '(cond a b c d))

(clojure.walk/macroexpand-all '(cond a b c d))



;;; quote, syntax-quote
(defmacro my-while
  [test & body]
  (list 'loop []
        (concat (list 'when test) body)
        '(recur)))

(defmacro my-while
  [test & body]
  `(loop []
     (when ~test
       ~@body
       (recur))))



(def foo 123)

[foo (quote foo) 'foo `foo]




(list `map `println [foo])

`(map println [~foo])

`(map println ~[foo])



(let [defs '((def x 123)
             (def y 456))]
  (concat (list 'do) defs))


(let [defs '((def x 123)
             (def y 456))]
  `(do ~@defs))

;; code body 에 많이 사용




;;;===============================================
;; 이름 충돌
(defmacro unhygienic
  [& body]
  `(let [x :oops]
     ~@body))

(unhygienic (println "x:" x))

(defmacro still-unhygienic
  [& body]
  `(let [~'x :oops]
     ~@body))

(still-unhygienic (println "x:" x))

(let [x :this-is-important]
  (still-unhygienic
    (println "x:" x)))


;; Gensyms to the Rescue
(defmacro hygienic
  [& body]
  (let [sym (gensym)]
    `(let [~sym :macro-value]
       ~@body)))

(let [x :important-value]
  (hygienic (println "x:" x)))



;; symbol ending in # (auto gensym)
(defmacro hygienic
  [& body]
  `(let [x# :macro-value]
     ~@body))


(defmacro auto-gensyms
  [& numbers]
  `(let [x# (rand-int 10)]
     (+ x# ~@numbers)))

(auto-gensyms 1 2 3 4 5)




;; 같은 `안에서만 같은 심볼로 생성됨
[`x# `x#]

(defmacro our-doto [expr & forms]
  `(let [obj# ~expr]
     ~@(map (fn [[f & args]]
              `(~f obj# ~@args)) forms)
     obj#))

;;=> gensym으로 해결
(defmacro our-doto [expr & forms]
  (let [obj (gensym "obj")]
    `(let [~obj ~expr]
       ~@(map (fn [[f & args]]
                `(~f ~obj ~@args)) forms)
       ~obj)))

(our-doto "It works"
          (println "I can't believe it")
          (println "I still can't believe it"))




;;;===============================================
;;; Double evaluation
(defmacro spy [x]
  `(do
     (println "spied" '~x ~x)
     ~x))

(spy 2)

(spy (rand-int 10))

(macroexpand-1 '(spy (rand-int 10)))



;;=> 
(defmacro spy [x]
  `(let [x# ~x]
     (println "spied" '~x x#)
     x#))

(spy (rand-int 10))

(macroexpand-1 '(spy (rand-int 10)))





;;;===============================================
;; ->, ->>
;; threading macro
(defn ensure-seq [x]
  (if (seq? x) x (list x)))

(defn insert-second
  "Insert x as the second item in seq y."
  [x ys]
  (let [ys (ensure-seq ys)]
    (concat (list (first ys) x)
            (rest ys))))
;;->
(defn insert-second
  "Insert x as the second item in seq y."
  [x ys]
  (let [ys (ensure-seq ys)]
    `(~(first ys) ~x ~@(rest ys))))
;;->
(defn insert-second
  "Insert x as the second item in seq y."
  [x ys]
  (let [ys (ensure-seq ys)]
    (list* (first ys) x (rest ys))))

(defmacro thread
  "Thread x through successive forms."
  ([x] x)
  ([x form] (insert-second x form))
  ([x form & more] `(thread (thread ~x ~form) ~@more)))

(thread [1 2 3] reverse (conj 4) println)



x: [1 2 3]
ys: (reverse (conj 4) println)
(reverse [1 2 3]) ((conj 4) println)

(conj [3 2 1] 4) (println)


;; function으로 구현하는 threading
(defn thread-fns
  ([x] x)
  ([x form] (form x))
  ([x form & more] (apply thread-fns (form x) more)))

(thread-fns [1 2 3] reverse #(conj % 4) prn)


(thread [1 2 3] .toString (.split " ") seq)

;; error
(thread-fns [1 2 3] .toString #(.split % " ") seq)
;;->
(thread-fns [1 2 3] #(.toString %) #(.split % " ") seq)




;; ->>
;; 결과를 last item으로 놓는다.
;; sequence transformation에 많이 사용됨. 
(->> (range 10) (map inc) (reduce +))







;;;===============================================
;;; &env
(defmacro spy-env []
  (let [ks (keys &env)]
    `(prn (zipmap '~ks [~@ks]))))

(let [x 1 y 2]
  (spy-env)
  (+ x y))

(zipmap [:a :b :c] [1 2 3])





(defmacro simplify
  [expr]
  (let [locals (set (keys &env))]
    (if (some locals (flatten expr))
      expr
      (do
        (println "Precomputing: " expr)
        (list `quote (eval expr))))))

(defn f
  [a b c]
  (+ a b c (simplify (apply + (range 5e7)))))
;; Precomputing

(f 1 2 3)


(defn f'
  [a b c]
  (simplify (apply + a b c (range 5e7))))

(f' 1 2 3)



;; Testing and debuggin &env usage
(@#'simplify nil {} '(inc 1))

(@#'simplify nil {'x nil} '(inc x))


;;;===============================================
;;; &form
(defmacro ontology
  [& triples]
  (every? #(or (== 3 (count %))
               (throw (IllegalArgumentException.
                       "All triples provided as arguments must have 3 elements")))
          triples)
  ;; build and emit pre-processed ontology here...
  )

(ontology ["Boston" :capital-of])



;;=>
(defmacro ontology
  [& triples]
  (every? #(or (== 3 (count %))
               (throw (IllegalArgumentException.
                       (format "`%s` provided to `%s` on line %s has < 3 elements"
                               %
                               (first &form)
                               (-> &form meta :line)))))
          triples)
  ;; build and emit pre-processed ontology here...
  )

(ontology ["Boston" :capital-of])


;; Preserving user-provided type hints
(set! *warn-on-reflection* true)

(defn first-char-of-either
  [a b]
  (.substring ^String (or a b) 0 1))

(first-char-of-either "aaa" "bbb")


(binding [*print-meta* true]
  (prn '^String (or a b)))

(binding [*print-meta* true]
  (prn (macroexpand '^String (or a b))))
;; type hint가 표시되지 않음.



;;---------
(meta [1 2 3])

(meta '^String [1 2 3])

(with-meta [1 2 3] {:my "meta"})

(meta (with-meta [1 2 3] {:my "meta"}))

(let [tmp (with-meta [1 2 3] {:my "meta"})]
  (meta (with-meta (gensym "res") (meta tmp))))
;;---------


(defmacro OR
  ([] nil)
  ([x]
     (let [result (with-meta (gensym "res") (meta &form))]
       `(let [~result ~x]
          ~result)))
  ([x & next]
     (let [result (with-meta (gensym "res") (meta &form))]
       `(let [or# ~x
              ~result (if or# or# (OR ~@next))]
          ~result))))

(binding [*print-meta* true]
  (prn (macroexpand '^String (OR a b))))

(defn first-char-of-any
  [a b]
  (.substring ^String (OR a b) 0 1))

(first-char-of-any "aaa" "bbb")

;; 
(defn preserve-metadata
  "Ensures that the body containing `expr` will carry the metadata from `&form`."
  [&form expr]
  (let [res (with-meta (gensym "res") (meta &form))]
    `(let [~res ~expr]
       ~res)))

(defmacro OR
  "Same as `clojure.core/or`, but preserves user-supplied metadata (e.g. type hints)."
  ([] nil)
  ([x] (preserve-metadata &form x))
  ([x & next]
     (preserve-metadata &form `(let [or# ~x]
                                 (if or# or# (or ~@next))))))



;; Testing Contextual Macros
(defn macroexpand1-env [env form]
  (if-let [[x & xs] (and (seq? form) (seq form))]
    (if-let [v (and (symbol? x) (resolve x))]
      (if (-> v meta :macro)
        (apply @v form env xs)
        form)
      form)
    form))

(macroexpand1-env '{} '(simplify (range 10)))

(macroexpand1-env '{range nil} '(simplify (range 10)))

(defmacro spy [expr]
  `(let [value# ~expr]
     (println (str "line #" ~(-> &form meta :line) ",")
              '~expr value#)
     value#))

(let [a 1
      a (spy (inc a))
      a (spy (inc a))]
  a)


(macroexpand1-env {} (with-meta '(spy (+ 1 1)) {:line 42}))




;;=
(defmacro if-all-let [bindings then else]
  (reduce (fn [subform binding]
            `(if-let [~@binding] ~subform ~else))
          then (reverse (partition 2 bindings))))

(defn macroexpand1-env [env form]
  (if-all-let [[x & xs] (and (seq? form) (seq form))
               v (and (symbol? x) (resolve x))
               _ (-> v meta :macro)]
              (apply @v form env xs)
              form))

(partition 2 '(a 1 b 2 c 3))

(macroexpand1-env {} (with-meta '(spy (+ 1 1)) {:line 42}))
