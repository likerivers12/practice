
;;; p83
'(a b :name 12.5)

['a 'b :name 12.5]

{:name "Chas" :age 31}

#{1 2 3}

{Math/PI "~3.14"
 [:composite "key"] 42
 nil "nothing"}

#{{:first-name "chas" :last-name "emerick"}
  {:first-name "brian" :last-name "carper"}
  {:first-name "christophe" :last-name "grand"}}




;;; p84
(def v [1 2 3])

(conj v 4)

(conj v 4 5)

(seq v)



(def m {:a 5 :b 6})

(conj m [:c 7])

(seq m)



(def s #{1 2 3})

(conj s 10)

(conj s 3 4)

(seq s)



(def lst '(1 2 3))

(conj lst 0)

(conj lst 0 -1)

(seq lst)




;;; p87
(defn swap-pairs
  [sequential]
  (into (empty sequential)
        (interleave
         (take-nth 2 (drop 1 sequential))
         (take-nth 2 sequential))))

(swap-pairs (apply list (range 10)))

(swap-pairs (apply vector (range 10)))



(defn map-map
  [f m]
  (into (empty m)
        (for [[k v] m]
          [k (f v)])))

(map-map inc (hash-map :z 5 :c 6 :a 0))

(map-map inc (sorted-map :z 5 :c 6 :a 0))




;;; p91
(doseq [x (range 3)]
  (println x))

(let [r (range 3)
      rst (rest r)]
  (prn (map str rst))
  (prn (map #(+ 100 %) r))
  (prn (conj r -1) (conj rst 42)))



;;; p92
;;; sequences are not lists
(let [s (range 1e6)]
  (time (count s)))

(let [s (apply list (range 1e6))]
  (time (count s)))







;;; p93
(defn random-ints
  "Returns a lazy seq of random integers in the range [0,limit)."
  [limit]
  (lazy-seq
   (cons (rand-int limit)
	 (random-ints limit))))

(take 10 (random-ints 50))


;; p94
(defn random-ints
  "Returns a lazy seq of random integers in the range [0,limit)."
  [limit]
  (lazy-seq
   (println "realizing random number")
   (cons (rand-int limit)
	 (random-ints limit))))

(def rands (take 10 (random-ints 50)))

(first rands)

(nth rands 3)

(count rands)

(count rands)




(repeatedly 10 (partial rand-int 50))
            

;;; p97
(doc iterate)

(doc reverse)



;;; p99
(split-with neg? (range -5 5))

;; ;;; Out-of-memoty 
;; (let [[t d] (split-with #(< % 12) (range 1e8))]
;;   [(count d) (count t)])

(let [[t d] (split-with #(< % 12) (range 1e8))]
  [(count t) (count d)])



;;; association
(def m {:a 1, :b 2, :c 3})

(get m :b)

(get m :d)

(get m :d "not-found")

(assoc m :d 4)

(dissoc m :b)

(assoc m
  :x 4
  :y 5
  :z 6)


(contains? [1 2 3] 0)







(find {:ethel nil} :lucy)

(find {:ethel nil} :ethel)

(if-let [e (find {:a 5 :b 6} :a)]
  (format "found %s => %s" (key e) (val e))
  "not found")





;;; p103 Indexed
(nth [:a :b :c] 2)

(get [:a :b :c] 2)

; error!
(nth [:a :b :c] 3)

(get [:a :b :c] 3)





;;; p104 Stack
(conj '() 1)

(conj '(2 1) 3)

(peek '(3 2 1))

(pop '(3 2 1))


(def x '(2 1))

(conj x 3)

x




;;; p106
(def sm (sorted-map :z 5 :x 9 :y 0 :b 2 :a 3 :c 4))

sm

(rseq sm)

(subseq sm <= :c)

(subseq sm > :b <= :y)

(rsubseq sm > :b <= :y)



(sort < (repeatedly 10 #(rand-int 100)))

(sort-by first > (map-indexed vector "Clojure"))






;;;
(defn interpolate
  "Takes a collection of points (as [x y] tuples), returning a function
which is a linear interpolation between thos points."
  [points]
  (let [results (into (sorted-map) (map vec points))]    (fn [x]
      (let [[xa ya] (first (rsubseq results <= x))
	    [xb yb] (first (subseq results > x))]
	(if (and xa xb)
	  (/ (+ (* ya (- xb x)) (* yb (- x xa)))
	     (- xb xa))
	  (or ya yb))))))

(def f (interpolate [[0 0] [10 10] [15 5]]))

(map f [2 10 12])





;;;
(defn naive-into [coll source]
  (reduce conj coll source))





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Conway's Game of Life
;;; p138
(defn empty-board
  "Creates an rectangular empty board of the specified width and height"
  [w h]
  (vec (repeat w (vec (repeat h nil)))))

(defn populate
  "Turns :on each of the cells specified as [y, x] coordinates."
  [board living-cells]
  (reduce (fn [board coordinates]
	    (assoc-in board coordinates :on))
	  board
	  living-cells))


(def glider (populate (empty-board 6 6) #{[2 0] [2 1] [2 2] [1 2] [0 1]}))

(defn neighbours
  [[x y]]
  (for [dx [-1 0 1]
	dy [-1 0 1] :when (not= 0 dx dy)]
    [(+ dx x) (+ dy y)]))

(defn count-neighbours
  [board loc]
  (count (filter #(get-in board %) (neighbours loc))))

(defn indexed-step
  "Yiedls the next state of the board, using indices to determine neighbors, liveness, etc."
  [board]
  (let [w (count board)
	h (count (first board))]
    (loop [new-board board x 0 y 0]
      (cond
       (>= x w) new-board
       (>= y h) (recur new-board (inc x) 0)
       :else
       (let [new-liveness
	     (case (count-neighbours board [x y])
		   2 (get-in board [x y])
		   3 :on
		   nil)]
	 (recur (assoc-in new-board [x y] new-liveness) x (inc y)))))))

(-> (iterate indexed-step glider) (nth 8) pprint)


(defn indexed-step2
  [board]
  (let [w (count board)
	h (count (first board))]
    (reduce
     (fn [new-board x]
       (reduce
	(fn [new-liveness y]
	  (let [new-liveness
		(case (count-neighbours board [x y])
		      2 (get-in board [x ]y)
		      3 :on
		      nil)]
	    (assoc-in new-board [x y] new-liveness)))
	new-board (range h)))
     board (range w))))

(defn indexed-step3 [board]
  (let [w (count board)
	h (count (first board))]
    (reduce
     (fn [new-board [x y]]
       (let [new-liveness
	     (case (count-neighbours board [x y])
		   2 (get-in board [x y]) 3 :on
		   nil)]
	 (assoc-in new-board [x y] new-liveness)))
     board (for [x (range h) y (range w)] [x y]))))



(defn window
  "Returns a lazy sequence of 3-item windows centered
around each item of coll, padded as necessary with
pad or nil."
  ([coll] (window nil coll))
  ([pad coll]
     (partition 3 1 (concat [pad] coll [pad]))))

(defn cell-block
  "Creates a sequences of 3x3 windows from a triple of 3 sequences."
  [[left mid right]]
  (window (map vector left mid right)))








;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; p146
(defn maze
  "Returns a random maze carved out of walls; walls is a set of
2-item sets #{a b} where a and b are locations.
The returned maze is a set of the remaining walls." [walls]
(let [paths (reduce (fn [index [a b]]
		      (merge-with into index {a [b] b [a]}))
		    {} (map seq walls))
      start-loc (rand-nth (keys paths))]
  (loop [walls walls
	 unvisited (disj (set (keys paths)) start-loc)]
    (if-let [loc (when-let [s (seq unvisited)] (rand-nth s))]
      (let [walk (iterate (comp rand-nth paths) loc)
	    steps (zipmap (take-while unvisited walk) (next walk))]
	(recur (reduce disj walls (map set steps))
	       (reduce disj unvisited (keys steps))))
      walls))))

;;; p147

(defn grid [w h]
  (set (concat
	(for [i (range (dec w)) j (range h)] #{[i j] [(inc i) j]})
	(for [i (range w) j (range (dec h))] #{[i j] [i (inc j)]}))))

(defn draw
  [w h maze]
  (doto (javax.swing.JFrame. "Maze")
    (.setContentPane
     (doto (proxy [javax.swing.JPanel] []
	     (paintComponent [^java.awt.Graphics g]
			     (let [g (doto ^java.awt.Graphics2D (.create g)
					   (.scale 10 10)
					   (.translate 1.5 1.5)
					   (.setStroke (java.awt.BasicStroke. 0.4)))]
			       (.drawRect g -1 -1 w h)
			       (doseq [[[xa ya] [xb yb]] (map sort maze)]
				 (let [[xc yc] (if (= xa xb)
						 [(dec xa) ya]
						 [xa (dec ya)])]
				   (.drawLine g xa ya xc yc))))))
       (.setPreferredSize (java.awt.Dimension.
			   (* 10 (inc w)) (* 10 (inc h))))))
			   .pack
			   (.setVisible true)))






;;;
(let [w 40, h 40
      grid (grid w h)
      walls (maze grid)
      labyrinth (reduce disj grid walls)
      places (distinct (apply concat labyrinth))
      theseus (rand-nth places)
      minotaur (rand-nth places)
      path (->> theseus
                (ariadne-zip labyrinth)
                (iterate z/next)
                (filter #(= minotaur (first (z/node %))))
                first z/path rest)]
  (draw w h walls path))
